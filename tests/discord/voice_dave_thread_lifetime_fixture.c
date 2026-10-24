/* Test-only real DAVE lifetime regression with a joined native thread. */
#include <dave/dave.h>
#include <dlfcn.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
static const char *library_path;
static int nodelete;
static void quiet_log(DAVELoggingSeverity level,const char *file,int line,const char *message){(void)level;(void)file;(void)line;(void)message;}
static void *worker(void *unused) {
 (void)unused;
 void *lib=dlopen(library_path,RTLD_NOW|RTLD_LOCAL|(nodelete?RTLD_NODELETE:0));assert(lib);
 __typeof__(&daveSessionCreate) create=dlsym(lib,"daveSessionCreate");
 __typeof__(&daveSessionInit) init=dlsym(lib,"daveSessionInit");
 __typeof__(&daveSessionDestroy) destroy=dlsym(lib,"daveSessionDestroy");
 __typeof__(&daveSessionGetMarshalledKeyPackage) package=dlsym(lib,"daveSessionGetMarshalledKeyPackage");
 __typeof__(&daveFree) release=dlsym(lib,"daveFree");
 assert(create && init && destroy && package && release);
 __typeof__(&daveSetLogSinkCallback) set_log=dlsym(lib,"daveSetLogSinkCallback");assert(set_log);set_log(quiet_log);
 DAVESessionHandle session=create(NULL,NULL,NULL,NULL);assert(session);
 init(session,1,100,"200");uint8_t *bytes=NULL;size_t length=0;package(session,&bytes,&length);assert(bytes && length>32);
 release(bytes);destroy(session);assert(dlclose(lib)==0);
 puts("session closed; owner thread exiting");fflush(stdout);return NULL;
}
int main(int argc,char **argv) {
 assert(argc==3);library_path=argv[1];nodelete=!strcmp(argv[2],"nodelete");void *pin=NULL;
 if(!strcmp(argv[2],"pinned")){pin=dlopen(library_path,RTLD_NOW|RTLD_LOCAL);assert(pin);}
 pthread_t thread;assert(pthread_create(&thread,NULL,worker,NULL)==0);assert(pthread_join(thread,NULL)==0);
 puts("owner thread fully joined");fflush(stdout);if(pin)assert(dlclose(pin)==0);return 0;
}
