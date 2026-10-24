#include <stdint.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <pthread.h>

/* Native-only callback storage: no pointer into moving Ernos objects. */
void *echo_voice_failure_state_create(void) {
    _Atomic uint64_t *state = malloc(sizeof(*state));
    if (state) atomic_init(state, 0);
    return state;
}
void echo_voice_failure_state_destroy(void *state) { free(state); }
uint64_t echo_voice_failure_count(void *state) {
    return state ? atomic_load_explicit((_Atomic uint64_t *)state, memory_order_relaxed) : 0;
}
void echo_voice_failure_callback(const char *source, const char *reason, void *state) {
    (void)source;
    (void)reason;
    if (state) atomic_fetch_add_explicit((_Atomic uint64_t *)state, 1, memory_order_relaxed);
}
/* The worker records operation status and failure counts, not MLS payloads,
   keys, identities or untrusted protocol strings from native debug logging. */
void echo_voice_log_sink(int severity, const char *file, int line, const char *message) {
    (void)severity; (void)file; (void)line; (void)message;
}

/* Keep the global sink installed until the last managed session closes. The
   caller retains both libraries for each hold and drops before unloading. */
typedef void (*voice_sink)(int, const char *, int, const char *);
typedef void (*voice_sink_setter)(voice_sink);
static pthread_mutex_t sink_lock = PTHREAD_MUTEX_INITIALIZER;
static voice_sink_setter sink_setter;
static unsigned sink_users;
int echo_voice_logging_hold(void *function) {
    if (!function) return 0;
    pthread_mutex_lock(&sink_lock);
    voice_sink_setter setter = (voice_sink_setter)function;
    if (sink_users && sink_setter != setter) {
        pthread_mutex_unlock(&sink_lock);
        return 0;
    }
    sink_setter = setter;
    sink_setter(echo_voice_log_sink);
    ++sink_users;
    pthread_mutex_unlock(&sink_lock);
    return 1;
}
void echo_voice_logging_drop(void) {
    pthread_mutex_lock(&sink_lock);
    if (sink_users && --sink_users == 0) {
        sink_setter(NULL);
        sink_setter = NULL;
    }
    pthread_mutex_unlock(&sink_lock);
}
