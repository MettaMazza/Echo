#!/usr/bin/env python3
"""Read-only publication checks. Does not run Echo or read private configuration."""
from pathlib import Path
import hashlib,json,re,subprocess,urllib.parse
r=Path(__file__).resolve().parents[1]
def git(*a):return subprocess.check_output(['git',*a],cwd=r,text=True)
files=sorted(set(git('ls-files','-z').split('\0')+git('ls-files','--others','--exclude-standard','-z').split('\0'))-{''})
errors=[];links=0;json_count=0
for f in files:
 p=r/f
 if not p.is_file():continue
 if f.endswith('.json'):
  try:json.loads(p.read_text());json_count+=1
  except ValueError as e:errors.append({'kind':'json','path':f,'error':str(e)})
 if not f.endswith('.md'):continue
 text=p.read_text()
 # Ignore fenced examples; their sample links need not resolve.
 text=re.sub(r'```.*?```','',text,flags=re.S)
 for m in re.finditer(r'!?\[[^\]\n]*\]\(([^)\n]+)\)',text):
  target=m[1].strip().split(' "')[0].strip('<>')
  if re.match(r'^[a-zA-Z][\w+.-]*:',target) or target.startswith('#'):continue
  dest=urllib.parse.unquote(target.split('#')[0])
  if not dest:continue
  links+=1
  if not (p.parent/dest).exists():errors.append({'kind':'link','path':f,'target':target})
manifest=json.loads((r/'docs/forensics/source-manifest.json').read_text())
for row in manifest['files']:
 p=r/row['path']
 if not p.is_file() or hashlib.sha256(p.read_bytes()).hexdigest()!=row['sha256']:errors.append({'kind':'source_hash','path':row['path']})
change=json.loads((r/'docs/releases/1.9/change-manifest.json').read_text())
for row in change['files']:
 p=r/row['path']
 if row['after_sha256'] and (not p.exists() or hashlib.sha256(p.read_bytes()).hexdigest()!=row['after_sha256']):errors.append({'kind':'change_hash','path':row['path']})
actual=set(git('diff','--name-only',change['baseline']).splitlines())|set(git('ls-files','--others','--exclude-standard').splitlines())
actual={x for x in actual if not x.startswith(('docs/forensics/','docs/reference/','docs/releases/1.9/'))}
missing=actual-{x['path'] for x in change['files']}
for f in sorted(missing):errors.append({'kind':'untraced_change','path':f})
check=subprocess.run(['git','diff','--check'],cwd=r,capture_output=True,text=True)
if check.returncode:errors.append({'kind':'whitespace','output':check.stdout})
result={'passed':not errors,'markdown_local_links_checked':links,'json_files_parsed':json_count,'source_hashes_checked':len(manifest['files']),'changed_file_hashes_checked':len(change['files']),'errors':errors,'method':'Local path targets, JSON parsing, SHA256 source/delta coverage and Git whitespace; not rendering, anchor validation, external URL availability or application execution.'}
print(json.dumps(result,indent=2))
raise SystemExit(0 if result['passed'] else 1)
