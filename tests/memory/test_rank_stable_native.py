"""Real native capture/reconstruction with deterministic ranking fixtures."""
from pathlib import Path
import subprocess,sys,tempfile
s=Path(sys.argv[1]).read_text()
for name in ['memory_semantic_prepare','memory_semantic_select','discord_memory_live_access_snapshot']:
 s=s.replace('long long '+name+'(long long database,','long long fixture_original_'+name+'(long long database,',1)
s=s.replace('int main(int argc, char** argv) {','int original_main(int argc, char** argv) {',1)
s+=r'''
#define S(x) ((long long)(x))
#define REQUIRE(c,n) if(!(c)){fprintf(stderr,"failed %d\n",n);return n;}
static int rank_second=0,live_access_calls=0;
long long discord_memory_live_access_snapshot(long long db,long long run,long long session,long long snapshot){live_access_calls++;return 1;}
long long memory_semantic_prepare(long long db,long long run){return 0;}
long long memory_semantic_select(long long db,long long run,long long claims){
 long long args=create_list();append_list(args,claims);
 long long row=history_collect_one(checkpoint_prepare(db,S(rank_second?
 "SELECT json_array(json(value)) FROM json_each(?1) WHERE json_extract(value,'$.quotation')='beta-marker' LIMIT 1;":
 "SELECT json_array(json(value)) FROM json_each(?1) WHERE json_extract(value,'$.quotation')='alpha-marker' LIMIT 1;"),args));
 return archive_list_text(row,1);
}
int main(int argc,char **argv){
 init_ep_args(argc,argv);__ep_init_constants();
 long long db=archive_open(S(argv[1]));REQUIRE(db,1);
 REQUIRE(!web_run_schema(db),2);
 REQUIRE(!strcmp((char*)delegation_schema(db),"applied"),20);
 REQUIRE(!web_pairing_invite(db,S("a")),3);
 REQUIRE(!web_device_pair(db,S("a"),S("a"),S("fixture"),S("secret-fixture")),4);
 long long status=group_create(db,S("a"),S("g"),S("discord"),S("server"),S("room"),S("silo:user-in-channel:fixture-room"));
 REQUIRE(!strcmp((char*)status,"created"),5);
 long long origin=group_route_submit(db,S("a"),S("g"),S("discord"),S("server"),S("room"),S("origin-run"),S("alpha-marker beta-marker"));
 REQUIRE(!strcmp((char*)archive_list_text(origin,0),"accepted"),6);
 long long st=agent_run_read(db,S("origin-run")),event=archive_list_text(st,1),src=archive_read_event(db,event),digest=archive_list_text(src,6);
 long long claim=knowledge_record(db,S("origin-run"),S("episodic"),S("fixture"),S("marker"),S("alpha-marker"),S("source-account"),event,digest,S(""),S(""),S(""));
 REQUIRE(strstr((char*)claim,"recorded:")== (char*)claim,7);
 char *identity=strdup((char*)claim+9);
 claim=knowledge_record(db,S("origin-run"),S("episodic"),S("fixture"),S("marker"),S("beta-marker"),S("source-account"),event,digest,S(""),S(""),S(""));
 REQUIRE(strstr((char*)claim,"recorded:")== (char*)claim,8);
 REQUIRE(!agent_cancel_run(db,S("origin-run")),9);
 group_route_submit(db,S("a"),S("g"),S("discord"),S("server"),S("room"),S("first"),S("First ranking"));
 REQUIRE(!memory_context_capture(db,S("first")),10);
 REQUIRE(live_access_calls==0,21);
 char *base=strdup((char*)memory_context_stable(db,S("first")));
 REQUIRE(strstr(base,"alpha-marker")&&!strstr(base,"beta-marker"),11);
 REQUIRE(!agent_cancel_run(db,S("first")),12);rank_second=1;
 group_route_submit(db,S("a"),S("g"),S("discord"),S("server"),S("room"),S("second"),S("Different ranking"));
 REQUIRE(!memory_context_capture(db,S("second")),13);
 REQUIRE(!strcmp(base,(char*)memory_context_stable(db,S("second"))),14);
 long long revision=memory_context_revision(db,S("second"));
 char *delta=(char*)memory_context_delta(db,S("second"),S("second"),revision);
 REQUIRE(strstr(delta,"beta-marker")&&!strstr(delta,"alpha-marker"),15);
 status=knowledge_visibility_set(db,S("second"),S("forget-alpha"),S(identity),0,S("forgotten"),S("fixture withdrawal"));
 REQUIRE(!strcmp((char*)status,"recorded"),16);
 REQUIRE(!memory_context_capture(db,S("second")),17);
 REQUIRE(!strstr((char*)memory_context_stable(db,S("second")),"alpha-marker"),18);
 REQUIRE(live_access_calls==0,22);
 status=group_create(db,S("a"),S("cross-group"),S("discord"),S("server"),S("other-room"),S("silo:user-in-channel:fixture-other"));
 REQUIRE(!strcmp((char*)status,"created"),23);
 origin=group_route_submit(db,S("a"),S("cross-group"),S("discord"),S("server"),S("other-room"),S("cross-run"),S("Cross-channel fixture"));
 REQUIRE(!strcmp((char*)archive_list_text(origin,0),"accepted"),24);
 REQUIRE(!discord_memory_prepare(db,S("second")),25);
 REQUIRE(live_access_calls==1,26);
 REQUIRE(!archive_verify_all(db),19);
 puts("PASS: native capture retains selected prefix across changed rankings, appends new selected recall, and rebases after forgetting; same-audience recall avoids redundant live checks while cross-channel recall still requires them; archive verified");return 0;
}
'''
p=Path(tempfile.mkdtemp(prefix='echo-rank-prefix-')).resolve();(p/'test.c').write_text(s)
with (p/'build.log').open('w') as log:subprocess.run(['clang',str(p/'test.c'),'-o',str(p/'test'),'-lpthread','-DEP_HAS_SQLITE','-lsqlite3'],stdout=log,stderr=log,check=True)
r=subprocess.run([str(p/'test'),str(p/'archive.sqlite')],capture_output=True,text=True);print(p);print(r.stdout);print(r.stderr);raise SystemExit(r.returncode)
