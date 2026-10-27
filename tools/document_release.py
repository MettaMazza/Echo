#!/usr/bin/env python3
"""Rebuild public source references and the 1.9 baseline delta; never read live data.
Documentation tooling only. Echo application and orchestration remain Ernos.
"""
from pathlib import Path
import collections, datetime, hashlib, json, os, re, subprocess
ROOT = Path(__file__).resolve().parents[1]
BASE = os.environ.get('ECHO_DOC_BASE', '3466df5')
NOW = datetime.datetime.now(datetime.timezone.utc).isoformat()
def git(*args):
    return subprocess.check_output(['git', *args], cwd=ROOT)
def sha(b): return hashlib.sha256(b).hexdigest()
def save(path, value):
    p=ROOT/path; p.parent.mkdir(parents=True,exist_ok=True)
    p.write_text(value if isinstance(value,str) else json.dumps(value,indent=2,ensure_ascii=False)+'\n')
def symbols(text):
    result={}; lines=text.splitlines(keepends=True)
    starts=[(i,re.match(r'(?:external )?define (\w+)',s)) for i,s in enumerate(lines)]
    starts=[(i,m[1]) for i,m in starts if m]
    for n,(i,name) in enumerate(starts):
        end=starts[n+1][0] if n+1<len(starts) else len(lines)
        result[name]={'line':i+1,'signature':lines[i].strip(),'sha256':sha(''.join(lines[i:end]).encode())}
    return result
known=sorted(set(git('ls-files','-z').decode().split('\0')+git('ls-files','--others','--exclude-standard','-z').decode().split('\0'))-{''})
texts={p:(ROOT/p).read_text() for p in known if p.endswith(('.ep','.c','.h','.py','.sh')) and (ROOT/p).is_file() and not p.startswith('engine/ernos-overrides/')}
refs=collections.defaultdict(set)
for p,t in texts.items():
    if p.startswith('tests/'):
        for name in set(re.findall(r'\b(\w+)\s*\(',t)): refs[name].add(p)
files=[]; tools=[]; env=collections.defaultdict(list); routes=[]
for p,t in sorted(texts.items()):
    b=(ROOT/p).read_bytes(); defs=symbols(t); imports=[]
    for i,line in enumerate(t.splitlines(),1):
        m=re.match(r'import "([^"]+)"',line)
        if m:
            dest=os.path.normpath(str(Path(p).parent/(m[1]+'.ep')))
            imports.append({'name':m[1],'resolved':dest if (ROOT/dest).is_file() else None})
        for v in re.findall(r'get_env\("([^"\n]+)"\)',line): env[v].append({'path':p,'line':i})
        if p=='app/web/routes.ep':
            for v in re.findall(r'"(/api/[^"\n]+)"',line): routes.append({'path':v,'source':p,'line':i})
    row={'path':p,'bytes':len(b),'lines':len(t.splitlines()),'sha256':sha(b),'definitions':[dict(name=n,**d) for n,d in defs.items()],'imports':imports,'tests_referencing_definitions':sorted(set().union(*(refs[n] for n in defs)) if defs else set()),'native_entrypoint':'main' in defs}
    files.append(row)
    if p.startswith('app/'):
        for literal in re.findall(r'"(?:\\.|[^"\\])*"',t):
            try: value=json.loads(literal)
            except ValueError: continue
            for m in re.finditer(r'\{"type"\s*:\s*"function"',value):
                try:
                    obj,_=json.JSONDecoder().raw_decode(value[m.start():]); tools.append({'source':p,**obj['function']})
                except (ValueError,KeyError): pass
save('docs/forensics/source-manifest.json',{'observed_utc':NOW,'method':'Complete byte reads, SHA256, lexical definitions/imports/test references. Includes native bridges and external test harnesses. Not a full semantic audit or executed-test claim.','files':files})
summary={'observed_utc':NOW,'application_files':sum(p.startswith('app/') for p in texts),'test_and_fixture_files':sum(p.startswith('tests/') for p in texts),'source_files':len(files),'source_lines':sum(x['lines'] for x in files),'definition_signatures':sum(len(x['definitions']) for x in files),'tool_names':len(set(x['name'] for x in tools))}
save('docs/forensics/inventory-summary.json',summary)
for root,title in [('app','APPLICATION'),('tests','TESTS')]:
    lines=[f'# {title.title()} source reference — Echo 1.9','', 'Complete lexical source inventory. Test presence is not a passing result. [Current release](../releases/1.9/README.md).','']
    for f in files:
        if not f['path'].startswith(root+'/'): continue
        p=f['path']; lines += [f'## `{p}`','',f'[{p}](../../{p}) · {f["lines"]} lines · SHA-256 `{f["sha256"]}`','']
        lines+=['Imports: '+(', '.join(f'[{v["name"]}](../../{v["resolved"]})' if v['resolved'] else '`'+v['name']+'` (toolchain)' for v in f['imports']) or 'none')+'.','','```text']
        lines += [str(d['line'])+': '+d['signature'] for d in f['definitions']]+['```',''] if f['definitions'] else ['Non-Ernos support/fixture; inspect linked source.','```','']
        if f['tests_referencing_definitions']: lines+=['Test references: '+', '.join(f'[{p}](../../{p})' for p in f['tests_referencing_definitions'])+'.','']
    save('docs/reference/'+title+'.md','\n'.join(lines))
unique={(v['source'],v['name'],json.dumps(v.get('parameters'),sort_keys=True)):v for v in tools}
save('docs/reference/TOOL-SCHEMAS.json',{'observed_utc':NOW,'method':'Literal schemas, not evidence that a capability is enabled or accepted. Dynamic authorization remains in source.','tools':list(unique.values())})
save('docs/reference/ENVIRONMENT.json',{'observed_utc':NOW,'values_excluded':True,'variables':dict(sorted(env.items()))})
save('docs/reference/HTTP-ROUTES.json',{'observed_utc':NOW,'method':'Literal web routes; not endpoint acceptance.','routes':routes})
basepaths=set(git('ls-tree','-r','--name-only',BASE).decode().splitlines())
changed=set(git('diff','--name-only',BASE).decode().splitlines())|set(known)-basepaths
rows=[]
for p in sorted(changed):
    if p.startswith(('docs/forensics/','docs/reference/','docs/releases/1.9/')): continue
    old=git('show',BASE+':'+p) if p in basepaths else b''
    new=(ROOT/p).read_bytes() if (ROOT/p).is_file() else b''
    if old==new: continue
    before=symbols(old.decode(errors='replace')) if p.endswith('.ep') else {}
    after=symbols(new.decode(errors='replace')) if p.endswith('.ep') else {}
    names=sorted(n for n in before.keys()|after.keys() if before.get(n,{}).get('sha256')!=after.get(n,{}).get('sha256'))
    rows.append({'path':p,'status':'added' if p not in basepaths else 'modified' if new else 'deleted','before_sha256':sha(old) if p in basepaths else None,'after_sha256':sha(new) if new else None,'before_bytes':len(old),'after_bytes':len(new),'changed_definitions':names,'test_references':sorted(set().union(*(refs[n] for n in names)) if names else set())})
save('docs/releases/1.9/change-manifest.json',{'observed_utc':NOW,'baseline':BASE,'baseline_ref':'origin/release/echo-1.9 at 2026-10-27 task start','method':'Every changed publishable file compared byte-for-byte to last pushed release tree; per-definition lexical changes for Ernos. Generated references/forensic/release files excluded from recursive self-hashing; tracked by Git. Test references are lexical, not pass evidence.','files':rows})
lines=['# File-by-file change ledger — 1.9','',f'Baseline: `{BASE}`. Each row links to the complete current source; exact before/after hashes and changed definitions are in [the manifest](change-manifest.json). Generated reference files are regenerated separately and covered by Git history.','', '| File | Change | Changed Ernos definitions |','|---|---|---:|']
lines += [f'| [{x["path"]}](../../../{x["path"]}) | {x["status"]} | {len(x["changed_definitions"])} |' for x in rows]
save('docs/releases/1.9/FILE-CHANGES.md','\n'.join(lines)+'\n')
support=[]
for p in known:
    if p in texts or p.startswith(('docs/forensics/','docs/reference/','docs/releases/')) or not (ROOT/p).is_file(): continue
    b=(ROOT/p).read_bytes();support.append({'path':p,'bytes':len(b),'sha256':sha(b)})
save('docs/forensics/repository-manifest.json',{'observed_utc':NOW,'method':'Supporting files, excluding self-referential generated indexes and release documents.','files':support})
print(json.dumps({**summary,'changed_files':len(rows)},indent=2))
