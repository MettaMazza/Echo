"""Operator-authorized, hash-pinned Phase Two application and tool release; preserves installed acceptance and rollback."""
from pathlib import Path
import os,json,sys,hashlib,shutil,subprocess,time,re,plistlib,urllib.request
stage=Path(sys.argv[1]);root=Path('/Library/Application Support/EchoRSI');spec=json.loads((stage/'package.json').read_text());progress=stage/'progress.json';decision=stage/'decision.json'
assert os.geteuid()==0,'Administrator execution required'
sys.path.insert(0,str(Path(__file__).resolve().parent))
from qol_notice import append_notice
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def report(phase,**fields):
 p=stage/'progress.tmp';p.write_text(json.dumps(dict(phase=phase,**fields)));p.chmod(0o644);os.replace(p,progress)
def wait(expected):
 deadline=time.monotonic()+180
 while time.monotonic()<deadline:
  if decision.exists():
   x=json.loads(decision.read_text())
   if x.get('phase')==expected:
    if x.get('decision')!='proceed':raise RuntimeError('Operator preflight rejected '+expected)
    return
  time.sleep(.2)
 raise TimeoutError('Coordinator did not acknowledge '+expected)
def write(p,data,mode):
 p=Path(p);tmp=p.with_name(p.name+'.tmp');tmp.write_bytes(data);tmp.chmod(mode)
 with tmp.open('rb') as f:os.fsync(f.fileno())
 os.replace(tmp,p)
def service_status(label):
 p=subprocess.run(['launchctl','print','system/local.echo.'+label],capture_output=True,text=True,timeout=10)
 if p.returncode==113:return {'loaded':False,'pid':None,'running':False}
 if p.returncode!=0:raise RuntimeError('Cannot inspect service '+label)
 m=re.search(r'^\s*pid = (\d+)$',p.stdout,re.M)
 return {'loaded':True,'pid':int(m[1]) if m else None,'running':bool(re.search(r'^\s*state = running$',p.stdout,re.M))}
def state(label):return service_status(label)['pid']
def stop_service(label):
 if not service_status(label)['loaded']:return
 p=subprocess.run(['launchctl','bootout','system/local.echo.'+label],capture_output=True,text=True,timeout=60)
 if p.returncode!=0 and service_status(label)['loaded']:raise RuntimeError('Service bootout failed: '+label)
 deadline=time.monotonic()+60
 while service_status(label)['loaded'] and time.monotonic()<deadline:time.sleep(.1)
 if service_status(label)['loaded']:raise RuntimeError('Service did not unload: '+label)
def start_service(label):
 current=service_status(label)
 if not current['loaded']:
  subprocess.run(['launchctl','bootstrap','system',str(root/'launchd'/('local.echo.'+label+'.plist'))],check=True,capture_output=True,text=True,timeout=60)
 deadline=time.monotonic()+30
 while time.monotonic()<deadline:
  current=service_status(label)
  if current['running'] and current['pid']:return
  time.sleep(.1)
 raise RuntimeError('Service did not become ready: '+label)
def verify_services():
 before={label:service_status(label) for label in labels}
 if not all(item['running'] and item['pid'] for item in before.values()):raise RuntimeError('A service is not running')
 deadline=time.monotonic()+300
 while True:
  try:
   with urllib.request.urlopen('http://127.0.0.1:49675/health',timeout=5) as f:
    if json.load(f).get('status')=='ready':break
  except (OSError,ValueError):pass
  if time.monotonic()>=deadline:raise RuntimeError('Web health failed after startup grace period')
  time.sleep(.25)
 time.sleep(1)
 if {label:service_status(label) for label in labels}!=before:raise RuntimeError('Service restarted during health check')
active=root/'releases/active.json';oldbytes=active.read_bytes();assert hashlib.sha256(oldbytes).hexdigest()==spec['active_sha256'],'Active release changed'
old=json.loads(oldbytes);oldsource=Path(old['source_root']);oldrelease=oldsource.parent;release=root/'releases'/spec['release'];source=release/'source'
assert not release.exists(),'Candidate release already exists'
for rel,pin in spec['files'].items():assert digest(stage/'files'/rel)==pin,'Staged file changed'
services=[x['name'] for x in old['services']];labels=services+['rsi-controller'];stopped=False;activated=False;maintenance_owned=False;persistent_created=[]
qol_path=root/'notices/qol.json';qol_before=qol_path.read_bytes() if qol_path.exists() else None;qol_changed=False
research_environment={}
monitor_prepared=None;monitor_activated=False
mail_root=None
tool_prepared=None;tool_swapped=False;tool_previous=None;pipeline_previous=None;pipeline_changed=False;pipeline_next=None
tool_destination=Path('/Library/LaunchDaemons/local.echo.tool-registry.plist')
if tool_destination.exists():
 assert not tool_destination.is_symlink() and tool_destination.stat().st_uid==0 and not tool_destination.stat().st_mode & 0o022,'Unprotected tool service definition'
 tool_previous=tool_destination.read_bytes()
 previous_job=plistlib.loads(tool_previous)
 assert previous_job.get('Label')=='local.echo.tool-registry' and str(previous_job['ProgramArguments'][0]).startswith(str(root/'tools/releases')+'/'),'Different tool service already exists'
elif service_status('tool-registry')['loaded']:
 raise RuntimeError('Unmanaged tool service already loaded')
def start_tool():
 subprocess.run(['launchctl','bootstrap','system',str(tool_destination)],check=True,capture_output=True,text=True,timeout=60)
 deadline=time.monotonic()+30
 while time.monotonic()<deadline:
  current=service_status('tool-registry')
  if current['running'] and current['pid']:return
  time.sleep(.1)
 raise RuntimeError('Tool worker did not become ready')
try:
 marker=root/'maintenance'
 assert not marker.exists(),'Another maintenance operation is active'
 # Restore missing jobs first; keep intake held until deployment is ready.
 for label in labels:start_service(label)
 verify_services()
 # Commit installed controller startup independently of candidate deployment.
 for label in labels:
  original=root/'launchd'/('local.echo.'+label+'.plist');data=original.read_bytes();job=plistlib.loads(data)
  assert job['Label']=='local.echo.'+label and job.get('RunAtLoad') and job.get('KeepAlive'),'Unexpected service definition'
  executable=Path(job['ProgramArguments'][0]);assert executable in [root/'bin/rsi_service',root/'bin/rsi_controller'],'Unexpected privileged executable'
  assert executable.stat().st_uid==0 and not executable.stat().st_mode & 0o022,'Unprotected service executable'
  destination=Path('/Library/LaunchDaemons')/original.name
  if destination.exists():assert destination.read_bytes()==data,'Different persistent service already exists'
  else:
   write(destination,data,0o644);os.chown(destination,0,0);persistent_created.append(destination)

 report('setup-complete',persistent_services=labels);wait('setup-complete')
 report('restored');wait('restored')
 if spec.get('tool_installation'):
  assert digest(stage/'tool_installation.py')==spec['tool_installation']['module_sha256'],'Tool installer changed'
  from tool_installation import prepare_tool_installation
  tool_prepared=prepare_tool_installation(stage,spec,root,old)
 if spec.get('research_catalog'):
  assert tool_prepared is not None,'Research catalogue requires the prepared persistent-template installation'
  assert digest(stage/'research_installation.py')==spec['research_catalog']['module_sha256'],'Research installer changed'
  from research_installation import prepare_research_installation
  research_environment=prepare_research_installation(stage,spec,root,old)
 if spec.get('mail_installation'):
  assert tool_prepared is not None,'Mailbox profiles require persistent RSI template preparation'
  assert digest(stage/'gmail_installation.py')==spec['mail_installation']['module_sha256'],'Mailbox installer changed'
  from gmail_installation import prepare_gmail_installation,gmail_profile_environment
  mail_root=prepare_gmail_installation(spec,root)
 shutil.copytree(oldrelease,release,ignore=shutil.ignore_patterns('acceptance-work'))
 for rel in spec['files']:
  dst=source/rel;dst.parent.mkdir(parents=True,exist_ok=True)
  if dst.exists():dst.chmod(0o644)
  shutil.copyfile(stage/'files'/rel,dst);assert digest(dst)==spec['files'][rel],'Copied file pin mismatch';dst.chmod(0o555 if rel in spec['executables'] else 0o444)
 sm={'files':[{'path':str(p.relative_to(source)),'sha256':digest(p)} for p in sorted((source/'app').rglob('*.ep'))]};smp=release/'source-manifest.json';write(smp,json.dumps(sm).encode(),0o444)
 manifest=dict(old,source_root=str(source),source_manifest=str(smp),source_manifest_sha256=digest(smp));entries=[]
 for entry in old['services']:
  profilepath=Path(entry['profile']);text=profilepath.read_text().replace(str(oldrelease),str(release));profile=json.loads(text);executable=Path(profile['executable']);pin=digest(executable)
  env=dict(v.split('=',1) for v in profile['environment'])
  env['ECHO_RUNTIME_BUILD_SHA256']=pin
  env.update(research_environment)
  if spec.get('qol_config'):
   env['ECHO_QOL_CONFIG']=str(root/'notices/qol.json')
  if tool_prepared:
   env['ECHO_TOOL_REGISTRY_PATH']=tool_prepared['registry']
   for key in ['ECHO_EXECUTION_RUNTIME','ECHO_DISCORD_EXECUTION_RUNTIME']:
    if key in env:env[key]=tool_prepared['execution_runtime']
  protected=json.loads(env.get('ECHO_HOST_SECRET_ROOTS','[]'))
  for key,target in [('vault_root','ECHO_EXTERNAL_VAULT_ROOT'),('spool_root','ECHO_EXTERNAL_SPOOL_ROOT')]:
   if key in spec:
    env[target]=spec[key]
    if spec[key] not in protected:protected.append(spec[key])
  if research_environment:
   research_root=str(Path(research_environment['ECHO_RESEARCH_CATALOG']).parent)
   if research_root not in protected:protected.append(research_root)
  env['ECHO_HOST_SECRET_ROOTS']=json.dumps(protected,separators=(',',':'))
  if mail_root:env=gmail_profile_environment(env,mail_root,entry['name']=='agent')
  profile['environment']=[key+'='+value for key,value in env.items()]
  if entry['name']=='web' and 'client.js' in spec['files']:
   assert profile['arguments'][0]=='49675','Unexpected web port'
   profile['arguments'][1]=str(source/'client.js')
  dst=release/profilepath.name;write(dst,json.dumps(profile).encode(),0o600);entries.append(dict(entry,profile=str(dst),profile_sha256=digest(dst),executable_sha256=pin))
 manifest['services']=entries
 if tool_prepared:
  pipeline_path=root/'config/pipeline.json';pipeline_previous=pipeline_path.read_bytes();pipeline=json.loads(pipeline_previous)
  template_path=Path(pipeline['service_templates'])
  assert digest(template_path)==pipeline['service_templates_sha256'],'Installed RSI templates changed'
  templates=json.loads(template_path.read_bytes())
  for template in templates['services']:
   profile=json.loads(template['profile']);env=dict(v.split('=',1) for v in profile['environment'])
   env.update(research_environment)
   if spec.get('qol_config'):env['ECHO_QOL_CONFIG']=str(root/'notices/qol.json')
   env['ECHO_TOOL_REGISTRY_PATH']=tool_prepared['registry']
   for key in ['ECHO_EXECUTION_RUNTIME','ECHO_DISCORD_EXECUTION_RUNTIME']:
    if key in env:env[key]=tool_prepared['execution_runtime']
   env['ECHO_EXTERNAL_VAULT_ROOT']=spec['vault_root'];env['ECHO_EXTERNAL_SPOOL_ROOT']=spec['spool_root']
   secrets=json.loads(env.get('ECHO_HOST_SECRET_ROOTS','[]'))
   for value in [spec[k] for k in ('vault_root','spool_root') if k in spec]:
    if value not in secrets:secrets.append(value)
   if research_environment:
    research_root=str(Path(research_environment['ECHO_RESEARCH_CATALOG']).parent)
    if research_root not in secrets:secrets.append(research_root)
   env['ECHO_HOST_SECRET_ROOTS']=json.dumps(secrets,separators=(',',':'))
   if mail_root:env=gmail_profile_environment(env,mail_root,template['name']=='agent')
   profile['environment']=[key+'='+value for key,value in env.items()]
   if template['name']=='web' and 'client.js' in spec['files']:profile['arguments'][1]=str(source/'client.js')
   template['profile']=json.dumps(profile,separators=(',',':'))
  next_templates=release/'service-templates.json';write(next_templates,json.dumps(templates).encode(),0o600)
  pipeline['service_templates']=str(next_templates);pipeline['service_templates_sha256']=digest(next_templates)
  pipeline_next=json.dumps(pipeline).encode()

 if spec.get('qol_config') and not tool_prepared:
  pipeline_path=root/'config/pipeline.json';pipeline_previous=pipeline_path.read_bytes();pipeline=json.loads(pipeline_previous)
  template_path=Path(pipeline['service_templates'])
  assert digest(template_path)==pipeline['service_templates_sha256'],'Installed RSI templates changed'
  templates=json.loads(template_path.read_bytes())
  for template in templates['services']:
   profile=json.loads(template['profile']);env=dict(v.split('=',1) for v in profile['environment'])
   env['ECHO_QOL_CONFIG']=str(root/'notices/qol.json')
   profile['environment']=[key+'='+value for key,value in env.items()]
   template['profile']=json.dumps(profile,separators=(',',':'))
  next_templates=release/'service-templates.json';write(next_templates,json.dumps(templates).encode(),0o600)
  pipeline['service_templates']=str(next_templates);pipeline['service_templates_sha256']=digest(next_templates)
  pipeline_next=json.dumps(pipeline).encode()

 for artifact in manifest['build_report']['artifacts']:
  if artifact['name'] in spec['executables']:
   artifact['sha256']=digest(source/artifact['name']);artifact['source_sha256']=digest(source/'app'/(artifact['name']+'.ep'));artifact['build_report']=spec['build_reports'][artifact['name']]
 if spec.get('self_monitor') and not any(a['name']=='self_monitor_bridge' for a in manifest['build_report']['artifacts']):
  manifest['build_report']['artifacts'].append({'name':'self_monitor_bridge','sha256':digest(source/'self_monitor_bridge'),'source_sha256':digest(source/'app/self_monitor_bridge.ep'),'build_report':spec['build_reports']['self_monitor_bridge']})
 launcher=json.loads((root/'config/launcher.json').read_text())
 uid=int(launcher['uid']);gid=int(launcher['gid'])
 for value in [spec[k] for k in ('vault_root','spool_root') if k in spec]:
  directory=Path(value)
  assert directory.is_absolute() and not directory.is_symlink(),'Unsafe external storage path'
  directory.mkdir(parents=True,exist_ok=True,mode=0o700)
  assert directory.resolve()==directory,'Noncanonical external storage path'
  os.chown(directory,uid,gid);directory.chmod(0o700)
 if spec.get('self_monitor'):
  from self_monitor_installation import prepare_monitor,activate_monitor,restore_monitor
  monitor_prepared=prepare_monitor(spec,source,old,uid)
 candidate=release/'candidate.json';write(candidate,json.dumps(manifest).encode(),0o444)
 acceptance=root/'config/acceptance.json'
 test_workspace=release/'acceptance-work';test_workspace.mkdir(mode=0o700)
 test_environment=dict(os.environ,TMPDIR=str(test_workspace.resolve()))
 for name,args in [('release-integrity',[str(root/'bin/rsi_health'),str(candidate)]),('fixed-acceptance',[str(root/'bin/rsi_acceptance'),str(acceptance),digest(acceptance),str(candidate)])]:
  p=subprocess.run(args,capture_output=True,text=True,env=test_environment);assert p.returncode==0,name+' failed with exit '+str(p.returncode)
 shutil.copyfile(stage/'test-receipt.json',release/'operator-test-receipt.json');(release/'operator-test-receipt.json').chmod(0o444)
 report('prepared',release=str(release),fixed_acceptance_passed=True,integrity_passed=True);wait('prepared')
 if not maintenance_owned:
  assert not marker.exists(),'Another maintenance operation is active'
 write(marker,spec['release'].encode(),0o644);maintenance_owned=True
 # Intake is held by the maintenance marker. The coordinator verifies no
 # effects are in flight; launchd alone owns stopping and reaping its jobs.
 report('quiescent');wait('quiescent')
 stopped=True
 if tool_prepared:stop_service('tool-registry')
 for label in reversed(labels):stop_service(label)
 report('stopped');wait('stopped')
 assert active.read_bytes()==oldbytes,'Active release changed during maintenance'
 write(release/'previous-active.json',oldbytes,0o400);write(active,candidate.read_bytes(),0o444);activated=True
 if spec.get('qol_config'):
  qol=spec['qol_config']
  assert set(qol) in ({'me_file','recall_awareness','change_notices','disclaimer_flag','tokenizer_port','notice_lines','notices_file','refresh_controller_notices'}, {'me_file','recall_awareness','change_notices','disclaimer_flag','tokenizer_port','notice_lines','notices_file','refresh_controller_notices','self_monitor'}),'Invalid QoL configuration'
  if 'self_monitor' in qol:assert type(qol['self_monitor']) is bool
  assert all(type(qol[key]) is bool for key in ('me_file','recall_awareness','change_notices','disclaimer_flag'))
  assert qol['refresh_controller_notices'] is True
  assert qol['tokenizer_port']==8080 and qol['notice_lines']==5 and qol['notices_file']==str(root/'notices/latest.json')
  write(qol_path,json.dumps(qol,sort_keys=True).encode(),0o644);qol_changed=True
 append_notice('deployment','Echo application source deployment activated.',digest(candidate),'operator',event_id='deploy:'+spec['release'])
 if spec.get('qol_config'):
  append_notice('prompt',"Installed Echo's requested working-state context, recall coverage/session index and disclaimer-awareness clause.",digest(candidate),'operator',event_id='qol-prompt:'+spec['release'])
  append_notice('toolset','Reserved scratchpad note me now supports a 4000-token cap, revision checks and open-thread expiry.',digest(candidate),'operator',event_id='qol-tools:'+spec['release'])
  append_notice('policy','Enabled four independent QoL flags; existing audience and memory permissions are unchanged.',digest(qol_path),'operator',event_id='qol-flags:'+spec['release'])
 if pipeline_next is not None:
  assert (root/'config/pipeline.json').read_bytes()==pipeline_previous,'Installed pipeline changed during maintenance'
  write(release/'previous-pipeline.json',pipeline_previous,0o400)
  pipeline_changed=True;write(root/'config/pipeline.json',pipeline_next,0o600)

 for label in labels:start_service(label)
 verify_services()
 if tool_prepared:
  tool_swapped=True
  write(tool_destination,Path(tool_prepared['plist']).read_bytes(),0o644);os.chown(tool_destination,0,0)
  start_tool()
 if monitor_prepared:
  monitor_activated=True;activate_monitor(monitor_prepared)
  append_notice('toolset','Enabled separate background self-monitor reviews: scoped findings only; no automatic RSI submissions or deployments.',digest(candidate),'operator',event_id='self-monitor:'+spec['release'])
 report('started',release=str(release));wait('started')
 (root/'maintenance').unlink();maintenance_owned=False;report('resumed',release=str(release));wait('resumed')
 report('complete',release=str(release),fixed_acceptance_passed=True,integrity_passed=True,persistent_services=labels,automatic_rsi_deployment=True,tool_registry=tool_prepared)
except BaseException as error:
 recovery_errors=[]
 def recover(name,action):
  try:action()
  except BaseException as problem:recovery_errors.append(name+':'+type(problem).__name__)
 if monitor_activated:
  recover('restore-monitor',lambda:restore_monitor(monitor_prepared))
 if activated:
  # Hold intake throughout rollback, including failures detected after resume.
  recover('hold-intake',lambda:write(root/'maintenance',spec['release'].encode(),0o644));maintenance_owned=True
  recover('restore-manifest',lambda:write(active,oldbytes,0o444))
  if qol_changed:
   if qol_before is None:recover('remove-qol-config',lambda:qol_path.unlink(missing_ok=True))
   else:recover('restore-qol-config',lambda:write(qol_path,qol_before,0o644))
  recover('revert-notice',lambda:append_notice('revert','Echo application reverted to the preceding verified release.',hashlib.sha256(oldbytes).hexdigest(),'operator',event_id='revert:'+spec['release']))
 if pipeline_changed:
  recover('restore-pipeline',lambda:write(root/'config/pipeline.json',pipeline_previous,0o600))
 if stopped or maintenance_owned:
  if tool_prepared:
   recover('stop-tool-registry',lambda:stop_service('tool-registry'))
   if tool_swapped:
    if tool_previous is None:
     if tool_destination.exists():recover('remove-new-tool-service',tool_destination.unlink)
    else:recover('restore-tool-service-definition',lambda:write(tool_destination,tool_previous,0o644))
   if tool_previous is not None:recover('restart-previous-tool-service',start_tool)
  for label in labels:
   def restore(label=label):
    if activated:stop_service(label)
    start_service(label)
   recover('restore-'+label,restore)
  recover('verify-restored-services',verify_services)
 if maintenance_owned and (root/'maintenance').exists() and not recovery_errors:
  recover('remove-maintenance',(root/'maintenance').unlink)
 report('recovery-incomplete' if recovery_errors else 'failed-restored',error_type=type(error).__name__,detail=str(error)[:200],recovery_errors=recovery_errors,persistent_services=labels if len(persistent_created)==len(labels) else [])
 raise
