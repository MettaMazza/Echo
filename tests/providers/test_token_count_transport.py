from pathlib import Path
import json,subprocess,tempfile,threading,socket,struct,sqlite3
from http.server import BaseHTTPRequestHandler,ThreadingHTTPServer
import argparse
parser=argparse.ArgumentParser(description='Exercise native llama preflight against actual TCP disconnects and HTTP failures. No model or credentials required.')
parser.add_argument('--compiled-source',type=Path,required=True,help='Generated C for the current Discord agent, including llama_preflight')
args=parser.parse_args();temporary=tempfile.TemporaryDirectory();b=Path(temporary.name);n=b/'native';n.mkdir()
main=r'''
#define S(x) ((long long)(x))
int main(int argc,char**argv){init_ep_args(argc,argv);__ep_init_constants();long long db=archive_open(S(argv[1]));if(!db)return 2;long long result=llama_preflight(db,S("fixture"),S("request"),atoi(argv[2]),S("{\"model\":\"fixture\",\"messages\":[{\"role\":\"user\",\"content\":\"Count this exact read-only fixture.\"}]}"),32,-1,65536);printf("%s\n",(char*)archive_list_text(result,0));sql_close(db);return 0;}
'''
(n/'token_counter_probe.c').write_text(args.compiled_source.read_text().split('int main(int argc, char** argv) {')[0]+main)
subprocess.run(['clang','-w',str(n/'token_counter_probe.c'),'-o',str(n/'token_counter_probe'),'-lpthread','-DEP_HAS_SQLITE','-lsqlite3'],check=True)
results=[]
for mode,expected,expected_posts,retries in [('valid','ready',1,0),('reset','ready',2,1),('empty400','ready',2,1),('semantic','token-count-failed',1,0),('malformed','token-count-failed',1,0),('repeat-reset','token-count-failed',2,1)]:
 posts=[]
 class Handler(BaseHTTPRequestHandler):
  def log_message(self,*args):pass
  def response(self,code,body):
   data=body.encode();self.send_response(code);self.send_header('Content-Length',str(len(data)));self.send_header('Content-Type','application/json');self.send_header('Connection','close');self.end_headers();self.wfile.write(data)
  def do_GET(self):
   if self.path=='/v1/models':self.response(200,'{"data":[{"id":"fixture","meta":{"n_ctx":4096,"n_ctx_train":4096}}]}')
   elif self.path=='/props':self.response(200,'{"default_generation_settings":{"n_ctx":4096,"params":{"n_predict":-1}}}')
   else:self.response(404,'{}')
  def do_POST(self):
   assert self.path=='/v1/chat/completions/input_tokens'
   posts.append(self.rfile.read(int(self.headers['Content-Length'])))
   if mode=='repeat-reset' or (mode=='reset' and len(posts)==1):
    self.connection.setsockopt(socket.SOL_SOCKET,socket.SO_LINGER,struct.pack('ii',1,0));self.close_connection=True;return
   if mode=='empty400' and len(posts)==1:self.response(400,'');return
   if mode=='semantic':self.response(400,'{"error":"fixture semantic rejection"}');return
   if mode=='malformed':self.response(200,'{');return
   self.response(200,'{"input_tokens":12}')
 server=ThreadingHTTPServer(('127.0.0.1',0),Handler);thread=threading.Thread(target=server.serve_forever,daemon=True);thread.start()
 try:
  with tempfile.TemporaryDirectory() as tmp:
   db=Path(tmp)/'archive.sqlite';p=subprocess.run([str(n/'token_counter_probe'),str(db),str(server.server_port)],capture_output=True,text=True,timeout=30);assert p.returncode==0 and p.stdout.strip()==expected,(mode,p.returncode,p.stdout)
   c=sqlite3.connect(db);actual=c.execute("select count(*) from echo_events where kind='provider_token_count_retry'").fetchone()[0];assert actual==retries and len(posts)==expected_posts and all(x==posts[0] for x in posts),(mode,actual,len(posts));c.close()
   results.append({'case':mode,'status':expected,'exact_count_requests':len(posts),'durable_retries':actual,'passed':True})
 finally:server.shutdown();server.server_close();thread.join()
(b/'token-counter-test.json').write_text(json.dumps(results,indent=2));print(json.dumps(results,indent=2))
