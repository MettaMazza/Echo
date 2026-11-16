from pathlib import Path
import subprocess,json,tempfile,sys,hashlib
p=Path(__file__).resolve().parent
s=Path(sys.argv[1]).read_text().replace('int main(int argc, char** argv) {','int original_main(int argc, char** argv) {',1)
s+=r'''
static long long read_test(const char *p){FILE *f=fopen(p,"rb");if(!f)exit(90);fseek(f,0,SEEK_END);long n=ftell(f);rewind(f);char *s=calloc(n+1,1);if(fread(s,1,n,f)!=n)exit(91);fclose(f);return (long long)s;}
int main(int argc,char **argv){init_ep_args(argc,argv);__ep_init_constants();
 if(!strcmp(argv[1],"body")){long long sources=read_test(argv[2]),candidate=read_test(argv[3]);long long body=recap_review_body((long long)argv[4],sources,candidate);puts((char*)recap_structured_body(body,(long long)"{\"adapter\":\"llama.cpp\"}",2));return 0;}
 if(!strcmp(argv[1],"patchbody")){puts((char*)recap_patch_body((long long)argv[4],read_test(argv[2]),read_test(argv[3]),(long long)"{}"));return 0;}
 if(!strcmp(argv[1],"same")){printf("%lld\n",recap_review_same_issues(read_test(argv[2]),read_test(argv[3])));return 0;}
 long long sources=read_test(argv[2]),candidate=read_test(argv[3]),raw=read_test(argv[4]);long long report=recap_bound_review_expand(sources,candidate,raw);puts((char*)recap_review_validate(sources,candidate,report));return 0;}
'''
(p/'test.c').write_text(s)
with (p/'test-build.log').open('w') as f:subprocess.run(['clang',str(p/'test.c'),'-o',str(p/'test_protocol'),'-lpthread','-DEP_HAS_SQLITE','-lsqlite3'],stdout=f,stderr=f,check=True)
tmp=Path(tempfile.mkdtemp(prefix='echo-recap-protocol-'))
def save(name,v):f=tmp/name;f.write_text(json.dumps(v,separators=(',',':')));return f
sources={'source_end':'e2','records':[{'event':'e1'},{'event':'e2'}]}
candidate={'source_end':'e2','summary':'Structured continuity state','state':{'facts':[{'text':'Echo claimed X; Maria disputed it.','sources':['e1','e2']}]}}
sf=save('sources',sources);cf=save('candidate',candidate)
base={'candidate_digest':hashlib.sha256(cf.read_bytes()).hexdigest(),'source_end':'e2','issues':[],'coverage':'complete','source_digest':hashlib.sha256(sf.read_bytes()).hexdigest()}
issue={'event':'e1','index':0,'category':'facts','entry_index':0,'candidate_text':candidate['state']['facts'][0]['text'],'explanation':'Missing the later correction.','correction':'Preserve the correction from e1.','verdict':'omitted'}
def check(name,r,want):
 f=save(name,r);out=subprocess.check_output([str(p/'test_protocol'),'validate',str(sf),str(cf),str(f)],text=True).strip();assert out==want,(name,out,want);print('PASS',name,flush=True)
check('covered',base,'review-pass')
r=dict(base,issues=[issue]);check('genuine-defect',r,'review-repair')
check('reference-only-defect',dict(base,issues=[{k:v for k,v in issue.items() if k not in ['index','candidate_text']}]),'review-repair')
check('wrong-source-position',dict(r,issues=[dict(issue,event='e2')]),'invalid-review')
check('obsolete-candidate-text',dict(r,issues=[dict(issue,candidate_text='Old claim X is verified.')]),'invalid-review')
check('missing-correction',dict(r,issues=[dict(issue,correction='')]),'invalid-review')
check('incomplete-coverage',dict(r,coverage='incomplete'),'invalid-review')
check('duplicate-issue',dict(r,issues=[issue,issue]),'invalid-review')
check('wrong-source-digest',dict(base,source_digest='bad'),'invalid-review')
check('legacy-contradictory-review',{'candidate_digest':base['candidate_digest'],'source_end':'e2','assessments':[{'verdict':'omitted','explanation':'Already covered.'},'covered'],'findings':[]},'invalid-review')
a=save('a',{'issue_keys':['e1:facts:0:omitted','e2:facts:1:contradicted']});b=save('b',{'issue_keys':['e2:facts:1:contradicted','e1:facts:0:omitted'],'different_wording':'yes'})
assert subprocess.check_output([str(p/'test_protocol'),'same',str(a),str(b)],text=True).strip()=='1';print('PASS order-independent repeated-defect detection')
body=json.loads(subprocess.check_output([str(p/'test_protocol'),'body',str(sf),str(cf),'fixture-model'],text=True))
assert body['chat_template_kwargs']['enable_thinking'] and body['reasoning_effort']=='high'
sc=body['response_format']['json_schema']['schema'];assert sc['properties']['coverage']['const']=='complete' and sc['properties']['source_digest']['enum']==[hashlib.sha256(sf.read_bytes()).hexdigest()]
assert 'verdicts' not in sc['properties']
assert 'index' not in sc['properties']['issues']['items']['properties'] and 'candidate_text' not in sc['properties']['issues']['items']['properties']
assert all('Zero-based source positions' not in m.get('content','') for m in body['messages'])
assert 'maximum' not in sc['properties']['issues']['items']['properties']['entry_index'];print('PASS reasoning and exhaustive review schema')
assert json.loads(body['messages'][-1]['content'].split('\n',1)[1])==sc
assert body['messages'][1]['content']==sf.read_text()
print('PASS model-visible review schema equals decoder schema')
patch=json.loads(subprocess.check_output([str(p/'test_protocol'),'patchbody',str(sf),str(cf),'fixture-model'],text=True))
psc=patch['response_format']['json_schema']['schema']
assert json.loads(patch['messages'][-1]['content'].split('\n',1)[1])==psc
assert set(psc['required'])=={'base_digest','edits'}
assert set(psc['properties']['edits']['items']['required'])=={'category','index','operation','entry'}
assert patch['messages'][1]['content']==sf.read_text()
assert patch['reasoning_effort']=='high' and patch['chat_template_kwargs']['enable_thinking']
print('PASS model-visible patch schema equals decoder schema; source and reasoning preserved')
(p/'test-result.json').write_text(json.dumps({'exit':0,'checks':14,'fixture':str(tmp)},indent=2))
