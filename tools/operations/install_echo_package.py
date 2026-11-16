"""Run this prepared release only after reviewing package.json. No credentials logged."""
from pathlib import Path
import hashlib,json,os,re,sqlite3,subprocess,sys,time,urllib.request
stage=Path(__file__).resolve().parent
root=Path('/Library/Application Support/EchoRSI')
spec=json.loads((stage/'package.json').read_text())
services=['web','agent','activity','queue_controls','gateway','actions','interaction_ack','rsi-controller']
runtime=Path(spec['runtime'])
archives=[runtime/'data/mlx-browser-001.sqlite',runtime/'data/discord-integration-20261020-123419/agent.sqlite']
journal=runtime/'data/discord-integration-20261020-123419/journal.sqlite'
allowed_backlog=set(spec['preserved_stopped_runs'])
def sha(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def inspect_once():
 for path in archives:
  with sqlite3.connect('file:'+str(path)+'?mode=ro',uri=True,timeout=5) as c:
   for run,state in c.execute("SELECT run,state FROM echo_runs WHERE state NOT IN ('completed','cancelled','failed')"):
    if state!='ready' or run not in allowed_backlog:raise RuntimeError('A conversation is active; retry after it finishes.')
   if c.execute("SELECT count(*) FROM echo_tool_calls WHERE state IN ('queued','running','uncertain')").fetchone()[0]:raise RuntimeError('Tool work is pending.')
 with sqlite3.connect('file:'+str(journal)+'?mode=ro',uri=True,timeout=5) as c:
  if c.execute("SELECT count(*) FROM discord_agent_jobs WHERE state='claimed'").fetchone()[0]:raise RuntimeError('Discord has claimed work.')
  if c.execute("SELECT count(*) FROM discord_outbox WHERE state='sending'").fetchone()[0]:raise RuntimeError('Discord delivery is in flight.')
 for port in [8080,8084]:
  with urllib.request.urlopen(f'http://127.0.0.1:{port}/slots',timeout=15) as f:
   if any(x.get('is_processing') for x in json.load(f)):raise RuntimeError('Inference is active.')
 # Inspect socket state without lsof's process/filesystem traversal, which
 # can hang on a stalled evidence volume even when speech is idle.
 speech=subprocess.run(['/usr/sbin/netstat','-an','-p','tcp'],capture_output=True,text=True,timeout=5)
 if speech.returncode:raise RuntimeError('Could not inspect speech activity.')
 for line in speech.stdout.splitlines():
  fields=line.split()
  if len(fields)>=6 and fields[5]=='ESTABLISHED' and any(address.endswith('.49787') for address in fields[3:5]):
   raise RuntimeError('Speech is active.')
def inspect():
 deadline=time.monotonic()+45
 while True:
  try:return inspect_once()
  except RuntimeError as error:
   if str(error)!='Discord delivery is in flight.' or time.monotonic()>=deadline:raise
   time.sleep(.5)
def health(include_tools=False):
 result={}
 for name in services+(['tool-registry'] if include_tools else []):
  p=subprocess.run(['/bin/launchctl','print','system/local.echo.'+name],capture_output=True,text=True,timeout=10)
  match=re.search(r'^\s*pid = (\d+)$',p.stdout,re.M)
  if p.returncode or not match or not re.search(r'^\s*state = running$',p.stdout,re.M):raise RuntimeError('Service not running: '+name)
  result[name]=int(match[1])
 with urllib.request.urlopen('http://127.0.0.1:49675/health',timeout=15) as f:
  if json.load(f).get('status')!='ready':raise RuntimeError('Web health failed.')
 return result
if sha(root/'releases/active.json')!=spec['active_sha256']:raise RuntimeError('Installed release changed; prepare against its current manifest.')
for rel,pin in spec['files'].items():
 if sha(stage/'files'/rel)!=pin:raise RuntimeError('Prepared file changed: '+rel)
if sha(stage/'install-root.py')!=spec['installer_sha256']:raise RuntimeError('Installer changed.')
if spec.get('tool_installation'):
 if sha(stage/'tool_installation.py')!=spec['tool_installation']['module_sha256']:raise RuntimeError('Tool installer changed.')
 fixture=stage/'tool-fixture'
 if sha(fixture/'qualify-tool-registration.py')!=spec['tool_installation']['fixture_installer_sha256']:raise RuntimeError('Fixture installer changed.')
 subprocess.run([sys.executable,str(fixture/'qualify-tool-registration.py'),str(fixture/'package.json'),'--sha256',spec['tool_installation']['fixture_manifest_sha256'],'--check-only'],check=True)
if spec.get('research_catalog'):
 if sha(stage/'research-catalog.json')!=spec['research_catalog']['sha256']:raise RuntimeError('Research catalogue changed.')
 if sha(stage/'research_installation.py')!=spec['research_catalog']['module_sha256']:raise RuntimeError('Research installer changed.')
if spec.get('mail_installation'):
 if sha(stage/'gmail_installation.py')!=spec['mail_installation']['module_sha256']:raise RuntimeError('Mailbox installer changed.')
inspect();health()
if '--check-only' in sys.argv:
 print('Prepared files verified; current services healthy; no inference, tool, claimed Discord or speech work. Stopped backlog is preserved.');sys.exit(0)
# The root-owned Echo helper retains acceptance/rollback and never opens a password prompt.
helper='/Library/PrivilegedHelperTools/local.echo.admin'
if not Path(helper).is_file():raise RuntimeError('Install the Echo administration helper once before deployment.')
p=subprocess.Popen(['/usr/bin/sudo','-n',helper,('deploy-owner' if spec.get('owner_source_pin_update') else 'deploy'),str(stage)])
handled=set()
while p.poll() is None:
 progress=stage/'progress.json'
 if progress.exists():
  data=json.loads(progress.read_text());phase=data['phase']
  if phase not in handled and phase in ['setup-complete','restored','prepared','quiescent','stopped','started','resumed']:
   decision='proceed'
   try:
    inspect()
    if phase!='stopped':
     include_tools=bool(spec.get('tool_installation')) and phase in ['started','resumed']
     first=health(include_tools);time.sleep(1)
     if health(include_tools)!=first:raise RuntimeError('Service restarted during health observation.')
   except Exception as error:
    decision='abort';print(type(error).__name__+': '+str(error),flush=True)
   tmp=stage/'decision.tmp';tmp.write_text(json.dumps({'phase':phase,'decision':decision}));os.replace(tmp,stage/'decision.json');handled.add(phase)
 time.sleep(.2)
if p.returncode:raise SystemExit(p.returncode)
result=json.loads((stage/'progress.json').read_text())
if result['phase']!='complete':raise RuntimeError('Installation did not complete.')
inspect();result['services']=health(bool(spec.get('tool_installation')));result['idle_verified']=True
result['stopped_backlog_preserved']=True
result['model_and_speech_services_restarted']=False
(stage/'deployment-result.json').write_text(json.dumps(result,indent=2))
print('Application release installed and healthy; no active inference/tool/speech work; stopped backlog preserved.')
