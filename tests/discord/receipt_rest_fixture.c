/* Test-only libcurl ABI. No network or credentials are used. */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include <sqlite3.h>
typedef struct { char url[1024]; FILE *body,*headers; const char *payload; } Request;
static int mode,calls,quiet_seen;
int fixture_quiet_seen(void) { return quiet_seen; } static char journal[4096];
int fixture_configure(int value,const char *path) { mode=value; calls=0; snprintf(journal,sizeof(journal),"%s",path); return 1; }
int fixture_calls(void) { return calls; }
void *curl_easy_init(void) { return calloc(1,sizeof(Request)); }
void curl_easy_cleanup(void *p) { free(p); }
void *curl_slist_append(void *p,const char *s) { (void)s; return p?p:malloc(1); }
void curl_slist_free_all(void *p) { free(p); }
int curl_easy_setopt(void *p,int opt,...) {
 Request *r=p; va_list a; va_start(a,opt);
 if(opt>=10000) { void *v=va_arg(a,void*); if(opt==10002) snprintf(r->url,sizeof(r->url),"%s",(char*)v); if(opt==10015)r->payload=v; if(opt==10001)r->body=v; if(opt==10029)r->headers=v; }
 else (void)va_arg(a,long);
 va_end(a); return 0;
}
int curl_easy_getinfo(void *p,int opt,...) { (void)p; va_list a; va_start(a,opt); long *out=va_arg(a,long*); *out=opt==2097154?200:0; va_end(a); return 0; }
int curl_easy_perform(void *p) {
 Request *r=p; calls++; const char *route=strstr(r->url,"/api/v10"); if(!route||!r->body||!r->headers)return 3; route+=8;
 const char *body=NULL; char response[1024];
 if(!strcmp(route,"/channels/300/messages")) {
  quiet_seen=r->payload && strstr(r->payload,"\"flags\":4096")!=NULL;
  const char *n=r->payload?strstr(r->payload,"\"nonce\":\""):NULL; if(!n)return 3; n+=9; const char *end=strchr(n,'"'); if(!end||end-n>100)return 3;
  snprintf(response,sizeof(response),"{\"id\":\"700\",\"channel_id\":\"300\",\"author\":{\"id\":\"900\"},\"nonce\":\"%.*s\"}",(int)(end-n),n);body=response;
 }

 if(!strcmp(route,"/channels/300")) body="{\"id\":\"300\",\"guild_id\":\"200\",\"type\":0,\"permission_overwrites\":[]}";
 if(!strcmp(route,"/guilds/200")) body="{\"id\":\"200\",\"owner_id\":\"999\"}";
 if(!strcmp(route,"/guilds/200/roles")) body="[{\"id\":\"200\",\"permissions\":\"3214336\"}]";
 if(!strcmp(route,"/guilds/200/members/500")) body="{\"user\":{\"id\":\"500\"},\"roles\":[],\"communication_disabled_until\":null}";
 if(!strcmp(route,"/guilds/200/members/900")) body="{\"user\":{\"id\":\"900\"},\"roles\":[],\"communication_disabled_until\":null}";
 if(!strcmp(route,"/channels/300/messages/100")) body="{\"id\":\"100\",\"channel_id\":\"300\",\"guild_id\":\"200\",\"type\":0,\"author\":{\"id\":\"500\"},\"mentions\":[{\"id\":\"900\"}],\"content\":\"Join authorised voice\"}";
 if(!strcmp(route,"/channels/600")) {
  body=mode==1?"{\"id\":\"600\",\"guild_id\":\"200\",\"type\":2,\"permission_overwrites\":[{\"id\":\"900\",\"type\":1,\"allow\":\"0\",\"deny\":\"2097152\"}]}":"{\"id\":\"600\",\"guild_id\":\"200\",\"type\":2,\"permission_overwrites\":[]}";
  if(mode==3) body="{\"id\":\"600\",\"guild_id\":\"200\",\"type\":2,\"permission_overwrites\":[{\"id\":\"777\",\"type\":1,\"allow\":\"0\",\"deny\":\"1024\"}]}";
  if(mode==2){ sqlite3 *db=NULL; if(sqlite3_open(journal,&db)!=SQLITE_OK)return 3; int rc=sqlite3_exec(db,"UPDATE discord_gateway SET sequence=sequence+1 WHERE singleton=1",NULL,NULL,NULL); sqlite3_close(db); if(rc!=SQLITE_OK)return 3; }
 }
 if(!body)return 3;
 fputs("HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n",r->headers); fputs(body,r->body); return 0;
}
