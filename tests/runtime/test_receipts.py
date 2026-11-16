from pathlib import Path
import subprocess,tempfile,sys
p=Path(__file__).resolve().parent
s=Path(sys.argv[1]).read_text().replace('int main(int argc, char** argv) {','int original_main(int argc, char** argv) {',1)
needle='    void* sym = dlsym((void*)handle, (const char*)name);';assert needle in s
s=s.replace(needle,'    if (!strcmp((char*)name,"fsync")) return (long long)probe_flush;\n'+needle,1)
pos=s.rfind('long long ep_dlsym(long long handle, long long name) {')
s=s[:pos]+'static int probe_flush_count=0; static int probe_flush(int fd){probe_flush_count++;return fsync(fd);}\n'+s[pos:]+(p/'receipt_fixture.c').read_text()
with tempfile.TemporaryDirectory(prefix='echo-receipts-') as directory:
 root=Path(directory);(root/'test.c').write_text(s)
 subprocess.run(['clang','-dynamiclib',str(p/'mock_curl.c'),'-o',str(root/'mock.dylib')],check=True)
 with (root/'compile.log').open('w') as f:r=subprocess.run(['clang',str(root/'test.c'),'-o',str(root/'test'),'-lpthread','-DEP_HAS_SQLITE','-lsqlite3'],stdout=f,stderr=f)
 if r.returncode:raise RuntimeError((root/'compile.log').read_text())
 r=subprocess.run([str(root/'test'),str(root/'mock.dylib'),str(root/'journal.sqlite')],text=True,capture_output=True,timeout=30)
 print(r.stdout);print(r.stderr)
 assert r.returncode==0,r.returncode
