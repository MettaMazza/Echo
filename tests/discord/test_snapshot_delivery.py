"""Native scheduling gate: delivery precedes full external-drive snapshots."""
from pathlib import Path
import subprocess,sys,tempfile
s=Path(sys.argv[1]).read_text().replace('int main(int argc, char** argv) {','int original_main(int argc, char** argv) {',1)
s+=r'''
#define S(x) ((long long)(x))
#define CHECK(c,n) if(!(c)){fprintf(stderr,"failed %d\n",n);return n;}
int main(int argc,char **argv){
 init_ep_args(argc,argv);__ep_init_constants();long long db=sql_open(S(argv[1]));CHECK(db,1);
 CHECK(!sql_execute(db,S("CREATE TABLE discord_outbox(identity TEXT PRIMARY KEY,inbox TEXT,state TEXT);CREATE TABLE discord_delivery_retry_clock(identity TEXT PRIMARY KEY,next_ms INTEGER);")),2);
 CHECK(discord_snapshot_delivery_pending(db,S("a"))==0,3);
 sql_execute(db,S("INSERT INTO discord_outbox VALUES('activity-page:old','a','pending'),('discord-message:other:final:0','b','pending');"));
 CHECK(discord_snapshot_delivery_pending(db,S("a"))==0,4);
 sql_execute(db,S("INSERT INTO discord_outbox VALUES('discord-message:a:final:0','a','pending'),('discord-message:a:final:1','a','pending');"));
 CHECK(discord_snapshot_delivery_pending(db,S("a"))==1,5);
 sql_execute(db,S("UPDATE discord_outbox SET state='sent' WHERE identity='discord-message:a:final:0';"));
 CHECK(discord_snapshot_delivery_pending(db,S("a"))==1,6);
 sql_execute(db,S("UPDATE discord_outbox SET state='sending' WHERE identity='discord-message:a:final:1';"));
 CHECK(discord_snapshot_delivery_pending(db,S("a"))==1,7);
 sql_execute(db,S("UPDATE discord_outbox SET state='uncertain' WHERE identity='discord-message:a:final:1';"));
 CHECK(discord_snapshot_delivery_pending(db,S("a"))==0,8);
 sql_execute(db,S("UPDATE discord_outbox SET state='pending' WHERE identity='discord-message:a:final:1';INSERT INTO discord_delivery_retry_clock VALUES('discord-message:a:final:1',123);"));
 CHECK(discord_snapshot_delivery_pending(db,S("a"))==0,9);
 sql_execute(db,S("UPDATE discord_outbox SET state='sent' WHERE inbox='a' AND identity LIKE 'discord-message:%';"));
 CHECK(discord_snapshot_delivery_pending(db,S("a"))==0,10);
 puts("PASS: wait for every ready reply chunk and in-flight POST; preserve backup on retry/uncertainty; ignore activity backlog and other conversations");return sql_close(db);
}
'''
p=Path(tempfile.mkdtemp(prefix='echo-snapshot-delivery-')).resolve();(p/'test.c').write_text(s)
with (p/'build.log').open('w') as log:subprocess.run(['clang',str(p/'test.c'),'-o',str(p/'test'),'-lpthread','-DEP_HAS_SQLITE','-lsqlite3'],stdout=log,stderr=log,check=True)
r=subprocess.run([str(p/'test'),str(p/'journal.sqlite')],capture_output=True,text=True);print(p);print(r.stdout);print(r.stderr);raise SystemExit(r.returncode)
