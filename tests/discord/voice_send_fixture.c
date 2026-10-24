#include <stddef.h>
#include <stdint.h>
#include <string.h>
static char text_body[4096];
const char *fixture_text(void) { return text_body; }
static int fail_write, calls, opcode, cleanups;
void fixture_reset(int fail) { fail_write=fail; calls=0; opcode=-1; cleanups=0; text_body[0]=0; }
int fixture_calls(void) { return calls; }
int fixture_opcode(void) { return opcode; }
int fixture_cleanups(void) { return cleanups; }
int curl_ws_send(void *handle, const void *data, size_t length, size_t *sent, long long offset, unsigned int flags) {
    (void)handle; (void)offset;
    calls++;
    if (!data || length<2 || (flags!=1 && flags!=2)) return 43;
    if (flags==1) { if (length>=sizeof(text_body)) return 43; memcpy(text_body,data,length); text_body[length]=0; }
    opcode=((const uint8_t*)data)[0];
    *sent=fail_write ? length-1 : length;
    return 0;
}
void curl_easy_cleanup(void *handle) { (void)handle; cleanups++; }
/* Test-only curl_ws_recv ABI with one queued complete frame. */
struct frame_meta { int age; int flags; long long offset; long long bytesleft; size_t len; };
static struct frame_meta meta;
static uint8_t incoming[65536];
static size_t incoming_length;
static int incoming_ready;
int fixture_receive(const void *data, size_t length, int flags) {
    if (length>sizeof(incoming)) return 0;
    memcpy(incoming,data,length); incoming_length=length; incoming_ready=1;
    meta=(struct frame_meta){0,flags,0,0,length}; return 1;
}
int curl_ws_recv(void *handle, void *buffer, size_t capacity, size_t *received, const struct frame_meta **metadata) {
    (void)handle;
    if (!incoming_ready) return 81;
    if (capacity<incoming_length) return 43;
    memcpy(buffer,incoming,incoming_length); *received=incoming_length; *metadata=&meta;
    incoming_ready=0; return 0;
}

/* Test-only interrupted UDP read: unlike EAGAIN it proves no queue boundary. */
static int udp_error = 4;
int *__error(void) { return &udp_error; }
long recv(int fd, void *buffer, size_t capacity, int flags) {
    (void)fd; (void)buffer; (void)capacity; (void)flags;
    return -1;
}
