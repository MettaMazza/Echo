from pathlib import Path
import subprocess,json,sys,tempfile,os
workspace=tempfile.TemporaryDirectory(prefix='echo-latency-native-')
p=Path(workspace.name).resolve()
s=Path(sys.argv[1]).read_text().replace('int main(int argc, char** argv) {','int original_main(int argc, char** argv) {',1)
# Count real filesystem operations in this test driver only.
probe=r'''
#include <sys/stat.h>
static int probe_mkdir_count=0,probe_flush_count=0,probe_race=0,probe_fail_flush=0;
static int probe_mkdir(int fd,const char *name,int mode){probe_mkdir_count++;int r=mkdirat(fd,name,mode);if(probe_race&&r==0){errno=EEXIST;return -1;}return r;}
static int probe_flush(int fd){probe_flush_count++;if(probe_fail_flush){errno=EIO;return -1;}return fsync(fd);}
'''
pos=s.index('long long ep_dlsym(long long handle, long long name) {',s.index('// Select:'))
# The Windows/WASM alternatives may precede the host implementation; inject
# declarations once and intercept the actual dlsym branch only.
s=probe+s if False else s
needle='    void* sym = dlsym((void*)handle, (const char*)name);'
assert needle in s
s=s.replace(needle,'    if (!strcmp((char*)name,"mkdirat")) return (long long)probe_mkdir;\n    if (!strcmp((char*)name,"fsync")) return (long long)probe_flush;\n'+needle,1)
pos=s.rfind('long long ep_dlsym(long long handle, long long name) {');s=s[:pos]+probe+s[pos:]
s+=r'''
static long long fixture(const char *path){FILE*f=fopen(path,"rb");if(!f)exit(90);fseek(f,0,SEEK_END);long n=ftell(f);rewind(f);char*s=calloc(n+1,1);fread(s,1,n,f);fclose(f);return(long long)s;}
#define CHECK(x) do{if(!(x)){fprintf(stderr,"check failed line %d\n",__LINE__);return 1;}}while(0)
int main(int argc,char **argv){init_ep_args(argc,argv);__ep_init_constants();if(!strcmp(argv[1],"render")){puts((char*)group_memory_selected_context(fixture(argv[2])));return 0;}long long lib=ep_dlopen((long long)"/usr/lib/libSystem.B.dylib");long long root=workspace_root_open(lib,(long long)argv[2]);CHECK(root>=0);long long child;probe_mkdir_count=probe_flush_count=0;for(int i=0;i<5;i++){child=workspace_directory_open(lib,root,(long long)"a/b",1);CHECK(child>=0);net_close(child);}CHECK(probe_mkdir_count==0&&probe_flush_count==0);child=workspace_directory_open(lib,root,(long long)"a/new/x",1);CHECK(child>=0&&probe_mkdir_count==2&&probe_flush_count==2);net_close(child);probe_mkdir_count=probe_flush_count=0;CHECK(workspace_directory_open(lib,root,(long long)"link/x",1)<0);CHECK(workspace_directory_open(lib,root,(long long)"plain/x",1)<0);CHECK(workspace_directory_open(lib,root,(long long)"../escape",1)<0);CHECK(workspace_directory_open(lib,root,(long long)"absent/x",0)<0);CHECK(probe_mkdir_count==0&&probe_flush_count==0);probe_race=1;child=workspace_directory_open(lib,root,(long long)"race",1);CHECK(child>=0&&probe_flush_count==1);net_close(child);probe_race=0;probe_fail_flush=1;CHECK(workspace_directory_open(lib,root,(long long)"flush-failure",1)<0);probe_fail_flush=0;net_close(root);puts("PASS: existing paths perform zero writes; missing paths durable; races, symlinks, files, traversal and flush failures handled");return 0;}
'''
(p/'latency-test.c').write_text(s)
with (p/'latency-test-build.log').open('w') as f:subprocess.run(['clang',str(p/'latency-test.c'),'-o',str(p/'latency-test'),'-lpthread','-DEP_HAS_SQLITE','-lsqlite3'],stdout=f,stderr=f,check=True)
evidence=p;tmp=Path(tempfile.mkdtemp(prefix='native-regression-',dir=evidence));root=tmp/'directories';(root/'a/b').mkdir(parents=True);(root/'outside').mkdir();(root/'link').symlink_to(root/'outside',target_is_directory=True);(root/'plain').write_text('file')
print(subprocess.check_output([str(p/'latency-test'),'directories',str(root)],text=True).strip())
def render(records):
 f=tmp/'records.json';f.write_text(json.dumps(records,ensure_ascii=False));return subprocess.check_output([str(p/'latency-test'),'render',str(f)],text=True).rstrip('\n')
marker='\nAdditional query-relevant recalled records, using their own references and shared_fields:\n'
def decode(text):
 if not text:return []
 if 'Memories: ' not in text:return []
 chunks=[text.split('Memories: ',1)[1]]
 if marker in text:chunks.append(text.split(marker,1)[1])
 records=[]
 for chunk in chunks:
  enc,_=json.JSONDecoder().raw_decode(chunk)
  if enc==[]:continue
  for row in enc['records']:
   row=dict(enc['shared_fields'],**row)
   for k in enc['reference_fields']:
    if k in row:
     v=row[k]
     if isinstance(v,str):row[k]=enc['references'][v]
     elif isinstance(v,dict) and 'literal' in v:row[k]=v['literal']
   records.append(row)
 return records
records=[dict(id='claim:'+str(i),identity='claim:'+str(i),source_event='event:'+str(i),source_digest='a'*64,source_session='session:one',source_sequence=i,source_recorded_at='unix-ms:1000',source_recorded_utc='1970-01-01T00:00:01Z',occurred='unix-ms:1000',source_kind='user',category='preference' if i%2==0 else 'fact',quotation='Verbatim quotation — r0 is literal text.',evidence='source-account') for i in range(6)];text=render(records)
canon=lambda rs:sorted(json.dumps(x,sort_keys=True,ensure_ascii=False) for x in rs)
assert canon(decode(text))==canon(records)
standing=lambda r:r.get('source_kind')=='user' and r.get('category') in ['preference','correction','relationship']
a=[r for r in records if standing(r)];b=[r for r in records if not standing(r)];assert a and b
changed=[dict(b[0],quotation='A different query result — exact Unicode retained.',id='new-source',identity='new-source')]
other=render(a+changed);assert text.split(marker)[0]==other.split(marker)[0]
for fixture_records in [a,b,[],[dict(a[0],source_recorded_at={'literal':'r0'},source_session=None)]]:
 assert canon(decode(render(fixture_records)))==canon(fixture_records)
assert render({'invalid':'object'})==''
assert canon(decode(other))==canon(a+changed)
print('PASS: every record and provenance field round-trips exactly; standing prefix unchanged across query changes; empty and category-only cases preserved')
result={'exit':0,'records_roundtripped':len(records),'standing_records':len(a),'query_records':len(b),'standing_prefix_chars':len(text.split(marker)[0]),'total_chars':len(text),'filesystem_cases':8,'fixture':str(tmp)};(p/'test-result.json').write_text(json.dumps(result,indent=2));print(json.dumps(result))
