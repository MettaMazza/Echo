#include <stdio.h>
#include <string.h>
#include <unistd.h>
/* Test-only local helper. No Discord or network implementation. */
int main(int argc,char **argv) {
    if(argc!=12 || strcmp(argv[1],"--model") || strcmp(argv[3],"--file") || strcmp(argv[4],"input.wav") || strcmp(argv[5],"--language") || strcmp(argv[6],"auto") || strcmp(argv[7],"--output-txt") || strcmp(argv[8],"--output-json") || strcmp(argv[9],"--output-file") || strcmp(argv[10],"transcript") || strcmp(argv[11],"--print-progress"))return 40;
    char mode[64]={0},header[12]={0}; FILE *f=fopen(argv[2],"rb");if(!f)return 41;fread(mode,1,63,f);fclose(f);
    f=fopen("input.wav","rb");if(!f)return 42;fread(header,1,12,f);fclose(f);if(memcmp(header,"RIFF",4)||memcmp(header+8,"WAVE",4))return 43;
    f=fopen("helper-started","wb");if(!f)return 44;fprintf(f,"%d",(int)getpid());fclose(f);
    if(!strcmp(mode,"blocked"))sleep(30);
    fwrite("out\0binary\xff",1,11,stdout);fputs("recognition fixture\n",stderr);
    if(!strcmp(mode,"failed"))return 12;
    f=fopen("transcript.txt","wb");if(!f)return 45;fputs(" Fixture speech.\n",f);fclose(f);
    if(!strcmp(mode,"missing"))return 0;
    f=fopen("transcript.json","wb");if(!f)return 46;
    if(!strcmp(mode,"invalid"))fputs("{broken",f);
    else fputs("{\"transcription\":[{\"timestamps\":{\"from\":\"00:00:00.000\",\"to\":\"00:00:00.020\"},\"offsets\":{\"from\":0,\"to\":20},\"text\":\" Fixture speech.\"}]}",f);
    fclose(f);return 0;
}
