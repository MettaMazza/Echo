from pathlib import Path
import subprocess,tempfile,sys
# Feed C emitted by epc_gc_literal for app/discord_actions.ep. Only the
# authorization resolver and speech provider are mocked; exercise the real
# worker, journal, status settlement, and retry scheduling. No live network.
s=Path(sys.argv[1]).read_text()
assert 'Speech generation failed. No audio was attached.' in s
s=s.replace('long long discord_speech_deliver(long long library,','long long real_discord_speech_deliver(long long library,',1)
s=s.replace('long long discord_message_action_resolve(long long journal,','long long real_discord_message_action_resolve(long long journal,',1)
s=s.replace('int main(int argc, char** argv) {','int real_main(int argc, char** argv) {',1)
s+='''
static const char *fixture_status;
long long discord_message_action_resolve(long long journal,long long body,long long bot,long long admins) {
 long long a=history_status((long long)"bound-needs-live-access"); append_list(a,(long long)"speak"); return a;
}
long long discord_speech_deliver(long long library,long long token,long long journal,long long database,long long interaction,long long body,long long bot,long long admins,long long directory_path,long long stop_file) { return (long long)fixture_status; }
int main(int argc,char **argv) {
 init_ep_args(argc,argv); __ep_init_constants();
 long long j=discord_store_open((long long)argv[1],(long long)"900");
 if(!j || discord_action_worker_schema(j)) return 1;
 const char *states[]={"speech:helper-failed","speech:artifact-missing","rate-limited","speech:busy","speech:speech-in-progress-or-interrupted"};
 for(int i=0;i<5;i++) {
 char id[64],sql[512];snprintf(id,sizeof id,"interaction:fixture%d",i);
 discord_inbox_record(j,(long long)id,(long long)"INTERACTION_CREATE",(long long)"{}");
 if(discord_interaction_queue(j,(long long)id)) return 2;
 snprintf(sql,sizeof sql,"UPDATE discord_interaction_jobs SET state='acknowledged' WHERE identity='%s';",id); sql_execute(j,(long long)sql);
 fixture_status=states[i];
 long long result=discord_action_worker_tick((long long)"",(long long)"",j,0,(long long)"900",create_list(),(long long)"",(long long)"/nonexistent-speech-test-stop");
 if(strcmp((char*)result,i<2?"denied":states[i])) return 10+i;
 snprintf(sql,sizeof sql,"SELECT state,detail FROM discord_interaction_jobs WHERE identity='%s';",id);
 long long row=history_collect_one(archive_prepare(j,(long long)sql));
 if(strcmp((char*)archive_list_text(row,1),i<2?"denied":"acknowledged")) return 20+i;
 if(i<2 && strcmp((char*)archive_list_text(row,2),states[i])) return 30+i;
 if(strcmp((char*)discord_action_worker_tick((long long)"",(long long)"",j,0,(long long)"900",create_list(),(long long)"",(long long)"/nonexistent-speech-test-stop"),"idle")) return 40+i;
 }
 puts("PASS: saved speech failures terminal; rate limit and busy requests deferred; no immediate replay"); return 0;
}
'''
p=Path(tempfile.mkdtemp(prefix='echo-speech-failure-'));(p/'test.c').write_text(s)
with (p/'build.log').open('w') as log:subprocess.run(['clang',str(p/'test.c'),'-o',str(p/'test'),'-lpthread','-DEP_HAS_SQLITE','-lsqlite3'],stdout=log,stderr=log,check=True)
x=subprocess.run([str(p/'test'),str(p/'journal.sqlite')],capture_output=True,text=True);print(p);print(x.stdout);print('exit',x.returncode);raise SystemExit(x.returncode)
