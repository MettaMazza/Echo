/* Test-only activation race; preserves the original join message exactly. */
#define curl_easy_perform voice_base_perform
#include "voice_rest_fixture.c"
#undef curl_easy_perform
static int advanced_once, activation_source_reads;
int fixture_activation_source_reads(void) { return activation_source_reads; }
int curl_easy_perform(void *pointer) {
 Request *r=pointer;
 if((mode==22 || mode==23 || mode==24) && strstr(r->url,"/channels/300/messages/100")) {
  activation_source_reads++;
  if(advanced_once) { if(mode==23)mode=1; return voice_base_perform(pointer); }
  sqlite3 *db=NULL;
  if(sqlite3_open(journal,&db)!=SQLITE_OK)return 3;
  int rc=SQLITE_OK;
  if(mode==24) rc=sqlite3_exec(db,"INSERT INTO discord_dispatch(session,sequence,kind,digest) SELECT session,sequence+1,'MESSAGE_CREATE','aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa' FROM discord_gateway WHERE singleton=1",NULL,NULL,NULL);
  if(rc==SQLITE_OK) rc=sqlite3_exec(db,"UPDATE discord_gateway SET sequence=sequence+1 WHERE singleton=1",NULL,NULL,NULL);
  sqlite3_close(db);
  if(rc!=SQLITE_OK)return 3;
  advanced_once=1;
 }
 return voice_base_perform(pointer);
}
