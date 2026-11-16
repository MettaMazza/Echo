#!/usr/bin/python3 -I
"""Local owner's delegated Echo administration. No shell or arbitrary command verb."""
import fcntl, hashlib, json, os, pathlib, pwd, re, shutil, stat, subprocess, sys, tempfile, time
P=pathlib.Path
ROOT=P('/Library/Application Support/EchoRSI')
HOME=ROOT/'administration'
ENV={'PATH':'/usr/bin:/bin:/usr/sbin:/sbin','HOME':'/var/root'}
SERVICES=('web','agent','activity','queue_controls','gateway','actions','interaction_ack','rsi-controller','tool-registry','primary','embedding','omni','speech','proxy')
def fail(message): raise RuntimeError(message)
def regular(path):
 fd=os.open(path,os.O_RDONLY|os.O_NOFOLLOW)
 with os.fdopen(fd,'rb') as f:
  if not stat.S_ISREG(os.fstat(f.fileno()).st_mode):fail('Not a regular file')
  return f.read()
def publish(directory,name,data):
 fd,tmp=tempfile.mkstemp(prefix='.echo-admin-',dir=str(directory))
 try:
  with os.fdopen(fd,'wb') as f:f.write(data);f.flush();os.fsync(f.fileno())
  os.chmod(tmp,0o644);os.replace(tmp,directory/name)
 finally:
  if os.path.exists(tmp):os.unlink(tmp)
def service(verb,name):
 if name not in SERVICES:fail('Unknown Echo service')
 target='system/local.echo.'+name
 if verb=='status':return subprocess.run(['/bin/launchctl','print',target],env=ENV).returncode
 if verb=='stop':args=['/bin/launchctl','bootout',target]
 elif verb=='start':args=['/bin/launchctl','bootstrap','system',str(ROOT/'launchd'/('local.echo.'+name+'.plist'))]
 else:args=['/bin/launchctl','kickstart','-k',target]
 return subprocess.run(args,env=ENV).returncode

def prepare(stage):
 stage=P(stage)
 if not stage.is_absolute() or stage.resolve()!=stage or stage.stat().st_uid!=int(os.environ['SUDO_UID']):fail('Package must be canonical and owned by the authorized local user')
 if stage.stat().st_mode & 0o022:fail('Package must not be writable by other accounts')
 raw=regular(stage/'package.json');spec=json.loads(raw)
 if not re.fullmatch(r'[A-Za-z0-9][A-Za-z0-9._-]{0,100}',spec['release']):fail('Invalid release name')
 runtime=P(spec['runtime'])
 config=json.loads((HOME/'policy.json').read_bytes())
 if str(runtime)!=config['runtime']:fail('Different Echo runtime')
 for key in ('vault_root','spool_root'):
  if key in spec and spec[key] not in config['storage_roots']:fail('Unregistered Echo storage root')
 if spec.get('mail_installation') and spec['mail_installation']['mail_root'] not in config['storage_roots']:fail('Unregistered Echo mailbox root')
 snapshot=P(tempfile.mkdtemp(prefix='package-',dir=str(HOME/'work')))
 def copy(rel,pin=None):
  p=P(rel)
  if p.is_absolute() or '..' in p.parts or (stage/p).resolve()!=stage/p:fail('Indirect package path')
  data=regular(stage/p)
  if pin and hashlib.sha256(data).hexdigest()!=pin:fail('Package hash mismatch: '+rel)
  dst=snapshot/p;dst.parent.mkdir(parents=True,exist_ok=True);dst.write_bytes(data);dst.chmod(0o444)
 for rel,pin in spec['files'].items():
  p=P(rel)
  if p.is_absolute() or '..' in p.parts or rel in ('','.') or not re.fullmatch(r'[A-Za-z0-9_./-]+',rel):fail('Invalid release path')
  if len(p.parts)==1 and rel not in config['artifacts'] and rel!='client.js':fail('Unknown application artifact')
  if len(p.parts)>1 and p.parts[0] not in ('app','tests','assets'):fail('Unknown application directory')
  copy('files/'+rel,pin)
 if not set(spec['executables']).issubset(set(spec['files']) & set(config['artifacts'])):fail('Invalid executable set')
 copy('test-receipt.json')
 for key,module in [('tool_installation','tool_installation.py'),('research_catalog','research_installation.py'),('mail_installation','gmail_installation.py')]:
  if spec.get(key):
   trusted=(HOME/module).read_bytes()
   if hashlib.sha256(trusted).hexdigest()!=spec[key]['module_sha256']:fail('Installer module is not enrolled')
   (snapshot/module).write_bytes(trusted)
 if spec.get('research_catalog'):copy('research-catalog.json',spec['research_catalog']['sha256'])
 if spec.get('tool_installation'):
  settings=spec['tool_installation'];copy('tool-fixture/package.json',settings['fixture_manifest_sha256'])
  fixture=json.loads((snapshot/'tool-fixture/package.json').read_bytes())
  expected={'tool_registration_controller','tool_registry_check','test_registered_workflow','execute_guardian','tool_behavior_probe','tool_behavior_probe.ep'}
  if set(fixture['files'])!=expected:fail('Invalid tool fixture')
  for rel,item in fixture['files'].items():copy('tool-fixture/'+rel,item['sha256'])
  trusted=(HOME/'qualify-tool-registration.py').read_bytes()
  if hashlib.sha256(trusted).hexdigest()!=settings['fixture_installer_sha256']:fail('Tool installer is not enrolled')
  (snapshot/'tool-fixture/qualify-tool-registration.py').write_bytes(trusted)
 (snapshot/'package.json').write_bytes(raw)
 # Installer code is root-owned. Never execute a script supplied by the package.
 shutil.copyfile(HOME/'install-root.py',snapshot/'install-root.py')
 return stage,snapshot


def private_json(path,value):
 private_bytes(path,json.dumps(value,sort_keys=True).encode())

def private_bytes(path,data):
 fd,tmp=tempfile.mkstemp(prefix='.owner-pin-',dir=str(path.parent))
 try:
  with os.fdopen(fd,'wb') as f:f.write(data);f.flush();os.fsync(f.fileno())
  os.chmod(tmp,0o600);os.replace(tmp,path)
 finally:
  if os.path.exists(tmp):os.unlink(tmp)

def owner_pin_recover():
 record=HOME/'owner-pin-update.json'
 if not record.exists():return
 tx=json.loads(regular(record))
 if tx['state']!='pending':return
 active=json.loads(regular(ROOT/'releases/active.json'))
 source=P(active['source_root'])
 matches=lambda pins:all(hashlib.sha256(regular(source/name)).hexdigest()==pin for name,pin in pins.items())
 if matches(tx['new_pins']):which='new';state='committed'
 elif matches(tx['old_pins']):which='old';state='rolled-back'
 else:fail('Active protected source differs from both owner-update baselines')
 for name in ('acceptance','pipeline'):
  path=ROOT/'config'/(name+'.json')
  current=json.loads(regular(path))
  if name=='pipeline' and which=='new':
   if current.get('acceptance_spec')!=tx['new_pipeline']['acceptance_spec'] or current.get('acceptance_spec_sha256')!=tx['new_pipeline']['acceptance_spec_sha256']:fail('Committed pipeline acceptance reference changed')
   continue
  if current not in (tx['old_'+name],tx['new_'+name]):fail('Owner configuration changed during pin update')
  private_bytes(path,bytes.fromhex(tx[which+'_'+name+'_bytes']))
 tx['state']=state;private_json(record,tx)
 private_json(P(tx['receipt']),tx)

def owner_pin_prepare(snapshot):
 spec=json.loads(regular(snapshot/'package.json'))
 active_bytes=regular(ROOT/'releases/active.json')
 if hashlib.sha256(active_bytes).hexdigest()!=spec['active_sha256']:fail('Active release changed before owner pin update')
 active=json.loads(active_bytes);source=P(active['source_root'])
 old=json.loads(regular(ROOT/'config/acceptance.json'))
 new=json.loads(json.dumps(old));old_pins={};new_pins={}
 for entry in new.get('protected_sources',[]):
  name=entry['path']
  if name not in spec['files']:continue
  if not name.startswith('app/') or '..' in P(name).parts:fail('Invalid protected source path')
  if hashlib.sha256(regular(source/name)).hexdigest()!=entry['sha256']:fail('Installed source no longer matches its protected pin')
  replacement=hashlib.sha256(regular(snapshot/'files'/name)).hexdigest()
  if replacement!=spec['files'][name]:fail('Owner replacement differs from captured package')
  if replacement!=entry['sha256']:
   old_pins[name]=entry['sha256'];new_pins[name]=replacement;entry['sha256']=replacement
 if not old_pins:return
 # The owner can update source versions, never select/omit acceptance tests,
 # replace the installed evaluator, or remove a protected path via this verb.
 pipeline=json.loads(regular(ROOT/'config/pipeline.json'))
 if pipeline.get('acceptance_spec')!=str(ROOT/'config/acceptance.json'):fail('Unexpected acceptance specification')
 if pipeline.get('acceptance_spec_sha256')!=hashlib.sha256(regular(ROOT/'config/acceptance.json')).hexdigest():fail('Acceptance specification pin changed')
 updated=dict(pipeline);updated['acceptance_spec_sha256']=hashlib.sha256(json.dumps(new,sort_keys=True).encode()).hexdigest()
 tx=dict(state='pending',old_acceptance=old,new_acceptance=new,old_pipeline=pipeline,new_pipeline=updated,old_pins=old_pins,new_pins=new_pins,receipt=str(snapshot/'owner-pin-update.json'))
 for name in ('acceptance','pipeline'):
  tx['old_'+name+'_bytes']=regular(ROOT/'config'/(name+'.json')).hex()
  tx['new_'+name+'_bytes']=json.dumps(tx['new_'+name],sort_keys=True).encode().hex()
 private_json(HOME/'owner-pin-update.json',tx);private_json(P(tx['receipt']),tx)
 private_json(ROOT/'config/acceptance.json',new);private_json(ROOT/'config/pipeline.json',updated)
 return True


def storage_device():
 import ctypes
 lib=ctypes.CDLL('/usr/lib/libSystem.B.dylib')
 buf=ctypes.create_string_buffer(2168*256)
 count=lib.getfsstat(buf,len(buf),2)
 if count<0:fail('Cannot inspect cached mount table')
 for i in range(count):
  row=buf.raw[i*2168:(i+1)*2168]
  mount=row[88:1112].split(b'\0',1)[0].decode()
  if mount=='/Volumes/One Touch':
   device=row[1112:2136].split(b'\0',1)[0].decode()
   if not re.fullmatch(r'/dev/disk[0-9]+s[0-9]+',device):fail('Unexpected Echo volume device')
   return device
 fail('Echo volume is not mounted; refusing to guess a device')

def recover_storage():
 device=storage_device()
 if not (ROOT/'maintenance').exists():fail('Put Echo in maintenance before storage recovery')
 # Only the registered volume. No erase, repair, arbitrary paths, or shell.
 for command,timeout in [(['/sbin/umount','-f','/Volumes/One Touch'],30),(['/usr/sbin/diskutil','mount',device],60)]:
  child=subprocess.Popen(command,env=ENV,start_new_session=True)
  try:code=child.wait(timeout=timeout)
  except subprocess.TimeoutExpired:
   import signal
   os.killpg(child.pid,signal.SIGKILL)
   fail('Storage operation timed out; no further recovery attempted')
  if code:fail('Storage operation failed: '+str(code))
 return 0


def maintenance(action):
 marker=ROOT/'maintenance'
 if action=='on':
  publish(ROOT,'maintenance',b'Echo operator maintenance\n');return 0
 if action!='off':fail('Unknown maintenance action')
 import urllib.request
 for port in (8080,8081,8084,49675):
  with urllib.request.urlopen('http://127.0.0.1:'+str(port)+'/health',timeout=5) as response:
   if json.load(response).get('status') not in ('ok','ready'):fail('Echo service is not ready')
 storage_device()
 # Check external storage as the enrolled user before releasing intake.
 uid=json.loads((HOME/'policy.json').read_bytes())['uid'];account=pwd.getpwuid(uid)
 probe="import os,tempfile; fd,p=tempfile.mkstemp(prefix='echo-recovery-',dir='/Volumes/One Touch/Echo/evidence'); os.write(fd,b'recovery check'); os.fsync(fd); os.close(fd); os.unlink(p)"
 child=subprocess.Popen(['/usr/bin/python3','-I','-c',probe],cwd='/Users/Shared',env=ENV,user=uid,group=account.pw_gid,extra_groups=os.getgrouplist(account.pw_name,account.pw_gid),start_new_session=True)
 try:code=child.wait(timeout=10)
 except subprocess.TimeoutExpired:
  import signal
  os.killpg(child.pid,signal.SIGKILL);fail('External storage check timed out; intake remains held')
 if code:fail('External storage check failed; intake remains held')
 marker.unlink(missing_ok=True);return 0


def diagnose_storage():
 # Observe only the already-running repair of the Echo device. No signals,
 # filesystem writes on that device, mounting, or arbitrary command arguments.
 rows=subprocess.check_output(['/bin/ps','-axo','pid=,args='],text=True).splitlines()
 matches=[]
 for row in rows:
  fields=row.strip().split()
  if len(fields)>=3 and fields[1]=='/System/Library/Filesystems/exfat.fs/Contents/Resources/./fsck_exfat' and fields[-1]=='/dev/rdisk4s2':matches.append(fields[0])
 if len(matches)!=1:fail('Expected exactly one existing Echo filesystem check')
 pid=matches[0]
 parent=HOME/'diagnostics';parent.mkdir(mode=0o755,exist_ok=True)
 if parent.is_symlink() or parent.stat().st_uid!=0 or parent.stat().st_mode & 0o022:fail('Unprotected diagnostics directory')
 output=P(tempfile.mkdtemp(prefix='storage-',dir=str(parent)))
 results={}
 commands=[('disk-io.log',['/usr/bin/fs_usage','-w','-f','diskio','-t','12',pid],20),('process-sample.log',['/usr/bin/sample',pid,'1','-file',str(output/'sample.txt')],10)]
 for name,command,timeout in commands:
  with (output/name).open('w') as log:
   try:results[name]=subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,env=ENV,timeout=timeout).returncode
   except subprocess.TimeoutExpired:results[name]='diagnostic-timeout'
 uid=json.loads((HOME/'policy.json').read_bytes())['uid'];gid=pwd.getpwuid(uid).pw_gid
 for file in output.iterdir():file.chmod(0o600);os.chown(file,uid,gid)
 os.chown(output,uid,gid);output.chmod(0o700)
 print(json.dumps({'repair_pid':int(pid),'directory':str(output),'results':results}));return 0

def main():
 if os.geteuid()!=0 or os.environ.get('SUDO_UID')!=str(json.loads((HOME/'policy.json').read_bytes())['uid']):fail('Only the enrolled local owner can administer Echo')
 args=sys.argv[1:]
 if args==['status']:
  active=json.loads((ROOT/'releases/active.json').read_bytes())
  print(json.dumps({'unattended':True,'source_root':active['source_root'],'commands':['status','start <Echo service>','stop <Echo service>','restart <Echo service>','storage-status','recover-storage','maintenance on|off','diagnose-storage','check-package <directory>','deploy <directory>','deploy-owner <directory>']}));return 0
 if len(args)==2 and args[0] in ('start','stop','restart'):return service(args[0],args[1])
 if len(args)==2 and args[0]=='maintenance':return maintenance(args[1])
 if args==['storage-status']:print(json.dumps({'mount':'/Volumes/One Touch','device':storage_device()}));return 0
 if args==['recover-storage']:return recover_storage()
 if args==['diagnose-storage']:return diagnose_storage()
 if len(args)!=2 or args[0] not in ('deploy','deploy-owner','check-package'):fail('Unsupported operation')
 with (HOME/'operation.lock').open('a') as lock:
  fcntl.flock(lock,fcntl.LOCK_EX|fcntl.LOCK_NB)
  owner_pin_recover()
  stage,snapshot=prepare(args[1])
  if args[0]=='check-package':print('PASS: pinned package captured; protected installer selected; no deployment performed.');return 0
  if args[0]=='deploy-owner' and owner_pin_prepare(snapshot):
   # The controller pins its configuration for each process lifetime.
   # Reload it before the installer's stable-service observation.
   service('restart','rsi-controller')
  control=snapshot/'control';control.mkdir()
  proc=subprocess.Popen(['/usr/bin/python3','-I',str(snapshot/'install-root.py'),str(snapshot),str(control)],cwd=str(snapshot),env=ENV)
  last=None
  while proc.poll() is None:
   progress=control/'progress.json'
   if progress.exists():
    data=progress.read_bytes()
    if data!=last:publish(stage,'progress.json',data);last=data
   try:
    decision=regular(stage/'decision.json')
    if len(decision)<4096:
     item=json.loads(decision)
     if item.get('decision') in ('proceed','abort'):publish(control,'decision.json',decision)
   except (FileNotFoundError,ValueError):pass
   time.sleep(.1)
  if (control/'progress.json').exists():publish(stage,'progress.json',(control/'progress.json').read_bytes())
  owner_pin_recover()
  return proc.returncode
if __name__=='__main__':
 try:sys.exit(main())
 except Exception as error:print(type(error).__name__+': '+str(error),file=sys.stderr);sys.exit(1)
