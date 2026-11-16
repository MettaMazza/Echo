from pathlib import Path
import subprocess,sys,tempfile
s=Path(sys.argv[1]).read_text()
s=s.replace('long long discord_access_get_receipted(long long library,','long long original_discord_access_get_receipted(long long library,',1)
s=s.replace('int main(int argc, char** argv) {','int original_main(int argc, char** argv) {',1)
s+=r'''
static int requests=0,fail=0;
long long discord_access_get_receipted(long long library,long long token,long long journal,long long directory,long long route,long long bucket,long long major,long long journal_receipt) {
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

 sql_execute(j,(long long)"CREATE TABLE discord_gateway_health(singleton INTEGER PRIMARY KEY,generation TEXT,status TEXT,observed_ms INTEGER);");
 char health[300];snprintf(health,sizeof health,"INSERT INTO discord_gateway_health VALUES(1,'connection-a','heartbeat-ack',%lld)",ep_time_now_ms());sql_execute(j,(long long)health);
 #define STABLE(b) discord_access_snapshot_get(0,0,j,0,(long long)"/channels/300",(long long)"GET:channel",(long long)"300",b)
 #define MEMBER(b) discord_access_snapshot_get(0,0,j,0,(long long)"/guilds/200/members/900",(long long)"GET:member",(long long)"200",b)
 requests=0;batch=create_list();STABLE(batch);MEMBER(batch);MEMBER(batch);if(requests!=2)return 10;
 batch=discord_access_continuation_snapshot(batch);STABLE(batch);MEMBER(batch);if(requests!=3)return 11;
 discord_inbox_record(j,(long long)"audience:revoke",(long long)"CHANNEL_UPDATE",(long long)"{}");
 batch=discord_access_continuation_snapshot(batch);STABLE(batch);if(requests!=4)return 12;
 sql_execute(j,(long long)"UPDATE discord_gateway_health SET generation='connection-b'");
 batch=discord_access_continuation_snapshot(batch);STABLE(batch);if(requests!=5)return 13;
 sql_execute(j,(long long)"UPDATE discord_gateway_health SET status='disconnected'");
 batch=discord_access_continuation_snapshot(batch);STABLE(batch);if(requests!=6)return 14;
 batch=discord_access_continuation_snapshot(batch);STABLE(batch);if(requests!=7)return 15;
 sql_execute(j,(long long)"UPDATE discord_gateway_health SET status='heartbeat-ack'");
 batch=create_list();STABLE(batch);if(requests!=8)return 16;
 set_list(get_list(batch,0),5,ep_time_now_ms()-120001);
 batch=discord_access_continuation_snapshot(batch);STABLE(batch);if(requests!=9)return 17;
 sql_execute(j,(long long)"UPDATE discord_gateway_health SET observed_ms=0");
 batch=discord_access_continuation_snapshot(batch);STABLE(batch);if(requests!=10)return 18;
 puts("PASS: cross-pass audience reuse; membership refreshed; permission changes, reconnect, disconnect, stale heartbeat and expiry invalidate; failed requests never cached");return 0;
}
'''
p=Path(tempfile.mkdtemp(prefix='echo-access-snapshot-')).resolve();(p/'test.c').write_text(s)
with (p/'build.log').open('w') as log:subprocess.run(['clang',str(p/'test.c'),'-o',str(p/'test'),'-lpthread','-DEP_HAS_SQLITE','-lsqlite3'],stdout=log,stderr=log,check=True)
r=subprocess.run([str(p/'test'),str(p/'journal.sqlite')],capture_output=True,text=True);print(p);print(r.stdout);print(r.stderr);raise SystemExit(r.returncode)
