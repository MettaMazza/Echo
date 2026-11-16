from pathlib import Path
import subprocess,re,json,os,argparse,tempfile
parser=argparse.ArgumentParser(description="Force collection on a second thread while building host execution profiles.")
parser.add_argument('--compiled-source',type=Path,required=True)
parser.add_argument('--baseline',type=Path)
args=parser.parse_args()
workspace=tempfile.TemporaryDirectory(prefix='echo-host-profile-gc-')
b=Path(workspace.name)
main=r'''
int main(int argc,char**argv){init_ep_args(argc,argv);__ep_init_constants();setenv("ECHO_HOST_PROTECTED_ROOTS","[\"/tmp/echo-fixture-protected\"]",1);setenv("ECHO_HOST_SECRET_ROOTS","[\"/tmp/echo-fixture-secrets\"]",1);for(int i=0;i<50;i++){char*p=(char*)host_execution_profile();if(!p||!strstr(p,"(deny file-write* (subpath \"/tmp/echo-fixture-protected\"))")||!strstr(p,"(deny file-read* file-write* (subpath \"/tmp/echo-fixture-secrets\"))"))return 3;}puts("PASS: policy roots survive forced collection; both deny rules preserved");return 0;}
'''
results=[]
for label,path in ([('before',args.baseline)] if args.baseline else [])+[('after',args.compiled_source)]:
 c=path.read_text().split('int main(int argc, char** argv) {')[0];pat=r'^long long provider_quote\([^\n]*\) \{.*?^\}';match=re.search(pat,c,re.M|re.S);assert match
 body=match[0];assert 'ep_gc_maybe_collect();' in body;body=body.replace('ep_gc_maybe_collect();','ep_gc_maybe_collect(); if (strstr((char*)value,"echo-fixture-")) { pthread_t collector; pthread_create(&collector,NULL,fixture_collect,NULL); pthread_join(collector,NULL); }',1);c=c[:match.start()]+'static void* fixture_collect(void* p) { pthread_mutex_lock(&ep_gc_mutex); ep_gc_collect_minor(); pthread_mutex_unlock(&ep_gc_mutex); return NULL; }\n'+body+c[match.end():]
 src=b/(label+'-test.c');src.write_text(c+main);exe=b/(label+'-test');subprocess.run(['clang','-w','-fsanitize=address',str(src),'-o',str(exe),'-lpthread','-DEP_HAS_SQLITE','-lsqlite3'],check=True)
 env=os.environ.copy();env['ASAN_OPTIONS']='detect_leaks=0:abort_on_error=0';p=subprocess.run([str(exe)],capture_output=True,text=True,env=env);receipt={'version':label,'exit':p.returncode,'use_after_free':'heap-use-after-free' in p.stderr,'passed':p.returncode==0};results.append(receipt);print(receipt,flush=True);(b/(label+'-diagnostic.txt')).write_text(p.stderr)
assert results[-1]['passed']
if args.baseline: assert results[0]['use_after_free']
workspace.cleanup()
