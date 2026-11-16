#!/usr/bin/env python3
"""Idle-only monitor coordinator. Never resumes or mutates conversation runs."""
import argparse
import fcntl
import json
import logging
from logging.handlers import RotatingFileHandler
import os
from pathlib import Path
import sqlite3
import time
import sys
import urllib.request

sys.path.insert(0, str(Path(__file__).resolve().parent))
from review import readonly, review


def initialise(path):
    db = sqlite3.connect(path, timeout=1)
    db.execute('PRAGMA journal_mode=DELETE')
    db.execute('PRAGMA max_page_count=2048')  # 8 MiB at the default 4 KiB page size.
    db.executescript('''
        CREATE TABLE IF NOT EXISTS cursor(archive TEXT PRIMARY KEY, sequence INTEGER NOT NULL);
        CREATE TABLE IF NOT EXISTS pending(archive TEXT, run TEXT, terminal INTEGER,
            after_sequence INTEGER NOT NULL DEFAULT 0, retry_after REAL NOT NULL DEFAULT 0,
            failures INTEGER NOT NULL DEFAULT 0, PRIMARY KEY(archive,run,terminal));
        CREATE TABLE IF NOT EXISTS budget(day TEXT PRIMARY KEY, reports INTEGER NOT NULL);
    ''')
    db.commit()
    return db


def discover(state, archive):
    path = str(Path(archive).resolve())
    with readonly(archive) as source:
        prior = state.execute('SELECT sequence FROM cursor WHERE archive=?', (path,)).fetchone()
        if prior is None:
            # Start with future work. Installation never replays historical tests.
            latest = source.execute('SELECT COALESCE(MAX(sequence),0) FROM echo_events').fetchone()[0]
            state.execute('INSERT INTO cursor VALUES(?,?)', (path, latest))
            state.commit()
            return
        available = max(0, 128 - state.execute('SELECT COUNT(*) FROM pending').fetchone()[0])
        if not available:
            return
        rows = source.execute('''SELECT e.sequence,r.run FROM echo_events e
            JOIN echo_runs r ON r.last_event=e.event
            WHERE e.sequence>? AND e.kind IN ('assistant_final','agent_failure','agent_cancelled')
            AND r.state IN ('completed','failed','cancelled')
            ORDER BY e.sequence LIMIT ?''', (prior[0], min(32, available))).fetchall()
    for sequence, run in rows:
        state.execute('INSERT OR IGNORE INTO pending(archive,run,terminal) VALUES(?,?,?)',
                      (path, run, sequence))
        state.execute('UPDATE cursor SET sequence=? WHERE archive=?', (sequence, path))
    state.commit()


def idle(config):
    # Fail closed on unavailable health/archives. Calls are localhost only.
    if config.get('maintenance') and Path(config['maintenance']).exists():
        return False
    for port in config['ports']:
        with urllib.request.urlopen(f'http://127.0.0.1:{port}/slots', timeout=1) as response:
            slots = json.load(response)
        if not slots or any(slot['is_processing'] for slot in slots):
            return False
    for archive in config['archives']:
        with readonly(archive) as db:
            if db.execute("SELECT 1 FROM echo_runs WHERE state IN ('generating','tools') LIMIT 1").fetchone():
                return False
            if db.execute("SELECT 1 FROM echo_tool_calls WHERE run IN (SELECT run FROM echo_runs WHERE state NOT IN ('completed','failed','cancelled')) AND state IN ('queued','running','uncertain') LIMIT 1").fetchone():
                return False
    return True


def tick(state, config, directory):
    if not config.get('enabled', False) or not idle(config):
        return 'deferred'
    for archive in config['archives']:
        discover(state, archive)
    day = time.strftime('%Y-%m-%d', time.gmtime())
    budget = state.execute('SELECT reports FROM budget WHERE day=?', (day,)).fetchone()
    # At most 384 KiB/day of findings, without copying source sessions/logs.
    if budget and budget[0] >= 16:
        return 'daily-budget'
    row = state.execute('''SELECT archive,run,terminal,after_sequence FROM pending
        WHERE retry_after<=? ORDER BY terminal LIMIT 1''', (time.time(),)).fetchone()
    if row is None:
        return 'idle'
    archive, run, terminal, after = row
    try:
        result = review(config['bridge'], archive, run, after, config['model_port'],
                        config['secret_files'], directory)
        if result['status'] == 'saved':
            state.execute('INSERT INTO budget VALUES(?,1) ON CONFLICT(day) DO UPDATE SET reports=reports+1', (day,))
        if result['complete']:
            state.execute('DELETE FROM pending WHERE archive=? AND run=? AND terminal=?', row[:3])
        else:
            state.execute('UPDATE pending SET after_sequence=?,failures=0,retry_after=? WHERE archive=? AND run=? AND terminal=?',
                          (result['next_after'], time.time()+60, *row[:3]))
        state.execute('DELETE FROM budget WHERE day < date(?,\'-7 days\')', (day,))
        state.commit()
        return result['status']
    except PermissionError:
        # Revocation is respected; it is not classified as a defect or retried.
        state.execute('DELETE FROM pending WHERE archive=? AND run=? AND terminal=?', row[:3])
        state.commit()
        return 'authority-unavailable'
    except Exception as problem:
        logging.getLogger("echo.self_monitor").exception("Review deferred")
        # No raw exception/model/session output enters global service logs.
        state.execute('''UPDATE pending SET failures=failures+1,
            retry_after=? + MIN(3600,60 * (failures+1))
            WHERE archive=? AND run=? AND terminal=?''', (time.time(), *row[:3]))
        state.commit()
        return 'review-deferred:' + type(problem).__name__


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--config', required=True)
    parser.add_argument('--state-directory', required=True)
    parser.add_argument('--once', action='store_true')
    args = parser.parse_args()
    os.umask(0o077)
    directory = Path(args.state_directory)
    directory.mkdir(mode=0o700, parents=False, exist_ok=True)
    if directory.stat().st_mode & 0o077:
        raise PermissionError('monitor state directory must be private')
    if (directory/'errors.log').is_symlink():
        raise PermissionError('invalid monitor log path')
    logger = logging.getLogger('echo.self_monitor')
    logger.addHandler(RotatingFileHandler(directory/'errors.log',maxBytes=65536,backupCount=1))
    logger.setLevel(logging.ERROR)
    logger.propagate = False
    with (directory/'worker.lock').open('a') as lock:
        fcntl.flock(lock, fcntl.LOCK_EX | fcntl.LOCK_NB)
        state = initialise(directory/'state.sqlite')
        while True:
            try:
                config = json.loads(Path(args.config).read_text())
                ports = [*config['ports'], config['model_port']]
                if any(type(p) is not int or not 1 <= p <= 65535 for p in ports):
                    raise ValueError('invalid local port')
                status = tick(state, config, directory)
            except Exception as problem:
                logger.exception('Monitor deferred')
                status = 'monitor-unavailable:' + type(problem).__name__
            # No periodic disk log or receipt spam. Status changes only.
            if status != locals().get('previous'):
                value = {'monitor':status,'updated_unix':time.time()}
                temporary = directory/'status.tmp'
                temporary.write_text(json.dumps(value))
                temporary.replace(directory/'status.json')
                print(json.dumps(value), flush=True)
            previous = status
            if args.once:
                return
            time.sleep(60)


if __name__ == '__main__':
    main()
