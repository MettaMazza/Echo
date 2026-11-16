"""Durable, public-safe deployment notices, plus an explicit operator command.

Imported by the existing installer. This does not read conversation content or
expose protected review reasoning. Model-visible summaries contain only the
operator-supplied public line, actor, category, timestamp and version.
"""
from pathlib import Path
import argparse, datetime, fcntl, hashlib, json, os, tempfile

DEFAULT = Path('/Library/Application Support/EchoRSI/notices')
KINDS = {'deployment','revert','prompt','policy','routing','toolset','operator'}

def append_notice(kind, summary, version, actor='operator', directory=DEFAULT, event_id=None, occurred=None):
    directory = Path(directory)
    if kind not in KINDS or not all(isinstance(x,str) and x and len(x)<=500 and '\n' not in x and '\r' not in x for x in (summary,version,actor)):
        raise ValueError('Notices require a category and bounded single-line public summary, actor and version')
    directory.mkdir(mode=0o755,parents=True,exist_ok=True)
    if directory.is_symlink():raise ValueError('Indirect notice directory')
    lock=os.open(directory/'notice.lock',os.O_RDWR|os.O_CREAT|os.O_NOFOLLOW,0o600)
    try:
        fcntl.flock(lock,fcntl.LOCK_EX)
        log=directory/'events.jsonl';latest=directory/'latest.json'
        records=[];previous=''
        if log.exists():
            if log.is_symlink():raise ValueError('Indirect notice log')
            with log.open() as stream:
                for line in stream:
                    if not line.strip():continue
                    row=json.loads(line);saved=row.pop('digest')
                    if row.get('previous')!=previous or hashlib.sha256(json.dumps(row,sort_keys=True,separators=(',',':')).encode()).hexdigest()!=saved:raise ValueError('Notice log integrity failure')
                    row['digest']=saved;previous=saved;records.append(row)
        duplicate=next((r for r in records if event_id and r.get('event_id')==event_id),None)
        if duplicate:
            row=duplicate
        else:
            row={'sequence':records[-1]['sequence']+1 if records else 1,'at':occurred or datetime.datetime.now(datetime.timezone.utc).isoformat(),'kind':kind,'summary':summary,'version':version,'actor':actor,'event_id':event_id or '', 'previous':previous}
            row['digest']=hashlib.sha256(json.dumps(row,sort_keys=True,separators=(',',':')).encode()).hexdigest()
            fd=os.open(log,os.O_WRONLY|os.O_APPEND|os.O_CREAT|os.O_NOFOLLOW,0o600)
            with os.fdopen(fd,'a') as out:
                out.write(json.dumps(row,separators=(',',':'))+'\n');out.flush();os.fsync(out.fileno())
            records.append(row)
        # Also repair the derived tail after a crash between log and tail writes.
        fd,name=tempfile.mkstemp(prefix='.notice-',dir=directory)
        try:
            with os.fdopen(fd,'w') as out:
                json.dump(records[-256:],out,separators=(',',':'));out.flush();os.fsync(out.fileno())
            os.chmod(name,0o644);os.replace(name,latest)
            directory_fd=os.open(directory,os.O_RDONLY)
            try:os.fsync(directory_fd)
            finally:os.close(directory_fd)
        finally:
            if os.path.exists(name):os.unlink(name)
        return row
    finally:os.close(lock)

def project_ledger(ledger, directory=DEFAULT):
    """Read only terminal effect metadata. Never select private review text."""
    import sqlite3,re
    directory=Path(directory);directory.mkdir(mode=0o755,parents=True,exist_ok=True)
    cursor_path=directory/'ledger-cursor.json'
    cursor=json.loads(cursor_path.read_text()).get('sequence',0) if cursor_path.exists() else 0
    with sqlite3.connect('file:'+str(ledger)+'?mode=ro',uri=True,timeout=.15) as db:
        rows=db.execute("SELECT t.sequence,t.event,t.occurred,json_extract(t.payload,'$.phase'),json_extract(p.payload,'$.after_sha256'),json_extract(p.payload,'$.before_sha256') FROM echo_events t JOIN echo_events p ON p.session=t.session AND p.kind='protected_operation_prepared' WHERE t.sequence>? AND t.kind='protected_operation_transition' AND json_extract(t.payload,'$.phase') IN ('applied','rolled-back') ORDER BY t.sequence LIMIT 64",(cursor,)).fetchall()
    for seq,event,at,phase,after,before in rows:
        version=after if phase=='applied' else before
        if not isinstance(version,str) or not re.fullmatch('[0-9a-f]{64}',version):raise ValueError('Invalid controller effect digest')
        if at.startswith('unix-ms:'):at=datetime.datetime.fromtimestamp(int(at[8:])/1000,datetime.timezone.utc).isoformat()
        append_notice('deployment' if phase=='applied' else 'revert','Protected controller change '+phase+'.',version,actor='protected-controller',directory=directory,event_id='controller:'+hashlib.sha256(event.encode()).hexdigest(),occurred=at)
        cursor=seq
    fd,name=tempfile.mkstemp(prefix='.cursor-',dir=directory)
    try:
        with os.fdopen(fd,'w') as f:json.dump({'sequence':cursor},f);f.flush();os.fsync(f.fileno())
        os.chmod(name,0o600);os.replace(name,cursor_path)
    finally:
        if os.path.exists(name):os.unlink(name)
    return len(rows)

def refresh_controller():
    import signal
    # Bounded local read; a contended ledger never blocks a conversation.
    signal.setitimer(signal.ITIMER_REAL,.75)
    root=Path('/Library/Application Support/EchoRSI')
    def trusted(path):
        path=Path(path);st=path.stat()
        if path.is_symlink() or st.st_uid!=0 or st.st_mode&0o022:raise ValueError('Untrusted controller configuration')
        return json.loads(path.read_text())
    pipeline=trusted(root/'config/pipeline.json')
    config=trusted(pipeline['controller_configuration'])
    ledger=Path(config['ledger_directory'])/config['ledger_name']
    if ledger.is_symlink() or ledger.stat().st_uid!=0 or ledger.stat().st_mode&0o077:raise ValueError('Unprotected controller ledger')
    return project_ledger(ledger)

def main():
    import sys
    if sys.argv[1:]==['--refresh-controller']:
        if os.geteuid()!=0:raise SystemExit('Administrator entry required')
        refresh_controller();return
    parser=argparse.ArgumentParser(description='Record a public-safe Echo change notice. Never put private conversation content in the summary.')
    parser.add_argument('--kind',required=True,choices=sorted(KINDS));parser.add_argument('--summary',required=True);parser.add_argument('--version',required=True);parser.add_argument('--actor',default='operator');parser.add_argument('--event-id')
    args=parser.parse_args()
    if os.geteuid()!=0:raise SystemExit('Use the scoped Echo administration helper: notice --kind ... --summary ... --version ...')
    print(json.dumps(append_notice(args.kind,args.summary,args.version,args.actor,event_id=args.event_id)))
if __name__=='__main__':main()
