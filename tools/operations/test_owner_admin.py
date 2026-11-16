from pathlib import Path
import tempfile,json,hashlib
s=Path(__file__).with_name('echo_owner_admin.py').read_text();ns={'__name__':'fixture'};exec(compile(s,'owner-admin.py','exec'),ns)
sha=lambda b:hashlib.sha256(b).hexdigest()
with tempfile.TemporaryDirectory() as tmp:
 r=Path(tmp);h=r/'administration';h.mkdir();(r/'config').mkdir();(r/'releases').mkdir();source=r/'source';(source/'app/discord').mkdir(parents=True);(source/'app/discord/rest.ep').write_bytes(b'old');snapshot=r/'snapshot';(snapshot/'files/app/discord').mkdir(parents=True);(snapshot/'files/app/discord/rest.ep').write_bytes(b'new')
 active=json.dumps({'source_root':str(source)}).encode();(r/'releases/active.json').write_bytes(active)
 a={'protected_sources':[{'path':'app/discord/rest.ep','sha256':sha(b'old')}],'tests':['keep me'],'files':['keep me too']};old=json.dumps(a,indent=2).encode();(r/'config/acceptance.json').write_bytes(old)
 pipeline={'acceptance_spec':str(r/'config/acceptance.json'),'acceptance_spec_sha256':sha(old),'unrelated':'preserved'};oldp=json.dumps(pipeline,indent=2).encode();(r/'config/pipeline.json').write_bytes(oldp)
 (snapshot/'package.json').write_text(json.dumps({'active_sha256':sha(active),'files':{'app/discord/rest.ep':sha(b'new')}}));ns.update(ROOT=r,HOME=h)
 ns['owner_pin_prepare'](snapshot);new=(r/'config/acceptance.json').read_bytes();assert json.loads(new)['tests']==a['tests'];assert json.loads(new)['files']==a['files'];assert json.loads((r/'config/pipeline.json').read_bytes())['acceptance_spec_sha256']==sha(new)
 ns['owner_pin_recover']();assert (r/'config/acceptance.json').read_bytes()==old;assert (r/'config/pipeline.json').read_bytes()==oldp
 ns['owner_pin_prepare'](snapshot);(source/'app/discord/rest.ep').write_bytes(b'new');ns['owner_pin_recover']();assert (r/'config/acceptance.json').read_bytes()==new;assert json.loads((h/'owner-pin-update.json').read_bytes())['state']=='committed'
print('PASS: exact rollback, committed matching pins, unchanged fixed tests, durable recovery journal')
