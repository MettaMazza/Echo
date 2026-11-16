from pathlib import Path
import subprocess,sqlite3,tempfile,fcntl,time,json,os
out=Path(__file__).resolve().parent
binary=Path(os.environ['ECHO_SNAPSHOT_TEST_BINARY'])
p=Path(tempfile.mkdtemp(prefix='echo-snapshot-regression-'));db=p/'agent.sqlite';dest=p/'snapshots';dest.mkdir()
def run(mode,directory=dest,expire=True):
 if expire and db.exists():
  with sqlite3.connect(db) as c:
   if c.execute("select 1 from sqlite_master where name='echo_snapshot_retention'").fetchone():c.execute('update echo_snapshot_retention set completed_ms=completed_ms-3600001')
 return subprocess.run([str(binary),str(db),str(directory),mode],capture_output=True,text=True,timeout=30)
assert run('seed').returncode==0
lock=os.open(str(db)+'.runtime.lock.snapshot',os.O_RDWR|os.O_CREAT,0o600);fcntl.flock(lock,fcntl.LOCK_EX)
children=[subprocess.Popen([str(binary),str(db),str(dest),'snapshot'],stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True) for _ in range(4)]
children.append(subprocess.Popen([str(binary),str(db),str(dest),'late'],stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True))
end=time.monotonic()+5
while time.monotonic()<end:
 with sqlite3.connect(db) as c:n=c.execute('select count(*) from echo_events').fetchone()[0]
 with sqlite3.connect(db) as c:
  try:tickets=c.execute('select generation from echo_snapshot_requests').fetchone()[0]
  except sqlite3.OperationalError:tickets=0
 if n==2 and tickets==5:break
 time.sleep(.05)
assert n==2 and tickets==5 and all(x.poll() is None for x in children)
fcntl.flock(lock,fcntl.LOCK_UN);os.close(lock)
for x in children:
 stdout,stderr=x.communicate(timeout=30);assert x.returncode==0,(x.returncode,stdout,stderr)
assert len(list(dest.glob('*.sqlite')))==1
assert run('next').returncode==0
assert len(list(dest.glob('*.sqlite')))==2
with sqlite3.connect(db) as c:before=c.execute('select covered_sequence,path from echo_snapshot_progress').fetchone()
assert run('failed-destination',p/'absent').returncode!=0
with sqlite3.connect(db) as c:assert c.execute('select covered_sequence,path from echo_snapshot_progress').fetchone()==before
assert run('snapshot').returncode==0
with sqlite3.connect(db) as c:head=c.execute('select max(sequence) from echo_events').fetchone()[0];coverage,path=c.execute('select covered_sequence,path from echo_snapshot_progress').fetchone()
with sqlite3.connect(path) as c:
 assert coverage==head==c.execute('select last_sequence from echo_snapshot_receipt').fetchone()[0]
 assert c.execute('pragma integrity_check').fetchone()[0]=='ok'
with sqlite3.connect(db) as c:c.execute('create table control_state(value text)');c.execute("insert into control_state values('preserved')")
assert run('snapshot').returncode==0
with sqlite3.connect(db) as c:path=c.execute('select path from echo_snapshot_progress').fetchone()[0]
with sqlite3.connect(path) as c:assert c.execute('select value from control_state').fetchone()[0]=='preserved'
# Simulate no capacity through the same native statfs64 boundary, without filling a disk.
shim=dest/'disk-full.c';shim.write_text('#include <stdint.h>\n#include <string.h>\nint statfs64(const char *path, void *out) { memset(out,0,2168); ((uint32_t*)out)[0]=4096; return 0; }\n')
subprocess.run(['/usr/bin/clang','-dynamiclib',str(shim),'-o',str(dest/'disk-full.dylib')],check=True,capture_output=True)
with sqlite3.connect(db) as c:before=c.execute('select * from echo_snapshot_progress').fetchone();ticket=c.execute('select generation from echo_snapshot_requests').fetchone()[0]
count=len(list(dest.glob('*.sqlite')))
assert run('low-capacity').returncode==0
assert len(list(dest.glob('*.sqlite')))==count
with sqlite3.connect(db) as c:
 assert c.execute('select * from echo_snapshot_progress').fetchone()==before
 assert c.execute('select generation from echo_snapshot_requests').fetchone()[0]==ticket+1
 assert c.execute('select reason,available_bytes from echo_snapshot_deferrals').fetchone()==('insufficient-capacity',0)
assert run('snapshot').returncode==0
with sqlite3.connect(db) as c:
 assert c.execute('select count(*) from echo_snapshot_deferrals').fetchone()[0]==0
 assert c.execute('select covered_request from echo_snapshot_progress').fetchone()[0]==ticket+2
with sqlite3.connect(db) as c:before=c.execute('select * from echo_snapshot_progress').fetchone();ticket=c.execute('select generation from echo_snapshot_requests').fetchone()[0]
count=len(list(dest.glob('*.sqlite')))
assert run('snapshot',expire=False).returncode==0
assert len(list(dest.glob('*.sqlite')))==count
with sqlite3.connect(db) as c:
 assert c.execute('select * from echo_snapshot_progress').fetchone()==before
 assert c.execute('select generation from echo_snapshot_requests').fetchone()[0]==ticket+1
 assert c.execute('select reason from echo_snapshot_deferrals').fetchone()[0]=='interval'
# Verification failure after VACUUM must remove its partial candidate.
with sqlite3.connect(db) as c:
 c.execute('create table fixture_parent(id integer primary key)')
 c.execute('create table fixture_child(id integer references fixture_parent(id))')
 c.execute('insert into fixture_child values(99)')
 before=c.execute('select * from echo_snapshot_progress').fetchone()
count=len(list(dest.glob('*.sqlite')))
assert run('snapshot').returncode!=0
assert len(list(dest.glob('*.sqlite')))==count
with sqlite3.connect(db) as c:
 assert c.execute('select * from echo_snapshot_progress').fetchone()==before
 c.execute('drop table fixture_child');c.execute('drop table fixture_parent')
assert run('snapshot').returncode==0
# Native retention is bounded and only deletes its registered generated names.
import uuid,shutil
with sqlite3.connect(db) as c:
 c.execute('delete from echo_snapshot_retention')
 candidates=[]
 for i in range(14):
  f=dest/('agent-'+str(uuid.uuid4())+'.sqlite');shutil.copyfile(path,f);side=dest/('._'+f.name);side.write_bytes(b'metadata');stamp=1000000000000+(i//2)*86400000+i
  c.execute('insert into echo_snapshot_retention values(?,?)',(str(f),stamp));candidates.append((f,stamp))
 unrelated=dest/'keep.txt';unrelated.write_text('keep')
assert run('prune',expire=False).returncode==0
with sqlite3.connect(db) as c:
 kept={x[0] for x in c.execute('select path from echo_snapshot_retention')};assert len(kept)<=10
 for f,stamp in candidates:
  assert f.exists()==(str(f) in kept)
  assert (dest/('._'+f.name)).exists()==(str(f) in kept)
 outside=p/'outside.sqlite';outside.write_text('never delete')
 c.execute('insert into echo_snapshot_retention values(?,?)',(str(outside),1))
assert run('prune',expire=False).returncode!=0
assert outside.read_text()=='never delete' and unrelated.read_text()=='keep'
result={'failed_candidate_removed':True,'interval_preserves_pending_ticket':True,'bounded_retention':True,'unrelated_files_preserved':True,'low_capacity_did_not_copy':True,'low_capacity_preserved_pending_request':True,'capacity_recovery_verified':True,'non_event_state_preserved':True,'concurrent_workers':5,'snapshots_for_overlapping_requests':1,'new_committed_events_backed_up':True,'failed_backup_did_not_advance_coverage':True,'failed_backup_recovered':True,'verified_snapshot_receipt':True,'fixture':str(p)}
print(json.dumps(result))
