from pathlib import Path
import subprocess,sys,tempfile
s=Path(sys.argv[1]).read_text()
s=s.replace('long long discord_access_get(long long library,','long long original_discord_access_get(long long library,',1)
s=s.replace('int main(int argc, char** argv) {','int original_main(int argc, char** argv) {',1)
s+=r'''
static int requests=0,fail=0;
long long discord_access_get(long long library,long long token,long long journal,long long directory,long long route,long long bucket,long long major) {
 requests++; long long result=history_status((long long)(fail?"rate-limited":"observed"));
 append_list(result,(long long)"{}");append_list(result,(long long)"fixture-digest");return result;
}
int main(int argc,char **argv) {
 init_ep_args(argc,argv);__ep_init_constants();
 long long j=discord_store_open((long long)argv[1],(long long)"900"),batch=create_list();
 if(!j)return 1;
 #define GET(b,r) discord_access_snapshot_get(0,0,j,0,(long long)r,(long long)"GET",(long long)"200",b)
 GET(batch,"/guilds/200"); GET(batch,"/guilds/200");if(requests!=1)return 2;
 GET(batch,"/guilds/201");if(requests!=2)return 3;
 GET(create_list(),"/guilds/200");if(requests!=3)return 4;
 discord_inbox_record(j,(long long)"audience:test",(long long)"GUILD_ROLE_UPDATE",(long long)"{\"guild_id\":\"200\"}");
 GET(batch,"/guilds/200");if(requests!=4)return 5;
 fail=1;GET(batch,"/failed");GET(batch,"/failed");if(requests!=6)return 6;
 fail=0;GET(batch,"/failed");GET(batch,"/failed");if(requests!=7)return 7;
 puts("PASS: same-pass duplicate GET reused; routes isolated; new pass refetches; permission updates invalidate; failures never cached");return 0;
}
'''
p=Path(tempfile.mkdtemp(prefix='echo-access-snapshot-')).resolve();(p/'test.c').write_text(s)
with (p/'build.log').open('w') as log:subprocess.run(['clang',str(p/'test.c'),'-o',str(p/'test'),'-lpthread','-DEP_HAS_SQLITE','-lsqlite3'],stdout=log,stderr=log,check=True)
r=subprocess.run([str(p/'test'),str(p/'journal.sqlite')],capture_output=True,text=True);print(p);print(r.stdout);print(r.stderr);raise SystemExit(r.returncode)
