static long long fixture_gate;
static const char *fixture_journal_path,*fixture_archive_path;
static volatile int fixture_observed=0,fixture_memory_finished=0,fixture_followup=0;
#define FSTR(s) ((long long)(s))
long long agent_loop_inner_provider_managed(long long database,long long run,long long profile,long long reserve,long long cancellation,long long read_size,long long library,long long stop){
 long long owner=agent_run_read(database,run);ep_gc_push_root(&owner);
 long long session=archive_list_text(owner,0);ep_gc_push_root(&session);
 int failed=archive_append(database,session,FSTR("fixture-request"),FSTR("model_request"),FSTR("{\"run_id\":\"discord-message:100\",\"parent_event\":\"discord-message:100:user\"}"),FSTR("fixture"));
 failed|=agent_run_transition(database,run,FSTR("ready"),FSTR("discord-message:100:user"),FSTR("generating"),FSTR("fixture-request"));
 failed|=archive_append(database,session,FSTR("fixture-final"),FSTR("assistant_final"),FSTR("{\"run_id\":\"discord-message:100\",\"parent_event\":\"fixture-request\",\"content\":\"Remembered.\"}"),FSTR("fixture"));
 failed|=agent_run_transition(database,run,FSTR("generating"),FSTR("fixture-request"),FSTR("completed"),FSTR("fixture-final"));ep_gc_pop_roots(2);return FSTR(failed?"fixture-error":"completed");
}
long long agent_memory_prepare_parallel(long long path,long long run,long long profile,long long reserve,long long cancellation,long long read_size,long long completed){
 ep_gc_push_root(&path);ep_gc_push_root(&completed);receive_channel(fixture_gate);
 long long db=archive_open(path);long long owner=agent_run_read(db,run);ep_gc_push_root(&owner);
 long long saved=archive_append(db,archive_list_text(owner,0),FSTR("fixture-memory-finished"),FSTR("memory_indexed"),FSTR("{\"source_event\":\"discord-message:100:user\"}"),FSTR("fixture"));
 fixture_memory_finished=(saved==0);sql_close(db);send_channel(completed,0);ep_gc_pop_roots(3);return 0;
}
static void *observe_foreground(void *unused){
 char bottom;ep_gc_register_thread(&bottom);long long db=archive_open(FSTR(fixture_archive_path));long long journal=discord_store_open(FSTR(fixture_journal_path),FSTR("900"));
 long long start=ep_time_now_ms();
 while(ep_time_now_ms()-start<5000){
  long long stmt=ep_sqlite3_prepare_v2(db,FSTR("SELECT 1 FROM echo_events WHERE event='fixture-final:foreground-complete'"));int found=ep_sqlite3_step(stmt)==100;ep_sqlite3_finalize(stmt);
  if(found){fixture_observed=!fixture_memory_finished&&discord_foreground_release(journal,db,FSTR("message:create:100"))==1;long long next=discord_worker_claim(journal);fixture_followup=!strcmp((char*)archive_list_text(next,1),"message:create:101");break;}
  usleep(1000);
 }
 send_channel(fixture_gate,1);sql_close(journal);sql_close(db);ep_gc_unregister_thread();return 0;
}
int main(int argc,char **argv){
 init_ep_args(argc,argv);__ep_init_constants();fixture_journal_path=argv[1];fixture_archive_path=argv[2];
 long long journal=discord_store_open(FSTR(argv[1]),FSTR("900")),db=discord_agent_open(FSTR(argv[2]),FSTR("900"));
 if(!journal||!db||discord_worker_schema(journal)||discord_foreground_release_schema(journal))return 10;
 if(strcmp((char*)delegation_schema(db),"applied"))return 11;
 long long guilds=create_list(),admins=create_list();ep_gc_push_root(&guilds);ep_gc_push_root(&admins);append_list(guilds,FSTR("200"));append_list(admins,FSTR("400"));
 const char *body="{\"id\":\"100\",\"channel_id\":\"300\",\"guild_id\":\"200\",\"flags\":0,\"type\":0,\"author\":{\"id\":\"500\"},\"mentions\":[{\"id\":\"900\"}],\"content\":\"I prefer green tea.\"}";
 discord_inbox_record(journal,FSTR("message:create:100"),FSTR("MESSAGE_CREATE"),FSTR(body));discord_worker_claim(journal);
 long long submitted=discord_agent_submit(journal,db,FSTR("message:create:100"),FSTR("900"),guilds,admins);if(strcmp((char*)archive_list_text(submitted,0),"accepted"))return 12;
 const char *followup="{\"id\":\"101\",\"channel_id\":\"300\",\"guild_id\":\"200\",\"flags\":0,\"type\":0,\"author\":{\"id\":\"500\"},\"mentions\":[{\"id\":\"900\"}],\"content\":\"Follow up\"}";
 discord_inbox_record(journal,FSTR("message:create:101"),FSTR("MESSAGE_CREATE"),FSTR(followup));
 fixture_gate=create_channel();ep_gc_push_root(&fixture_gate);pthread_t observer;pthread_create(&observer,0,observe_foreground,0);
 long long profile=provider_llama_profile(FSTR("fixture-model"),8080);ep_gc_push_root(&profile);
 long long result=agent_loop_provider_managed(db,FSTR("discord-message:100"),profile,8192,-1,65536,FSTR(""));pthread_join(observer,0);
 printf("{\"result\":\"%s\",\"released_before_memory_finished\":%d,\"followup_claimed\":%d,\"memory_joined_and_persisted\":%d}\n",(char*)result,fixture_observed,fixture_followup,fixture_memory_finished);
 return strcmp((char*)result,"completed")||!fixture_observed||!fixture_followup||!fixture_memory_finished;
}
