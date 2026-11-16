from pathlib import Path
import re,subprocess,sys,tempfile,json,os
p=Path(__file__).resolve().parent
s=Path(sys.argv[1]).read_text().replace('int main(int argc, char** argv) {','int original_main(int argc, char** argv) {',1)
for name in ['agent_loop_inner_provider_managed','agent_memory_prepare_parallel']:
 s,n=re.subn(r'long long '+name+r'(\([^;{}]*\) \{)',r'long long original_'+name+r'\1',s,count=1);assert n==1
s+=(p/'foreground_fixture.c').read_text()
with tempfile.TemporaryDirectory(prefix='echo-foreground-order-') as directory:
 root=Path(directory);(root/'test.c').write_text(s)
 with (root/'compile.log').open('w') as log:r=subprocess.run(['clang',str(root/'test.c'),'-o',str(root/'test'),'-lpthread','-DEP_HAS_SQLITE','-lsqlite3'],stdout=log,stderr=log)
 if r.returncode:raise RuntimeError((root/'compile.log').read_text())
 (root/'fixture-secret').write_text('fixture-secret-only-not-a-real-credential')
 env=dict(os.environ,ECHO_DISCORD_REDACTION_FILES=json.dumps([str(root/'fixture-secret')]))
 r=subprocess.run([str(root/'test'),str(root/'journal.sqlite'),str(root/'archive.sqlite')],capture_output=True,text=True,timeout=20,env=env)
 print(r.stdout,flush=True);print(r.stderr,flush=True)
 assert r.returncode==0,(r.returncode,r.stderr)
 print('PASS: reply release and follow-up claim precede extraction join; memory receipt persists')
