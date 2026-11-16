from pathlib import Path
import json,os,pwd,shutil,subprocess,sys,tempfile
assert os.geteuid()==0
stage=Path(__file__).resolve().parent
root=Path('/Library/Application Support/EchoRSI/administration')
root.mkdir(mode=0o755,exist_ok=True)
assert not root.is_symlink()
os.chown(root,0,0);root.chmod(0o755)
for name in ['install-root.py','tool_installation.py','research_installation.py','gmail_installation.py','qualify-tool-registration.py','qol_notice.py','self_monitor_installation.py','policy.json']:
 dst=root/name
 assert not dst.is_symlink()
 data=(stage/name).read_bytes()
 tmp=root/(name+'.new');tmp.write_bytes(data);os.chown(tmp,0,0);tmp.chmod(0o644);os.replace(tmp,dst)
(root/'work').mkdir(mode=0o700,exist_ok=True)
os.chown(root/'work',0,0);(root/'work').chmod(0o700)
helper=Path('/Library/PrivilegedHelperTools/local.echo.admin')
helper.parent.mkdir(mode=0o755,exist_ok=True)
assert helper.parent.stat().st_uid==0 and not helper.parent.stat().st_mode & 0o022
assert not helper.is_symlink()
tmp=helper.with_suffix('.new');tmp.write_bytes((stage/'echo-admin.py').read_bytes());os.chown(tmp,0,0);tmp.chmod(0o755);os.replace(tmp,helper)
uid=json.loads((root/'policy.json').read_bytes())['uid'];user=pwd.getpwuid(uid).pw_name
assert user.replace('_','').isalnum()
rule=f'{user} ALL=(root) NOPASSWD: /Library/PrivilegedHelperTools/local.echo.admin\n'
sudo=Path('/private/etc/sudoers.d');sudo.mkdir(mode=0o755,exist_ok=True)
assert not sudo.is_symlink()
fd,path=tempfile.mkstemp(prefix='echo-admin-',dir='/private/etc')
try:
 os.write(fd,rule.encode());os.close(fd);os.chmod(path,0o440)
 subprocess.run(['/usr/sbin/visudo','-cf',path],check=True)
 destination=sudo/'echo-admin';old=destination.read_bytes() if destination.exists() else None
 os.replace(path,destination)
 try:subprocess.run(['/usr/sbin/visudo','-c'],check=True)
 except BaseException:
  if old is None:destination.unlink()
  else:destination.write_bytes(old);destination.chmod(0o440)
  raise
finally:
 if os.path.exists(path):os.unlink(path)
print('Installed scoped password-free Echo administration for '+user)
