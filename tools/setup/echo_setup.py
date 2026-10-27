"""Echo setup: hardware inventory, verified bundles, recovery and desktop entry.

No credentials are embedded, printed, inherited by probes, or downloaded.
Readiness means verified checks passed, never merely that files were copied.
"""
from __future__ import annotations
import argparse, ctypes, hashlib, json, os, platform, plistlib, re, shutil
import socket, subprocess, sys, tempfile, time, urllib.request, uuid, webbrowser
from pathlib import Path

SYSTEM_ROOT = Path('/Library/Application Support/EchoRSI')
SERVICES = ('web','agent','activity','queue_controls','gateway','actions','interaction_ack','rsi-controller')

def command(args, timeout=15):
    p = subprocess.run(args, capture_output=True, text=True, timeout=timeout)
    if p.returncode:
        raise RuntimeError(f'{Path(args[0]).name} exited {p.returncode}')
    return p.stdout.strip()

def hardware(system=None, machine=None):
    system, machine = system or platform.system(), machine or platform.machine().lower()
    arch = {'aarch64':'arm64','amd64':'x86_64','x64':'x86_64'}.get(machine.lower(),machine.lower())
    ram = 0
    gpus = []
    if system == 'Darwin':
        ram = int(command(['/usr/sbin/sysctl','-n','hw.memsize']))
        gpus = [{'backend':'metal','name':'Apple Silicon' if arch=='arm64' else 'macOS GPU'}]
    elif system == 'Windows':
        class Memory(ctypes.Structure):
            _fields_ = [('length',ctypes.c_ulong),('load',ctypes.c_ulong)] + [(x,ctypes.c_ulonglong) for x in ('total','available','page_total','page_available','virtual_total','virtual_available','extended')]
        m=Memory();m.length=ctypes.sizeof(m)
        if ctypes.windll.kernel32.GlobalMemoryStatusEx(ctypes.byref(m)):ram=m.total
    else:
        for line in Path('/proc/meminfo').read_text().splitlines():
            if line.startswith('MemTotal:'):ram=int(line.split()[1])*1024
    if system != 'Darwin' and shutil.which('nvidia-smi'):
        try:
            for row in command(['nvidia-smi','--query-gpu=name,memory.total','--format=csv,noheader,nounits']).splitlines():
                name,mem=row.rsplit(',',1);gpus.append({'backend':'cuda','name':name.strip(),'memory_bytes':int(mem.strip())*1024**2})
        except (RuntimeError,ValueError,subprocess.TimeoutExpired):pass
    return {'system':system,'architecture':arch,'cpu_threads':os.cpu_count() or 1,'memory_bytes':ram,'gpus':gpus,'free_disk_bytes':shutil.disk_usage(Path.home()).free,
            'platform_key': {'Darwin':'macos','Windows':'windows','Linux':'linux'}.get(system,system.lower())+'-'+arch,
            'validation_notice':'Only macOS has production end-to-end evidence. Windows/Linux setup code is unverified; native application ports are not yet packaged.'}

def select_model(hw, catalog):
    # Account separately for weights, KV cache, OS, buffers and installation space.
    candidates=[m for m in catalog if hw['memory_bytes']>=m['minimum_ram_bytes'] and hw['free_disk_bytes']>=m['bytes']*2+4*1024**3]
    if not candidates:raise RuntimeError('No catalog model fits the available RAM and disk. No partial installation was activated.')
    return max(candidates,key=lambda m:m['minimum_ram_bytes'])

def digest(path):
    h=hashlib.sha256()
    with Path(path).open('rb') as f:
        for block in iter(lambda:f.read(1024*1024),b''):h.update(block)
    return h.hexdigest()

def safe_relative(root, relative):
    p=Path(relative)
    if p.is_absolute() or '..' in p.parts or not p.parts:raise ValueError('Unsafe package path')
    out=Path(root)/p
    if out.is_symlink() or not out.resolve().is_relative_to(Path(root).resolve()):raise ValueError('Package path escapes bundle')
    return out

def verify_bundle(bundle, hw):
    bundle=Path(bundle).resolve();manifest=json.loads((bundle/'manifest.json').read_text())
    if manifest['platform']!=hw['platform_key']:raise RuntimeError('Native bundle does not match this hardware and operating system')
    for relative,pin in manifest['files'].items():
        path=safe_relative(bundle,relative)
        if not path.is_file() or digest(path)!=pin:raise RuntimeError('Package integrity check failed: '+relative)
    if not manifest.get('checks'):raise RuntimeError('Package has no end-to-end acceptance checks')
    return manifest

def download(url, destination, expected_hash, expected_size):
    if not url.startswith('https://'):raise ValueError('Downloads require HTTPS')
    destination=Path(destination);destination.parent.mkdir(parents=True,exist_ok=True)
    if destination.exists() and destination.stat().st_size==expected_size and digest(destination)==expected_hash:return
    partial=destination.with_suffix(destination.suffix+'.partial')
    try:
        with urllib.request.urlopen(url,timeout=60) as r,partial.open('wb') as f:
            shutil.copyfileobj(r,f,1024*1024)
        if partial.stat().st_size!=expected_size or digest(partial)!=expected_hash:raise RuntimeError('Download integrity verification failed')
        os.replace(partial,destination)
    except BaseException:
        partial.unlink(missing_ok=True);raise

def installed_health(root=SYSTEM_ROOT):
    manifest=json.loads((root/'releases/active.json').read_text());source=Path(manifest['source_root'])
    # A host-owned installed manifest is authoritative; no secret profiles read.
    for artifact in manifest['build_report']['artifacts']:
        if digest(source/artifact['name'])!=artifact['sha256']:raise RuntimeError('Installed binary integrity failed: '+artifact['name'])
    pids={}
    for name in SERVICES:
        status=command(['/bin/launchctl','print','system/local.echo.'+name])
        pid=re.search(r'^\s*pid = (\d+)$',status,re.M)
        if not pid or not re.search(r'^\s*state = running$',status,re.M):raise RuntimeError('Service not running: '+name)
        pids[name]=int(pid[1])
        if not (Path('/Library/LaunchDaemons')/('local.echo.'+name+'.plist')).is_file():raise RuntimeError('Service is not persistent: '+name)
    with urllib.request.urlopen('http://127.0.0.1:49675/health',timeout=5) as response:
        if json.load(response).get('status')!='ready':raise RuntimeError('Echo HTTP health failed')
    if (root/'maintenance').exists():raise RuntimeError('Echo is in maintenance; setup cannot claim readiness')
    return {'release':source.parent.name,'binary_integrity':'passed','services':pids,'web_health':'ready'}

def install_bundle(bundle, destination, hw):
    manifest=verify_bundle(bundle,hw);destination=Path(destination);destination.parent.mkdir(parents=True,exist_ok=True)
    # Install into a sibling and activate only after all package-defined checks.
    staging=Path(tempfile.mkdtemp(prefix='.echo-install-',dir=destination.parent));previous=None
    try:
        for relative in manifest['files']:
            dst=safe_relative(staging,relative);dst.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(safe_relative(bundle,relative),dst)
            if relative in manifest.get('executables',[]):dst.chmod(0o755)
        for check in manifest['checks']:
            executable=safe_relative(staging,check['executable'])
            if check['executable'] not in manifest.get('executables',[]):raise RuntimeError('Acceptance executable is not pinned as executable')
            args=[str(x).replace('${INSTALL}',str(staging)) for x in check.get('arguments',[])]
            subprocess.run([str(executable),*args],check=True,capture_output=True,timeout=check.get('timeout_seconds',300),env={'PATH':os.defpath,'HOME':str(Path.home())})
        if destination.exists():
            previous=destination.with_name(destination.name+'.previous-'+uuid.uuid4().hex);os.replace(destination,previous)
        os.replace(staging,destination)
        receipt={'status':'package-verified','platform':hw['platform_key'],'checks':len(manifest['checks']),'previous':str(previous) if previous else None}
        (destination/'installation-receipt.json').write_text(json.dumps(receipt,indent=2));return receipt
    except BaseException:
        shutil.rmtree(staging,ignore_errors=True)
        if previous and not destination.exists():os.replace(previous,destination)
        raise

def desktop_entry(url='http://127.0.0.1:49675', desktop=None, system=None):
    desktop=Path(desktop or Path.home()/'Desktop');desktop.mkdir(parents=True,exist_ok=True);system=system or platform.system()
    if not re.fullmatch(r'https?://(?:127\.0\.0\.1|localhost)(?::\d+)?/?',url):raise ValueError('Shortcut must target local Echo')
    if system=='Darwin':
        path=desktop/'Echo.webloc';path.write_bytes(plistlib.dumps({'URL':url}))
    elif system=='Windows':
        path=desktop/'Echo.url';path.write_text('[InternetShortcut]\nURL='+url+'\n')
    else:
        path=desktop/'Echo.desktop';path.write_text('[Desktop Entry]\nType=Link\nName=Echo\nURL='+url+'\nIcon=applications-internet\n');path.chmod(0o755)
    return str(path)

def main():
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--scan',action='store_true');parser.add_argument('--bundle',type=Path);parser.add_argument('--destination',type=Path,default=Path.home()/'.local/share/Echo');parser.add_argument('--no-open',action='store_true');args=parser.parse_args();hw=hardware()
    if args.scan:print(json.dumps(hw,indent=2));return 0
    if platform.system()=='Darwin' and (SYSTEM_ROOT/'releases/active.json').exists():
        result=installed_health();result['shortcut']=desktop_entry();result['status']='existing-installation-verified';print(json.dumps(result,indent=2))
        if not args.no_open:webbrowser.open('http://127.0.0.1:49675')
        return 0
    if args.bundle:
        result=install_bundle(args.bundle,args.destination,hw);print(json.dumps(result,indent=2));return 0
    raise RuntimeError('A verified native Echo bundle is required for a fresh installation. Windows/Linux native ports are not packaged. Hardware scan is available with --scan. Existing macOS production installation is supported; no unsupported system is labelled ready.')

if __name__=='__main__':
    try:raise SystemExit(main())
    except Exception as exc:print('Setup did not complete: '+str(exc),file=sys.stderr);raise SystemExit(1)
