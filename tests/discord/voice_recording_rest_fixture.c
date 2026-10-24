/* Test-only Discord REST snapshots; reuses the no-network voice fixture ABI. */
#define curl_easy_perform voice_base_perform
#define curl_easy_getinfo voice_base_getinfo
#include "voice_rest_fixture.c"
#undef curl_easy_perform
#undef curl_easy_getinfo
#include <stdatomic.h>
#include <unistd.h>
/* Native executable acceptance can choose a fixture response before dlopen.
 * This library never performs network IO and is never a production dependency. */
__attribute__((constructor)) static void fixture_recording_environment(void) {
 const char *value=getenv("ECHO_TEST_RECORDING_MODE");
 if(value && !strcmp(value,"missing"))mode=13;
 if(value && !strcmp(value,"deny-origin"))mode=16;
 if(value && !strcmp(value,"blocked-publication"))mode=19;
 if(value && !strcmp(value,"fail-source"))mode=21;
 if(value && !strcmp(value,"advance-once"))mode=22;
 const char *journal_path=getenv("ECHO_TEST_RECORDING_JOURNAL");
 if(journal_path)snprintf(journal,sizeof journal,"%s",journal_path);
}
static char fixture_archive_path[4096];
int fixture_recording_archive(const char *path) { if(!path || strlen(path)>=sizeof fixture_archive_path)return 0;snprintf(fixture_archive_path,sizeof fixture_archive_path,"%s",path);return 1; }
static _Atomic int blocking_entered, blocking_release;
static _Atomic int advanced_once;
int fixture_recording_blocked(void) { return atomic_load(&blocking_entered); }
int fixture_recording_release(void) { atomic_store(&blocking_release,1); return 1; }
static char offer_body[8192], notice_body[8192];
static int posts, patches;
int fixture_recording_patches(void) { return patches; }
int fixture_recording_posts(void) { return posts; }
const char *fixture_recording_offer_body(void) { return offer_body; }
const char *fixture_recording_notice(void) { return notice_body; }
int fixture_recording_offer(const char *body) { if(!body || strlen(body)>=sizeof offer_body)return 0; snprintf(offer_body,sizeof offer_body,"%s",body); return 1; }
int curl_easy_getinfo(void *p,int opt,...) {
 Request *r=p; va_list a; va_start(a,opt); long *out=va_arg(a,long*); va_end(a);
 long status=200;
 if(((mode>=13 && mode<=15) || mode==19 || mode==21) && strstr(r->url,"/channels/300/messages/700") && !strcmp(r->method,"GET")) status=mode==15?403:404;
 *out=opt==2097154?status:0; return 0;
}
int curl_easy_perform(void *p) {
 Request *r=p; const char *route=strstr(r->url,"/api/v10");
 if(!route||!r->body||!r->headers)return 3;
 route+=8;
 if(mode==22 && !strcmp(route,"/channels/300/messages/100")) {
  if(!atomic_exchange(&advanced_once,1)) {
   sqlite3 *db=NULL;if(sqlite3_open(journal,&db)!=SQLITE_OK)return 3;
   int rc=sqlite3_exec(db,"UPDATE discord_gateway SET sequence=sequence+1 WHERE singleton=1",NULL,NULL,NULL);
   sqlite3_close(db);if(rc!=SQLITE_OK)return 3;
  }
 }
 if((mode==19 || mode==21) && !strcmp(route,"/channels/300/messages/101")) {
  const char *marker=getenv("ECHO_TEST_RECORDING_MARKER");
  if(marker) { FILE *f=fopen(marker,"a");if(f){fputs("source-read\n",f);fclose(f);} }
  if(mode==19)usleep(3000000);
  return 3;
 }
 if(mode==18 && !strcmp(route,"/channels/300/messages/101")) {
  sqlite3 *db=NULL;if(!fixture_archive_path[0] || sqlite3_open(fixture_archive_path,&db)!=SQLITE_OK)return 3;
  int rc=sqlite3_exec(db,"UPDATE echo_devices SET revoked=1 WHERE device=(SELECT device FROM echo_tool_authority WHERE run='discord-message:101' LIMIT 1)",NULL,NULL,NULL);
  sqlite3_close(db);if(rc!=SQLITE_OK)return 3;
 }
 if(mode==16 && !strcmp(route,"/channels/300")) {
  fputs("HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n",r->headers);
  fputs("{\"id\":\"300\",\"guild_id\":\"200\",\"type\":0,\"permission_overwrites\":[{\"id\":\"500\",\"type\":1,\"allow\":\"0\",\"deny\":\"1024\"}]}",r->body);calls++;return 0;
 }
 if(mode==17 && !strcmp(route,"/channels/300/messages/101")) {
  sqlite3 *db=NULL;if(sqlite3_open(journal,&db)!=SQLITE_OK)return 3;
  int rc=sqlite3_exec(db,"INSERT OR IGNORE INTO discord_inbox(identity,kind,payload,digest,received_ms) SELECT 'audience:fixture-publication','CHANNEL_UPDATE',payload,digest,received_ms FROM discord_inbox LIMIT 1",NULL,NULL,NULL);
  sqlite3_close(db);if(rc!=SQLITE_OK)return 3;
 }
 if(((mode>=13 && mode<=15) || mode==19 || mode==21) && !strcmp(route,"/channels/300/messages/700") && !strcmp(r->method,"GET")) {
  fputs(mode==15?"HTTP/1.1 403 Forbidden\r\nContent-Type: application/json\r\n\r\n":"HTTP/1.1 404 Not Found\r\nContent-Type: application/json\r\n\r\n",r->headers);
  fputs((mode==13 || mode==19 || mode==21)?"{\"code\":10008,\"message\":\"Unknown Message\"}":"{\"code\":50001,\"message\":\"Missing Access\"}",r->body);calls++;return 0;
 }
 if(mode==10 && !strcmp(route,"/channels/300") && !atomic_exchange(&blocking_entered,1)) {
  int remaining=5000;
  while(!atomic_load(&blocking_release) && remaining-->0)usleep(1000);
  if(!atomic_load(&blocking_release))return 3;
 }
 if(!strcmp(route,"/channels/300/messages/700") && !strcmp(r->method,"PATCH") && r->request && notice_body[0]) {
  sqlite3 *db=NULL;sqlite3_stmt *st=NULL;
  if(sqlite3_open(":memory:",&db)!=SQLITE_OK)return 3;
  if(sqlite3_prepare_v2(db,"SELECT json_patch(?1,?2)",-1,&st,NULL)!=SQLITE_OK){sqlite3_close(db);return 3;}
  sqlite3_bind_text(st,1,notice_body,-1,SQLITE_TRANSIENT);sqlite3_bind_text(st,2,r->request,-1,SQLITE_TRANSIENT);
  if(sqlite3_step(st)!=SQLITE_ROW){sqlite3_finalize(st);sqlite3_close(db);return 3;}
  snprintf(notice_body,sizeof notice_body,"%s",sqlite3_column_text(st,0));sqlite3_finalize(st);sqlite3_close(db);patches++;calls++;
  if(mode==11)return 3; /* edit applied; response lost */
  fputs("HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n",r->headers);fputs(notice_body,r->body);return 0;
 }
 const char *body=NULL;
 if(mode==12 && !strcmp(route,"/channels/600")) {
  fputs("HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n",r->headers);
  fputs("{\"id\":\"600\",\"guild_id\":\"200\",\"type\":2,\"permission_overwrites\":[{\"id\":\"500\",\"type\":1,\"allow\":\"0\",\"deny\":\"1048576\"}]}",r->body);calls++;return 0;
 }
 if(!strcmp(route,"/guilds/200/members/501")) body="{\"user\":{\"id\":\"501\",\"bot\":true},\"roles\":[],\"communication_disabled_until\":null}";
 if(!strcmp(route,"/channels/300/messages") && !strcmp(r->method,"POST") && r->request) {
  sqlite3 *db=NULL; sqlite3_stmt *st=NULL;
  if(sqlite3_open(":memory:",&db)!=SQLITE_OK)return 3;
  const char *q="SELECT json_set(?1,'$.id',CASE WHEN json_type(?1,'$.components') IS NULL THEN '700' ELSE CAST(709+?2 AS TEXT) END,'$.channel_id','300','$.author',json('{\"id\":\"900\"}')),json_type(?1,'$.components') IS NOT NULL";
  if(sqlite3_prepare_v2(db,q,-1,&st,NULL)!=SQLITE_OK) { sqlite3_close(db); return 3; }
  sqlite3_bind_text(st,1,r->request,-1,SQLITE_TRANSIENT);
  sqlite3_bind_int(st,2,posts);
  if(sqlite3_step(st)!=SQLITE_ROW) { sqlite3_finalize(st);sqlite3_close(db);return 3; }
  char *target=sqlite3_column_int(st,1)?offer_body:notice_body;
  snprintf(target,8192,"%s",sqlite3_column_text(st,0));
  sqlite3_finalize(st);sqlite3_close(db);posts++;calls++;
  if(mode==7)return 3; /* server stored message, response lost */
  fputs("HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n",r->headers);
  fputs(target,r->body);return 0;
 }
 if(!strncmp(route,"/channels/300/messages/71",sizeof "/channels/300/messages/71" - 1) && offer_body[0]) {
  body=offer_body;
  if(mode==9) {
   sqlite3 *db=NULL;
   if(sqlite3_open(journal,&db)!=SQLITE_OK)return 3;
   int rc=sqlite3_exec(db,"UPDATE discord_gateway SET sequence=sequence+1 WHERE singleton=1",NULL,NULL,NULL);
   sqlite3_close(db);if(rc!=SQLITE_OK)return 3;
  }
 }
 if(!strcmp(route,"/channels/300/messages/100"))
  body="{\"id\":\"100\",\"channel_id\":\"300\",\"guild_id\":\"200\",\"type\":0,\"author\":{\"id\":\"500\"},\"mentions\":[{\"id\":\"900\"}],\"content\":\"Authorised voice test\"}";
 if(!strcmp(route,"/channels/300/messages/101"))
  body=mode==6?"{\"id\":\"101\",\"channel_id\":\"300\",\"author\":{\"id\":\"501\"},\"content\":\"changed\"}":"{\"id\":\"101\",\"channel_id\":\"300\",\"guild_id\":\"200\",\"type\":0,\"author\":{\"id\":\"500\"},\"mentions\":[{\"id\":\"900\"}],\"content\":\"Authorised voice test\"}";
 if(!strcmp(route,"/channels/300/messages/700")) {
  body=mode==4?"{\"id\":\"700\",\"channel_id\":\"300\",\"author\":{\"id\":\"900\"},\"content\":\"removed recording notice\"}":
   "{\"id\":\"700\",\"channel_id\":\"300\",\"author\":{\"id\":\"900\"},\"content\":\"Voice transcription requested in <#600>. Echo will retain audio and attributed transcripts only for participants who explicitly allow transcription for their current voice session. Each participant can decline or revoke below; leaving or reconnecting requires new consent. Transcripts are restricted to this channel's audience. Capture stops if this notice or its access is removed.\"}";
  if(notice_body[0] && mode!=4)body=notice_body;
  if(mode==5) return 3; /* unavailable notice */
 }
 if(!body)return voice_base_perform(p);
 calls++;
 fputs("HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n",r->headers);
 fputs(body,r->body);
 return 0;
}
