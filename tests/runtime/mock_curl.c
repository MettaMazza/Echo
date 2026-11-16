#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
typedef struct {FILE *body,*headers;const char *url,*proxy,*noproxy,*protocols;long follow,peer,host;int code;} Easy;
void *curl_easy_init(void){return calloc(1,sizeof(Easy));}
int curl_easy_setopt(Easy *e,int k,...){va_list v;va_start(v,k);if(k>=10000){void*p=va_arg(v,void*);switch(k){case 10001:e->body=p;break;case 10029:e->headers=p;break;case 10002:e->url=p;break;case 10004:e->proxy=p;break;case 10177:e->noproxy=p;break;case 10318:e->protocols=p;break;}}else{long n=va_arg(v,long);if(k==52)e->follow=n;if(k==64)e->peer=n;if(k==81)e->host=n;}va_end(v);return 0;}
void *curl_slist_append(void*p,const char*s){return p?p:malloc(1);}
void curl_slist_free_all(void*p){free(p);}
int curl_easy_perform(Easy*e){if(!e->url||strncmp(e->url,"https://discord.com/api/v10/",26)||e->follow||e->peer!=1||e->host!=2||!e->proxy||*e->proxy||!e->noproxy||strcmp(e->noproxy,"*")||!e->protocols||strcmp(e->protocols,"https"))return 99;const char*m=getenv("FIXTURE_MODE");e->code=m&&!strcmp(m,"denied")?403:m&&!strcmp(m,"limited")?429:200;fprintf(e->headers,"HTTP/2 %d\r\nx-ratelimit-bucket: fixture\r\nx-ratelimit-limit: 10\r\nx-ratelimit-remaining: 9\r\nx-ratelimit-reset-after: 0\r\n\r\n",e->code);if(m&&!strcmp(m,"invalid"))fputs("invalid JSON",e->body);else if(e->code==429)fputs("{\"retry_after\":1,\"global\":false}",e->body);else fputs("{\"id\":\"1562\",\"roles\":[],\"label\":\"café 🌀\"}",e->body);return m&&!strcmp(m,"failed")?28:0;}
int curl_easy_getinfo(Easy*e,int k,...){va_list v;va_start(v,k);void*p=va_arg(v,void*);va_end(v);*(long long*)p=k==2097154?e->code:100;return 0;}
void curl_easy_cleanup(void*p){free(p);}
