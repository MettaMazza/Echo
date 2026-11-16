#!/usr/bin/env python3
"""Bounded, findings-only review of one authenticated Echo run.

The native bridge owns permission checks and publication. This process has no
RSI submission, host execution, deployment or message-sending model tools.
"""
from __future__ import annotations
import argparse
from contextlib import contextmanager
import hashlib
import itertools
import json
import os
from pathlib import Path
import re
import sqlite3
import subprocess
import tempfile
import time
import urllib.request

FIELDS = {'title', 'severity', 'observation', 'hypothesis', 'evidence',
          'recommendation', 'acceptance_test', 'confidence'}
MAX_REPORT_BYTES = 24000
MAX_EVIDENCE_BYTES = 48000
MAX_FIELD_CHARS = 3000


def unique_object(pairs):
    result = {}
    for key, value in pairs:
        if key in result:
            raise ValueError('duplicate JSON key')
        result[key] = value
    return result


def validate_report(raw, event_ids):
    if len(raw.encode()) > MAX_REPORT_BYTES:
        raise ValueError('oversize report')
    report = json.loads(raw, object_pairs_hook=unique_object)
    if not isinstance(report, dict) or set(report) != {'findings'}:
        raise ValueError('invalid report shape')
    findings = report['findings']
    if not isinstance(findings, list) or len(findings) > 8:
        raise ValueError('invalid findings')
    for finding in findings:
        if not isinstance(finding, dict) or set(finding) != FIELDS:
            raise ValueError('invalid finding shape')
        for field in FIELDS - {'evidence'}:
            value = finding[field]
            if not isinstance(value, str) or not value.strip() or len(value) > MAX_FIELD_CHARS:
                raise ValueError('invalid finding field')
        for field in ('severity', 'confidence'):
            if finding[field] not in ('low', 'medium', 'high'):
                raise ValueError('invalid enum')
        evidence = finding['evidence']
        if (not isinstance(evidence, list) or not 1 <= len(evidence) <= 16
                or any(not isinstance(e, str) or e not in event_ids for e in evidence)):
            raise ValueError('evidence outside reviewed batch')
    return report


def bridge_call(bridge, archive, operation, run, *args):
    result = subprocess.run([str(bridge), str(archive), operation, run, *map(str, args)],
                            capture_output=True, timeout=15, check=True)
    if len(result.stdout) > 65536:
        raise ValueError('oversize bridge result')
    return json.loads(result.stdout)


@contextmanager
def readonly(archive):
    # No schema migrations, checkpointing, backup, VACUUM or writes here.
    connection = sqlite3.connect(Path(archive).resolve().as_uri() + '?mode=ro',
                                 uri=True, timeout=0.25)
    try:
        connection.execute('PRAGMA query_only=ON')
        deadline = time.monotonic() + 1.0
        connection.set_progress_handler(lambda: int(time.monotonic() > deadline), 1000)
        yield connection
    finally:
        connection.close()


def evidence_page(archive, session, run, after=0):
    """Bounded source pages; assemble consecutive text deltas without a model.

    Range endpoints remain exact, citable archive IDs. This avoids spending a
    review call on sixty individual token chunks while retaining emitted
    reasoning/content verbatim. Unknown protocol records remain raw evidence.
    """
    events, size, read_count = [], 0, 0
    with readonly(archive) as db:
        boundary = db.execute('''SELECT o.sequence,l.sequence,r.state,
            o.event,substr(o.payload,1,8000),length(o.payload),o.occurred
            FROM echo_runs r JOIN echo_events o ON o.event=r.objective_event
            JOIN echo_events l ON l.event=r.last_event WHERE r.run=? AND r.session=?''',
            (run, session)).fetchone()
        if not boundary or boundary[2] not in ('completed', 'failed', 'cancelled'):
            raise ValueError('run is not settled')
        first, last, state, objective_id, objective, objective_size, objective_time = boundary
        cursor = max(after, first - 1)
        query = '''WITH bounded AS (
            SELECT sequence,event,kind,occurred,CASE
            WHEN kind='model_request' AND json_valid(payload)
            THEN json_set(json_remove(payload,'$.request_body'),'$.request_body_omitted',1)
            ELSE payload END AS body
            FROM echo_events WHERE session=? AND sequence>? AND sequence<=?
            AND kind<>'scratchpad_revision' ORDER BY sequence LIMIT 4096)
            SELECT sequence,event,kind,occurred,substr(body,1,12000),length(body) FROM bounded'''
        for sequence, event, kind, occurred, payload, total in db.execute(query, (session,cursor,last)):
            truncated = total > 12000
            if not truncated:
                try:
                    payload = json.loads(payload)
                except ValueError:
                    pass
            piece = None
            request = None
            if kind == 'provider_event' and isinstance(payload, dict) and isinstance(payload.get('request_event'),str):
                request = payload.get('request_event')
                try:
                    data = json.loads(payload.get('data',''))
                    choices = data.get('choices',[])
                    if len(choices) == 1:
                        delta = choices[0].get('delta',{})
                        if (isinstance(delta,dict) and delta and
                            set(delta) <= {'role','content','reasoning_content','reasoning'} and
                            all(isinstance(v,str) or v is None for v in delta.values()) and
                            choices[0].get('finish_reason') is None):
                            piece = {k:v for k,v in delta.items() if k != 'role' and v}
                except (ValueError,TypeError,AttributeError):
                    pass
            entry = dict(sequence=sequence,event=event,kind=kind,occurred=occurred,
                         payload=payload,payload_truncated=truncated)
            merge = (piece is not None and events and
                     events[-1].get('stream_request') == request and
                     events[-1]['kind'] == 'assembled_provider_text')
            cost = len(json.dumps(piece if merge else entry,ensure_ascii=False).encode())
            if events and size + cost > MAX_EVIDENCE_BYTES:
                break
            if merge:
                previous = events[-1]
                for key,value in piece.items():
                    previous['payload'][key] = previous['payload'].get(key,'') + value
                previous['last_event'] = event
                previous['last_sequence'] = sequence
                previous['last_occurred'] = occurred
                previous['source_event_count'] += 1
            elif piece is not None:
                entry.update(kind='assembled_provider_text',payload=piece,
                             stream_request=request,last_event=event,last_sequence=sequence,
                             last_occurred=occurred,source_event_count=1,
                             provenance='Verbatim text assembled from this consecutive immutable event range.')
                events.append(entry)
            else:
                events.append(entry)
            size += cost
            cursor = sequence
            read_count += 1
        if read_count == 0:
            cursor = last
    if objective_size <= 8000:
        try:
            objective = json.loads(objective)
        except ValueError:
            pass
    return dict(session=session,run=run,state=state,events=events,
                objective=dict(event=objective_id,occurred=objective_time,
                               payload=objective,payload_truncated=objective_size>8000),
                next_after=cursor,through=last,complete=cursor>=last,bounded_excerpt=True)


def recent_findings(archive, scope):
    with readonly(archive) as db:
        rows = db.execute('''SELECT substr(json_extract(e.payload,'$.text'),1,24000)
            FROM echo_events e WHERE e.kind='scratchpad_revision'
            AND json_extract(e.payload,'$.principal')=?1
            AND substr(json_extract(e.payload,'$.name'),1,13)='self-monitor:'
            AND json_extract(e.payload,'$.deleted')=0
            AND NOT EXISTS(SELECT 1 FROM echo_events n WHERE n.kind='scratchpad_revision'
                AND n.sequence>e.sequence AND json_extract(n.payload,'$.principal')=?1
                AND json_extract(n.payload,'$.name')=json_extract(e.payload,'$.name'))
            ORDER BY e.sequence DESC LIMIT 4''',(scope,)).fetchall()
    summaries = []
    for (text,) in rows:
        try:
            prior = json.loads(text.split('\n',1)[1])
            for finding in prior['findings'][:3]:
                summaries.append({'title':str(finding['title'])[:160],
                                  'observation':str(finding['observation'])[:400]})
        except (ValueError,TypeError,KeyError,IndexError):
            continue
    return summaries


def redact(value, secrets):
    """Credential scrubbing is separate from model-based issue assessment."""
    if isinstance(value, dict):
        return {k: '[REDACTED]' if k.lower() in
                {'password', 'passwd', 'api_key', 'access_token', 'auth_token', 'secret_key'}
                else redact(v, secrets) for k, v in value.items()}
    if isinstance(value, list):
        return [redact(v, secrets) for v in value]
    if not isinstance(value, str):
        return value
    for secret in secrets:
        value = value.replace(secret, '[REDACTED]')
    value = re.sub(r'(?i)(Bearer|Basic)\s+[A-Za-z0-9_./+=-]{12,}', '[REDACTED]', value)
    value = re.sub(r'(?i)(api[_-]?key|access[_-]?token|auth[_-]?token|password|passwd|secret[_-]?key)\s*[=:]\s*["\']?[^\s"\',;<>]+', '[REDACTED]', value)
    return value


def review(bridge, archive, run, after, port, secret_files, report_dir):
    scope = bridge_call(bridge, archive, 'scope', run)
    if scope.get('status') != 'authorized':
        raise PermissionError('source authority unavailable')
    # Fail closed when required credential sources cannot be read.
    secrets = [Path(path).read_text().strip() for path in secret_files]
    if not secrets or any(len(secret) < 8 for secret in secrets):
        raise ValueError('credential redaction is not configured')
    page = evidence_page(archive, scope['session'], run, after)
    if not page['events']:
        return dict(status='empty', next_after=page['next_after'], complete=page['complete'])
    page['previous_findings_in_same_scope'] = recent_findings(archive, scope['scope'])
    clean = redact(page, secrets)
    prompt = Path(__file__).with_name('monitor_prompt.txt').read_text()
    body = dict(stream=False, echo_background=True, cache_prompt=False, max_tokens=4096,
                response_format={"type": "json_object"},
                messages=[dict(role='system', content=prompt),
                          dict(role='user', content=json.dumps(clean, ensure_ascii=False))])
    request = urllib.request.Request(f'http://127.0.0.1:{port}/v1/chat/completions',
                                     data=json.dumps(body).encode(),
                                     headers={'Content-Type': 'application/json'})
    with urllib.request.urlopen(request, timeout=180) as response:
        data = response.read(262145)
    if len(data) > 262144:
        raise ValueError('oversize model response')
    result = json.loads(data)
    choice = result['choices'][0]
    if choice.get('finish_reason') != 'stop':
        raise ValueError('incomplete model review')
    raw = choice['message']['content']
    citations = {page['objective']['event']}
    for event in page['events']:
        citations.add(event['event'])
        if 'last_event' in event:
            citations.add(event['last_event'])
    report = validate_report(raw, citations)
    safe_report = redact(report, secrets)
    if safe_report != report:
        raise ValueError('report contains credential material')
    # Revocation during generation prevents publication; scope cannot be chosen
    # by the model, a public admin or a configuration override.
    if bridge_call(bridge, archive, 'scope', run) != scope:
        raise PermissionError('source authority changed')
    key = hashlib.sha256(json.dumps([str(Path(archive).resolve()), run,
                                   [[e['event'], e.get('last_event')] for e in page['events']]],
                                  separators=(',', ':')).encode()).hexdigest()
    report_dir = Path(report_dir)
    if report_dir.stat().st_mode & 0o077:
        raise PermissionError('report directory must be private')
    if len(list(itertools.islice(report_dir.glob('review-*.json'), 32))) >= 32:
        raise RuntimeError('temporary report capacity reached')
    fd, path = tempfile.mkstemp(prefix='review-', suffix='.json', dir=report_dir)
    try:
        with os.fdopen(fd, 'w') as output:
            json.dump(report, output, ensure_ascii=False)
        published = bridge_call(bridge, archive, 'publish', run, key, path)
    finally:
        os.unlink(path)
    if published.get('status') not in ('saved', 'already-recorded', 'no-findings'):
        raise ValueError('publication rejected')
    return dict(status=published['status'], next_after=page['next_after'],
                complete=page['complete'], findings=len(report['findings']))


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--bridge', required=True)
    parser.add_argument('--archive', required=True)
    parser.add_argument('--run', required=True)
    parser.add_argument('--after', type=int, default=0)
    parser.add_argument('--port', type=int, default=8080)
    parser.add_argument('--secret-file', action='append', required=True)
    parser.add_argument('--report-dir', required=True)
    args = parser.parse_args()
    if not 1 <= args.port <= 65535 or args.after < 0:
        parser.error('invalid port or cursor')
    print(json.dumps(review(args.bridge, args.archive, args.run, args.after,
                            args.port, args.secret_file, args.report_dir)))


if __name__ == '__main__':
    main()
