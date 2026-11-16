#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc,char **argv){
  const char *body=NULL,*url=NULL; int disabled=argc>1&&!strcmp(argv[1],"--disable");
  for(int i=1;i<argc;i++){
    if(strstr(argv[i],"fixture-private-key"))return 20;
    if(!strcmp(argv[i],"--data-binary")&&i+1<argc)body=argv[i+1];
    if(!strcmp(argv[i],"--url")&&i+1<argc)url=argv[i+1];
  }
  if(!disabled||!body||body[0]!='@'||!url||getenv("HTTPS_PROXY"))return 21;
  char headers[4096];size_t used=0;ssize_t n;
  while((n=read(0,headers+used,sizeof(headers)-used-1))>0){used+=(size_t)n;if(used>=sizeof(headers)-1)return 22;}
  if(n<0)return 23;headers[used]=0;
  if(!strstr(headers,"fixture-private-key"))return 24;
  if(strstr(url,"anthropic")&&(!strstr(headers,"x-api-key: ")||!strstr(headers,"anthropic-version: 2023-06-01")))return 25;
  if(strstr(url,"openai")&&!strstr(headers,"Authorization: Bearer "))return 26;
  FILE *f=fopen(body+1,"r");if(!f)return 27;int mode=fgetc(f);fclose(f);
  if(mode=='J'||mode=='K'||mode=='Q'||mode=='P'||mode=='U'||mode=='R'){
    const char *answer="data: {\"choices\":[{\"index\":0,\"delta\":{\"content\":\"hé🌍\"},\"finish_reason\":\"stop\"}]}\n\ndata: [DONE]\n\n";
    if(strstr(url,"anthropic"))answer=
      "event: message_start\ndata: {\"type\":\"message_start\",\"message\":{\"role\":\"assistant\",\"content\":[]}}\n\n"
      "event: content_block_start\ndata: {\"type\":\"content_block_start\",\"index\":0,\"content_block\":{\"type\":\"text\",\"text\":\"hé🌍\"}}\n\n"
      "event: content_block_stop\ndata: {\"type\":\"content_block_stop\",\"index\":0}\n\n"
      "event: message_delta\ndata: {\"type\":\"message_delta\",\"delta\":{\"stop_reason\":\"end_turn\"}}\n\n"
      "event: message_stop\ndata: {\"type\":\"message_stop\"}\n\n";
    int status=mode=='U'?401:mode=='R'?429:200;
    if(status!=200)answer="{\"error\":\"fixture provider failure\"}";
    printf("HTTP/1.1 %d Fixture\r\nContent-Type: %s\r\nRetry-After: 7\r\nContent-Length: %zu\r\n\r\n",status,status==200?"text/event-stream":"application/json",mode=='Q'?strlen(answer)/2:strlen(answer));
    fwrite(answer,1,(mode=='P'||mode=='Q')?strlen(answer)/2:strlen(answer),stdout);fflush(stdout);
    if(mode=='Q')sleep(60);
    return mode=='K'?7:0;
  }
  const char *wire="HTTP/1.1 200 OK\r\nContent-Type: text/event-stream\r\nContent-Length: 14\r\n\r\ndata: [DONE]\n\n";
  if(write(1,wire,strlen(wire))!=(ssize_t)strlen(wire))return 28;
  if(mode=='S')sleep(60);
  if(mode=='F')return 7;
  return 0;
}
