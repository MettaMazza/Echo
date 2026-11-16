"""Prepare the one-time administrator enrollment; routine operations use echo-admin."""
from pathlib import Path
import argparse,json,os,shutil
p=argparse.ArgumentParser();p.add_argument('destination',type=Path);p.add_argument('--runtime',type=Path,required=True);args=p.parse_args()
source=Path(__file__).resolve().parent;dest=args.destination.resolve()
dest.mkdir(mode=0o700,parents=True,exist_ok=True)
for name in ['echo-admin.py','tool_installation.py','research_installation.py','gmail_installation.py','qualify-tool-registration.py','qol_notice.py','self_monitor_installation.py']:
 shutil.copyfile(source/name,dest/name)
shutil.copyfile(source/'install-unattended-admin.py',dest/'bootstrap.py')
text=(source/'install-prepared-release-root.py').read_text()
text=text.replace("progress=stage/'progress.json';decision=stage/'decision.json'", "control=Path(sys.argv[2]);progress=control/'progress.json';decision=control/'decision.json';sys.path.insert(0,str(stage))")
text=text.replace("p=stage/'progress.tmp'", "p=control/'progress.tmp'")
(dest/'install-root.py').write_text(text)
root=Path('/Library/Application Support/EchoRSI')
active=json.loads((root/'releases/active.json').read_bytes())
artifacts={x['name'] for x in active['build_report']['artifacts']}
for ep in (source.parent.parent/'app').glob('*.ep'):
 if ep.stem not in ('rsi_controller','rsi_initialize','rsi_acceptance','rsi_health','rsi_service'):artifacts.add(ep.stem)
policy={'uid':os.getuid(),'runtime':str(args.runtime.resolve()),'artifacts':sorted(artifacts),'storage_roots':['/Volumes/One Touch/Echo-private/external-api/vault','/Volumes/One Touch/Echo-private/external-api/requests','/Volumes/One Touch/Echo-private/gmail']}
(dest/'policy.json').write_text(json.dumps(policy,indent=2))
print('Prepared one-time enrollment:',dest/'bootstrap.py')
