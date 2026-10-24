from pathlib import Path
import json, subprocess, sys, tempfile, wave

# Exercise the emitted native implementation with isolated archives and a
# synthetic complete recording larger than Discord's observed 20 MiB ceiling.
source = Path(sys.argv[1]).read_text().replace('int main(int argc, char** argv) {', 'int original_main(int argc, char** argv) {', 1)
source += r'''
int main(int argc, char **argv) {
 init_ep_args(argc,argv); __ep_init_constants();
 long long j=discord_store_open((long long)argv[1],(long long)"900");
 long long d=discord_agent_open((long long)argv[2],(long long)"900");
 if (!j || !d || strcmp((char*)delegation_schema(d),"applied")) return 1;
 const char *body="{\"id\":\"100\",\"channel_id\":\"300\",\"guild_id\":\"200\",\"type\":0,\"author\":{\"id\":\"400\"},\"mentions\":[{\"id\":\"900\"}],\"content\":\"Synthetic speech encoding test\"}";
 discord_inbox_record(j,(long long)"message:create:100",(long long)"MESSAGE_CREATE",(long long)body);
 long long guilds=create_list(), admins=create_list();append_list(guilds,(long long)"200");append_list(admins,(long long)"400");
 long long submitted=discord_agent_submit(j,d,(long long)"message:create:100",(long long)"900",guilds,admins);
 if(strcmp((char*)archive_list_text(submitted,0),"accepted")) return 2;
 long long session=archive_list_text(submitted,1),device=(long long)"discord:400",input=(long long)"synthetic-wav";
 if(web_attachment_schema(d)) return 9;
 if(web_attachment_register(d,device,session,input,(long long)argv[3],(long long)"synthetic.wav",(long long)"audio/wav")) return 3;
 long long result=discord_audio_decode_format(d,device,session,input,(long long)argv[4],2);
 if(strcmp((char*)archive_list_text(result,0),"decoded")){puts((char*)archive_list_text(result,0));return 4;}
 long long attachment=archive_list_text(result,1),row=web_attachment_owned(d,device,attachment);
 if(strcmp((char*)archive_list_text(row,3),"audio/mp4") || string_to_int(archive_list_text(row,5))>=20971520) return 5;
 long long repeated=discord_audio_decode_format(d,device,session,input,(long long)argv[4],2);
 if(strcmp((char*)archive_list_text(repeated,1),(char*)attachment)) return 6;
 long long denied=discord_audio_decode_format(d,(long long)"discord:999",session,input,(long long)argv[4],2);
 if(!strcmp((char*)archive_list_text(denied,0),"decoded")) return 7;
 row=web_attachment_owned(d,device,attachment);
 puts((char*)archive_list_text(row,1));
 long long multi=discord_file_multipart(archive_list_text(row,1),archive_list_text(row,4),string_to_int(archive_list_text(row,5)),20971520,(long long)"{}",(long long)"Echo-reply.m4a",(long long)"audio/mp4");
 if(strcmp((char*)archive_list_text(multi,0),"multipart-ready")){puts((char*)archive_list_text(multi,0));return 8;}
 discord_audio_multipart_release(multi);
 puts((char*)archive_list_text(row,1));puts("PASS: full recording encoded under limit; cached derivative; foreign account denied; verified multipart");return 0;
}
'''
root=Path(tempfile.mkdtemp(prefix='echo-speech-encoding-')).resolve()
(root/'test.c').write_text(source)
with wave.open(str(root/'input.wav'),'wb') as w:
 w.setnchannels(1); w.setsampwidth(2); w.setframerate(24000)
 for _ in range(600): w.writeframes(b'\0\0'*24000)
(root/'stage').mkdir()
with (root/'build.log').open('w') as log:
 subprocess.run(['clang',str(root/'test.c'),'-o',str(root/'test'),'-lpthread','-DEP_HAS_SQLITE','-lsqlite3'],stdout=log,stderr=log,check=True)
result=subprocess.run([str(root/'test'),str(root/'journal.sqlite'),str(root/'agent.sqlite'),str(root/'input.wav'),str(root/'stage')],capture_output=True,text=True)
print(root);print(result.stdout);print(result.stderr);print('exit',result.returncode)
if result.returncode: raise SystemExit(result.returncode)
encoded=next(line for line in result.stdout.splitlines() if line.endswith('/output.m4a'))
profile=json.loads(Path(sys.argv[2]).read_text());ffmpeg=profile['ffmpeg']
subprocess.run([ffmpeg,'-v','error','-i',encoded,'-c:a','pcm_s16le',str(root/'roundtrip.wav')],check=True)
with wave.open(str(root/'roundtrip.wav')) as w:
 duration=w.getnframes()/w.getframerate();assert abs(duration-600)<0.1,duration
print('PASS: complete 600 second duration preserved')
