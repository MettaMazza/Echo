"""Install the bounded, non-privileged findings worker; no model can call this."""
from pathlib import Path
import json
import hashlib
import os
import plistlib
import pwd
import subprocess
import time
import sys

LABEL = 'local.echo.self-monitor'
ROOT = Path('/Library/Application Support/EchoRSI')
STATE = Path('/Volumes/One Touch/Echo-private/self-monitor')
DEST = Path('/Library/LaunchDaemons')/(LABEL+'.plist')


def prepare_monitor(spec, source, old, uid):
    assert set(spec['self_monitor']) == {'enabled','module_sha256'}
    assert spec['self_monitor']['enabled'] is True
    assert os.path.ismount('/Volumes/One Touch'), 'External volume unavailable'
    assert STATE.parent.is_dir() and STATE.parent.resolve() == STATE.parent
    assert not STATE.is_symlink()
    STATE.mkdir(mode=0o700, exist_ok=True)
    assert STATE.resolve() == STATE
    account = pwd.getpwuid(uid)
    os.chown(STATE, uid, account.pw_gid)
    STATE.chmod(0o700)
    for name in ('self_monitor_bridge','app/monitor/review.py','app/monitor/worker.py','app/monitor/monitor_prompt.txt'):
        assert name in spec['files'] and (source/name).is_file(), 'Missing pinned monitor file'
    agent = next(e for e in old['services'] if e['name'] == 'agent')
    profile = json.loads(Path(agent['profile']).read_text())
    environment = dict(v.split('=',1) for v in profile['environment'])
    secrets = json.loads(environment['ECHO_DISCORD_REDACTION_FILES'])
    assert secrets and all(Path(p).is_absolute() and Path(p).is_file() for p in secrets)
    runtime = Path(spec['runtime'])
    archives = [runtime/'data/mlx-browser-001.sqlite', runtime/'data/discord-integration-20261020-123419/agent.sqlite']
    assert all(p.is_file() for p in archives)
    directory = ROOT/'monitor'
    directory.mkdir(mode=0o755,exist_ok=True)
    assert directory.resolve() == directory and directory.stat().st_uid == 0
    config = {'enabled':True,'bridge':str(source/'self_monitor_bridge'),
              'archives':list(map(str,archives)),'ports':[8080,8084],
              'model_port':8080,'secret_files':secrets,'maintenance':str(ROOT/'maintenance')}
    config_path = directory/'config.json'
    # Use Echo's existing protected service launcher and permanent UID drop.
    # A standalone Python LaunchDaemon is a different macOS privacy identity.
    profile_path = directory/'profile.json'
    launcher_path = directory/'launcher.json'
    manifest_path = directory/'active.json'
    interpreter = Path(sys.executable).resolve(strict=True)
    assert interpreter.stat().st_uid == 0 and interpreter.stat().st_nlink == 1
    profile = {'executable':str(interpreter),'directory':str(directory),
               'arguments':['-I','-B',str(source/'app/monitor/worker.py'),
                            '--config',str(config_path),'--state-directory',str(STATE)],
               'environment':['PATH=/usr/bin:/bin:/usr/sbin:/sbin']}
    profile_bytes = json.dumps(profile,sort_keys=True).encode()
    manifest = {'services':[{'name':'self-monitor','profile':str(profile_path),
                 'profile_sha256':hashlib.sha256(profile_bytes).hexdigest(),
                 'executable_sha256':hashlib.sha256(interpreter.read_bytes()).hexdigest()}]}
    launcher = {'active_manifest':str(manifest_path),'uid':uid,'gid':account.pw_gid}
    job = {'Label':LABEL,
           'ProgramArguments':[str(ROOT/'bin/rsi_service'),str(launcher_path),'self-monitor'],
           'WorkingDirectory':str(directory),'RunAtLoad':True,
           'KeepAlive':{'SuccessfulExit':False},'ThrottleInterval':60,
           'ProcessType':'Background','LowPriorityIO':True,'Nice':15,
           'StandardOutPath':'/dev/null','StandardErrorPath':'/dev/null'}
    protected = {profile_path:(profile_bytes,0o600),
                 manifest_path:(json.dumps(manifest).encode(),0o444),
                 launcher_path:(json.dumps(launcher).encode(),0o600)}
    previous = {str(p): p.read_bytes() if p.exists() else None
                for p in (DEST, ROOT/'launchd'/(LABEL+'.plist'),config_path,*protected)}
    return {'config':config,'config_path':config_path,'job':job,'previous':previous,'protected':protected,
            'previous_modes':{name:Path(name).stat().st_mode & 0o777 for name,data in previous.items() if data is not None}}


def stop_monitor():
    check=subprocess.run(['launchctl','print','system/'+LABEL],capture_output=True)
    if check.returncode != 0:
        return
    subprocess.run(['launchctl','bootout','system/'+LABEL],check=True,capture_output=True,timeout=30)
    deadline=time.monotonic()+30
    while time.monotonic()<deadline:
        if subprocess.run(['launchctl','print','system/'+LABEL],capture_output=True).returncode != 0:
            return
        time.sleep(.1)
    raise TimeoutError('Monitor did not stop')


def publish(path, data, mode=0o644):
    path=Path(path);temporary=path.with_suffix(path.suffix+'.tmp')
    assert not path.is_symlink() and not temporary.is_symlink()
    temporary.write_bytes(data);temporary.chmod(mode);os.replace(temporary,path)


def activate_monitor(prepared):
    stop_monitor()
    publish(prepared['config_path'],json.dumps(prepared['config']).encode())
    for path,(data,mode) in prepared['protected'].items():
        publish(path,data,mode)
    job=plistlib.dumps(prepared['job'])
    publish(DEST,job);publish(ROOT/'launchd'/(LABEL+'.plist'),job)
    started=time.time()
    subprocess.run(['launchctl','bootstrap','system',str(DEST)],check=True,capture_output=True,timeout=30)
    deadline=time.monotonic()+15
    while time.monotonic()<deadline:
        check=subprocess.run(['launchctl','print','system/'+LABEL],capture_output=True,text=True,timeout=3)
        status_path=STATE/'status.json'
        if check.returncode==0 and 'state = running' in check.stdout and status_path.exists():
            status=json.loads(status_path.read_text())
            if status.get('updated_unix',0)>=started and status.get('monitor') in ('deferred','idle'):
                return
        time.sleep(.2)
    raise RuntimeError('Monitor did not initialise healthy bounded state')


def restore_monitor(prepared):
    stop_monitor()
    for name,data in prepared['previous'].items():
        path=Path(name)
        if data is None:path.unlink(missing_ok=True)
        else:publish(path,data,prepared['previous_modes'][name])
    if prepared['previous'][str(DEST)] is not None:
        subprocess.run(['launchctl','bootstrap','system',str(DEST)],check=True,capture_output=True,timeout=30)
