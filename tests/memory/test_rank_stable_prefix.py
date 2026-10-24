"""Exercise the production prefix query with changing recall rankings."""
from pathlib import Path
import json
import re
import sqlite3

root = Path(__file__).resolve().parents[2]
source = (root / 'app/memory/context_snapshot.ep').read_text()
scope = json.loads(re.search(r'return (".*")', (root / 'app/archive/session_scope.ep').read_text()).group(1))
query = scope + json.loads(re.search(r'set prior_query to concat\(session_scope_sql\(\) and (".*")\)', source).group(1))
db = sqlite3.connect(':memory:')
db.execute('CREATE TABLE echo_events(sequence INTEGER PRIMARY KEY,session TEXT,event TEXT,kind TEXT,payload TEXT,digest TEXT)')
record = dict(identity='claim-1', quotation='retained exact passage', source_event='source-1', source_digest='digest-1', runtime_status='current')
eligible = dict(record, quotation='Before. retained exact passage After.')
payload = dict(run_id='first', revision='revision-1', base='selected recall, retained verbatim', records=[record])
db.execute('INSERT INTO echo_events VALUES(1,?,?,?,?,?)', ('session-1','snapshot-1','memory_context_snapshot',json.dumps(payload),'snapshot-digest'))
def read(records, session='session-1', revision='revision-1'):
    return db.execute(query, (session,revision,'second',json.dumps(records))).fetchone()
assert read([eligible]) == (payload['base'],), 'A changed semantic ranking must retain an authorised previously selected passage'
assert read([]) is None, 'Revoked/removed sources must invalidate the prefix'
for field in ('source_event','source_digest','runtime_status'):
    assert read([dict(eligible, **{field:'changed'})]) is None, field
assert read([dict(eligible, quotation='different words')]) is None
assert read([eligible], revision='revision-2') is None
assert read([eligible], session='sibling') is None
# Verified branch ancestry can retain only snapshots before its endpoint.
db.execute('INSERT INTO echo_events VALUES(2,?,?,?,?,?)', ('branch','branch-event','session_branch',json.dumps(dict(source='session-1',endpoint='snapshot-1',digest='snapshot-digest',sequence='1',mode='fork')),'branch-digest'))
assert read([eligible], session='branch') == (payload['base'],)
db.execute('UPDATE echo_events SET sequence=3 WHERE event="snapshot-1"')
assert read([eligible], session='branch') is None
print('PASS: rank changes retain exact authorised passages; revoked, altered, sibling, revision and ancestry-boundary cases reject retention')
