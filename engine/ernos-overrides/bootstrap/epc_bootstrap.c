#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#ifdef __wasm__
#define _SETJMP_H
typedef int jmp_buf[1];
#define setjmp(buf) (0)
#define longjmp(buf, val) abort()

// Mock pthreads for single-threaded WASM
typedef struct { int lock_state; } pthread_mutex_t;
typedef struct { int cond_state; } pthread_cond_t;
typedef struct { int rw_state; } pthread_rwlock_t;
typedef int pthread_t;
typedef int pthread_attr_t;
#define PTHREAD_MUTEX_INITIALIZER {0}
#define PTHREAD_COND_INITIALIZER {0}
#define PTHREAD_RWLOCK_INITIALIZER {0}
#define pthread_mutex_init(m, a) ((void)(a), (m)->lock_state = 0, 0)
#define pthread_mutex_lock(m) ((m)->lock_state = 1, 0)
#define pthread_mutex_unlock(m) ((m)->lock_state = 0, 0)
#define pthread_mutex_trylock(m) ((m)->lock_state == 0 ? ((m)->lock_state = 1, 0) : 1)
#define pthread_mutex_destroy(m) ((void)(m), 0)
#define pthread_cond_init(c, a) ((void)(a), (c)->cond_state = 0, 0)
#define pthread_cond_wait(c, m) ((void)(c), (void)(m), 0)
#define pthread_cond_signal(c) ((void)(c), 0)
#define pthread_cond_broadcast(c) ((void)(c), 0)
#define pthread_cond_destroy(c) ((void)(c), 0)
#define pthread_rwlock_init(r, a) ((void)(a), (r)->rw_state = 0, 0)
#define pthread_rwlock_rdlock(r) ((r)->rw_state = 1, 0)
#define pthread_rwlock_wrlock(r) ((r)->rw_state = 2, 0)
#define pthread_rwlock_unlock(r) ((r)->rw_state = 0, 0)
#define pthread_rwlock_destroy(r) ((void)(r), 0)
#define pthread_create(t, a, f, arg) ((void)(t), (void)(a), (void)(f), (void)(arg), 0)
#define pthread_join(t, r) ((void)(t), (void)(r), 0)
#define pthread_detach(t) ((void)(t), 0)
#else
#include <setjmp.h>
#include <limits.h>
#endif
#include <signal.h>
#include <time.h>
#ifndef _WIN32
#include <unistd.h>
#endif
/* Terminal control for read_key/terminal_columns/terminal_rows: raw keyboard
   input and window size, each with a native path per platform. */
#if defined(_WIN32)
#include <conio.h>
#include <io.h>
#elif !defined(__wasm__)
#include <termios.h>
#include <sys/ioctl.h>
#endif
#if defined(__APPLE__)
#include <mach/mach.h>
#endif
#if defined(__linux__)
#include <sys/random.h>
#endif
#include <fcntl.h>

/* Cryptographically secure random bytes. Uses the OS CSPRNG: arc4random on
   Apple/BSD, getrandom(2) on Linux (falling back to /dev/urandom), and a
   /dev/urandom read elsewhere. Only if all of those are unavailable does it
   fall back to rand() — never on a supported platform. */
static void ep_secure_random_bytes(unsigned char* buf, size_t n) {
#if defined(__APPLE__) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__)
    arc4random_buf(buf, n);
#else
    size_t got = 0;
  #if defined(__linux__)
    while (got < n) {
        ssize_t r = getrandom(buf + got, n - got, 0);
        if (r <= 0) break;
        got += (size_t)r;
    }
  #endif
    if (got < n) {
        FILE* f = fopen("/dev/urandom", "rb");
        if (f) {
            got += fread(buf + got, 1, n - got, f);
            fclose(f);
        }
    }
    while (got < n) {
        buf[got++] = (unsigned char)(rand() & 0xFF);
    }
#endif
}

/* Try/catch infrastructure */
static jmp_buf ep_try_buf;
static volatile int ep_try_active = 0;

static void ep_signal_handler(int sig) {
    if (ep_try_active) {
        ep_try_active = 0;
        longjmp(ep_try_buf, sig);
    }
    /* Outside try: print error and exit */
    const char* name = sig == SIGSEGV ? "segmentation fault (null pointer or invalid memory access)"
                     : sig == SIGFPE  ? "arithmetic error (division by zero)"
                     : sig == SIGABRT ? "aborted"
                     : "unknown signal";
    fprintf(stderr, "\nRuntime Error: %s (signal %d)\n", name, sig);

    /* Write to daemon/general log file if environment variable is set */
    const char* daemon_log = getenv("ERNOS_DAEMON_LOG");
    if (!daemon_log || daemon_log[0] == '\0') {
        daemon_log = getenv("ERNOS_LOG_FILE");
    }
    if (daemon_log && daemon_log[0] != '\0') {
        FILE* f = fopen(daemon_log, "ab");
        if (f) {
            time_t rawtime;
            time(&rawtime);
            struct tm * timeinfo = localtime(&rawtime);
            char time_buf[80];
            if (timeinfo) {
                strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", timeinfo);
            } else {
                snprintf(time_buf, sizeof(time_buf), "%lld", (long long)rawtime);
            }
            fprintf(f, "[%s] FATAL: Runtime Error: %s (signal %d)\n", time_buf, name, sig);
            fclose(f);
        }
    }

    _exit(128 + sig);
}

#ifdef _MSC_VER
static void ep_install_signal_handlers(void);
#pragma section(".CRT$XCU", read)
__declspec(allocate(".CRT$XCU")) static void (*_ep_init_signals)(void) = ep_install_signal_handlers;
static void ep_install_signal_handlers(void) {
#else
__attribute__((constructor))
static void ep_install_signal_handlers(void) {
#endif
    signal(SIGFPE, ep_signal_handler);
    signal(SIGSEGV, ep_signal_handler);
    signal(SIGABRT, ep_signal_handler);
#ifdef _WIN32
    { WSADATA wsa; WSAStartup(MAKEWORD(2,2), &wsa); }
#endif
}

#if defined(__wasm__)
  typedef int ep_thread_t;
  typedef int ep_mutex_t;
  typedef int ep_cond_t;
  #define ep_mutex_init(m) (void)(0)
  #define ep_mutex_lock(m) (void)(0)
  #define ep_mutex_unlock(m) (void)(0)
  #define ep_cond_init(c) (void)(0)
  #define ep_cond_wait(c, m) (void)(0)
  #define ep_cond_signal(c) (void)(0)
#elif defined(_WIN32)
  #include <winsock2.h>
  #include <ws2tcpip.h>
  #include <windows.h>
  #pragma comment(lib, "ws2_32.lib")
  typedef HANDLE ep_thread_t;
  typedef CRITICAL_SECTION ep_mutex_t;
  typedef CONDITION_VARIABLE ep_cond_t;
  #define ep_mutex_init(m) InitializeCriticalSection(m)
  #define ep_mutex_lock(m) EnterCriticalSection(m)
  #define ep_mutex_unlock(m) LeaveCriticalSection(m)
  #define ep_cond_init(c) InitializeConditionVariable(c)
  #define ep_cond_wait(c, m) SleepConditionVariableCS(c, m, INFINITE)
  #define ep_cond_signal(c) WakeConditionVariable(c)
#else
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
  #include <unistd.h>
  #include <netdb.h>
  #include <fcntl.h>
  #include <errno.h>
  #include <sys/select.h>
  #include <pthread.h>
  typedef pthread_t ep_thread_t;
  typedef pthread_mutex_t ep_mutex_t;
  typedef pthread_cond_t ep_cond_t;
  #define ep_mutex_init(m) pthread_mutex_init(m, NULL)
  #define ep_mutex_lock(m) pthread_mutex_lock(m)
  #define ep_mutex_unlock(m) pthread_mutex_unlock(m)
  #define ep_cond_init(c) pthread_cond_init(c, NULL)
  #define ep_cond_wait(c, m) pthread_cond_wait(c, m)
  #define ep_cond_signal(c) pthread_cond_signal(c)
#endif

/* ========== Ernos Mark-and-Sweep Garbage Collector ========== */

#include <setjmp.h>
#if !defined(__wasm__) && !defined(_WIN32)
#include <pthread.h>
#endif

typedef enum {
    EP_OBJ_LIST,
    EP_OBJ_STRING,
    EP_OBJ_STRUCT,
    EP_OBJ_CLOSURE,
    EP_OBJ_MAP
} EpObjKind;

typedef struct EpGCObject {
    EpObjKind kind;
    int marked;
    void* ptr;                /* actual allocation pointer */
    long long size;           /* payload size for structs */
    long long num_fields;     /* number of fields for structs (each is long long) */
    int generation;           /* 0 = Nursery/young, 1 = Old */
    struct EpGCObject* next;  /* intrusive linked list */
} EpGCObject;

long long ep_time_now_ms(void);
long long ep_sleep_ms(long long ms);

typedef struct EpTask EpTask;
typedef struct {
    long long chan;
    int completed;
    long long value;
    EpTask* waiting_task;
} EpFuture;

static long long ep_await_future(EpFuture* fut);

struct EpTask {
    long long (*step)(void*); /* pointer to step function */
    void* args;               /* pointer to step state arguments */
    long long args_size_bytes; /* size of args struct for GC tracing */
    EpTask* next;             /* run-queue link pointer */
    EpFuture* fut;            /* future associated with this task */
    int state;                /* coroutine execution state */
    int is_cancelled;         /* cancellation flag for structured concurrency */
    struct EpTask* parent;    /* parent task for structured concurrency cancellation */
};

/* Event Loop Scheduler Globals & Functions */
static EpTask* ep_run_queue_head = NULL;
static EpTask* ep_run_queue_tail = NULL;
static EpTask* ep_current_task = NULL;
static int ep_event_loop_fd = -1; /* epoll or kqueue fd */
static int ep_active_io_sources = 0;

static void ep_task_enqueue(EpTask* task) {
    if (!task) return;
    task->next = NULL;
    if (ep_run_queue_tail) {
        ep_run_queue_tail->next = task;
        ep_run_queue_tail = task;
    } else {
        ep_run_queue_head = ep_run_queue_tail = task;
    }
}

static EpTask* ep_task_dequeue(void) {
    if (!ep_run_queue_head) return NULL;
    EpTask* task = ep_run_queue_head;
    ep_run_queue_head = ep_run_queue_head->next;
    if (!ep_run_queue_head) ep_run_queue_tail = NULL;
    return task;
}

#ifndef __wasm__
#ifdef __APPLE__
#include <sys/event.h>
#else
#include <sys/epoll.h>
#endif
#endif

static void ep_async_loop_init(void) {
    if (ep_event_loop_fd != -1) return;
#ifdef __wasm__
    ep_event_loop_fd = 999;
#elif defined(__APPLE__)
    ep_event_loop_fd = kqueue();
#else
    ep_event_loop_fd = epoll_create1(0);
#endif
}

typedef struct EpTimer {
    long long expiry_ms;
    EpTask* task;
    struct EpTimer* next;
} EpTimer;
static EpTimer* ep_timers_head = NULL;

static void ep_async_register_timer(long long timeout_ms, EpTask* task) {
    long long expiry = ep_time_now_ms() + timeout_ms;
    EpTimer* timer = (EpTimer*)malloc(sizeof(EpTimer));
    timer->expiry_ms = expiry;
    timer->task = task;
    timer->next = NULL;

    /* Insert sorted */
    if (!ep_timers_head || expiry < ep_timers_head->expiry_ms) {
        timer->next = ep_timers_head;
        ep_timers_head = timer;
    } else {
        EpTimer* cur = ep_timers_head;
        while (cur->next && cur->next->expiry_ms <= expiry) {
            cur = cur->next;
        }
        timer->next = cur->next;
        cur->next = timer;
    }
}

static long long ep_get_next_timer_timeout(void) {
    if (!ep_timers_head) return -1; /* block indefinitely */
    long long now = ep_time_now_ms();
    long long diff = ep_timers_head->expiry_ms - now;
    return diff < 0 ? 0 : diff;
}

static void ep_process_expired_timers(void) {
    long long now = ep_time_now_ms();
    while (ep_timers_head && ep_timers_head->expiry_ms <= now) {
        EpTimer* expired = ep_timers_head;
        ep_timers_head = ep_timers_head->next;
        ep_task_enqueue(expired->task);
        free(expired);
    }
}

static void ep_async_register_read(int fd, EpTask* task) {
#ifdef __wasm__
    (void)fd;
    (void)task;
#else
    ep_async_loop_init();
    ep_active_io_sources++;
#ifdef __APPLE__
    struct kevent ev;
    EV_SET(&ev, fd, EVFILT_READ, EV_ADD | EV_ONESHOT, 0, 0, task);
    kevent(ep_event_loop_fd, &ev, 1, NULL, 0, NULL);
#else
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLONESHOT;
    ev.data.ptr = task;
    if (epoll_ctl(ep_event_loop_fd, EPOLL_CTL_ADD, fd, &ev) < 0) {
        epoll_ctl(ep_event_loop_fd, EPOLL_CTL_MOD, fd, &ev);
    }
#endif
#endif
}

static void ep_async_wait_step(long long timeout) {
#ifdef __wasm__
    if (timeout > 0) {
        ep_sleep_ms(timeout);
    }
#else
#ifdef __APPLE__
    struct kevent events[16];
    struct timespec ts;
    struct timespec* p_ts = NULL;
    if (timeout >= 0) {
        ts.tv_sec = timeout / 1000;
        ts.tv_nsec = (timeout % 1000) * 1000000;
        p_ts = &ts;
    }
    int n = kevent(ep_event_loop_fd, NULL, 0, events, 16, p_ts);
    for (int i = 0; i < n; i++) {
        EpTask* t = (EpTask*)events[i].udata;
        ep_task_enqueue(t);
        ep_active_io_sources--;
    }
#else
    struct epoll_event events[16];
    int n = epoll_wait(ep_event_loop_fd, events, 16, (int)timeout);
    for (int i = 0; i < n; i++) {
        EpTask* t = (EpTask*)events[i].data.ptr;
        ep_task_enqueue(t);
        ep_active_io_sources--;
    }
#endif
#endif
    ep_process_expired_timers();
}

static void ep_async_loop_run(void) {
    ep_async_loop_init();
    while (ep_run_queue_head || ep_timers_head || ep_active_io_sources > 0) {
        /* 1. Run all runnable tasks */
        while (ep_run_queue_head) {
            EpTask* task = ep_task_dequeue();
            if (task->is_cancelled) {
                if (task->fut) {
                    task->fut->completed = 1;
                    task->fut->value = -1;
                }
                free(task->args);
                free(task);
                continue;
            }
            ep_current_task = task;
            long long res = task->step(task->args);
            ep_current_task = NULL;
            if (res != -999999) {
                if (task->fut) {
                    task->fut->value = res;
                    task->fut->completed = 1;
                    if (task->fut->waiting_task) {
                        ep_task_enqueue(task->fut->waiting_task);
                        task->fut->waiting_task = NULL;
                    }
                }
                free(task->args);
                free(task);
            }
        }

        /* 2. If no tasks runnable, wait for I/O / timers */
        if (!ep_run_queue_head) {
            long long timeout = ep_get_next_timer_timeout();
            if (timeout == -1 && !ep_timers_head && ep_active_io_sources == 0) {
                break;
            }

            if (ep_event_loop_fd == -1) {
                if (timeout > 0) {
                    ep_sleep_ms(timeout);
                }
                ep_process_expired_timers();
                continue;
            }

            ep_async_wait_step(timeout);
        }
    }
}

static long long ep_await_future(EpFuture* fut) {
    if (!fut) return 0;
    while (!fut->completed) {
        if (ep_run_queue_head) {
            EpTask* task = ep_task_dequeue();
            if (task) {
                if (task->is_cancelled) {
                    if (task->fut) {
                        task->fut->completed = 1;
                        task->fut->value = -1;
                    }
                    free(task->args);
                    free(task);
                } else {
                    EpTask* saved_current = ep_current_task;
                    ep_current_task = task;
                    long long res = task->step(task->args);
                    ep_current_task = saved_current;
                    if (res != -999999) {
                        if (task->fut) {
                            task->fut->value = res;
                            task->fut->completed = 1;
                            if (task->fut->waiting_task) {
                                ep_task_enqueue(task->fut->waiting_task);
                                task->fut->waiting_task = NULL;
                            }
                        }
                        free(task->args);
                        free(task);
                    }
                }
            }
        } else {
            long long timeout = ep_get_next_timer_timeout();
            if (timeout == -1 && !ep_timers_head && ep_active_io_sources == 0) {
                fprintf(stderr, "Deadlock detected: awaiting incomplete future with no active tasks or timers.\n");
                exit(1);
            }
            if (ep_event_loop_fd == -1) {
                if (timeout > 0) {
                    ep_sleep_ms(timeout);
                }
                ep_process_expired_timers();
            } else {
                ep_async_wait_step(timeout);
            }
        }
    }
    return fut->value;
}

static EpGCObject* ep_gc_register(void* ptr, EpObjKind kind);
long long create_list(void);
long long append_list(long long list_ptr, long long value);

typedef struct {
    EpFuture* futures[128];
    int count;
    int has_error;
} EpTaskGroup;

typedef struct {
    EpFuture* fut;
    int timer_fired;
} EpTimeoutArgs;

static EpTask* ep_find_task_by_future(EpFuture* fut) {
    if (!fut) return NULL;
    EpTask* cur = ep_run_queue_head;
    while (cur) {
        if (cur->fut == fut) return cur;
        cur = cur->next;
    }
    EpTimer* timer = ep_timers_head;
    while (timer) {
        if (timer->task && timer->task->fut == fut) return timer->task;
        timer = timer->next;
    }
    return NULL;
}

static void ep_cancel_task(EpTask* task) {
    if (!task) return;
    task->is_cancelled = 1;
    if (task->fut) {
        task->fut->completed = 1;
        task->fut->value = -1;
    }
    // Cancel children in run queue
    EpTask* cur = ep_run_queue_head;
    while (cur) {
        if (cur->parent == task) {
            ep_cancel_task(cur);
        }
        cur = cur->next;
    }
    // Cancel children in timers queue
    EpTimer* timer = ep_timers_head;
    while (timer) {
        if (timer->task && timer->task->parent == task) {
            ep_cancel_task(timer->task);
        }
        timer = timer->next;
    }
}

static long long create_task_group(void) {
    EpTaskGroup* tg = (EpTaskGroup*)calloc(1, sizeof(EpTaskGroup));
    tg->count = 0;
    tg->has_error = 0;
    { EpGCObject* _go = ep_gc_register(tg, EP_OBJ_STRUCT); if(_go) _go->num_fields = 0; }
    return (long long)tg;
}

static long long add_task_group(long long group_ptr, long long fut_ptr) {
    EpTaskGroup* tg = (EpTaskGroup*)group_ptr;
    EpFuture* fut = (EpFuture*)fut_ptr;
    if (!tg || !fut) return 0;
    if (tg->count < 128) {
        tg->futures[tg->count++] = fut;
        // Associate the task's parent with the current task so it's cancellation-linked
        EpTask* task = ep_find_task_by_future(fut);
        if (task) {
            task->parent = ep_current_task;
        }
    }
    return 0;
}

static long long wait_task_group(long long group_ptr) {
    EpTaskGroup* tg = (EpTaskGroup*)group_ptr;
    if (!tg) return 0;

    long long ep_wait_group_spin = 0;
    int all_done = 0;
    while (!all_done) {
        all_done = 1;
        for (int i = 0; i < tg->count; i++) {
            EpFuture* fut = tg->futures[i];
            if (!fut->completed) {
                all_done = 0;
                break;
            }
        }
        
        if (all_done) break;
        
        if (ep_run_queue_head) {
            EpTask* task = ep_task_dequeue();
            if (task) {
                if (task->is_cancelled) {
                    if (task->fut) {
                        task->fut->completed = 1;
                        task->fut->value = -1;
                    }
                    free(task->args);
                    free(task);
                } else {
                    EpTask* saved_current = ep_current_task;
                    ep_current_task = task;
                    long long res = task->step(task->args);
                    ep_current_task = saved_current;
                    if (res != -999999) {
                        if (task->fut) {
                            task->fut->value = res;
                            task->fut->completed = 1;
                            if (task->fut->waiting_task) {
                                ep_task_enqueue(task->fut->waiting_task);
                                task->fut->waiting_task = NULL;
                            }
                        }
                        free(task->args);
                        free(task);
                    }
                }
            }
        } else {
            long long timeout = ep_get_next_timer_timeout();
            if (timeout == -1 && !ep_timers_head && ep_active_io_sources == 0) {
                /* No coroutine tasks/timers/IO to drive. The futures may still be
                   completed by detached worker THREADS (the self-hosted compiler
                   emits thread-based async), so poll for their completion rather
                   than declaring deadlock. Bounded so a genuinely stuck group
                   still fails instead of hanging forever. */
                ep_sleep_ms(1);
                if (++ep_wait_group_spin > 60000) {
                    fprintf(stderr, "Deadlock detected: waiting on task group with no active tasks or timers.\n");
                    exit(1);
                }
                continue;
            }
            ep_wait_group_spin = 0;
            if (ep_event_loop_fd == -1) {
                if (timeout > 0) {
                    ep_sleep_ms(timeout);
                }
                ep_process_expired_timers();
            } else {
                ep_async_wait_step(timeout);
            }
        }
        
        // Propagate cancellation/failure inside task group
        for (int i = 0; i < tg->count; i++) {
            EpFuture* fut = tg->futures[i];
            if (fut->completed && fut->value == -1) {
                tg->has_error = 1;
                for (int j = 0; j < tg->count; j++) {
                    EpFuture* other_fut = tg->futures[j];
                    if (!other_fut->completed) {
                        EpTask* other_task = ep_find_task_by_future(other_fut);
                        if (other_task) {
                            ep_cancel_task(other_task);
                        } else {
                            other_fut->completed = 1;
                            other_fut->value = -1;
                        }
                    }
                }
            }
        }
    }
    
    long long list = create_list();
    for (int i = 0; i < tg->count; i++) {
        append_list(list, tg->futures[i]->value);
    }
    return list;
}

static long long ep_timeout_timer_step(void* r) {
    EpTimeoutArgs* args = (EpTimeoutArgs*)r;
    if (args && args->fut && !args->fut->completed) {
        args->timer_fired = 1;
        EpTask* task = ep_find_task_by_future(args->fut);
        if (task) {
            ep_cancel_task(task);
        } else {
            args->fut->completed = 1;
            args->fut->value = -1;
        }
    }
    return 0;
}

static long long async_timeout(long long timeout_ms, long long fut_ptr) {
    EpFuture* fut = (EpFuture*)fut_ptr;
    if (!fut) return -1;
    if (fut->completed) return fut->value;
    
    EpTimeoutArgs* args = (EpTimeoutArgs*)malloc(sizeof(EpTimeoutArgs));
    args->fut = fut;
    args->timer_fired = 0;
    
    EpTask* timer_task = (EpTask*)malloc(sizeof(EpTask));
    timer_task->step = ep_timeout_timer_step;
    timer_task->args = args;
    timer_task->args_size_bytes = sizeof(EpTimeoutArgs);
    timer_task->fut = NULL;
    timer_task->state = 0;
    timer_task->is_cancelled = 0;
    timer_task->parent = NULL;
    
    ep_async_register_timer(timeout_ms, timer_task);
    
    while (!fut->completed && !(args->timer_fired)) {
        if (ep_run_queue_head) {
            EpTask* task = ep_task_dequeue();
            if (task) {
                if (task->is_cancelled) {
                    if (task->fut) {
                        task->fut->completed = 1;
                        task->fut->value = -1;
                    }
                    free(task->args);
                    free(task);
                } else {
                    EpTask* saved_current = ep_current_task;
                    ep_current_task = task;
                    long long res = task->step(task->args);
                    ep_current_task = saved_current;
                    if (res != -999999) {
                        if (task->fut) {
                            task->fut->value = res;
                            task->fut->completed = 1;
                            if (task->fut->waiting_task) {
                                ep_task_enqueue(task->fut->waiting_task);
                                task->fut->waiting_task = NULL;
                            }
                        }
                        free(task->args);
                        free(task);
                    }
                }
            }
        } else {
            long long timeout = ep_get_next_timer_timeout();
            if (timeout == -1 && !ep_timers_head && ep_active_io_sources == 0) {
                break;
            }
            if (ep_event_loop_fd == -1) {
                if (timeout > 0) {
                    ep_sleep_ms(timeout);
                }
                ep_process_expired_timers();
            } else {
                ep_async_wait_step(timeout);
            }
        }
    }
    
    return fut->value;
}

/* ── Awaitable async socket-readability ─────────────────────────────────────
   `await async_wait_readable(fd)` suspends the calling async task until `fd` is
   readable, letting the event loop run other tasks (e.g. another agent waiting on
   its own LLM socket) meanwhile. Mirrors sleep_ms: build a future, register a
   oneshot read-readiness task with the loop, return the future. When fd becomes
   readable, ep_async_wait_step re-enqueues the task; its step completes the future
   and wakes whoever awaited it. This is what lets I/O-bound agents run concurrently
   on ONE thread — no OS threads, no shared-heap GC race. */
typedef struct { EpFuture* fut; } EpReadReadyArgs;
static long long ep_read_ready_step(void* r) {
    EpReadReadyArgs* args = (EpReadReadyArgs*)r;
    if (args && args->fut) {
        args->fut->completed = 1;
        args->fut->value = 1;
        if (args->fut->waiting_task) {
            ep_task_enqueue(args->fut->waiting_task);
            args->fut->waiting_task = NULL;
        }
    }
    return 0;
}
long long async_wait_readable(long long fd) {
    EpFuture* fut = (EpFuture*)malloc(sizeof(EpFuture));
    fut->completed = 0;
    fut->value = 0;
    fut->waiting_task = NULL;
    fut->chan = 0;
    { EpGCObject* _go = ep_gc_register(fut, EP_OBJ_STRUCT); if(_go) _go->num_fields = 3; }
    EpReadReadyArgs* args = (EpReadReadyArgs*)malloc(sizeof(EpReadReadyArgs));
    args->fut = fut;
    EpTask* task = (EpTask*)malloc(sizeof(EpTask));
    task->step = ep_read_ready_step;
    task->args = args;
    task->args_size_bytes = sizeof(EpReadReadyArgs);
    task->fut = NULL;
    task->state = 0;
    task->is_cancelled = 0;
    task->parent = ep_current_task;
    ep_async_register_read((int)fd, task);
    return (long long)fut;
}

typedef struct {
    EpFuture* fut;
} EpSleepTimerArgs;

static long long ep_sleep_timer_step(void* r) {
    EpSleepTimerArgs* args = (EpSleepTimerArgs*)r;
    if (args && args->fut) {
        args->fut->completed = 1;
        args->fut->value = 0;
        if (args->fut->waiting_task) {
            ep_task_enqueue(args->fut->waiting_task);
            args->fut->waiting_task = NULL;
        }
    }
    return 0;
}

static long long sleep_ms(long long ms) {
    /* Outside the event loop (no task is being stepped) a registered timer
       would never fire on its own, so the cooperative path used to sleep for
       0 ms. Block for real instead, and hand back an already-completed
       future so `await sleep_ms(...)` from synchronous code still works. */
    if (!ep_current_task) {
        if (ms > 0) ep_sleep_ms(ms);
        EpFuture* done = (EpFuture*)malloc(sizeof(EpFuture));
        done->completed = 1;
        done->value = 0;
        done->waiting_task = NULL;
        done->chan = 0;
        { EpGCObject* _go = ep_gc_register(done, EP_OBJ_STRUCT); if(_go) _go->num_fields = 3; }
        return (long long)done;
    }
    EpFuture* fut = (EpFuture*)malloc(sizeof(EpFuture));
    fut->completed = 0;
    fut->value = 0;
    fut->waiting_task = NULL;
    fut->chan = 0;
    { EpGCObject* _go = ep_gc_register(fut, EP_OBJ_STRUCT); if(_go) _go->num_fields = 3; }

    EpSleepTimerArgs* args = (EpSleepTimerArgs*)malloc(sizeof(EpSleepTimerArgs));
    args->fut = fut;
    
    EpTask* task = (EpTask*)malloc(sizeof(EpTask));
    task->step = ep_sleep_timer_step;
    task->args = args;
    task->args_size_bytes = sizeof(EpSleepTimerArgs);
    task->fut = NULL;
    task->state = 0;
    task->is_cancelled = 0;
    task->parent = ep_current_task;
    
    ep_async_register_timer(ms, task);
    return (long long)fut;
}

static long long cancel_task(long long fut_ptr) {
    EpFuture* fut = (EpFuture*)fut_ptr;
    if (fut) {
        EpTask* task = ep_find_task_by_future(fut);
        if (task) {
            ep_cancel_task(task);
        } else {
            fut->completed = 1;
            fut->value = -1;
        }
    }
    return 0;
}

/* Closure environment — captures travel with the function pointer */
#define EP_CLOSURE_MAGIC 0x4550434C4FL
typedef struct {
    long long magic;
    long long fn_ptr;
    long long env[];  /* flexible array of captured values */
} EpClosure;

/* GC globals */
static EpGCObject* ep_gc_head = NULL;
static long long ep_gc_count = 0;
static long long ep_gc_threshold = 4096;
static int ep_gc_enabled = 1;
static long long ep_gc_nursery_count = 0;
static long long ep_gc_nursery_threshold = 512;
static int ep_gc_minor_count = 0;
static int ep_gc_major_count = 0;
static void** ep_gc_remembered_set = NULL;
static long long ep_gc_remembered_cap = 0;
static long long ep_gc_remembered_size = 0;
/* Single mutex for ALL GC and thread registry operations.
   Previous design had two mutexes (ep_gc_mutex + ep_thread_registry_mutex)
   which caused deadlock under concurrent channel load: thread A held gc_mutex
   and waited for registry_mutex, thread B held registry_mutex and waited for
   gc_mutex. Single lock eliminates the ordering problem. */
#ifdef __wasm__
#define __thread
#endif
static pthread_mutex_t ep_gc_mutex = PTHREAD_MUTEX_INITIALIZER;

/* Stop-the-world coordination. The collector sets ep_gc_stop_requested and, in
   ep_gc_stop_the_world(), waits until every *other* registered thread has parked
   at a safepoint (ep_gc_park_if_stopped). This guarantees mark/sweep never runs
   concurrently with a mutator changing its roots or an object's fields — the
   "marking races with running mutators" hazard. All three fields are touched
   only while holding ep_gc_mutex (the lock-free reads of ep_gc_stop_requested at
   safepoints are a benign optimization: a missed set just defers parking to the
   next safepoint, and the collector's bounded wait covers it). */
static volatile int ep_gc_stop_requested = 0;
static int ep_gc_parked_count = 0;
static pthread_cond_t ep_gc_resume_cond = PTHREAD_COND_INITIALIZER;

/* Function pointer for channel scanning — set after EpChannel is defined.
   GC mark calls this to scan values in-transit in channel buffers. */
static void (*ep_gc_scan_channels_major)(void) = NULL;
static void (*ep_gc_scan_channels_minor)(void) = NULL;
/* Function pointers for marking top-level constant/global variables, which are
   GC roots that live outside any function frame. Set by __ep_init_constants. */
static void (*ep_gc_mark_globals_major)(void) = NULL;
static void (*ep_gc_mark_globals_minor)(void) = NULL;
/* Function pointers for map value traversal — set after EpMap is defined.
   GC mark calls these to recursively mark values stored in maps. */
static void (*ep_gc_mark_map_values)(void* ptr) = NULL;
static void (*ep_gc_mark_map_values_minor)(void* ptr) = NULL;

/* Thread registry for GC root scanning in multi-threaded environment */
#define EP_MAX_THREADS 256
static __thread void* volatile ep_thread_local_top = NULL;
static __thread void* ep_thread_local_bottom = NULL;

static void* volatile* ep_thread_tops[EP_MAX_THREADS];
static void* ep_thread_bottoms[EP_MAX_THREADS];
static volatile int ep_thread_active[EP_MAX_THREADS];
static int ep_num_threads = 0;

/* Per-thread GC root state — heap-allocated, stable across thread lifetime.
   Previous design stored raw pointers to __thread arrays (ep_gc_root_stack,
   ep_gc_root_sp) in the global registry. When a thread exited, the __thread
   storage was freed, leaving dangling pointers that ep_gc_mark would
   dereference → segfault. Now each thread gets a heap-allocated state struct
   that survives thread exit and is only recycled when the slot is reused. */
typedef struct {
    long long* roots[4096];  /* copy of root pointers, updated under lock */
    volatile int sp;         /* current root stack pointer */
} EpThreadGCState;

static EpThreadGCState* ep_thread_gc_states[EP_MAX_THREADS];

/* Shadow stack for explicit GC roots — thread-local to prevent cross-thread corruption */
#define EP_GC_MAX_ROOTS 4096
static __thread long long* ep_gc_root_stack[EP_GC_MAX_ROOTS];
static __thread int ep_gc_root_sp = 0;
static __thread int ep_thread_slot = -1;

/* ep_gc_root_sp is the *logical* shadow-stack depth. It always advances on
   push and retreats on pop so that per-frame push/pop counts stay balanced.
   Array storage is capped at EP_GC_MAX_ROOTS: once the stack is full, further
   roots are counted but not stored (those deep-overflow locals are simply not
   traced) — crucially, we never overwrite or drop an outer frame's stored
   roots, which the old "silently skip the push but still pop" path did. */
static void ep_gc_push_root(long long* root) {
    int idx = ep_gc_root_sp;
    ep_gc_root_sp++;
    if (idx < EP_GC_MAX_ROOTS) {
        ep_gc_root_stack[idx] = root;
        /* Update the heap-allocated state so GC mark can see it safely */
        if (ep_thread_slot >= 0 && ep_thread_gc_states[ep_thread_slot]) {
            ep_thread_gc_states[ep_thread_slot]->roots[idx] = root;
            ep_thread_gc_states[ep_thread_slot]->sp =
                (ep_gc_root_sp < EP_GC_MAX_ROOTS) ? ep_gc_root_sp : EP_GC_MAX_ROOTS;
        }
    }
}
static void ep_gc_pop_roots(long long count) {
    ep_gc_root_sp -= (int)count;
    if (ep_gc_root_sp < 0) ep_gc_root_sp = 0;
    /* Update the heap-allocated state (clamped to the array bound) */
    if (ep_thread_slot >= 0 && ep_thread_gc_states[ep_thread_slot]) {
        ep_thread_gc_states[ep_thread_slot]->sp =
            (ep_gc_root_sp < EP_GC_MAX_ROOTS) ? ep_gc_root_sp : EP_GC_MAX_ROOTS;
    }
}

/* Park the calling thread if the collector has stopped the world.
   MUST be called with ep_gc_mutex held. The thread's shadow stack (its precise
   root set) is stable while parked, so the collector can scan it race-free. */
static void ep_gc_park_if_stopped(void) {
    if (!ep_gc_stop_requested) return;
    /* Spill registers onto the stack and publish this thread's current stack top
       so the collector can conservatively scan its frozen C stack while parked —
       this catches roots held only in registers/temporaries that the precise
       shadow stack does not yet record. _dummy is declared below _pregs, so its
       (lower) address bounds a scan range that covers the spilled registers. */
    jmp_buf _pregs;
    volatile char _top_marker;  /* function-scope: stays valid while parked */
    memset(&_pregs, 0, sizeof(_pregs));
    setjmp(_pregs);
    /* _top_marker is declared after _pregs, so its (lower) address bounds a scan
       range [&_top_marker, stack_bottom] that covers the spilled registers. */
    ep_thread_local_top = (void*)&_top_marker;
    __sync_synchronize();  /* publish shadow-stack + top writes before parking */
    ep_gc_parked_count++;
    while (ep_gc_stop_requested) {
        pthread_cond_wait(&ep_gc_resume_cond, &ep_gc_mutex);
    }
    ep_gc_parked_count--;
}

/* Begin a stop-the-world pause. MUST be called with ep_gc_mutex held.
   Waits (briefly releasing the lock so blocked mutators can reach a safepoint)
   until all other registered threads have parked. After a bounded fallback
   (~50ms) it proceeds anyway: any thread that hasn't parked by then is blocked
   or idle with a stable shadow stack, so scanning it is still safe in practice. */
static void ep_gc_stop_the_world(void) {
    ep_gc_stop_requested = 1;
    /* Actively-running threads reach a safepoint (every allocation and every
       function entry) within microseconds, so they park on the first spin or
       two. The bound only caps the rare case where a thread is blocked/idle
       (e.g. just entered a channel op) and won't park — those have a stable
       shadow stack, so proceeding to scan them is safe. ~40 * 250us ≈ 10ms. */
    for (int spins = 0; spins < 40; spins++) {
        int others = 0;
        for (int t = 0; t < ep_num_threads; t++) {
            if (ep_thread_active[t] && t != ep_thread_slot) others++;
        }
        if (others <= 0 || ep_gc_parked_count >= others) return;
        pthread_mutex_unlock(&ep_gc_mutex);
#ifdef _WIN32
        Sleep(1);
#elif !defined(__wasm__)
        usleep(250);
#endif
        pthread_mutex_lock(&ep_gc_mutex);
    }
}

/* End a stop-the-world pause and wake all parked threads. MUST hold ep_gc_mutex. */
static void ep_gc_start_the_world(void) {
    ep_gc_stop_requested = 0;
    pthread_cond_broadcast(&ep_gc_resume_cond);
}

static void ep_gc_register_thread(void* stack_bottom) {
    ep_thread_local_bottom = stack_bottom;
    ep_thread_local_top = stack_bottom;
    
    pthread_mutex_lock(&ep_gc_mutex);
    int slot = -1;
    for (int i = 0; i < ep_num_threads; i++) {
        if (!ep_thread_active[i]) {
            slot = i;
            break;
        }
    }
    if (slot == -1 && ep_num_threads < EP_MAX_THREADS) {
        slot = ep_num_threads++;
    }
    if (slot != -1) {
        ep_thread_tops[slot] = &ep_thread_local_top;
        ep_thread_bottoms[slot] = stack_bottom;
        /* Allocate or reuse heap state for this slot */
        if (!ep_thread_gc_states[slot]) {
            ep_thread_gc_states[slot] = (EpThreadGCState*)calloc(1, sizeof(EpThreadGCState));
        }
        ep_thread_gc_states[slot]->sp = 0;
        ep_thread_slot = slot;
        __sync_synchronize();  /* Memory barrier: state must be visible before active */
        ep_thread_active[slot] = 1;
    }
    pthread_mutex_unlock(&ep_gc_mutex);
}

static void ep_gc_unregister_thread(void) {
    pthread_mutex_lock(&ep_gc_mutex);
    for (int i = 0; i < ep_num_threads; i++) {
        if (ep_thread_active[i] && ep_thread_tops[i] == &ep_thread_local_top) {
            /* Zero root count FIRST — even if ep_gc_mark races past the
               active check, it will see sp=0 and walk no roots instead
               of dereferencing stale __thread pointers */
            if (ep_thread_gc_states[i]) {
                ep_thread_gc_states[i]->sp = 0;
            }
            __sync_synchronize();  /* Memory barrier: sp=0 visible before deactivation */
            ep_thread_active[i] = 0;
            ep_thread_slot = -1;
            break;
        }
    }
    pthread_mutex_unlock(&ep_gc_mutex);
}

#define EP_GC_UPDATE_TOP() { volatile int _dummy; ep_thread_local_top = (void*)&_dummy; }

/* Simple open-addressed hash map with linear probing for O(1) GC object lookup */
typedef struct {
    void* key;
    EpGCObject* value;
} EpGCEntry;

static EpGCEntry* ep_gc_table = NULL;
static long long ep_gc_table_cap = 0;
static long long ep_gc_table_size = 0;

/* Bucket index for a pointer key. The previous hash was ((uintptr_t)key % cap)
   with cap a power of two; malloc returns 16-byte-aligned pointers, so the low 4
   bits are always 0 and only every 16th bucket was ever a home slot. That caused
   catastrophic primary clustering -> O(n) probe runs -> ep_gc_table_remove's
   rehash became O(n^2), which (under the single global GC mutex) wedged the whole
   node when a large object list was freed. A splitmix64 finalizer avalanches all
   bits, so even the low bits taken by the (cap-1) mask are well distributed. */
static inline long long ep_gc_index(void* key, long long cap) {
    uint64_t z = (uint64_t)(uintptr_t)key;
    z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ULL;
    z = (z ^ (z >> 27)) * 0x94d049bb133111ebULL;
    z = z ^ (z >> 31);
    return (long long)(z & (uint64_t)(cap - 1));   /* cap is always a power of two */
}

/* Insert without growing — assumes a free slot exists. Used by the resize and by
   ep_gc_table_remove's rehash, neither of which may trigger a (re)allocation of
   the table mid-iteration. */
static void ep_gc_table_place(void* key, EpGCObject* value) {
    long long idx = ep_gc_index(key, ep_gc_table_cap);
    while (ep_gc_table[idx].key != NULL) {
        if (ep_gc_table[idx].key == key) {
            ep_gc_table[idx].value = value;
            return;
        }
        idx = (idx + 1) & (ep_gc_table_cap - 1);
    }
    ep_gc_table[idx].key = key;
    ep_gc_table[idx].value = value;
    ep_gc_table_size++;
}

static void ep_gc_table_insert(void* key, EpGCObject* value) {
    if (ep_gc_table_size * 2 >= ep_gc_table_cap) {
        long long old_cap = ep_gc_table_cap;
        long long new_cap = old_cap == 0 ? 512 : old_cap * 2;
        EpGCEntry* new_table = (EpGCEntry*)calloc(new_cap, sizeof(EpGCEntry));
        EpGCEntry* old_table = ep_gc_table;
        ep_gc_table = new_table;
        ep_gc_table_cap = new_cap;
        ep_gc_table_size = 0;
        for (long long i = 0; i < old_cap; i++) {
            if (old_table[i].key != NULL) {
                ep_gc_table_place(old_table[i].key, old_table[i].value);
            }
        }
        free(old_table);
    }
    ep_gc_table_place(key, value);
}

static EpGCObject* ep_gc_table_get(void* key) {
    if (ep_gc_table_cap == 0) return NULL;
    long long idx = ep_gc_index(key, ep_gc_table_cap);
    while (ep_gc_table[idx].key != NULL) {
        if (ep_gc_table[idx].key == key) return ep_gc_table[idx].value;
        idx = (idx + 1) & (ep_gc_table_cap - 1);
    }
    return NULL;
}

static void ep_gc_table_remove(void* key) {
    if (ep_gc_table_cap == 0) return;
    long long idx = ep_gc_index(key, ep_gc_table_cap);
    while (ep_gc_table[idx].key != NULL) {
        if (ep_gc_table[idx].key == key) {
            ep_gc_table[idx].key = NULL;
            ep_gc_table[idx].value = NULL;
            ep_gc_table_size--;
            /* Backward-shift rehash of the rest of this cluster. Re-place (no
               resize: size is not growing) so a mid-iteration realloc can never
               free the table out from under this loop. */
            long long next_idx = (idx + 1) & (ep_gc_table_cap - 1);
            while (ep_gc_table[next_idx].key != NULL) {
                void* rehash_key = ep_gc_table[next_idx].key;
                EpGCObject* rehash_val = ep_gc_table[next_idx].value;
                ep_gc_table[next_idx].key = NULL;
                ep_gc_table[next_idx].value = NULL;
                ep_gc_table_size--;
                ep_gc_table_place(rehash_key, rehash_val);
                next_idx = (next_idx + 1) & (ep_gc_table_cap - 1);
            }
            return;
        }
        idx = (idx + 1) & (ep_gc_table_cap - 1);
    }
}



/* Register a new GC object */
static EpGCObject* ep_gc_register(void* ptr, EpObjKind kind) {
    if (!ptr) return NULL;
    pthread_mutex_lock(&ep_gc_mutex);
    ep_gc_park_if_stopped();  /* safepoint: don't allocate/touch the table mid-collection */
    EpGCObject* obj = (EpGCObject*)malloc(sizeof(EpGCObject));
    if (!obj) {
        pthread_mutex_unlock(&ep_gc_mutex);
        return NULL;
    }
    obj->kind = kind;
    obj->marked = 0;
    obj->ptr = ptr;
    obj->size = 0;
    obj->num_fields = 0;
    obj->generation = 0;
    obj->next = ep_gc_head;
    ep_gc_head = obj;
    ep_gc_count++;
    ep_gc_nursery_count++;
    ep_gc_table_insert(ptr, obj);
    pthread_mutex_unlock(&ep_gc_mutex);
    return obj;
}

/* Find GC object by pointer.
   Takes ep_gc_mutex because ep_gc_table_insert may realloc+free the table
   concurrently (from another thread's allocation). Mutator-side callers
   (write barrier, free_struct/free_map/free_list, to-string) must use this
   locking variant; code already holding the mutex (mark/sweep) calls
   ep_gc_table_get directly to avoid a non-recursive double-lock deadlock. */
static EpGCObject* ep_gc_find(void* ptr) {
    pthread_mutex_lock(&ep_gc_mutex);
    ep_gc_park_if_stopped();  /* safepoint */
    EpGCObject* obj = ep_gc_table_get(ptr);
    pthread_mutex_unlock(&ep_gc_mutex);
    return obj;
}

/* Write barrier for generational GC: tracks references from old objects (gen 1) to young objects (gen 0).
   The whole operation runs under ep_gc_mutex so the table lookups and the
   remembered-set update see a consistent table (no race with a concurrent
   resize) and use the no-lock ep_gc_table_get to avoid re-entering the lock. */
static void ep_gc_write_barrier(void* host_ptr, long long val) {
    if (val == 0) return;
    pthread_mutex_lock(&ep_gc_mutex);
    ep_gc_park_if_stopped();  /* safepoint: don't update the remembered set mid-collection */
    EpGCObject* host_obj = ep_gc_table_get(host_ptr);
    EpGCObject* val_obj = ep_gc_table_get((void*)val);
    if (host_obj && val_obj && host_obj->generation == 1 && val_obj->generation == 0) {
        /* Check if already in remembered set */
        int found = 0;
        for (long long i = 0; i < ep_gc_remembered_size; i++) {
            if (ep_gc_remembered_set[i] == (void*)val) {
                found = 1;
                break;
            }
        }
        if (!found) {
            if (ep_gc_remembered_size >= ep_gc_remembered_cap) {
                long long new_cap = ep_gc_remembered_cap == 0 ? 128 : ep_gc_remembered_cap * 2;
                void** new_set = (void**)realloc(ep_gc_remembered_set, new_cap * sizeof(void*));
                if (new_set) {
                    ep_gc_remembered_set = new_set;
                    ep_gc_remembered_cap = new_cap;
                }
            }
            if (ep_gc_remembered_size < ep_gc_remembered_cap) {
                ep_gc_remembered_set[ep_gc_remembered_size++] = (void*)val;
            }
        }
    }
    pthread_mutex_unlock(&ep_gc_mutex);
}

/* Forward declarations for list type (needed by GC mark) */
typedef struct {
    long long* data;
    long long length;
    long long capacity;
} EpList;

/* A real heap object (list/map/string) is malloc'd, so its address is far above
   the never-mapped first page. EP values that are NOT pointers — small ints,
   booleans, and JSON type-tags (2=string, 3=list, 4=object) — land in [0,4096).
   Guarding the object accessors with this turns "deref a non-pointer" (the cause
   of the read_transcripts segfault, and that whole class) into a safe null return
   instead of a daemon-killing SIGSEGV. One comparison; negligible on hot paths. */
#define EP_BADPTR(p) (((unsigned long long)(p)) < 4096ULL)

/* Mark a single object and recursively mark its children */
static void ep_gc_mark_object(void* ptr) {
    if (!ptr) return;
    /* Runs under ep_gc_mutex (held by the collector) — use the no-lock lookup. */
    EpGCObject* obj = ep_gc_table_get(ptr);
    if (!obj || obj->marked) return;
    obj->marked = 1;

    if (obj->kind == EP_OBJ_LIST) {
        EpList* list = (EpList*)ptr;
        for (long long i = 0; i < list->length; i++) {
            long long val = list->data[i];
            if (val != 0) {
                ep_gc_mark_object((void*)val);
            }
        }
    } else if (obj->kind == EP_OBJ_STRUCT) {
        long long* fields = (long long*)ptr;
        for (long long i = 0; i < obj->num_fields; i++) {
            if (fields[i] != 0) {
                ep_gc_mark_object((void*)fields[i]);
            }
        }
    } else if (obj->kind == EP_OBJ_MAP) {
        if (ep_gc_mark_map_values) ep_gc_mark_map_values(ptr);
    }
}

/* Mark a single object and recursively mark its children (only if it is Gen 0) */
static void ep_gc_mark_object_minor(void* ptr) {
    if (!ptr) return;
    /* Runs under ep_gc_mutex (held by the collector) — use the no-lock lookup. */
    EpGCObject* obj = ep_gc_table_get(ptr);
    if (!obj || obj->generation != 0 || obj->marked) return;
    obj->marked = 1;

    if (obj->kind == EP_OBJ_LIST) {
        EpList* list = (EpList*)ptr;
        for (long long i = 0; i < list->length; i++) {
            long long val = list->data[i];
            if (val != 0) {
                ep_gc_mark_object_minor((void*)val);
            }
        }
    } else if (obj->kind == EP_OBJ_STRUCT) {
        long long* fields = (long long*)ptr;
        for (long long i = 0; i < obj->num_fields; i++) {
            if (fields[i] != 0) {
                ep_gc_mark_object_minor((void*)fields[i]);
            }
        }
    } else if (obj->kind == EP_OBJ_MAP) {
        if (ep_gc_mark_map_values_minor) ep_gc_mark_map_values_minor(ptr);
    }
}

/* Conservatively scan every registered thread's C stack and mark any word that
   looks like a tracked pointer. The collector spills its own registers and
   publishes its top here; all other threads are parked at a safepoint with their
   registers spilled and top published (ep_gc_park_if_stopped), so their stacks
   are frozen. This complements the precise shadow stacks: it catches roots held
   only in registers/temporaries (e.g. a freshly allocated object not yet stored
   into a rooted slot). Non-pointer words are harmlessly ignored by ep_gc_find.

   Only run on MAJOR collections: minor collections rely on the precise shadow
   stacks plus the write barrier's remembered set (the standard generational
   approach), so they do no stack scan at all — which means there is no racy
   cross-thread stack read on the frequent minor path either. The expensive
   full-stack scan is paid only on the rarer major collection, where it pins
   any long-lived object reachable only via a register across many GCs.

   Marked no_sanitize_address: a conservative scan deliberately reads whole stack
   ranges (including ASAN redzones and out-of-frame slots), which is not a bug. */
#if defined(__SANITIZE_ADDRESS__)
# define EP_NO_ASAN __attribute__((no_sanitize_address))
#elif defined(__has_feature)
# if __has_feature(address_sanitizer)
#  define EP_NO_ASAN __attribute__((no_sanitize_address))
# endif
#endif
#ifndef EP_NO_ASAN
# define EP_NO_ASAN
#endif
EP_NO_ASAN
static void ep_gc_scan_thread_stacks(void) {
    jmp_buf _regs;
    volatile char _top_marker;
    memset(&_regs, 0, sizeof(_regs));
    setjmp(_regs);   /* spill the collector's own registers onto its stack */
    /* Publish the LOWEST of our own local addresses as this thread's live top, so the
       scanned range covers both the stack marker and the register-spill buffer whatever
       order the compiler laid them out (a missed _regs would drop a register-only root). */
    { char* _a = (char*)(void*)&_top_marker; char* _b = (char*)(void*)&_regs;
      ep_thread_local_top = (void*)((_a < _b) ? _a : _b); }
    for (int t = 0; t < ep_num_threads; t++) {
        if (!ep_thread_active[t]) continue;
        if (!ep_thread_tops[t]) continue;
        /* The published top comes from a char local, so it may not be pointer-aligned;
           mask DOWN to 8 bytes. Aligning down only widens the conservative window by a
           few harmless bytes — aligning up could skip the slot holding a live root.
           Unaligned void** dereferences are UB and produce a skewed scan window on
           strict platforms (caught by valgrind on Linux). */
        void** start = (void**)((uintptr_t)*ep_thread_tops[t] & ~(uintptr_t)7);
        void** end = (void**)ep_thread_bottoms[t];
        if (!start || !end) continue;
        if (start > end) { void** tmp = start; start = end; end = tmp; }
        for (void** cur = start; cur < end; cur++) {
            void* p = *cur;
            if (p) ep_gc_mark_object(p);
        }
    }
}

/* Mark phase: traverse from ALL threads' explicit GC roots.
   Uses the heap-allocated EpThreadGCState instead of raw __thread pointers. */
static void ep_gc_mark(void) {
    ep_gc_scan_thread_stacks();  /* conservative C-stack scan of all (parked) threads — major only */
    for (int t = 0; t < ep_num_threads; t++) {
        if (!ep_thread_active[t]) continue;
        EpThreadGCState* state = ep_thread_gc_states[t];
        if (!state) continue;
        int sp = state->sp;
        if (sp <= 0 || sp > EP_GC_MAX_ROOTS) continue;
        for (int i = 0; i < sp; i++) {
            long long* root_ptr = state->roots[i];
            if (!root_ptr) continue;
            long long val = *root_ptr;
            if (val != 0) {
                ep_gc_mark_object((void*)val);
            }
        }
    }
    /* Also mark from main thread's local root stack (thread 0 / unregistered) */
    int local_sp = ep_gc_root_sp;
    if (local_sp > EP_GC_MAX_ROOTS) local_sp = EP_GC_MAX_ROOTS;
    for (int i = 0; i < local_sp; i++) {
        long long val = *ep_gc_root_stack[i];
        if (val != 0) {
            ep_gc_mark_object((void*)val);
        }
    }
    /* Mark active tasks in the scheduler run queue */
    EpTask* task = ep_run_queue_head;
    while (task) {
        if (task->fut) {
            ep_gc_mark_object((void*)task->fut);
        }
        if (task->args && task->args_size_bytes > 0) {
            long long* ptr = (long long*)task->args;
            for (int i = 0; i < task->args_size_bytes / 8; i++) {
                long long val = ptr[i];
                if (val != 0) ep_gc_mark_object((void*)val);
            }
        }
        task = task->next;
    }
    /* Mark active tasks in the timers queue */
    EpTimer* timer = ep_timers_head;
    while (timer) {
        if (timer->task) {
            EpTask* t = timer->task;
            if (t->fut) {
                ep_gc_mark_object((void*)t->fut);
            }
            if (t->args && t->args_size_bytes > 0) {
                long long* ptr = (long long*)t->args;
                for (int i = 0; i < t->args_size_bytes / 8; i++) {
                    long long val = ptr[i];
                    if (val != 0) ep_gc_mark_object((void*)val);
                }
            }
        }
        timer = timer->next;
    }
    /* Mark top-level constant/global variables (roots outside any frame) */
    if (ep_gc_mark_globals_major) ep_gc_mark_globals_major();
    /* Scan all registered channel buffers — values in-transit have no root */
    if (ep_gc_scan_channels_major) ep_gc_scan_channels_major();
}

/* Conservatively scan the CURRENT thread's own live C stack and mark any YOUNG object it
   finds. This closes a use-after-free on the frequent minor path: a freshly-allocated
   argument temporary — e.g. the result of g() while f(g() and h()) is still evaluating
   h() — lives only on the C stack / in registers and is not yet on the precise shadow
   stack, so a minor collection triggered mid-expression would otherwise free it. Scanning
   ONLY the collecting thread's own stack is race-free (no cross-thread read) and cheap
   (one bounded stack, current thread only). Non-pointer words are harmlessly ignored by
   ep_gc_table_get; only generation-0 objects are marked. The setjmp spills register-held
   roots onto the stack so the scan can see them. */
EP_NO_ASAN
static void ep_gc_scan_own_stack_minor(void) {
    jmp_buf _regs;
    volatile char _marker;
    memset(&_regs, 0, sizeof(_regs));
    setjmp(_regs);   /* spill callee-saved registers into _regs, on the stack */
    void* bottom = ep_thread_local_bottom;
    if (!bottom) return;
    /* Start at the LOWEST of our own local addresses so the scanned range covers both
       the current stack top (_marker) and the register-spill buffer (_regs), regardless
       of how the compiler ordered these locals on the stack. Missing _regs would drop a
       root held only in a callee-saved register -> a rare use-after-free. */
    char* a = (char*)(void*)&_marker;
    char* b = (char*)(void*)&_regs;
    char* lo = (a < b) ? a : b;
    /* lo comes from a char local, so it may not be pointer-aligned; mask DOWN to 8
       bytes. Aligning down only widens the conservative window by a few harmless
       bytes — aligning up could skip the slot holding a live root. Unaligned void**
       dereferences are UB and skew the scan window on strict platforms (valgrind). */
    void** start = (void**)((uintptr_t)lo & ~(uintptr_t)7);
    void** end = (void**)bottom;
    if (start > end) { void** tmp = start; start = end; end = tmp; }
    for (void** cur = start; cur < end; cur++) {
        void* p = *cur;
        if (p) ep_gc_mark_object_minor(p);
    }
}

static void ep_gc_mark_minor(void) {
    /* Conservatively scan our OWN live C stack first, to catch freshly-allocated argument
       temporaries (only on the stack / in registers, not yet on the shadow stack) that a
       minor collection mid-expression would otherwise free. Own-thread only, so race-free. */
    ep_gc_scan_own_stack_minor();
    for (int t = 0; t < ep_num_threads; t++) {
        if (!ep_thread_active[t]) continue;
        EpThreadGCState* state = ep_thread_gc_states[t];
        if (!state) continue;
        int sp = state->sp;
        if (sp <= 0 || sp > EP_GC_MAX_ROOTS) continue;
        for (int i = 0; i < sp; i++) {
            long long* root_ptr = state->roots[i];
            if (!root_ptr) continue;
            long long val = *root_ptr;
            if (val != 0) {
                ep_gc_mark_object_minor((void*)val);
            }
        }
    }
    int local_sp = ep_gc_root_sp;
    if (local_sp > EP_GC_MAX_ROOTS) local_sp = EP_GC_MAX_ROOTS;
    for (int i = 0; i < local_sp; i++) {
        long long val = *ep_gc_root_stack[i];
        if (val != 0) {
            ep_gc_mark_object_minor((void*)val);
        }
    }
    /* Mark active tasks in the scheduler run queue for minor collection */
    EpTask* task = ep_run_queue_head;
    while (task) {
        if (task->fut) {
            ep_gc_mark_object_minor((void*)task->fut);
        }
        if (task->args && task->args_size_bytes > 0) {
            long long* ptr = (long long*)task->args;
            for (int i = 0; i < task->args_size_bytes / 8; i++) {
                long long val = ptr[i];
                if (val != 0) ep_gc_mark_object_minor((void*)val);
            }
        }
        task = task->next;
    }
    /* Mark active tasks in the timers queue for minor collection */
    EpTimer* timer = ep_timers_head;
    while (timer) {
        if (timer->task) {
            EpTask* t = timer->task;
            if (t->fut) {
                ep_gc_mark_object_minor((void*)t->fut);
            }
            if (t->args && t->args_size_bytes > 0) {
                long long* ptr = (long long*)t->args;
                for (int i = 0; i < t->args_size_bytes / 8; i++) {
                    long long val = ptr[i];
                    if (val != 0) ep_gc_mark_object_minor((void*)val);
                }
            }
        }
        timer = timer->next;
    }
    /* Also mark from the remembered set */
    for (long long i = 0; i < ep_gc_remembered_size; i++) {
        ep_gc_mark_object_minor(ep_gc_remembered_set[i]);
    }
    /* Mark top-level constant/global variables (roots outside any frame) */
    if (ep_gc_mark_globals_minor) ep_gc_mark_globals_minor();
    /* Scan all registered channel buffers — values in-transit have no root */
    if (ep_gc_scan_channels_minor) ep_gc_scan_channels_minor();
}

static void ep_gc_sweep_minor(void) {
    EpGCObject** cur = &ep_gc_head;
    while (*cur) {
        if ((*cur)->generation == 0) {
            if (!(*cur)->marked) {
                EpGCObject* garbage = *cur;
                *cur = garbage->next;
                ep_gc_table_remove(garbage->ptr);
                if (garbage->kind == EP_OBJ_LIST) {
                    EpList* list = (EpList*)garbage->ptr;
                    if (list) {
                        free(list->data);
                        free(list);
                    }
                } else if (garbage->kind == EP_OBJ_STRING) {
                    free(garbage->ptr);
                } else if (garbage->kind == EP_OBJ_STRUCT) {
                    free(garbage->ptr);
                } else if (garbage->kind == EP_OBJ_CLOSURE) {
                    free(garbage->ptr);
                } else if (garbage->kind == EP_OBJ_MAP) {
                    /* EpMap layout: entries*, capacity, size. Free entries then map. */
                    void** map_fields = (void**)garbage->ptr;
                    if (map_fields && map_fields[0]) free(map_fields[0]); /* entries */
                    free(garbage->ptr);
                }
                free(garbage);
                ep_gc_count--;
                ep_gc_nursery_count--;
            } else {
                (*cur)->marked = 0;
                (*cur)->generation = 1;
                ep_gc_nursery_count--;
                cur = &(*cur)->next;
            }
        } else {
            cur = &(*cur)->next;
        }
    }
    ep_gc_remembered_size = 0;
}

static void ep_gc_sweep_major(void) {
    EpGCObject** cur = &ep_gc_head;
    while (*cur) {
        if (!(*cur)->marked) {
            EpGCObject* garbage = *cur;
            *cur = garbage->next;
            ep_gc_table_remove(garbage->ptr);
            if (garbage->generation == 0) {
                ep_gc_nursery_count--;
            }
            if (garbage->kind == EP_OBJ_LIST) {
                EpList* list = (EpList*)garbage->ptr;
                if (list) {
                    free(list->data);
                    free(list);
                }
            } else if (garbage->kind == EP_OBJ_STRING) {
                free(garbage->ptr);
            } else if (garbage->kind == EP_OBJ_STRUCT) {
                free(garbage->ptr);
            } else if (garbage->kind == EP_OBJ_CLOSURE) {
                free(garbage->ptr);
            } else if (garbage->kind == EP_OBJ_MAP) {
                void** map_fields = (void**)garbage->ptr;
                if (map_fields && map_fields[0]) free(map_fields[0]);
                free(garbage->ptr);
            }
            free(garbage);
            ep_gc_count--;
        } else {
            (*cur)->marked = 0;
            if ((*cur)->generation == 0) {
                (*cur)->generation = 1;
                ep_gc_nursery_count--;
            }
            cur = &(*cur)->next;
        }
    }
    ep_gc_remembered_size = 0;
}

static void ep_gc_collect_minor(void) {
    if (!ep_gc_enabled) return;
    ep_gc_minor_count++;
    ep_gc_mark_minor();
    ep_gc_sweep_minor();
}

static void ep_gc_collect_major(void) {
    if (!ep_gc_enabled) return;
    ep_gc_major_count++;
    ep_gc_mark();
    ep_gc_sweep_major();
    ep_gc_threshold = ep_gc_count * 2;
    if (ep_gc_threshold < 4096) ep_gc_threshold = 4096;
}

/* Run a full GC collection — caller MUST hold ep_gc_mutex */
static void ep_gc_collect(void) {
    ep_gc_collect_major();
}

/* Maybe trigger GC if we've exceeded threshold. Also serves as the per-function
   GC safepoint: if another thread has stopped the world, park here until it's done. */
static void ep_gc_maybe_collect(void) {
    if (!ep_gc_enabled) return;  /* Early exit if GC suppressed (e.g. during channel ops) */
    /* Safepoint: lock-free fast check, then park under the lock if a collection
       is in progress on another thread. Keeps the no-GC path lock-free. */
    if (ep_gc_stop_requested) {
        pthread_mutex_lock(&ep_gc_mutex);
        ep_gc_park_if_stopped();
        pthread_mutex_unlock(&ep_gc_mutex);
    }
    /* Fast path: check thresholds before acquiring mutex.
       Counters are only incremented under the mutex, so worst case
       we miss one collection cycle — safe trade-off for avoiding
       a mutex lock/unlock (~20-50ns) on every function call. */
    if (ep_gc_nursery_count < ep_gc_nursery_threshold && ep_gc_count < ep_gc_threshold) return;
    EP_GC_UPDATE_TOP();
    pthread_mutex_lock(&ep_gc_mutex);
    /* Another thread may have started collecting between the check and the lock —
       park instead of racing it, then re-check thresholds under the lock. */
    ep_gc_park_if_stopped();
    if (ep_gc_nursery_count >= ep_gc_nursery_threshold || ep_gc_count >= ep_gc_threshold) {
        ep_gc_stop_the_world();
        if (ep_gc_nursery_count >= ep_gc_nursery_threshold) {
            ep_gc_collect_minor();
        }
        if (ep_gc_count >= ep_gc_threshold) {
            ep_gc_collect_major();
        }
        ep_gc_start_the_world();
    }
    pthread_mutex_unlock(&ep_gc_mutex);
}

/* Unregister an object (for explicit free — removes from GC tracking) */
static void ep_gc_unregister(void* ptr) {
    if (!ptr) return;
    pthread_mutex_lock(&ep_gc_mutex);
    ep_gc_park_if_stopped();  /* safepoint: don't mutate the table mid-collection */
    /* Clean up references from the remembered set to prevent dangling pointers */
    for (long long i = 0; i < ep_gc_remembered_size; ) {
        if (ep_gc_remembered_set[i] == ptr) {
            for (long long j = i; j < ep_gc_remembered_size - 1; j++) {
                ep_gc_remembered_set[j] = ep_gc_remembered_set[j + 1];
            }
            ep_gc_remembered_size--;
        } else {
            i++;
        }
    }
    ep_gc_table_remove(ptr);
    EpGCObject** cur = &ep_gc_head;
    while (*cur) {
        if ((*cur)->ptr == ptr) {
            EpGCObject* found = *cur;
            *cur = found->next;
            if (found->generation == 0) {
                ep_gc_nursery_count--;
            }
            free(found);
            ep_gc_count--;
            pthread_mutex_unlock(&ep_gc_mutex);
            return;
        }
        cur = &(*cur)->next;
    }
    pthread_mutex_unlock(&ep_gc_mutex);
}

/* Cleanup all remaining GC objects (called at program exit) */
static void ep_gc_shutdown(void) {
    ep_gc_enabled = 0;
    /* Only free GC bookkeeping structures, not the tracked objects themselves.
       The RAII auto-cleanup has already freed owned objects, and the OS will
       reclaim everything else on process exit. Attempting to free individual
       objects here causes double-free aborts when RAII and GC both track
       the same allocation. */
    EpGCObject* cur = ep_gc_head;
    while (cur) {
        EpGCObject* next = cur->next;
        free(cur);  /* free the GCObject wrapper only */
        cur = next;
    }
    ep_gc_head = NULL;
    ep_gc_count = 0;
    if (ep_gc_table) {
        free(ep_gc_table);
        ep_gc_table = NULL;
    }
    ep_gc_table_cap = 0;
    ep_gc_table_size = 0;
}

/* ========== End Garbage Collector ========== */

long long create_list(void);
long long append_list(long long list_ptr, long long value);
long long get_list(long long list_ptr, long long index);
long long set_list(long long list_ptr, long long index, long long value);
long long length_list(long long list_ptr);
long long free_list(long long list_ptr);
long long pop_list(long long list_ptr);
long long remove_list(long long list_ptr, long long index);
char* string_from_list(long long list_ptr);
long long string_to_list(const char* s);
long long string_length(const char* s);
long long display_string(const char* s);
long long screen_write(const char* s);
long long file_read(long long path_val);
long long file_write(long long path_val, long long content_val);
long long file_append(long long path_val, long long content_val);
long long file_exists(long long path_val);
long long string_contains(long long s_val, long long sub_val);
long long string_index_of(long long s_val, long long sub_val);
long long string_replace(long long s_val, long long old_val, long long new_val);
long long string_upper(long long s_val);
long long string_lower(long long s_val);
long long string_trim(long long s_val);
long long string_split(long long s_val, long long delim_val);
long long char_at(long long s_val, long long index);
long long char_from_code(long long code);
long long ep_abs(long long n);
long long json_get_string(long long json_val, long long key_val);
long long json_get_int(long long json_val, long long key_val);
long long json_get_bool(long long json_val, long long key_val);
long long ep_sha1(long long data_val);
long long ep_net_recv_bytes(long long fd, long long count);
long long ep_net_send_raw(long long fd, long long data_ptr, long long count);
long long channel_try_recv(long long chan_ptr, long long out_ptr);
long long channel_has_data(long long chan_ptr);
long long channel_select(long long channels_list, long long timeout_ms);
long long ep_auto_to_string(long long val);
long long ep_float_to_string(long long bits);

typedef struct EpChannel_ {
    long long* data;
    long long capacity;
    long long head;
    long long tail;
    long long size;
    ep_mutex_t mutex;
    ep_cond_t cond_recv;
    ep_cond_t cond_send;
} EpChannel;

/* Global channel registry — allows GC to scan values in-transit in channel buffers.
   Without this, an object sent to a channel but not yet received has NO GC root:
   the sender has popped it, the receiver hasn't pushed it, and the channel buffer
   is not scanned. The GC sweeps it → receiver gets a dangling pointer. */
#define EP_MAX_CHANNELS 1024
static EpChannel* ep_channel_registry[EP_MAX_CHANNELS];
static int ep_channel_count = 0;
static pthread_mutex_t ep_channel_registry_mutex = PTHREAD_MUTEX_INITIALIZER;

static void ep_register_channel(EpChannel* chan) {
    pthread_mutex_lock(&ep_channel_registry_mutex);
    if (ep_channel_count < EP_MAX_CHANNELS) {
        ep_channel_registry[ep_channel_count++] = chan;
    }
    pthread_mutex_unlock(&ep_channel_registry_mutex);
}

/* Channel scanning implementations — called by GC mark via function pointers.
   These are defined here (after EpChannel) so they can access struct fields. */
static void ep_gc_mark_object(void* ptr);     /* forward decl */
static void ep_gc_mark_object_minor(void* ptr); /* forward decl */

static void ep_gc_scan_channels_major_impl(void) {
    pthread_mutex_lock(&ep_channel_registry_mutex);
    for (int c = 0; c < ep_channel_count; c++) {
        EpChannel* chan = ep_channel_registry[c];
        if (!chan || chan->size <= 0) continue;
        ep_mutex_lock(&chan->mutex);
        for (long long j = 0; j < chan->size; j++) {
            long long idx = (chan->head + j) % chan->capacity;
            long long val = chan->data[idx];
            if (val != 0) ep_gc_mark_object((void*)val);
        }
        ep_mutex_unlock(&chan->mutex);
    }
    pthread_mutex_unlock(&ep_channel_registry_mutex);
}

static void ep_gc_scan_channels_minor_impl(void) {
    pthread_mutex_lock(&ep_channel_registry_mutex);
    for (int c = 0; c < ep_channel_count; c++) {
        EpChannel* chan = ep_channel_registry[c];
        if (!chan || chan->size <= 0) continue;
        ep_mutex_lock(&chan->mutex);
        for (long long j = 0; j < chan->size; j++) {
            long long idx = (chan->head + j) % chan->capacity;
            long long val = chan->data[idx];
            if (val != 0) ep_gc_mark_object_minor((void*)val);
        }
        ep_mutex_unlock(&chan->mutex);
    }
    pthread_mutex_unlock(&ep_channel_registry_mutex);
}

long long create_channel(void) {
    EpChannel* chan = malloc(sizeof(EpChannel));
    if (!chan) return 0;
    chan->capacity = 1024;
    chan->data = malloc(chan->capacity * sizeof(long long));
    chan->head = 0;
    chan->tail = 0;
    chan->size = 0;
    ep_mutex_init(&chan->mutex);
    ep_cond_init(&chan->cond_recv);
    ep_cond_init(&chan->cond_send);
    ep_register_channel(chan);
    return (long long)chan;
}

long long send_channel(long long chan_ptr, long long value) {
    EpChannel* chan = (EpChannel*)chan_ptr;
    if (!chan) return 0;
    /* Suppress GC during channel operations. The blocking condvar wait
       can interleave with GC mark/sweep on another thread, causing
       use-after-free when the GC sweeps objects that are live on a
       thread currently blocked in send/receive. Channel buffers contain
       raw long long values (not GC-tracked pointers), so suppressing
       GC here is safe. */
    int gc_was_enabled = ep_gc_enabled;
    ep_gc_enabled = 0;
    ep_mutex_lock(&chan->mutex);
    while (chan->size >= chan->capacity) {
        ep_cond_wait(&chan->cond_send, &chan->mutex);
    }
    chan->data[chan->tail] = value;
    chan->tail = (chan->tail + 1) % chan->capacity;
    chan->size += 1;
    ep_cond_signal(&chan->cond_recv);
    ep_mutex_unlock(&chan->mutex);
    ep_gc_enabled = gc_was_enabled;
    return value;
}

long long receive_channel(long long chan_ptr) {
    EpChannel* chan = (EpChannel*)chan_ptr;
    if (!chan) return 0;
    /* Suppress GC during channel receive — same rationale as send_channel */
    int gc_was_enabled = ep_gc_enabled;
    ep_gc_enabled = 0;
    ep_mutex_lock(&chan->mutex);
    while (chan->size <= 0) {
        ep_cond_wait(&chan->cond_recv, &chan->mutex);
    }
    long long value = chan->data[chan->head];
    chan->head = (chan->head + 1) % chan->capacity;
    chan->size -= 1;
    ep_cond_signal(&chan->cond_send);
    ep_mutex_unlock(&chan->mutex);
    ep_gc_enabled = gc_was_enabled;
    return value;
}

// Non-blocking receive — returns 1 if data was available, 0 if channel empty
long long channel_try_recv(long long chan_ptr, long long out_ptr) {
    EpChannel* chan = (EpChannel*)chan_ptr;
    if (!chan) return 0;
    ep_mutex_lock(&chan->mutex);
    if (chan->size <= 0) {
        ep_mutex_unlock(&chan->mutex);
        return 0;
    }
    long long value = chan->data[chan->head];
    chan->head = (chan->head + 1) % chan->capacity;
    chan->size -= 1;
    ep_cond_signal(&chan->cond_send);
    ep_mutex_unlock(&chan->mutex);
    if (out_ptr) {
        *((long long*)out_ptr) = value;
    }
    return 1;
}

// Check if channel has data without consuming it
long long channel_has_data(long long chan_ptr) {
    EpChannel* chan = (EpChannel*)chan_ptr;
    if (!chan) return 0;
    ep_mutex_lock(&chan->mutex);
    int has = (chan->size > 0) ? 1 : 0;
    ep_mutex_unlock(&chan->mutex);
    return has;
}

/* Leave a GC-safe blocking region (see channel_select). If a collection is
   in progress, wait for it to finish before running mutator code again. */
static void ep_gc_leave_blocking_region(void) {
    if (ep_thread_slot < 0) return;
    pthread_mutex_lock(&ep_gc_mutex);
    while (ep_gc_stop_requested) {
        pthread_cond_wait(&ep_gc_resume_cond, &ep_gc_mutex);
    }
    ep_gc_parked_count--;
    pthread_mutex_unlock(&ep_gc_mutex);
}

// Select: wait for any of N channels to have data, with timeout in ms
// channels_list is a list of channel pointers
// Returns index (0-based) of first ready channel, or -1 on timeout
long long channel_select(long long channels_list, long long timeout_ms) {
    EpList* list = (EpList*)channels_list;
    if (!list || list->length == 0) return -1;

#ifdef _WIN32
    ULONGLONG start_tick = GetTickCount64();
#else
    struct timespec start, now;
    clock_gettime(CLOCK_MONOTONIC, &start);
#endif

    /* A GC-safe blocking region. A thread sitting in a long select must not
       stall every stop-the-world (waking it to park costs ~0.5ms per major
       collection — allocation-heavy code on another thread pays that tens of
       thousands of times). Instead, pin this frame's roots once — spill the
       callee-saved registers into this frame and publish the frame as the
       thread's stack top — and count the thread as already parked. The
       collector then proceeds immediately and scans [this frame, stack
       bottom], a range that is frozen for the whole select: the poll loop
       below only grows the stack DOWNWARD from here (excluded from the scan)
       and allocates nothing. On every way out, ep_gc_leave_blocking_region
       waits for any in-flight collection before mutator code resumes.
       (Without any of this, the collector scans a live, mutating stack
       against a stale top, misses the channels list held in a callee-saved
       register, sweeps it, and the next poll dereferences freed memory.) */
    jmp_buf _pin_regs;
    volatile char _pin_marker;
    if (ep_thread_slot >= 0) {
        memset(&_pin_regs, 0, sizeof(_pin_regs));
        setjmp(_pin_regs);
        pthread_mutex_lock(&ep_gc_mutex);
        { char* _a = (char*)(void*)&_pin_marker; char* _b = (char*)(void*)&_pin_regs;
          ep_thread_local_top = (void*)((uintptr_t)((_a < _b) ? _a : _b) & ~(uintptr_t)7); }
        __sync_synchronize();
        ep_gc_parked_count++;
        pthread_mutex_unlock(&ep_gc_mutex);
    }

    while (1) {
        // Poll all channels
        for (long long i = 0; i < list->length; i++) {
            EpChannel* chan = (EpChannel*)list->data[i];
            if (chan) {
                ep_mutex_lock(&chan->mutex);
                if (chan->size > 0) {
                    ep_mutex_unlock(&chan->mutex);
                    ep_gc_leave_blocking_region();
                    return i;
                }
                ep_mutex_unlock(&chan->mutex);
            }
        }

        // Check timeout
        if (timeout_ms >= 0) {
#ifdef _WIN32
            ULONGLONG now_tick = GetTickCount64();
            long long elapsed = (long long)(now_tick - start_tick);
#else
            clock_gettime(CLOCK_MONOTONIC, &now);
            long long elapsed = (now.tv_sec - start.tv_sec) * 1000 + (now.tv_nsec - start.tv_nsec) / 1000000;
#endif
            if (elapsed >= timeout_ms) {
                ep_gc_leave_blocking_region();
                return -1;
            }
        }
        
        // Brief sleep to avoid busy-wait
#ifdef _WIN32
        Sleep(1);
#else
        usleep(1000); // 1ms
#endif
    }
}

#ifdef __wasm__
long long ep_net_connect(const char* host, long long port) {
    (void)host; (void)port;
    return -1;
}

long long ep_net_listen(long long port) {
    (void)port;
    return -1;
}

long long ep_net_accept(long long server_fd) {
    (void)server_fd;
    return -1;
}

long long ep_net_send(long long fd, const char* data) {
    (void)fd; (void)data;
    return 0;
}

long long ep_net_send_raw(long long fd, long long data_ptr, long long count) {
    (void)fd; (void)data_ptr; (void)count;
    return 0;
}

char* ep_net_recv(long long fd, long long max_len) {
    (void)fd; (void)max_len;
    char* empty = malloc(1);
    if (empty) empty[0] = '\0';
    return empty;
}

long long ep_net_close(long long fd) {
    (void)fd;
    return -1;
}

long long ep_sleep_ms(long long ms) {
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000;
    nanosleep(&ts, NULL);
    return 0;
}

long long ep_system(long long cmd) {
    (void)cmd;
    return -1;
}

long long ep_play_sound(long long path) {
    (void)path;
    return -1;
}

long long ep_dlopen(long long path) {
    (void)path;
    return 0;
}

long long ep_dlsym(long long handle, long long name) {
    (void)handle; (void)name;
    return 0;
}

long long ep_dlclose(long long handle) {
    (void)handle;
    return 0;
}
#else
long long ep_net_connect(const char* host, long long port) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) return -1;
    struct hostent* server = gethostbyname(host);
    if (!server) {
#ifdef _WIN32
        closesocket(sockfd);
#else
        close(sockfd);
#endif
        return -1;
    }
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    memcpy(&serv_addr.sin_addr.s_addr, server->h_addr_list[0], server->h_length);
    serv_addr.sin_port = htons(port);
#ifdef _WIN32
    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        closesocket(sockfd);
        return -1;
    }
#else
    // Bounded connect: an unreachable peer must not block ~75s on the OS SYN
    // timeout (this stalled node startup). Non-blocking connect + 5s select, then
    // restore blocking mode for the rest of the session.
    int _ep_flags = fcntl(sockfd, F_GETFL, 0);
    fcntl(sockfd, F_SETFL, _ep_flags | O_NONBLOCK);
    int _ep_cr = connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if (_ep_cr < 0) {
        if (errno != EINPROGRESS) { close(sockfd); return -1; }
        fd_set _ep_wset; FD_ZERO(&_ep_wset); FD_SET(sockfd, &_ep_wset);
        struct timeval _ep_tv; _ep_tv.tv_sec = 5; _ep_tv.tv_usec = 0;
        int _ep_sel = select(sockfd + 1, NULL, &_ep_wset, NULL, &_ep_tv);
        if (_ep_sel <= 0) { close(sockfd); return -1; } // timeout or error
        int _ep_so_err = 0; socklen_t _ep_slen = sizeof(_ep_so_err);
        if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &_ep_so_err, &_ep_slen) < 0 || _ep_so_err != 0) {
            close(sockfd);
            return -1;
        }
    }
    fcntl(sockfd, F_SETFL, _ep_flags);
#endif
    return sockfd;
}

long long ep_net_listen(long long port) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) return -1;
    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);
    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
#ifdef _WIN32
        closesocket(sockfd);
#else
        close(sockfd);
#endif
        return -1;
    }
    if (listen(sockfd, 10) < 0) {
#ifdef _WIN32
        closesocket(sockfd);
#else
        close(sockfd);
#endif
        return -1;
    }
    return sockfd;
}

long long ep_net_accept(long long server_fd) {
    struct sockaddr_in cli_addr;
    socklen_t clilen = sizeof(cli_addr);
    int newsockfd = accept((int)server_fd, (struct sockaddr*)&cli_addr, &clilen);
    if (newsockfd >= 0) {
        /* Bound how long a single recv/send may block so a slow or silent
           client cannot pin a handler thread forever (slowloris). */
        struct timeval tv;
        tv.tv_sec = 30;
        tv.tv_usec = 0;
        setsockopt(newsockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv));
        setsockopt(newsockfd, SOL_SOCKET, SO_SNDTIMEO, (const char*)&tv, sizeof(tv));
    }
    return newsockfd;
}

long long ep_net_send(long long fd, const char* data) {
    if (!data) return 0;
    return ep_net_send_raw(fd, (long long)data, (long long)strlen(data));
}

long long ep_net_send_raw(long long fd, long long data_ptr, long long count) {
    if (data_ptr == 0 || count <= 0) return 0;

    /* send() may write fewer bytes than requested. Keep sending until the
       explicit byte count is exhausted, including bytes after embedded NULs. */
    const char* data = (const char*)data_ptr;
    long long off = 0;
    while (off < count) {
#ifdef _WIN32
        int chunk = count - off > INT_MAX ? INT_MAX : (int)(count - off);
        int n = send((int)fd, data + off, chunk, 0);
        if (n < 0 && WSAGetLastError() == WSAEINTR) continue;
#else
        ssize_t n = send((int)fd, data + off, (size_t)(count - off), 0);
        if (n < 0 && errno == EINTR) continue;
#endif
        if (n <= 0) break;
        off += (long long)n;
    }
    return off;
}

char* ep_net_recv(long long fd, long long max_len) {
    char* buf = malloc(max_len + 1);
    if (!buf) {
        char* empty = malloc(1);
        if (empty) empty[0] = '\0';
        return empty;
    }
#ifdef _WIN32
    int n = recv((int)fd, buf, (int)max_len, 0);
#else
    ssize_t n = recv((int)fd, buf, max_len, 0);
#endif
    if (n < 0) n = 0;
    buf[n] = '\0';
    return buf;
}

long long ep_net_close(long long fd) {
#ifdef _WIN32
    return closesocket((int)fd);
#else
    return close((int)fd);
#endif
}

long long ep_sleep_ms(long long ms) {
#ifdef _WIN32
    Sleep((DWORD)ms);
#else
    usleep((useconds_t)(ms * 1000));
#endif
    return 0;
}

long long ep_system(long long cmd) {
    return (long long)system((const char*)cmd);
}

long long ep_play_sound(long long path) {
    char cmd[512];
    snprintf(cmd, sizeof(cmd), "afplay '%s' &", (const char*)path);
    return (long long)system(cmd);
}

/* ========== Dynamic Library Loading (FFI) ========== */
#ifndef _WIN32
#include <dlfcn.h>
#endif

long long ep_dlopen(long long path) {
#ifdef _WIN32
    HMODULE h = LoadLibraryA((const char*)path);
    return (long long)h;
#else
    const char* p = (const char*)path;
    void* handle = dlopen(p, RTLD_LAZY);
    return (long long)handle;
#endif
}

long long ep_dlsym(long long handle, long long name) {
#ifdef _WIN32
    FARPROC sym = GetProcAddress((HMODULE)handle, (const char*)name);
    return (long long)sym;
#else
    void* sym = dlsym((void*)handle, (const char*)name);
    return (long long)sym;
#endif
}

long long ep_dlclose(long long handle) {
#ifdef _WIN32
    return (long long)FreeLibrary((HMODULE)handle);
#else
    return (long long)dlclose((void*)handle);
#endif
}
#endif

/* Call a function pointer with 0..6 arguments.
   These are type-punned through long long — the C calling convention
   makes this work for integer and pointer arguments. */
typedef long long (*ep_fn0)(void);
typedef long long (*ep_fn1)(long long);
typedef long long (*ep_fn2)(long long, long long);
typedef long long (*ep_fn3)(long long, long long, long long);
typedef long long (*ep_fn4)(long long, long long, long long, long long);
typedef long long (*ep_fn5)(long long, long long, long long, long long, long long);
typedef long long (*ep_fn6)(long long, long long, long long, long long, long long, long long);
typedef long long (*ep_fn7)(long long, long long, long long, long long, long long, long long, long long);
typedef long long (*ep_fn8)(long long, long long, long long, long long, long long, long long, long long, long long);
typedef long long (*ep_fn9)(long long, long long, long long, long long, long long, long long, long long, long long, long long);
typedef long long (*ep_fn10)(long long, long long, long long, long long, long long, long long, long long, long long, long long, long long);

long long ep_dlcall0(long long fptr) {
    return ((ep_fn0)fptr)();
}
long long ep_dlcall1(long long fptr, long long a0) {
    return ((ep_fn1)fptr)(a0);
}
long long ep_dlcall2(long long fptr, long long a0, long long a1) {
    return ((ep_fn2)fptr)(a0, a1);
}
long long ep_dlcall3(long long fptr, long long a0, long long a1, long long a2) {
    return ((ep_fn3)fptr)(a0, a1, a2);
}
long long ep_dlcall4(long long fptr, long long a0, long long a1, long long a2, long long a3) {
    return ((ep_fn4)fptr)(a0, a1, a2, a3);
}
long long ep_dlcall5(long long fptr, long long a0, long long a1, long long a2, long long a3, long long a4) {
    return ((ep_fn5)fptr)(a0, a1, a2, a3, a4);
}
long long ep_dlcall6(long long fptr, long long a0, long long a1, long long a2, long long a3, long long a4, long long a5) {
    return ((ep_fn6)fptr)(a0, a1, a2, a3, a4, a5);
}
long long ep_dlcall7(long long fptr, long long a0, long long a1, long long a2, long long a3, long long a4, long long a5, long long a6) {
    return ((ep_fn7)fptr)(a0, a1, a2, a3, a4, a5, a6);
}
long long ep_dlcall8(long long fptr, long long a0, long long a1, long long a2, long long a3, long long a4, long long a5, long long a6, long long a7) {
    return ((ep_fn8)fptr)(a0, a1, a2, a3, a4, a5, a6, a7);
}
long long ep_dlcall9(long long fptr, long long a0, long long a1, long long a2, long long a3, long long a4, long long a5, long long a6, long long a7, long long a8) {
    return ((ep_fn9)fptr)(a0, a1, a2, a3, a4, a5, a6, a7, a8);
}
long long ep_dlcall10(long long fptr, long long a0, long long a1, long long a2, long long a3, long long a4, long long a5, long long a6, long long a7, long long a8, long long a9) {
    return ((ep_fn10)fptr)(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9);
}

/* ========== Float FFI: ep_dlcall_f* ========== */
/* For calling C functions that accept/return double values.
   Arguments are passed as long long (bit-punned doubles).
   Return value is a double bit-punned back to long long.
   Use ep_double_to_bits() / ep_bits_to_double() to convert. */

typedef union { long long i; double f; } ep_float_bits;

static inline double ep_ll_to_double(long long v) {
    ep_float_bits u; u.i = v; return u.f;
}
static inline long long ep_double_to_ll(double v) {
    ep_float_bits u; u.f = v; return u.i;
}

/* Convert between ErnosPlain float representation and raw bits */
long long ep_double_to_bits(long long float_val) {
    /* float_val is already an EP Float stored as long long bits */
    return float_val;
}
long long ep_bits_to_double(long long bits) {
    return bits;
}

/* Float function pointer typedefs */
typedef double (*ep_ff0)(void);
typedef double (*ep_ff1)(double);
typedef double (*ep_ff2)(double, double);
typedef double (*ep_ff3)(double, double, double);
typedef double (*ep_ff4)(double, double, double, double);
typedef double (*ep_ff5)(double, double, double, double, double);
typedef double (*ep_ff6)(double, double, double, double, double, double);

/* Call functions that take doubles and return double */
long long ep_dlcall_f0(long long fptr) {
    return ep_double_to_ll(((ep_ff0)fptr)());
}
long long ep_dlcall_f1(long long fptr, long long a0) {
    return ep_double_to_ll(((ep_ff1)fptr)(ep_ll_to_double(a0)));
}
long long ep_dlcall_f2(long long fptr, long long a0, long long a1) {
    return ep_double_to_ll(((ep_ff2)fptr)(ep_ll_to_double(a0), ep_ll_to_double(a1)));
}
long long ep_dlcall_f3(long long fptr, long long a0, long long a1, long long a2) {
    return ep_double_to_ll(((ep_ff3)fptr)(ep_ll_to_double(a0), ep_ll_to_double(a1), ep_ll_to_double(a2)));
}
long long ep_dlcall_f4(long long fptr, long long a0, long long a1, long long a2, long long a3) {
    return ep_double_to_ll(((ep_ff4)fptr)(ep_ll_to_double(a0), ep_ll_to_double(a1), ep_ll_to_double(a2), ep_ll_to_double(a3)));
}
long long ep_dlcall_f5(long long fptr, long long a0, long long a1, long long a2, long long a3, long long a4) {
    return ep_double_to_ll(((ep_ff5)fptr)(ep_ll_to_double(a0), ep_ll_to_double(a1), ep_ll_to_double(a2), ep_ll_to_double(a3), ep_ll_to_double(a4)));
}
long long ep_dlcall_f6(long long fptr, long long a0, long long a1, long long a2, long long a3, long long a4, long long a5) {
    return ep_double_to_ll(((ep_ff6)fptr)(ep_ll_to_double(a0), ep_ll_to_double(a1), ep_ll_to_double(a2), ep_ll_to_double(a3), ep_ll_to_double(a4), ep_ll_to_double(a5)));
}

/* Variants that take doubles but return int (for comparison functions etc.) */
typedef long long (*ep_fdi1)(double);
typedef long long (*ep_fdi2)(double, double);
typedef long long (*ep_fdi3)(double, double, double);

long long ep_dlcall_fd1(long long fptr, long long a0) {
    return ((ep_fdi1)fptr)(ep_ll_to_double(a0));
}
long long ep_dlcall_fd2(long long fptr, long long a0, long long a1) {
    return ((ep_fdi2)fptr)(ep_ll_to_double(a0), ep_ll_to_double(a1));
}
long long ep_dlcall_fd3(long long fptr, long long a0, long long a1, long long a2) {
    return ((ep_fdi3)fptr)(ep_ll_to_double(a0), ep_ll_to_double(a1), ep_ll_to_double(a2));
}
/* ========== End Float FFI ========== */
/* ========== End Dynamic Library Loading ========== */

unsigned long hash_string(const char* str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

typedef struct {
    char* key;
    long long value;
    int used;
} EpMapEntry;

typedef struct {
    EpMapEntry* entries;
    long long capacity;
    long long size;
} EpMap;

/* Map value traversal for GC — walks all entries and marks values.
   Called by ep_gc_mark_object() via function pointer. */
static void ep_gc_mark_map_values_impl(void* ptr) {
    EpMap* map = (EpMap*)ptr;
    if (!map || !map->entries) return;
    for (long long i = 0; i < map->capacity; i++) {
        if (map->entries[i].used && map->entries[i].value != 0) {
            ep_gc_mark_object((void*)map->entries[i].value);
        }
        /* Also mark keys if they are heap strings */
        if (map->entries[i].used && map->entries[i].key != NULL) {
            ep_gc_mark_object((void*)map->entries[i].key);
        }
    }
}

static void ep_gc_mark_map_values_minor_impl(void* ptr) {
    EpMap* map = (EpMap*)ptr;
    if (!map || !map->entries) return;
    for (long long i = 0; i < map->capacity; i++) {
        if (map->entries[i].used && map->entries[i].value != 0) {
            ep_gc_mark_object_minor((void*)map->entries[i].value);
        }
        if (map->entries[i].used && map->entries[i].key != NULL) {
            ep_gc_mark_object_minor((void*)map->entries[i].key);
        }
    }
}

long long create_map(void) {
    EpMap* map = malloc(sizeof(EpMap));
    if (!map) return 0;
    map->capacity = 16;
    map->size = 0;
    map->entries = calloc(map->capacity, sizeof(EpMapEntry));
    if (!map->entries) {
        free(map);
        return 0;
    }
    ep_gc_register(map, EP_OBJ_MAP);
    return (long long)map;
}

static void map_resize(EpMap* map, long long new_capacity) {
    EpMapEntry* old_entries = map->entries;
    long long old_capacity = map->capacity;
    map->capacity = new_capacity;
    map->entries = calloc(new_capacity, sizeof(EpMapEntry));
    map->size = 0;
    for (long long i = 0; i < old_capacity; i++) {
        if (old_entries[i].used && old_entries[i].key != NULL) {
            char* key = old_entries[i].key;
            long long value = old_entries[i].value;
            unsigned long h = hash_string(key) % new_capacity;
            while (map->entries[h].used) {
                h = (h + 1) % new_capacity;
            }
            map->entries[h].key = key;
            map->entries[h].value = value;
            map->entries[h].used = 1;
            map->size++;
        }
    }
    free(old_entries);
}

/* Convert a key value to a string — handles both string pointers and integers */
static const char* ep_map_key_str(long long key_val, char* buf, int bufsize) {
    if (key_val == 0) { buf[0] = '0'; buf[1] = '\0'; return buf; }
    /* Check if value is in plausible pointer range for a string */
    if (key_val > 0x100000) {
        const char* p = (const char*)(void*)key_val;
        unsigned char first = (unsigned char)*p;
        if ((first >= 0x20 && first < 0x7F) || first >= 0xC0 || first == 0) {
            return p; /* valid string pointer */
        }
    }
    snprintf(buf, bufsize, "%lld", key_val);
    return buf;
}

long long map_insert(long long map_ptr, long long key_val, long long value) {
    if (EP_BADPTR(map_ptr)) return 0;
    EpMap* map = (EpMap*)map_ptr;
    char keybuf[32];
    const char* key = ep_map_key_str(key_val, keybuf, sizeof(keybuf));
    if (!map) return 0;
    if (map->size * 2 >= map->capacity) {
        map_resize(map, map->capacity * 2);
    }
    unsigned long h = hash_string(key) % map->capacity;
    while (map->entries[h].used) {
        if (strcmp(map->entries[h].key, key) == 0) {
            map->entries[h].value = value;
            ep_gc_write_barrier((void*)map_ptr, value);
            return value;
        }
        h = (h + 1) % map->capacity;
    }
    map->entries[h].key = strdup(key);
    map->entries[h].value = value;
    map->entries[h].used = 1;
    map->size++;
    ep_gc_write_barrier((void*)map_ptr, value);
    return value;
}

long long map_get_val(long long map_ptr, long long key_val) {
    if (EP_BADPTR(map_ptr)) return 0;
    EpMap* map = (EpMap*)map_ptr;
    char keybuf[32];
    const char* key = ep_map_key_str(key_val, keybuf, sizeof(keybuf));
    if (!map) return 0;
    unsigned long h = hash_string(key) % map->capacity;
    long long start_h = h;
    while (map->entries[h].used) {
        if (map->entries[h].key && strcmp(map->entries[h].key, key) == 0) {
            return map->entries[h].value;
        }
        h = (h + 1) % map->capacity;
        if (h == start_h) break;
    }
    return 0;
}

/* map_set_str: store a string value (strdup'd copy) under a string key */
long long map_set_str(long long map_ptr, long long key_val, long long str_val) {
    /* Store the string pointer as a long long value — same as map_insert */
    return map_insert(map_ptr, key_val, str_val);
}

/* map_get_str: retrieve a string value from a map (returns char* as long long) */
long long map_get_str(long long map_ptr, long long key_val) {
    /* Same as map_get_val — the stored long long IS a char* pointer */
    return map_get_val(map_ptr, key_val);
}

long long map_contains(long long map_ptr, long long key_val) {
    if (EP_BADPTR(map_ptr)) return 0;
    EpMap* map = (EpMap*)map_ptr;
    char keybuf[32];
    const char* key = ep_map_key_str(key_val, keybuf, sizeof(keybuf));
    if (!map) return 0;
    unsigned long h = hash_string(key) % map->capacity;
    long long start_h = h;
    while (map->entries[h].used) {
        if (map->entries[h].key && strcmp(map->entries[h].key, key) == 0) {
            return 1;
        }
        h = (h + 1) % map->capacity;
        if (h == start_h) break;
    }
    return 0;
}

long long map_delete(long long map_ptr, long long key_val) {
    if (EP_BADPTR(map_ptr)) return 0;
    EpMap* map = (EpMap*)map_ptr;
    char keybuf[32];
    const char* key = ep_map_key_str(key_val, keybuf, sizeof(keybuf));
    if (!map) return 0;
    unsigned long h = hash_string(key) % map->capacity;
    long long start_h = h;
    while (map->entries[h].used) {
        if (map->entries[h].key && strcmp(map->entries[h].key, key) == 0) {
            free(map->entries[h].key);
            map->entries[h].key = NULL;
            map->entries[h].value = 0;
            map->entries[h].used = 0;
            map->size--;
            long long next_h = (h + 1) % map->capacity;
            while (map->entries[next_h].used) {
                char* k = map->entries[next_h].key;
                long long v = map->entries[next_h].value;
                map->entries[next_h].key = NULL;
                map->entries[next_h].value = 0;
                map->entries[next_h].used = 0;
                map->size--;
                map_insert(map_ptr, (long long)k, v);
                free(k);
                next_h = (next_h + 1) % map->capacity;
            }
            return 1;
        }
        h = (h + 1) % map->capacity;
        if (h == start_h) break;
    }
    return 0;
}

long long map_keys(long long map_ptr) {
    EpMap* map = (EpMap*)map_ptr;
    if (!map) return (long long)create_list();
    long long list = create_list();
    for (long long i = 0; i < map->capacity; i++) {
        if (map->entries[i].used && map->entries[i].key) {
            append_list(list, (long long)strdup(map->entries[i].key));
        }
    }
    return list;
}

long long map_values(long long map_ptr) {
    EpMap* map = (EpMap*)map_ptr;
    if (!map) return (long long)create_list();
    long long list = create_list();
    for (long long i = 0; i < map->capacity; i++) {
        if (map->entries[i].used && map->entries[i].key) {
            append_list(list, map->entries[i].value);
        }
    }
    return list;
}

long long map_size(long long map_ptr) {
    EpMap* map = (EpMap*)map_ptr;
    if (!map) return 0;
    return map->size;
}

long long free_map(long long map_ptr) {
    EpMap* map = (EpMap*)map_ptr;
    if (!map) return 0;
    /* Skip if already freed (idempotent) */
    if (!ep_gc_find(map)) return 0;
    ep_gc_unregister(map);
    for (long long i = 0; i < map->capacity; i++) {
        if (map->entries[i].used && map->entries[i].key != NULL) {
            free(map->entries[i].key);
        }
    }
    free(map->entries);
    free(map);
    return 0;
}

typedef struct {
    long long* data;
    long long capacity;
    long long head;
    long long tail;
    long long size;
} EpDeque;

long long create_deque(void) {
    EpDeque* dq = malloc(sizeof(EpDeque));
    if (!dq) return 0;
    dq->capacity = 16;
    dq->size = 0;
    dq->head = 0;
    dq->tail = 0;
    dq->data = malloc(dq->capacity * sizeof(long long));
    if (!dq->data) {
        free(dq);
        return 0;
    }
    return (long long)dq;
}

static void deque_resize(EpDeque* dq, long long new_capacity) {
    long long* new_data = malloc(new_capacity * sizeof(long long));
    for (long long i = 0; i < dq->size; i++) {
        new_data[i] = dq->data[(dq->head + i) % dq->capacity];
    }
    free(dq->data);
    dq->data = new_data;
    dq->capacity = new_capacity;
    dq->head = 0;
    dq->tail = dq->size;
}

long long deque_push_back(long long dq_ptr, long long value) {
    EpDeque* dq = (EpDeque*)dq_ptr;
    if (!dq) return 0;
    if (dq->size >= dq->capacity) {
        deque_resize(dq, dq->capacity * 2);
    }
    dq->data[dq->tail] = value;
    dq->tail = (dq->tail + 1) % dq->capacity;
    dq->size++;
    return value;
}

long long deque_push_front(long long dq_ptr, long long value) {
    EpDeque* dq = (EpDeque*)dq_ptr;
    if (!dq) return 0;
    if (dq->size >= dq->capacity) {
        deque_resize(dq, dq->capacity * 2);
    }
    dq->head = (dq->head - 1 + dq->capacity) % dq->capacity;
    dq->data[dq->head] = value;
    dq->size++;
    return value;
}

long long deque_pop_back(long long dq_ptr) {
    EpDeque* dq = (EpDeque*)dq_ptr;
    if (!dq || dq->size == 0) return 0;
    dq->tail = (dq->tail - 1 + dq->capacity) % dq->capacity;
    long long value = dq->data[dq->tail];
    dq->size--;
    return value;
}

long long deque_pop_front(long long dq_ptr) {
    EpDeque* dq = (EpDeque*)dq_ptr;
    if (!dq || dq->size == 0) return 0;
    long long value = dq->data[dq->head];
    dq->head = (dq->head + 1) % dq->capacity;
    dq->size--;
    return value;
}

long long deque_length(long long dq_ptr) {
    EpDeque* dq = (EpDeque*)dq_ptr;
    if (!dq) return 0;
    return dq->size;
}

long long free_deque(long long dq_ptr) {
    EpDeque* dq = (EpDeque*)dq_ptr;
    if (!dq) return 0;
    free(dq->data);
    free(dq);
    return 0;
}

/* Filesystem Operations */
#include <dirent.h>
#include <sys/stat.h>

long long fs_scan_dir(long long path_val) {
    const char* path = (const char*)path_val;
    long long list_ptr = create_list();
    if (!path) return list_ptr;
    DIR* d = opendir(path);
    if (!d) return list_ptr;
    struct dirent* dir;
    while ((dir = readdir(d)) != NULL) {
        if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) {
            continue;
        }
        char* name = strdup(dir->d_name);
        append_list(list_ptr, (long long)name);
    }
    closedir(d);
    return list_ptr;
}

long long fs_copy_file(long long src_val, long long dest_val) {
    const char* src = (const char*)src_val;
    const char* dest = (const char*)dest_val;
    if (!src || !dest) return 0;
    FILE* f_src = fopen(src, "rb");
    if (!f_src) return 0;
    FILE* f_dest = fopen(dest, "wb");
    if (!f_dest) {
        fclose(f_src);
        return 0;
    }
    char buf[4096];
    size_t n;
    while ((n = fread(buf, 1, sizeof(buf), f_src)) > 0) {
        fwrite(buf, 1, n, f_dest);
    }
    fclose(f_src);
    fclose(f_dest);
    return 1;
}

long long fs_delete_file(long long path_val) {
    const char* path = (const char*)path_val;
    if (!path) return 0;
    return remove(path) == 0 ? 1 : 0;
}

long long fs_move_file(long long src_val, long long dest_val) {
    const char* src = (const char*)src_val;
    const char* dest = (const char*)dest_val;
    if (!src || !dest) return 0;
    return rename(src, dest) == 0 ? 1 : 0;
}

long long fs_exists(long long path_val) {
    const char* path = (const char*)path_val;
    if (!path) return 0;
    struct stat st;
    return stat(path, &st) == 0 ? 1 : 0;
}

long long fs_is_dir(long long path_val) {
    const char* path = (const char*)path_val;
    if (!path) return 0;
    struct stat st;
    if (stat(path, &st) != 0) return 0;
    return S_ISDIR(st.st_mode) ? 1 : 0;
}

long long fs_is_file(long long path_val) {
    const char* path = (const char*)path_val;
    if (!path) return 0;
    struct stat st;
    if (stat(path, &st) != 0) return 0;
    return S_ISREG(st.st_mode) ? 1 : 0;
}

long long fs_get_size(long long path_val) {
    const char* path = (const char*)path_val;
    if (!path) return 0;
    struct stat st;
    if (stat(path, &st) != 0) return 0;
    return (long long)st.st_size;
}

/* HTTP Client */
#ifdef __wasm__
long long ep_http_request(long long method_val, long long url_val, long long headers_val, long long body_val) {
    (void)method_val; (void)url_val; (void)headers_val; (void)body_val;
    return (long long)strdup("Error: HTTP request is not supported on WebAssembly");
}
#else
long long ep_http_request(long long method_val, long long url_val, long long headers_val, long long body_val) {
    const char* method = (const char*)method_val;
    const char* url = (const char*)url_val;
    const char* headers = (const char*)headers_val;
    const char* body = (const char*)body_val;
    if (!method || !url) return (long long)strdup("");
    if (strncmp(url, "http://", 7) != 0) {
        return (long long)strdup("Error: only http:// protocol supported");
    }
    const char* host_start = url + 7;
    const char* path_start = strchr(host_start, '/');
    char host[256];
    char path[1024];
    if (path_start) {
        size_t host_len = path_start - host_start;
        if (host_len >= sizeof(host)) host_len = sizeof(host) - 1;
        strncpy(host, host_start, host_len);
        host[host_len] = '\0';
        strncpy(path, path_start, sizeof(path) - 1);
        path[sizeof(path) - 1] = '\0';
    } else {
        strncpy(host, host_start, sizeof(host) - 1);
        host[sizeof(host) - 1] = '\0';
        strcpy(path, "/");
    }
    int port = 80;
    char* colon = strchr(host, ':');
    if (colon) {
        *colon = '\0';
        port = atoi(colon + 1);
    }
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) return (long long)strdup("Error: socket creation failed");
    struct hostent* server = gethostbyname(host);
    if (!server) {
        close(sockfd);
        return (long long)strdup("Error: host resolution failed");
    }
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    memcpy(&serv_addr.sin_addr.s_addr, server->h_addr_list[0], server->h_length);
    serv_addr.sin_port = htons(port);
    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        close(sockfd);
        return (long long)strdup("Error: connection failed");
    }
    char req[4096];
    size_t body_len = body ? strlen(body) : 0;
    int req_len = snprintf(req, sizeof(req),
        "%s %s HTTP/1.1\r\n"
        "Host: %s\r\n"
        "Content-Length: %zu\r\n"
        "Connection: close\r\n"
        "%s%s"
        "\r\n",
        method, path, host, body_len, headers ? headers : "", (headers && strlen(headers) > 0 && headers[strlen(headers)-1] != '\n') ? "\r\n" : "");
    if (send(sockfd, req, req_len, 0) < 0) {
        close(sockfd);
        return (long long)strdup("Error: send failed");
    }
    if (body_len > 0) {
        if (send(sockfd, body, body_len, 0) < 0) {
            close(sockfd);
            return (long long)strdup("Error: send body failed");
        }
    }
    size_t resp_cap = 4096;
    size_t resp_len = 0;
    char* resp = malloc(resp_cap);
    if (!resp) {
        close(sockfd);
        return (long long)strdup("");
    }
    char recv_buf[4096];
    ssize_t n;
    while ((n = recv(sockfd, recv_buf, sizeof(recv_buf), 0)) > 0) {
        if (resp_len + n >= resp_cap) {
            resp_cap *= 2;
            char* new_resp = realloc(resp, resp_cap);
            if (!new_resp) {
                free(resp);
                close(sockfd);
                return (long long)strdup("Error: memory allocation failed");
            }
            resp = new_resp;
        }
        memcpy(resp + resp_len, recv_buf, n);
        resp_len += n;
    }
    resp[resp_len] = '\0';
    close(sockfd);
    // Strip HTTP headers — return only the body after \r\n\r\n
    char* http_body = strstr(resp, "\r\n\r\n");
    if (http_body) {
        http_body += 4;
        char* result = strdup(http_body);
        free(resp);
        return (long long)result;
    }
    return (long long)resp;
}
#endif

#define ROTRIGHT(word,bits) (((word) >> (bits)) | ((word) << (32-(bits))))
#define CH(x,y,z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define EP0(x) (ROTRIGHT(x,2) ^ ROTRIGHT(x,13) ^ ROTRIGHT(x,22))
#define EP1(x) (ROTRIGHT(x,6) ^ ROTRIGHT(x,11) ^ ROTRIGHT(x,25))
#define SIG0(x) (ROTRIGHT(x,7) ^ ROTRIGHT(x,18) ^ ((x) >> 3))
#define SIG1(x) (ROTRIGHT(x,17) ^ ROTRIGHT(x,19) ^ ((x) >> 10))

typedef struct {
    unsigned char data[64];
    unsigned int datalen;
    unsigned long long bitlen;
    unsigned int state[8];
} EP_SHA256_CTX;

static const unsigned int sha256_k[64] = {
    0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
    0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
    0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
    0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
    0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
    0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
    0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
    0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};

void ep_sha256_transform(EP_SHA256_CTX *ctx, const unsigned char *data) {
    unsigned int a, b, c, d, e, f, g, h, i, j, t1, t2, m[64];
    for (i = 0, j = 0; i < 16; ++i, j += 4)
        m[i] = (data[j] << 24) | (data[j + 1] << 16) | (data[j + 2] << 8) | (data[j + 3]);
    for ( ; i < 64; ++i)
        m[i] = SIG1(m[i - 2]) + m[i - 7] + SIG0(m[i - 15]) + m[i - 16];
    a = ctx->state[0]; b = ctx->state[1]; c = ctx->state[2]; d = ctx->state[3];
    e = ctx->state[4]; f = ctx->state[5]; g = ctx->state[6]; h = ctx->state[7];
    for (i = 0; i < 64; ++i) {
        t1 = h + EP1(e) + CH(e,f,g) + sha256_k[i] + m[i];
        t2 = EP0(a) + MAJ(a,b,c);
        h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    }
    ctx->state[0] += a; ctx->state[1] += b; ctx->state[2] += c; ctx->state[3] += d;
    ctx->state[4] += e; ctx->state[5] += f; ctx->state[6] += g; ctx->state[7] += h;
}

void ep_sha256_init(EP_SHA256_CTX *ctx) {
    ctx->datalen = 0; ctx->bitlen = 0;
    ctx->state[0] = 0x6a09e667; ctx->state[1] = 0xbb67ae85; ctx->state[2] = 0x3c6ef372; ctx->state[3] = 0xa54ff53a;
    ctx->state[4] = 0x510e527f; ctx->state[5] = 0x9b05688c; ctx->state[6] = 0x1f83d9ab; ctx->state[7] = 0x5be0cd19;
}

void ep_sha256_update(EP_SHA256_CTX *ctx, const unsigned char *data, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        ctx->data[ctx->datalen] = data[i];
        ctx->datalen++;
        if (ctx->datalen == 64) {
            ep_sha256_transform(ctx, ctx->data);
            ctx->bitlen += 512;
            ctx->datalen = 0;
        }
    }
}

void ep_sha256_final(EP_SHA256_CTX *ctx, unsigned char *hash) {
    unsigned int i = ctx->datalen;
    if (ctx->datalen < 56) {
        ctx->data[i++] = 0x80;
        while (i < 56) ctx->data[i++] = 0x00;
    } else {
        ctx->data[i++] = 0x80;
        while (i < 64) ctx->data[i++] = 0x00;
        ep_sha256_transform(ctx, ctx->data);
        memset(ctx->data, 0, 56);
    }
    ctx->bitlen += ctx->datalen * 8;
    ctx->data[63] = ctx->bitlen; ctx->data[62] = ctx->bitlen >> 8;
    ctx->data[61] = ctx->bitlen >> 16; ctx->data[60] = ctx->bitlen >> 24;
    ctx->data[59] = ctx->bitlen >> 32; ctx->data[58] = ctx->bitlen >> 40;
    ctx->data[57] = ctx->bitlen >> 48; ctx->data[56] = ctx->bitlen >> 56;
    ep_sha256_transform(ctx, ctx->data);
    for (i = 0; i < 4; ++i) {
        hash[i]      = (ctx->state[0] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 4]  = (ctx->state[1] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 8]  = (ctx->state[2] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 12] = (ctx->state[3] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 16] = (ctx->state[4] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 20] = (ctx->state[5] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 24] = (ctx->state[6] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 28] = (ctx->state[7] >> (24 - i * 8)) & 0x000000ff;
    }
}

char* ep_sha256(const char* s) {
    if (!s) s = "";
    EP_SHA256_CTX ctx;
    ep_sha256_init(&ctx);
    ep_sha256_update(&ctx, (const unsigned char*)s, strlen(s));
    unsigned char hash[32];
    ep_sha256_final(&ctx, hash);
    char* result = malloc(65);
    if (result) {
        for (int i = 0; i < 32; i++) {
            snprintf(result + (i * 2), 3, "%02x", hash[i]);
        }
        result[64] = '\0';
    }
    return result;
}

/* RFC 2104 HMAC-SHA256. Operates on raw bytes with explicit lengths (binary
   safe), so keys/messages containing NUL bytes hash correctly. Returns a
   malloc'd 64-char lowercase hex string. */
long long ep_hmac_sha256(long long key_ptr, long long key_len, long long msg_ptr, long long msg_len) {
    const unsigned char* key = (const unsigned char*)key_ptr;
    const unsigned char* msg = (const unsigned char*)msg_ptr;
    size_t klen = (size_t)key_len;
    size_t mlen = (size_t)msg_len;

    unsigned char k0[64];
    memset(k0, 0, sizeof(k0));
    if (klen > 64) {
        /* Keys longer than the block size are replaced by their hash. */
        EP_SHA256_CTX kc;
        ep_sha256_init(&kc);
        ep_sha256_update(&kc, key ? key : (const unsigned char*)"", klen);
        unsigned char kh[32];
        ep_sha256_final(&kc, kh);
        memcpy(k0, kh, 32);
    } else if (key) {
        memcpy(k0, key, klen);
    }

    unsigned char ipad[64], opad[64];
    for (int i = 0; i < 64; i++) {
        ipad[i] = k0[i] ^ 0x36;
        opad[i] = k0[i] ^ 0x5c;
    }

    /* inner = H((K0 ^ ipad) || message) */
    EP_SHA256_CTX ic;
    ep_sha256_init(&ic);
    ep_sha256_update(&ic, ipad, 64);
    if (msg && mlen) ep_sha256_update(&ic, msg, mlen);
    unsigned char inner[32];
    ep_sha256_final(&ic, inner);

    /* mac = H((K0 ^ opad) || inner) */
    EP_SHA256_CTX oc;
    ep_sha256_init(&oc);
    ep_sha256_update(&oc, opad, 64);
    ep_sha256_update(&oc, inner, 32);
    unsigned char mac[32];
    ep_sha256_final(&oc, mac);

    char* out = (char*)malloc(65);
    if (out) {
        for (int i = 0; i < 32; i++) {
            snprintf(out + (i * 2), 3, "%02x", mac[i]);
        }
        out[64] = '\0';
    }
    return (long long)out;
}

typedef struct {
    unsigned int count[2];
    unsigned int state[4];
    unsigned char buffer[64];
} EP_MD5_CTX;

#define F(x,y,z) (((x) & (y)) | (~(x) & (z)))
#define G(x,y,z) (((x) & (z)) | ((y) & ~(z)))
#define H(x,y,z) ((x) ^ (y) ^ (z))
#define I(x,y,z) ((y) ^ ((x) | ~(z)))
#define ROTATE_LEFT(x,n) (((x) << (n)) | ((x) >> (32-(n))))

#define FF(a,b,c,d,x,s,ac) { \
    (a) += F((b),(c),(d)) + (x) + (ac); \
    (a) = ROTATE_LEFT((a),(s)); \
    (a) += (b); \
}
#define GG(a,b,c,d,x,s,ac) { \
    (a) += G((b),(c),(d)) + (x) + (ac); \
    (a) = ROTATE_LEFT((a),(s)); \
    (a) += (b); \
}
#define HH(a,b,c,d,x,s,ac) { \
    (a) += H((b),(c),(d)) + (x) + (ac); \
    (a) = ROTATE_LEFT((a),(s)); \
    (a) += (b); \
}
#define II(a,b,c,d,x,s,ac) { \
    (a) += I((b),(c),(d)) + (x) + (ac); \
    (a) = ROTATE_LEFT((a),(s)); \
    (a) += (b); \
}

void ep_md5_init(EP_MD5_CTX *ctx) {
    ctx->count[0] = ctx->count[1] = 0;
    ctx->state[0] = 0x67452301;
    ctx->state[1] = 0xefcdab89;
    ctx->state[2] = 0x98badcfe;
    ctx->state[3] = 0x10325476;
}

void ep_md5_transform(unsigned int state[4], const unsigned char block[64]) {
    unsigned int a = state[0], b = state[1], c = state[2], d = state[3], x[16];
    for (int i = 0, j = 0; i < 16; i++, j += 4)
        x[i] = (block[j]) | (block[j+1] << 8) | (block[j+2] << 16) | (block[j+3] << 24);

    FF(a, b, c, d, x[0], 7, 0xd76aa478); FF(d, a, b, c, x[1], 12, 0xe8c7b756); FF(c, d, a, b, x[2], 17, 0x242070db); FF(b, c, d, a, x[3], 22, 0xc1bdceee);
    FF(a, b, c, d, x[4], 7, 0xf57c0faf); FF(d, a, b, c, x[5], 12, 0x4787c62a); FF(c, d, a, b, x[6], 17, 0xa8304613); FF(b, c, d, a, x[7], 22, 0xfd469501);
    FF(a, b, c, d, x[8], 7, 0x698098d8); FF(d, a, b, c, x[9], 12, 0x8b44f7af); FF(c, d, a, b, x[10], 17, 0xffff5bb1); FF(b, c, d, a, x[11], 22, 0x895cd7be);
    FF(a, b, c, d, x[12], 7, 0x6b901122); FF(d, a, b, c, x[13], 12, 0xfd987193); FF(c, d, a, b, x[14], 17, 0xa679438e); FF(b, c, d, a, x[15], 22, 0x49b40821);

    GG(a, b, c, d, x[1], 5, 0xf61e2562); GG(d, a, b, c, x[6], 9, 0xc040b340); GG(c, d, a, b, x[11], 14, 0x265e5a51); GG(b, c, d, a, x[0], 20, 0xe9b6c7aa);
    GG(a, b, c, d, x[5], 5, 0xd62f105d); GG(d, a, b, c, x[10], 9, 0x02441453); GG(c, d, a, b, x[15], 14, 0xd8a1e681); GG(b, c, d, a, x[4], 20, 0xe7d3fbc8);
    GG(a, b, c, d, x[9], 5, 0x21e1cde6); GG(d, a, b, c, x[14], 9, 0xc33707d6); GG(c, d, a, b, x[3], 14, 0xf4d50d87); GG(b, c, d, a, x[8], 20, 0x455a14ed);
    GG(a, b, c, d, x[13], 5, 0xa9e3e905); GG(d, a, b, c, x[2], 9, 0xfcefa3f8); GG(c, d, a, b, x[7], 14, 0x676f02d9); GG(b, c, d, a, x[12], 20, 0x8d2a4c8a);

    HH(a, b, c, d, x[5], 4, 0xfffa3942); HH(d, a, b, c, x[8], 11, 0x8771f681); HH(c, d, a, b, x[11], 16, 0x6d9d6122); HH(b, c, d, a, x[14], 23, 0xfde5380c);
    HH(a, b, c, d, x[1], 4, 0xa4beea44); HH(d, a, b, c, x[4], 11, 0x4bdecfa9); HH(c, d, a, b, x[7], 16, 0xf6bb4b60); HH(b, c, d, a, x[10], 23, 0xbebfbc70);
    HH(a, b, c, d, x[13], 4, 0x289b7ec6); HH(d, a, b, c, x[0], 11, 0xeaa127fa); HH(c, d, a, b, x[3], 16, 0xd4ef3085); HH(b, c, d, a, x[6], 23, 0x04881d05);
    HH(a, b, c, d, x[9], 4, 0xd9d4d039); HH(d, a, b, c, x[12], 11, 0xe6db99e5); HH(c, d, a, b, x[15], 16, 0x1fa27cf8); HH(b, c, d, a, x[2], 23, 0xc4ac5665);

    II(a, b, c, d, x[0], 6, 0xf4292244); II(d, a, b, c, x[7], 10, 0x432aff97); II(c, d, a, b, x[14], 15, 0xab9423a7); II(b, c, d, a, x[5], 21, 0xfc93a039);
    II(a, b, c, d, x[12], 6, 0x655b59c3); II(d, a, b, c, x[3], 10, 0x8f0ccc92); II(c, d, a, b, x[10], 15, 0xffeff47d); II(b, c, d, a, x[1], 21, 0x85845dd1);
    II(a, b, c, d, x[8], 6, 0x6fa87e4f); II(d, a, b, c, x[15], 10, 0xfe2ce6e0); II(c, d, a, b, x[6], 15, 0xa3014314); II(b, c, d, a, x[13], 21, 0x4e0811a1);
    II(a, b, c, d, x[4], 6, 0xf7537e82); II(d, a, b, c, x[11], 10, 0xbd3af235); II(c, d, a, b, x[2], 15, 0x2ad7d2bb); II(b, c, d, a, x[9], 21, 0xeb86d391);

    state[0] += a; state[1] += b; state[2] += c; state[3] += d;
}

void ep_md5_update(EP_MD5_CTX *ctx, const unsigned char *input, size_t input_len) {
    unsigned int i = 0, index = (ctx->count[0] >> 3) & 0x3F, part_len = 64 - index;
    ctx->count[0] += input_len << 3;
    if (ctx->count[0] < (input_len << 3)) ctx->count[1]++;
    ctx->count[1] += input_len >> 29;
    if (input_len >= part_len) {
        memcpy(&ctx->buffer[index], input, part_len);
        ep_md5_transform(ctx->state, ctx->buffer);
        for (i = part_len; i + 63 < input_len; i += 64)
            ep_md5_transform(ctx->state, &input[i]);
        index = 0;
    }
    memcpy(&ctx->buffer[index], &input[i], input_len - i);
}

void ep_md5_final(EP_MD5_CTX *ctx, unsigned char digest[16]) {
    unsigned char bits[8];
    bits[0] = ctx->count[0]; bits[1] = ctx->count[0] >> 8; bits[2] = ctx->count[0] >> 16; bits[3] = ctx->count[0] >> 24;
    bits[4] = ctx->count[1]; bits[5] = ctx->count[1] >> 8; bits[6] = ctx->count[1] >> 16; bits[7] = ctx->count[1] >> 24;
    unsigned int index = (ctx->count[0] >> 3) & 0x3F, pad_len = (index < 56) ? (56 - index) : (120 - index);
    unsigned char padding[64];
    memset(padding, 0, 64); padding[0] = 0x80;
    ep_md5_update(ctx, padding, pad_len);
    ep_md5_update(ctx, bits, 8);
    for (int i = 0; i < 4; i++) {
        digest[i*4]     = ctx->state[i];
        digest[i*4 + 1] = ctx->state[i] >> 8;
        digest[i*4 + 2] = ctx->state[i] >> 16;
        digest[i*4 + 3] = ctx->state[i] >> 24;
    }
}

char* ep_md5(const char* s) {
    if (!s) s = "";
    EP_MD5_CTX ctx;
    ep_md5_init(&ctx);
    ep_md5_update(&ctx, (const unsigned char*)s, strlen(s));
    unsigned char hash[16];
    ep_md5_final(&ctx, hash);
    char* result = malloc(33);
    if (result) {
        for (int i = 0; i < 16; i++) {
            snprintf(result + (i * 2), 3, "%02x", hash[i]);
        }
        result[32] = '\0';
    }
    return result;
}

char* read_file_content(const char* filepath) {
    char mode[3];
    mode[0] = 'r';
    mode[1] = 'b';
    mode[2] = '\0';
    FILE* f = fopen(filepath, mode);
    if (!f) {
        char* empty = malloc(1);
        if (empty) empty[0] = '\0';
        ep_gc_register(empty, EP_OBJ_STRING);
        return empty;
    }
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    char* buf = malloc(size + 1);
    if (!buf) {
        fclose(f);
        char* empty = malloc(1);
        if (empty) empty[0] = '\0';
        ep_gc_register(empty, EP_OBJ_STRING);
        return empty;
    }
    size_t read_bytes = fread(buf, 1, size, f);
    buf[read_bytes] = '\0';
    fclose(f);
    ep_gc_register(buf, EP_OBJ_STRING);
    return buf;
}

long long string_length(const char* s) {
    if (!s) return 0;
    return strlen(s);
}

long long get_character(const char* s, long long index) {
    if (!s) return 0;
    long long len = strlen(s);
    if (index < 0 || index >= len) return 0;
    return (unsigned char)s[index];
}

long long create_list(void) {
    EpList* list = malloc(sizeof(EpList));
    if (!list) return 0;
    list->capacity = 4;
    list->length = 0;
    list->data = malloc(list->capacity * sizeof(long long));
    ep_gc_register(list, EP_OBJ_LIST);
    return (long long)list;
}

long long get_list_data_ptr(long long list_ptr) {
    if (EP_BADPTR(list_ptr)) return 0;
    EpList* list = (EpList*)list_ptr;
    if (!list) return 0;
    return (long long)list->data;
}

long long append_list(long long list_ptr, long long value) {
    if (EP_BADPTR(list_ptr)) return 0;
    EpList* list = (EpList*)list_ptr;
    if (!list) return 0;
    if (list->length >= list->capacity) {
        list->capacity *= 2;
        list->data = realloc(list->data, list->capacity * sizeof(long long));
    }
    list->data[list->length] = value;
    list->length += 1;
    ep_gc_write_barrier((void*)list_ptr, value);
    return value;
}

long long get_list(long long list_ptr, long long index) {
    if (EP_BADPTR(list_ptr)) return 0;
    EpList* list = (EpList*)list_ptr;
    if (index < 0 || index >= list->length) return 0;
    return list->data[index];
}

long long set_list(long long list_ptr, long long index, long long value) {
    if (EP_BADPTR(list_ptr)) return 0;
    EpList* list = (EpList*)list_ptr;
    if (index < 0 || index >= list->length) return 0;
    list->data[index] = value;
    ep_gc_write_barrier((void*)list_ptr, value);
    return value;
}

long long length_list(long long list_ptr) {
    if (EP_BADPTR(list_ptr)) return 0;
    EpList* list = (EpList*)list_ptr;
    return list->length;
}

long long free_list(long long list_ptr) {
    EpList* list = (EpList*)list_ptr;
    if (!list) return 0;
    /* Skip if already freed (idempotent) */
    if (!ep_gc_find(list)) return 0;
    ep_gc_unregister(list);
    free(list->data);
    free(list);
    return 0;
}

static int sqlite_list_callback(void* arg, int argc, char** argv, char** col_names) {
    EpList* rows = (EpList*)arg;
    EpList* row = (EpList*)create_list();
    for (int i = 0; i < argc; i++) {
        char* val = argv[i] ? strdup(argv[i]) : strdup("");
        append_list((long long)row, (long long)val);
    }
    append_list((long long)rows, (long long)row);
    return 0;
}

long long sqlite_get_callback_ptr(long long dummy) {
    return (long long)sqlite_list_callback;
}

/* SQLite type-safe wrappers — marshal between int and long long */
#ifdef EP_HAS_SQLITE
typedef struct sqlite3 sqlite3;
int sqlite3_open(const char*, sqlite3**);
int sqlite3_close(sqlite3*);
int sqlite3_exec(sqlite3*, const char*, int(*)(void*,int,char**,char**), void*, char**);

long long ep_sqlite3_open(long long filename, long long db_ptr) {
    sqlite3* db = NULL;
    int rc = sqlite3_open((const char*)filename, &db);
    if (rc == 0 && db_ptr != 0) {
        *((long long*)db_ptr) = (long long)db;
    }
    return (long long)rc;
}

long long ep_sqlite3_close(long long db) {
    return (long long)sqlite3_close((sqlite3*)db);
}

long long ep_sqlite3_exec(long long db, long long sql, long long callback, long long cb_arg, long long errmsg_ptr) {
    return (long long)sqlite3_exec((sqlite3*)db, (const char*)sql,
        (int(*)(void*,int,char**,char**))(callback),
        (void*)cb_arg, (char**)errmsg_ptr);
}

/* Prepared-statement API for parameterized queries (defeats SQL injection). */
typedef struct sqlite3_stmt sqlite3_stmt;
int sqlite3_prepare_v2(sqlite3*, const char*, int, sqlite3_stmt**, const char**);
int sqlite3_bind_text(sqlite3_stmt*, int, const char*, int, void(*)(void*));
int sqlite3_bind_int64(sqlite3_stmt*, int, long long);
int sqlite3_step(sqlite3_stmt*);
int sqlite3_column_count(sqlite3_stmt*);
const unsigned char* sqlite3_column_text(sqlite3_stmt*, int);
long long sqlite3_column_int64(sqlite3_stmt*, int);
int sqlite3_finalize(sqlite3_stmt*);

long long ep_sqlite3_prepare_v2(long long db, long long sql) {
    sqlite3_stmt* stmt = NULL;
    int rc = sqlite3_prepare_v2((sqlite3*)db, (const char*)sql, -1, &stmt, NULL);
    if (rc != 0) return 0;
    return (long long)stmt;
}

long long ep_sqlite3_bind_text(long long stmt, long long idx, long long value) {
    /* SQLITE_TRANSIENT ((void*)-1): sqlite copies the bound string. The value is
       a bound parameter, never concatenated into SQL — this is the safe path. */
    return (long long)sqlite3_bind_text((sqlite3_stmt*)stmt, (int)idx,
        (const char*)value, -1, (void(*)(void*))(intptr_t)-1);
}

long long ep_sqlite3_bind_int(long long stmt, long long idx, long long value) {
    return (long long)sqlite3_bind_int64((sqlite3_stmt*)stmt, (int)idx, value);
}

long long ep_sqlite3_step(long long stmt) {
    return (long long)sqlite3_step((sqlite3_stmt*)stmt);
}

long long ep_sqlite3_column_count(long long stmt) {
    return (long long)sqlite3_column_count((sqlite3_stmt*)stmt);
}

long long ep_sqlite3_column_text(long long stmt, long long col) {
    const unsigned char* t = sqlite3_column_text((sqlite3_stmt*)stmt, (int)col);
    char* copy = (!t) ? strdup("") : strdup((const char*)t);
    /* Register the copy with the GC so it is reclaimed (not leaked) and so
       ep_auto_to_string recognizes it as a string deterministically via
       ep_gc_find, rather than relying on the memory-probe heuristic. */
    if (copy) ep_gc_register(copy, EP_OBJ_STRING);
    return (long long)copy;
}

long long ep_sqlite3_column_int(long long stmt, long long col) {
    return sqlite3_column_int64((sqlite3_stmt*)stmt, (int)col);
}

long long ep_sqlite3_finalize(long long stmt) {
    return (long long)sqlite3_finalize((sqlite3_stmt*)stmt);
}
#endif /* EP_HAS_SQLITE */

int ep_argc = 0;
char** ep_argv = NULL;

void init_ep_args(int argc, char** argv) {
    ep_argc = argc;
    ep_argv = argv;
    ep_gc_register_thread((void*)&argc);
    /* Wire up channel scanning for GC (defined after EpChannel struct) */
    ep_gc_scan_channels_major = ep_gc_scan_channels_major_impl;
    ep_gc_scan_channels_minor = ep_gc_scan_channels_minor_impl;
    /* Wire up map value traversal for GC (defined after EpMap struct) */
    ep_gc_mark_map_values = ep_gc_mark_map_values_impl;
    ep_gc_mark_map_values_minor = ep_gc_mark_map_values_minor_impl;
}

long long get_argument_count(void) {
    return ep_argc;
}

const char* get_argument(long long index) {
    if (index < 0 || index >= ep_argc) {
        char* empty = malloc(1);
        if (empty) empty[0] = '\0';
        return empty;
    }
    return ep_argv[index];
}

long long write_file_content(const char* filepath, const char* content) {
    char mode[3];
    mode[0] = 'w';
    mode[1] = 'b';
    mode[2] = '\0';
    FILE* f = fopen(filepath, mode);
    if (!f) return 0;
    size_t len = strlen(content);
    size_t written = fwrite(content, 1, len, f);
    fclose(f);
    return written == len ? 1 : 0;
}

long long run_command(const char* command) {
    if (!command) return -1;
    return system(command);
}

char* substring(const char* s, long long start, long long len) {
    if (!s) {
        char* empty = malloc(1);
        if (empty) empty[0] = '\0';
        ep_gc_register(empty, EP_OBJ_STRING);
        return empty;
    }
    long long total_len = strlen(s);
    if (start < 0 || start >= total_len || len <= 0) {
        char* empty = malloc(1);
        if (empty) empty[0] = '\0';
        ep_gc_register(empty, EP_OBJ_STRING);
        return empty;
    }
    if (start + len > total_len) {
        len = total_len - start;
    }
    char* sub = malloc(len + 1);
    if (!sub) {
        char* empty = malloc(1);
        if (empty) empty[0] = '\0';
        ep_gc_register(empty, EP_OBJ_STRING);
        return empty;
    }
    strncpy(sub, s + start, len);
    sub[len] = '\0';
    ep_gc_register(sub, EP_OBJ_STRING);
    return sub;
}

char* string_from_list(long long list_ptr) {
    EpList* list = (EpList*)list_ptr;
    if (!list) {
        char* empty = malloc(1);
        if (empty) empty[0] = '\0';
        ep_gc_register(empty, EP_OBJ_STRING);
        return empty;
    }
    char* s = malloc(list->length + 1);
    if (!s) {
        char* empty = malloc(1);
        if (empty) empty[0] = '\0';
        ep_gc_register(empty, EP_OBJ_STRING);
        return empty;
    }
    for (long long i = 0; i < list->length; i++) {
        s[i] = (char)list->data[i];
    }
    s[list->length] = '\0';
    ep_gc_register(s, EP_OBJ_STRING);
    return s;
}

// Inverse of string_from_list: convert a string to a list of its byte values in
// a single O(n) pass (one strlen + one copy). This lets callers iterate a string
// in O(n) total via O(1) get_list, instead of O(n) get_character per index
// (which is O(n^2) over the whole string).
long long string_to_list(const char* s) {
    EpList* list = malloc(sizeof(EpList));
    if (!list) return 0;
    long long len = s ? (long long)strlen(s) : 0;
    list->capacity = len > 0 ? len : 4;
    list->length = len;
    list->data = malloc(list->capacity * sizeof(long long));
    if (!list->data) {
        list->capacity = 0;
        list->length = 0;
        ep_gc_register(list, EP_OBJ_LIST);
        return (long long)list;
    }
    for (long long i = 0; i < len; i++) {
        list->data[i] = (unsigned char)s[i];
    }
    ep_gc_register(list, EP_OBJ_LIST);
    return (long long)list;
}

long long pop_list(long long list_ptr) {
    if (EP_BADPTR(list_ptr)) return 0;
    EpList* list = (EpList*)list_ptr;
    if (!list || list->length <= 0) return 0;
    list->length -= 1;
    return list->data[list->length];
}

long long remove_list(long long list_ptr, long long index) {
    if (EP_BADPTR(list_ptr)) return 0;
    EpList* list = (EpList*)list_ptr;
    if (!list || index < 0 || index >= list->length) return 0;
    long long removed = list->data[index];
    for (long long i = index; i < list->length - 1; i++) {
        list->data[i] = list->data[i + 1];
    }
    list->length -= 1;
    return removed;
}

long long display_string(const char* s) {
    if (s) puts(s);
    return 0;
}

/* Write text with NO trailing newline, and flush at once — for drawing to
   the screen where every byte's position matters (cursor moves, escape
   codes, a full-screen frame). puts()/display_string would append a
   newline and scroll a full-height frame; this does not. */
long long screen_write(const char* s) {
    if (s) {
        fputs(s, stdout);
        fflush(stdout);
    }
    return 0;
}

/* ========== File System Runtime ========== */
#include <sys/stat.h>
#ifdef _WIN32
  #include <io.h>
  #include <direct.h>
  #define mkdir(p, m) _mkdir(p)
  #define rmdir _rmdir
  #define getcwd _getcwd
  #define popen _popen
  #define pclose _pclose
  #define getpid _getpid
  #define setenv(k, v, o) _putenv_s(k, v)
  /* Minimal dirent polyfill for Windows */
  #include <windows.h>
  typedef struct { char d_name[260]; } ep_dirent;
  typedef struct { HANDLE hFind; WIN32_FIND_DATAA data; int first; } EP_DIR;
  static EP_DIR* ep_opendir(const char* p) {
      EP_DIR* d = (EP_DIR*)malloc(sizeof(EP_DIR));
      char buf[270]; snprintf(buf, sizeof(buf), "%s\\*", p);
      d->hFind = FindFirstFileA(buf, &d->data);
      d->first = 1;
      return (d->hFind == INVALID_HANDLE_VALUE) ? (free(d), (EP_DIR*)NULL) : d;
  }
  static ep_dirent* ep_readdir(EP_DIR* d) {
      static ep_dirent ent;
      if (d->first) { d->first = 0; strcpy(ent.d_name, d->data.cFileName); return &ent; }
      if (!FindNextFileA(d->hFind, &d->data)) return NULL;
      strcpy(ent.d_name, d->data.cFileName); return &ent;
  }
  static void ep_closedir(EP_DIR* d) { FindClose(d->hFind); free(d); }
  #define DIR EP_DIR
  #define dirent ep_dirent
  #define opendir ep_opendir
  #define readdir ep_readdir
  #define closedir ep_closedir
#else
  #include <dirent.h>
  #include <unistd.h>
#endif

long long ep_read_file(long long path_ptr) {
    const char* path = (const char*)path_ptr;
    FILE* f = fopen(path, "rb");
    if (!f) return (long long)"";
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    char* buf = (char*)malloc(size + 1);
    fread(buf, 1, size, f);
    buf[size] = '\0';
    fclose(f);
    return (long long)buf;
}

long long ep_write_file(long long path_ptr, long long content_ptr) {
    const char* path = (const char*)path_ptr;
    const char* content = (const char*)content_ptr;
    FILE* f = fopen(path, "wb");
    if (!f) return 0;
    fputs(content, f);
    fclose(f);
    return 1;
}

long long ep_append_file(long long path_ptr, long long content_ptr) {
    const char* path = (const char*)path_ptr;
    const char* content = (const char*)content_ptr;
    FILE* f = fopen(path, "ab");
    if (!f) return 0;
    fputs(content, f);
    fclose(f);
    return 1;
}

long long ep_file_exists(long long path_ptr) {
    const char* path = (const char*)path_ptr;
    struct stat st;
    return stat(path, &st) == 0 ? 1 : 0;
}

long long ep_is_directory(long long path_ptr) {
    const char* path = (const char*)path_ptr;
    struct stat st;
    if (stat(path, &st) != 0) return 0;
    return S_ISDIR(st.st_mode) ? 1 : 0;
}

long long ep_file_size(long long path_ptr) {
    const char* path = (const char*)path_ptr;
    struct stat st;
    if (stat(path, &st) != 0) return -1;
    return (long long)st.st_size;
}

long long ep_list_directory(long long path_ptr) {
    const char* path = (const char*)path_ptr;
    DIR* dir = opendir(path);
    if (!dir) return (long long)create_list();
    long long list = create_list();
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.' && (entry->d_name[1] == '\0' || 
            (entry->d_name[1] == '.' && entry->d_name[2] == '\0'))) continue;
        char* name = strdup(entry->d_name);
        append_list(list, (long long)name);
    }
    closedir(dir);
    return list;
}

long long ep_create_directory(long long path_ptr) {
    const char* path = (const char*)path_ptr;
    return mkdir(path, 0755) == 0 ? 1 : 0;
}

long long ep_remove_file(long long path_ptr) {
    const char* path = (const char*)path_ptr;
    return remove(path) == 0 ? 1 : 0;
}

long long ep_remove_directory(long long path_ptr) {
    const char* path = (const char*)path_ptr;
    return rmdir(path) == 0 ? 1 : 0;
}

long long ep_rename_file(long long old_ptr, long long new_ptr) {
    return rename((const char*)old_ptr, (const char*)new_ptr) == 0 ? 1 : 0;
}

long long ep_copy_file(long long src_ptr, long long dst_ptr) {
    const char* src = (const char*)src_ptr;
    const char* dst = (const char*)dst_ptr;
    FILE* fin = fopen(src, "rb");
    if (!fin) return 0;
    FILE* fout = fopen(dst, "wb");
    if (!fout) { fclose(fin); return 0; }
    char buf[8192];
    size_t n;
    while ((n = fread(buf, 1, sizeof(buf), fin)) > 0) {
        fwrite(buf, 1, n, fout);
    }
    fclose(fin);
    fclose(fout);
    return 1;
}

/* ========== Date/Time Runtime ========== */
#include <time.h>
#include <sys/time.h>

long long ep_time_now_ms(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (long long)tv.tv_sec * 1000LL + (long long)tv.tv_usec / 1000LL;
}

long long ep_time_now_sec(void) {
    return (long long)time(NULL);
}


long long ep_time_year(long long ts) {
    time_t t = (time_t)ts;
    struct tm* tm = localtime(&t);
    return tm ? tm->tm_year + 1900 : 0;
}

long long ep_time_month(long long ts) {
    time_t t = (time_t)ts;
    struct tm* tm = localtime(&t);
    return tm ? tm->tm_mon + 1 : 0;
}

long long ep_time_day(long long ts) {
    time_t t = (time_t)ts;
    struct tm* tm = localtime(&t);
    return tm ? tm->tm_mday : 0;
}

long long ep_time_hour(long long ts) {
    time_t t = (time_t)ts;
    struct tm* tm = localtime(&t);
    return tm ? tm->tm_hour : 0;
}

long long ep_time_minute(long long ts) {
    time_t t = (time_t)ts;
    struct tm* tm = localtime(&t);
    return tm ? tm->tm_min : 0;
}

long long ep_time_second(long long ts) {
    time_t t = (time_t)ts;
    struct tm* tm = localtime(&t);
    return tm ? tm->tm_sec : 0;
}

long long ep_time_weekday(long long ts) {
    time_t t = (time_t)ts;
    struct tm* tm = localtime(&t);
    return tm ? tm->tm_wday : 0;
}

long long ep_format_time(long long ts, long long fmt_ptr) {
    time_t t = (time_t)ts;
    struct tm* tm = localtime(&t);
    if (!tm) return (long long)"";
    char* buf = (char*)malloc(256);
    strftime(buf, 256, (const char*)fmt_ptr, tm);
    return (long long)buf;
}

/* ========== OS Runtime ========== */

long long ep_getenv(long long name_ptr) {
    const char* val = getenv((const char*)name_ptr);
    return val ? (long long)val : (long long)"";
}

long long ep_setenv(long long name_ptr, long long val_ptr) {
    return setenv((const char*)name_ptr, (const char*)val_ptr, 1) == 0 ? 1 : 0;
}

long long ep_get_cwd(void) {
    char* buf = (char*)malloc(4096);
    if (getcwd(buf, 4096)) return (long long)buf;
    free(buf);
    return (long long)"";
}

long long ep_os_name(void) {
    #if defined(__APPLE__)
    return (long long)"macos";
    #elif defined(__linux__)
    return (long long)"linux";
    #elif defined(_WIN32)
    return (long long)"windows";
    #else
    return (long long)"unknown";
    #endif
}

long long ep_arch_name(void) {
    #if defined(__aarch64__) || defined(__arm64__)
    return (long long)"arm64";
    #elif defined(__x86_64__)
    return (long long)"x86_64";
    #elif defined(__i386__)
    return (long long)"x86";
    #else
    return (long long)"unknown";
    #endif
}

long long ep_exit(long long code) {
    exit((int)code);
    return 0;
}

long long ep_get_pid(void) {
    return (long long)getpid();
}

long long ep_get_home_dir(void) {
    const char* home = getenv("HOME");
    return home ? (long long)home : (long long)"";
}

#ifdef __wasm__
long long ep_run_command(long long cmd_ptr) {
    (void)cmd_ptr;
    return (long long)"Error: running external commands is not supported on WebAssembly";
}
#else
long long ep_run_command(long long cmd_ptr) {
    const char* cmd = (const char*)cmd_ptr;
    FILE* fp = popen(cmd, "r");
    if (!fp) return (long long)"";
    char* result = (char*)malloc(65536);
    size_t total = 0;
    char buf[4096];
    while (fgets(buf, sizeof(buf), fp)) {
        size_t len = strlen(buf);
        memcpy(result + total, buf, len);
        total += len;
    }
    result[total] = '\0';
    pclose(fp);
    return (long long)result;
}
#endif

/* ========== HashMap helpers ========== */

long long ep_hash_string(long long s_ptr) {
    const char* s = (const char*)s_ptr;
    if (!s) return 0;
    unsigned long long hash = 5381;
    int c;
    while ((c = *s++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return (long long)hash;
}

long long ep_str_equals(long long a_ptr, long long b_ptr) {
    if (a_ptr == b_ptr) return 1;
    if (!a_ptr || !b_ptr) return 0;
    /* If either value looks like a small integer (not a valid heap pointer),
       fall back to integer comparison — strcmp would segfault. */
    if ((unsigned long long)a_ptr < 4096ULL || (unsigned long long)b_ptr < 4096ULL) return 0;
    return strcmp((const char*)a_ptr, (const char*)b_ptr) == 0 ? 1 : 0;
}

/* ========== Sync Primitives ========== */

#ifdef _WIN32
long long ep_mutex_create(void) {
    CRITICAL_SECTION* m = (CRITICAL_SECTION*)malloc(sizeof(CRITICAL_SECTION));
    InitializeCriticalSection(m);
    return (long long)m;
}
long long ep_mutex_lock_fn(long long m) {
    EnterCriticalSection((CRITICAL_SECTION*)m);
    return 1;
}
long long ep_mutex_unlock_fn(long long m) {
    LeaveCriticalSection((CRITICAL_SECTION*)m);
    return 1;
}
long long ep_mutex_trylock(long long m) {
    return TryEnterCriticalSection((CRITICAL_SECTION*)m) ? 1 : 0;
}
long long ep_mutex_destroy(long long m) {
    DeleteCriticalSection((CRITICAL_SECTION*)m);
    free((void*)m);
    return 0;
}
#else
long long ep_mutex_create(void) {
    pthread_mutex_t* m = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(m, NULL);
    return (long long)m;
}

long long ep_mutex_lock_fn(long long m) {
    return pthread_mutex_lock((pthread_mutex_t*)m) == 0 ? 1 : 0;
}

long long ep_mutex_unlock_fn(long long m) {
    return pthread_mutex_unlock((pthread_mutex_t*)m) == 0 ? 1 : 0;
}

long long ep_mutex_trylock(long long m) {
    return pthread_mutex_trylock((pthread_mutex_t*)m) == 0 ? 1 : 0;
}

long long ep_mutex_destroy(long long m) {
    pthread_mutex_destroy((pthread_mutex_t*)m);
    free((void*)m);
    return 0;
}
#endif

#ifdef _WIN32
long long ep_rwlock_create(void) {
    SRWLOCK* rwl = (SRWLOCK*)malloc(sizeof(SRWLOCK));
    InitializeSRWLock(rwl);
    return (long long)rwl;
}
long long ep_rwlock_read_lock(long long rwl) {
    AcquireSRWLockShared((SRWLOCK*)rwl);
    return 1;
}
long long ep_rwlock_write_lock(long long rwl) {
    AcquireSRWLockExclusive((SRWLOCK*)rwl);
    return 1;
}
long long ep_rwlock_unlock(long long rwl) {
    /* SRWLOCK does not have a single "unlock" — we try exclusive first.
       In practice the caller should know which lock was taken.
       ReleaseSRWLockExclusive on a shared lock is undefined, but
       the runtime guarantees matched lock/unlock pairs. We default
       to releasing the exclusive lock; shared unlock is handled
       by pairing read_lock -> read_unlock if needed later. */
    ReleaseSRWLockExclusive((SRWLOCK*)rwl);
    return 1;
}
long long ep_rwlock_destroy(long long rwl) {
    /* SRWLOCK has no destroy */
    free((void*)rwl);
    return 0;
}
#else
long long ep_rwlock_create(void) {
    pthread_rwlock_t* rwl = (pthread_rwlock_t*)malloc(sizeof(pthread_rwlock_t));
    pthread_rwlock_init(rwl, NULL);
    return (long long)rwl;
}

long long ep_rwlock_read_lock(long long rwl) {
    return pthread_rwlock_rdlock((pthread_rwlock_t*)rwl) == 0 ? 1 : 0;
}

long long ep_rwlock_write_lock(long long rwl) {
    return pthread_rwlock_wrlock((pthread_rwlock_t*)rwl) == 0 ? 1 : 0;
}

long long ep_rwlock_unlock(long long rwl) {
    return pthread_rwlock_unlock((pthread_rwlock_t*)rwl) == 0 ? 1 : 0;
}

long long ep_rwlock_destroy(long long rwl) {
    pthread_rwlock_destroy((pthread_rwlock_t*)rwl);
    free((void*)rwl);
    return 0;
}
#endif

#ifdef _MSC_VER
long long ep_atomic_create(long long initial) {
    volatile long long* a = (volatile long long*)malloc(sizeof(long long));
    InterlockedExchange64(a, initial);
    return (long long)a;
}
long long ep_atomic_load(long long a) {
    return InterlockedCompareExchange64((volatile long long*)a, 0, 0);
}
long long ep_atomic_store(long long a, long long value) {
    InterlockedExchange64((volatile long long*)a, value);
    return value;
}
long long ep_atomic_add(long long a, long long delta) {
    return InterlockedExchangeAdd64((volatile long long*)a, delta);
}
long long ep_atomic_sub(long long a, long long delta) {
    return InterlockedExchangeAdd64((volatile long long*)a, -delta);
}
long long ep_atomic_cas(long long a, long long expected, long long desired) {
    long long old = InterlockedCompareExchange64((volatile long long*)a, desired, expected);
    return (old == expected) ? 1 : 0;
}
#else
long long ep_atomic_create(long long initial) {
    long long* a = (long long*)malloc(sizeof(long long));
    __atomic_store_n(a, initial, __ATOMIC_SEQ_CST);
    return (long long)a;
}

long long ep_atomic_load(long long a) {
    return __atomic_load_n((long long*)a, __ATOMIC_SEQ_CST);
}

long long ep_atomic_store(long long a, long long value) {
    __atomic_store_n((long long*)a, value, __ATOMIC_SEQ_CST);
    return value;
}

long long ep_atomic_add(long long a, long long delta) {
    return __atomic_fetch_add((long long*)a, delta, __ATOMIC_SEQ_CST);
}

long long ep_atomic_sub(long long a, long long delta) {
    return __atomic_fetch_sub((long long*)a, delta, __ATOMIC_SEQ_CST);
}

long long ep_atomic_cas(long long a, long long expected, long long desired) {
    long long exp = expected;
    return __atomic_compare_exchange_n((long long*)a, &exp, desired, 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST) ? 1 : 0;
}
#endif

/* Barrier — portable polyfill (macOS lacks pthread_barrier_t) */
typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    unsigned count;
    unsigned target;
    unsigned generation;
} EpBarrier;

long long ep_barrier_create(long long count) {
    EpBarrier* b = (EpBarrier*)malloc(sizeof(EpBarrier));
    pthread_mutex_init(&b->mutex, NULL);
    pthread_cond_init(&b->cond, NULL);
    b->count = 0;
    b->target = (unsigned)count;
    b->generation = 0;
    return (long long)b;
}

long long ep_barrier_wait(long long bp) {
    EpBarrier* b = (EpBarrier*)bp;
    pthread_mutex_lock(&b->mutex);
    unsigned gen = b->generation;
    b->count++;
    if (b->count >= b->target) {
        b->count = 0;
        b->generation++;
        pthread_cond_broadcast(&b->cond);
        pthread_mutex_unlock(&b->mutex);
        return 1; /* serial thread */
    }
    while (gen == b->generation) {
        pthread_cond_wait(&b->cond, &b->mutex);
    }
    pthread_mutex_unlock(&b->mutex);
    return 0;
}

long long ep_barrier_destroy(long long bp) {
    EpBarrier* b = (EpBarrier*)bp;
    pthread_mutex_destroy(&b->mutex);
    pthread_cond_destroy(&b->cond);
    free(b);
    return 0;
}

/* Semaphore via mutex+condvar (portable) */
typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    long long value;
} EpSemaphore;

long long ep_semaphore_create(long long initial) {
    EpSemaphore* s = (EpSemaphore*)malloc(sizeof(EpSemaphore));
    pthread_mutex_init(&s->mutex, NULL);
    pthread_cond_init(&s->cond, NULL);
    s->value = initial;
    return (long long)s;
}

long long ep_semaphore_wait(long long sp) {
    EpSemaphore* s = (EpSemaphore*)sp;
    pthread_mutex_lock(&s->mutex);
    while (s->value <= 0) {
        pthread_cond_wait(&s->cond, &s->mutex);
    }
    s->value--;
    pthread_mutex_unlock(&s->mutex);
    return 1;
}

long long ep_semaphore_post(long long sp) {
    EpSemaphore* s = (EpSemaphore*)sp;
    pthread_mutex_lock(&s->mutex);
    s->value++;
    pthread_cond_signal(&s->cond);
    pthread_mutex_unlock(&s->mutex);
    return 1;
}

long long ep_semaphore_trywait(long long sp) {
    EpSemaphore* s = (EpSemaphore*)sp;
    pthread_mutex_lock(&s->mutex);
    if (s->value > 0) {
        s->value--;
        pthread_mutex_unlock(&s->mutex);
        return 1;
    }
    pthread_mutex_unlock(&s->mutex);
    return 0;
}

long long ep_semaphore_destroy(long long sp) {
    EpSemaphore* s = (EpSemaphore*)sp;
    pthread_mutex_destroy(&s->mutex);
    pthread_cond_destroy(&s->cond);
    free(s);
    return 0;
}

long long ep_condvar_create(void) {
    pthread_cond_t* cv = (pthread_cond_t*)malloc(sizeof(pthread_cond_t));
    pthread_cond_init(cv, NULL);
    return (long long)cv;
}

long long ep_condvar_wait(long long cv, long long m) {
    return pthread_cond_wait((pthread_cond_t*)cv, (pthread_mutex_t*)m) == 0 ? 1 : 0;
}

long long ep_condvar_signal(long long cv) {
    return pthread_cond_signal((pthread_cond_t*)cv) == 0 ? 1 : 0;
}

long long ep_condvar_broadcast(long long cv) {
    return pthread_cond_broadcast((pthread_cond_t*)cv) == 0 ? 1 : 0;
}

long long ep_condvar_destroy(long long cv) {
    pthread_cond_destroy((pthread_cond_t*)cv);
    free((void*)cv);
    return 0;
}

/* ========== Regex (simple stub — delegates to POSIX regex) ========== */
#include <regex.h>

long long ep_regex_match(long long pattern_ptr, long long text_ptr) {
    regex_t regex;
    const char* pattern = (const char*)pattern_ptr;
    const char* text = (const char*)text_ptr;
    int ret = regcomp(&regex, pattern, REG_EXTENDED | REG_NOSUB);
    if (ret) return 0;
    ret = regexec(&regex, text, 0, NULL, 0);
    regfree(&regex);
    return ret == 0 ? 1 : 0;
}

long long ep_regex_find(long long pattern_ptr, long long text_ptr) {
    regex_t regex;
    regmatch_t match;
    const char* pattern = (const char*)pattern_ptr;
    const char* text = (const char*)text_ptr;
    int ret = regcomp(&regex, pattern, REG_EXTENDED);
    if (ret) return (long long)"";
    ret = regexec(&regex, text, 1, &match, 0);
    if (ret != 0) { regfree(&regex); return (long long)""; }
    int len = match.rm_eo - match.rm_so;
    char* result = (char*)malloc(len + 1);
    memcpy(result, text + match.rm_so, len);
    result[len] = '\0';
    regfree(&regex);
    return (long long)result;
}

long long ep_regex_find_all(long long pattern_ptr, long long text_ptr) {
    regex_t regex;
    regmatch_t match;
    const char* pattern = (const char*)pattern_ptr;
    const char* text = (const char*)text_ptr;
    long long list = create_list();
    int ret = regcomp(&regex, pattern, REG_EXTENDED);
    if (ret) return list;
    const char* cursor = text;
    while (regexec(&regex, cursor, 1, &match, 0) == 0) {
        int len = match.rm_eo - match.rm_so;
        char* result = (char*)malloc(len + 1);
        memcpy(result, cursor + match.rm_so, len);
        result[len] = '\0';
        append_list(list, (long long)result);
        cursor += match.rm_eo;
        if (match.rm_eo == 0) break;
    }
    regfree(&regex);
    return list;
}

long long ep_regex_replace(long long pattern_ptr, long long text_ptr, long long repl_ptr) {
    /* Simple single-replacement via regex */
    regex_t regex;
    regmatch_t match;
    const char* pattern = (const char*)pattern_ptr;
    const char* text = (const char*)text_ptr;
    const char* repl = (const char*)repl_ptr;
    int ret = regcomp(&regex, pattern, REG_EXTENDED);
    if (ret) return text_ptr;
    ret = regexec(&regex, text, 1, &match, 0);
    if (ret != 0) { regfree(&regex); return text_ptr; }
    size_t tlen = strlen(text);
    size_t rlen = strlen(repl);
    size_t new_len = tlen - (match.rm_eo - match.rm_so) + rlen;
    char* result = (char*)malloc(new_len + 1);
    memcpy(result, text, match.rm_so);
    memcpy(result + match.rm_so, repl, rlen);
    memcpy(result + match.rm_so + rlen, text + match.rm_eo, tlen - match.rm_eo);
    result[new_len] = '\0';
    regfree(&regex);
    return (long long)result;
}

long long ep_regex_split(long long pattern_ptr, long long text_ptr) {
    long long list = create_list();
    /* Simple split: find matches and split around them */
    regex_t regex;
    regmatch_t match;
    const char* pattern = (const char*)pattern_ptr;
    const char* text = (const char*)text_ptr;
    int ret = regcomp(&regex, pattern, REG_EXTENDED);
    if (ret) {
        append_list(list, text_ptr);
        return list;
    }
    const char* cursor = text;
    while (regexec(&regex, cursor, 1, &match, 0) == 0) {
        int len = match.rm_so;
        char* part = (char*)malloc(len + 1);
        memcpy(part, cursor, len);
        part[len] = '\0';
        append_list(list, (long long)part);
        cursor += match.rm_eo;
        if (match.rm_eo == 0) break;
    }
    char* rest = strdup(cursor);
    append_list(list, (long long)rest);
    regfree(&regex);
    return list;
}

/* ========== Base64 ========== */
static const char b64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

long long ep_base64_encode(long long data_ptr) {
    const unsigned char* data = (const unsigned char*)data_ptr;
    size_t len = strlen((const char*)data);
    size_t out_len = 4 * ((len + 2) / 3);
    char* out = (char*)malloc(out_len + 1);
    size_t i, j = 0;
    for (i = 0; i < len; i += 3) {
        unsigned int n = data[i] << 16;
        if (i + 1 < len) n |= data[i+1] << 8;
        if (i + 2 < len) n |= data[i+2];
        out[j++] = b64_table[(n >> 18) & 63];
        out[j++] = b64_table[(n >> 12) & 63];
        out[j++] = (i + 1 < len) ? b64_table[(n >> 6) & 63] : '=';
        out[j++] = (i + 2 < len) ? b64_table[n & 63] : '=';
    }
    out[j] = '\0';
    return (long long)out;
}

long long ep_uuid_v4(void) {
    char* uuid = (char*)malloc(37);
    unsigned char bytes[16];
    ep_secure_random_bytes(bytes, 16);
    bytes[6] = (bytes[6] & 0x0F) | 0x40;
    bytes[8] = (bytes[8] & 0x3F) | 0x80;
    snprintf(uuid, 37, "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
        bytes[0], bytes[1], bytes[2], bytes[3],
        bytes[4], bytes[5], bytes[6], bytes[7],
        bytes[8], bytes[9], bytes[10], bytes[11],
        bytes[12], bytes[13], bytes[14], bytes[15]);
    return (long long)uuid;
}

long long file_read(long long path_val) {
    const char* path = (const char*)path_val;
    if (!path) return (long long)strdup("");
    FILE* f = fopen(path, "rb");
    if (!f) return (long long)strdup("");
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    char* buf = malloc(size + 1);
    if (!buf) { fclose(f); return (long long)strdup(""); }
    fread(buf, 1, size, f);
    buf[size] = '\0';
    fclose(f);
    ep_gc_register(buf, EP_OBJ_STRING);
    return (long long)buf;
}

long long file_write(long long path_val, long long content_val) {
    const char* path = (const char*)path_val;
    const char* content = (const char*)content_val;
    if (!path || !content) return 0;
    FILE* f = fopen(path, "wb");
    if (!f) return 0;
    size_t len = strlen(content);
    fwrite(content, 1, len, f);
    fclose(f);
    return 1;
}

long long file_append(long long path_val, long long content_val) {
    const char* path = (const char*)path_val;
    const char* content = (const char*)content_val;
    if (!path || !content) return 0;
    FILE* f = fopen(path, "ab");
    if (!f) return 0;
    size_t len = strlen(content);
    fwrite(content, 1, len, f);
    fclose(f);
    return 1;
}

long long file_exists(long long path_val) {
    const char* path = (const char*)path_val;
    if (!path) return 0;
    FILE* f = fopen(path, "r");
    if (f) { fclose(f); return 1; }
    return 0;
}

long long string_contains(long long s_val, long long sub_val) {
    const char* s = (const char*)s_val;
    const char* sub = (const char*)sub_val;
    if (!s || !sub) return 0;
    return strstr(s, sub) != NULL ? 1 : 0;
}

long long string_index_of(long long s_val, long long sub_val) {
    const char* s = (const char*)s_val;
    const char* sub = (const char*)sub_val;
    if (!s || !sub) return -1;
    const char* found = strstr(s, sub);
    if (!found) return -1;
    return (long long)(found - s);
}

long long string_replace(long long s_val, long long old_val, long long new_val) {
    const char* s = (const char*)s_val;
    const char* old_str = (const char*)old_val;
    const char* new_str = (const char*)new_val;
    if (!s || !old_str || !new_str) return (long long)strdup(s ? s : "");
    size_t old_len = strlen(old_str);
    size_t new_len = strlen(new_str);
    if (old_len == 0) return (long long)strdup(s);
    int count = 0;
    const char* p = s;
    while ((p = strstr(p, old_str)) != NULL) { count++; p += old_len; }
    size_t result_len = strlen(s) + count * (new_len - old_len);
    char* result = malloc(result_len + 1);
    if (!result) return (long long)strdup(s);
    char* dst = result;
    p = s;
    while (*p) {
        if (strncmp(p, old_str, old_len) == 0) {
            memcpy(dst, new_str, new_len);
            dst += new_len;
            p += old_len;
        } else {
            *dst++ = *p++;
        }
    }
    *dst = '\0';
    ep_gc_register(result, EP_OBJ_STRING);
    return (long long)result;
}

/* ========== Additional String Functions ========== */
#include <ctype.h>

long long string_upper(long long s_val) {
    const char* s = (const char*)s_val;
    if (!s) return (long long)strdup("");
    long long len = strlen(s);
    char* result = malloc(len + 1);
    for (long long i = 0; i < len; i++) result[i] = toupper((unsigned char)s[i]);
    result[len] = '\0';
    ep_gc_register(result, EP_OBJ_STRING);
    return (long long)result;
}

long long string_lower(long long s_val) {
    const char* s = (const char*)s_val;
    if (!s) return (long long)strdup("");
    long long len = strlen(s);
    char* result = malloc(len + 1);
    for (long long i = 0; i < len; i++) result[i] = tolower((unsigned char)s[i]);
    result[len] = '\0';
    ep_gc_register(result, EP_OBJ_STRING);
    return (long long)result;
}

long long string_trim(long long s_val) {
    const char* s = (const char*)s_val;
    if (!s) return (long long)strdup("");
    while (*s && isspace((unsigned char)*s)) s++;
    long long len = strlen(s);
    while (len > 0 && isspace((unsigned char)s[len - 1])) len--;
    char* result = malloc(len + 1);
    memcpy(result, s, len);
    result[len] = '\0';
    ep_gc_register(result, EP_OBJ_STRING);
    return (long long)result;
}

long long string_split(long long s_val, long long delim_val) {
    const char* s = (const char*)s_val;
    const char* delim = (const char*)delim_val;
    if (!s || !delim) return create_list();
    long long list = create_list();
    long long dlen = strlen(delim);
    if (dlen == 0) { append_list(list, s_val); return list; }
    const char* p = s;
    while (1) {
        const char* found = strstr(p, delim);
        long long partlen = found ? (found - p) : (long long)strlen(p);
        char* part = malloc(partlen + 1);
        memcpy(part, p, partlen);
        part[partlen] = '\0';
        ep_gc_register(part, EP_OBJ_STRING);
        append_list(list, (long long)part);
        if (!found) break;
        p = found + dlen;
    }
    return list;
}

long long char_at(long long s_val, long long index) {
    const char* s = (const char*)s_val;
    if (!s || index < 0 || index >= (long long)strlen(s)) return 0;
    return (unsigned char)s[index];
}

long long char_from_code(long long code) {
    char* result = malloc(2);
    result[0] = (char)code;
    result[1] = '\0';
    ep_gc_register(result, EP_OBJ_STRING);
    return (long long)result;
}

long long ep_abs(long long n) {
    return n < 0 ? -n : n;
}

// Auto-convert any value to string for string interpolation
long long ep_auto_to_string(long long val) {
    // If the value is 0, return "0"
    if (val == 0) return (long long)strdup("0");
    // Check if val is a GC-tracked string (heap-allocated)
    EpGCObject* obj = ep_gc_find((void*)val);
    if (obj && obj->kind == EP_OBJ_STRING) {
        return val; // It's a known string pointer
    }
    // Check if val is a static string literal (in .rodata/.data segment)
    // These aren't GC-tracked but ARE valid pointers. Use a safe probe:
    // only dereference if the address is in a readable memory page.
    if (val > 0x100000) {
#if defined(_WIN32)
        // Windows: use VirtualQuery to safely probe pointer validity
        MEMORY_BASIC_INFORMATION mbi;
        if (VirtualQuery((void*)val, &mbi, sizeof(mbi)) && mbi.State == MEM_COMMIT && !(mbi.Protect & (PAGE_NOACCESS | PAGE_GUARD))) {
            const char* p = (const char*)(void*)val;
            unsigned char first = (unsigned char)*p;
            if ((first >= 0x20 && first <= 0x7E) || (first >= 0xC0 && first <= 0xFD) || first == '\n' || first == '\t' || first == '\r' || first == 0) {
                return val; // Readable memory, looks like a string
            }
        }
#elif defined(__APPLE__)
        // macOS: use vm_read_overwrite to safely probe
        char probe;
        vm_size_t sz = 1;
        kern_return_t kr = vm_read_overwrite(mach_task_self(), (mach_vm_address_t)val, 1, (mach_vm_address_t)&probe, &sz);
        if (kr == KERN_SUCCESS) {
            unsigned char first = (unsigned char)probe;
            if ((first >= 0x20 && first <= 0x7E) || (first >= 0xC0 && first <= 0xFD) || first == '\n' || first == '\t' || first == '\r' || first == 0) {
                return val; // Readable memory, looks like a string
            }
        }
#else
        // Linux: use write() to /dev/null as a safe pointer probe
        // write() returns -1 with EFAULT for invalid pointers, no signal
        int devnull = open("/dev/null", 1); // O_WRONLY
        if (devnull >= 0) {
            ssize_t r = write(devnull, (const void*)val, 1);
            close(devnull);
            if (r == 1) {
                const char* p = (const char*)(void*)val;
                unsigned char first = (unsigned char)*p;
                if ((first >= 0x20 && first <= 0x7E) || (first >= 0xC0 && first <= 0xFD) || first == '\n' || first == '\t' || first == '\r' || first == 0) {
                    return val;
                }
            }
        }
#endif
    }
    // Otherwise, convert integer to string
    char* buf = (char*)malloc(32);
    snprintf(buf, 32, "%lld", val);
    ep_gc_register(buf, EP_OBJ_STRING);
    return (long long)buf;
}

/* Format a Float (double bits carried in a long long) as a string. F-string
   interpolation routes Float-typed expressions here: ep_auto_to_string cannot
   know the bits are a double and would print them as a huge integer. Uses the
   same %.15g format as `display` so a float reads identically both ways. */
long long ep_float_to_string(long long bits) {
    double d;
    memcpy(&d, &bits, sizeof(double));
    char* buf = (char*)malloc(40);
    snprintf(buf, 40, "%.15g", d);
    ep_gc_register(buf, EP_OBJ_STRING);
    return (long long)buf;
}

long long ep_random_int(long long min, long long max) {
    if (max <= min) return min;
    /* Draw from the OS CSPRNG with rejection sampling to avoid modulo bias. */
    unsigned long long range = (unsigned long long)(max - min) + 1ULL;
    unsigned long long limit = UINT64_MAX - (UINT64_MAX % range);
    unsigned long long r;
    do {
        ep_secure_random_bytes((unsigned char*)&r, sizeof(r));
    } while (r >= limit);
    return min + (long long)(r % range);
}

// JSON built-in functions
static const char* json_skip_ws(const char* p) {
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r') p++;
    return p;
}

static const char* json_skip_value(const char* p) {
    p = json_skip_ws(p);
    if (*p == '"') {
        p++;
        while (*p && *p != '"') { if (*p == '\\') p++; p++; }
        if (*p == '"') p++;
    } else if (*p == '{') {
        int depth = 1; p++;
        while (*p && depth > 0) {
            if (*p == '"') { p++; while (*p && *p != '"') { if (*p == '\\') p++; p++; } if (*p) p++; }
            else if (*p == '{') { depth++; p++; }
            else if (*p == '}') { depth--; p++; }
            else p++;
        }
    } else if (*p == '[') {
        int depth = 1; p++;
        while (*p && depth > 0) {
            if (*p == '"') { p++; while (*p && *p != '"') { if (*p == '\\') p++; p++; } if (*p) p++; }
            else if (*p == '[') { depth++; p++; }
            else if (*p == ']') { depth--; p++; }
            else p++;
        }
    } else {
        while (*p && *p != ',' && *p != '}' && *p != ']' && *p != ' ' && *p != '\n') p++;
    }
    return p;
}

static const char* json_find_key(const char* json, const char* key) {
    const char* p = json_skip_ws(json);
    if (*p != '{') return NULL;
    p++;
    while (*p) {
        p = json_skip_ws(p);
        if (*p == '}') return NULL;
        if (*p != '"') return NULL;
        p++;
        const char* ks = p;
        while (*p && *p != '"') { if (*p == '\\') p++; p++; }
        size_t klen = p - ks;
        if (*p == '"') p++;
        p = json_skip_ws(p);
        if (*p == ':') p++;
        p = json_skip_ws(p);
        if (klen == strlen(key) && strncmp(ks, key, klen) == 0) {
            return p;
        }
        p = json_skip_value(p);
        p = json_skip_ws(p);
        if (*p == ',') p++;
    }
    return NULL;
}

long long json_get_string(long long json_val, long long key_val) {
    const char* json = (const char*)json_val;
    const char* key = (const char*)key_val;
    if (!json || !key) return (long long)strdup("");
    const char* val = json_find_key(json, key);
    if (!val || *val != '"') return (long long)strdup("");
    val++;
    const char* end = val;
    while (*end && *end != '"') { if (*end == '\\') end++; end++; }
    size_t len = end - val;
    char* result = (char*)malloc(len + 1);
    // Handle escape sequences
    size_t di = 0;
    const char* si = val;
    while (si < end) {
        if (*si == '\\' && si + 1 < end) {
            si++;
            switch (*si) {
                case 'n': result[di++] = '\n'; break;
                case 't': result[di++] = '\t'; break;
                case 'r': result[di++] = '\r'; break;
                case '"': result[di++] = '"'; break;
                case '\\': result[di++] = '\\'; break;
                default: result[di++] = *si; break;
            }
        } else {
            result[di++] = *si;
        }
        si++;
    }
    result[di] = '\0';
    ep_gc_register(result, EP_OBJ_STRING);
    return (long long)result;
}

long long json_get_int(long long json_val, long long key_val) {
    const char* json = (const char*)json_val;
    const char* key = (const char*)key_val;
    if (!json || !key) return 0;
    const char* val = json_find_key(json, key);
    if (!val) return 0;
    return atoll(val);
}

long long json_get_bool(long long json_val, long long key_val) {
    const char* json = (const char*)json_val;
    const char* key = (const char*)key_val;
    if (!json || !key) return 0;
    const char* val = json_find_key(json, key);
    if (!val) return 0;
    if (strncmp(val, "true", 4) == 0) return 1;
    return 0;
}

// SHA-1 implementation (RFC 3174) for WebSocket handshake
static unsigned int sha1_left_rotate(unsigned int x, int n) {
    return (x << n) | (x >> (32 - n));
}

long long ep_sha1(long long data_val) {
    const unsigned char* data = (const unsigned char*)data_val;
    if (!data) return (long long)strdup("");
    size_t len = strlen((const char*)data);

    unsigned int h0 = 0x67452301, h1 = 0xEFCDAB89, h2 = 0x98BADCFE, h3 = 0x10325476, h4 = 0xC3D2E1F0;
    size_t new_len = len + 1;
    while (new_len % 64 != 56) new_len++;
    unsigned char* msg = (unsigned char*)calloc(new_len + 8, 1);
    memcpy(msg, data, len);
    msg[len] = 0x80;
    unsigned long long bits_len = (unsigned long long)len * 8;
    for (int i = 0; i < 8; i++) msg[new_len + 7 - i] = (unsigned char)(bits_len >> (i * 8));

    for (size_t offset = 0; offset < new_len + 8; offset += 64) {
        unsigned int w[80];
        for (int i = 0; i < 16; i++) {
            w[i] = ((unsigned int)msg[offset + i*4] << 24) | ((unsigned int)msg[offset + i*4+1] << 16) |
                    ((unsigned int)msg[offset + i*4+2] << 8) | (unsigned int)msg[offset + i*4+3];
        }
        for (int i = 16; i < 80; i++) w[i] = sha1_left_rotate(w[i-3] ^ w[i-8] ^ w[i-14] ^ w[i-16], 1);
        unsigned int a = h0, b = h1, c = h2, d = h3, e = h4;
        for (int i = 0; i < 80; i++) {
            unsigned int f, k;
            if (i < 20) { f = (b & c) | ((~b) & d); k = 0x5A827999; }
            else if (i < 40) { f = b ^ c ^ d; k = 0x6ED9EBA1; }
            else if (i < 60) { f = (b & c) | (b & d) | (c & d); k = 0x8F1BBCDC; }
            else { f = b ^ c ^ d; k = 0xCA62C1D6; }
            unsigned int temp = sha1_left_rotate(a, 5) + f + e + k + w[i];
            e = d; d = c; c = sha1_left_rotate(b, 30); b = a; a = temp;
        }
        h0 += a; h1 += b; h2 += c; h3 += d; h4 += e;
    }
    free(msg);

    // Return Base64-encoded hash directly (for WebSocket handshake)
    unsigned char hash[20];
    hash[0] = (h0>>24)&0xFF; hash[1] = (h0>>16)&0xFF; hash[2] = (h0>>8)&0xFF; hash[3] = h0&0xFF;
    hash[4] = (h1>>24)&0xFF; hash[5] = (h1>>16)&0xFF; hash[6] = (h1>>8)&0xFF; hash[7] = h1&0xFF;
    hash[8] = (h2>>24)&0xFF; hash[9] = (h2>>16)&0xFF; hash[10] = (h2>>8)&0xFF; hash[11] = h2&0xFF;
    hash[12] = (h3>>24)&0xFF; hash[13] = (h3>>16)&0xFF; hash[14] = (h3>>8)&0xFF; hash[15] = h3&0xFF;
    hash[16] = (h4>>24)&0xFF; hash[17] = (h4>>16)&0xFF; hash[18] = (h4>>8)&0xFF; hash[19] = h4&0xFF;

    // Base64 encode the 20-byte hash
    size_t b64_len = 4 * ((20 + 2) / 3);
    char* result = (char*)malloc(b64_len + 1);
    size_t j = 0;
    for (size_t bi = 0; bi < 20; bi += 3) {
        unsigned int n2 = ((unsigned int)hash[bi]) << 16;
        if (bi + 1 < 20) n2 |= ((unsigned int)hash[bi+1]) << 8;
        if (bi + 2 < 20) n2 |= (unsigned int)hash[bi+2];
        result[j++] = b64_table[(n2 >> 18) & 0x3F];
        result[j++] = b64_table[(n2 >> 12) & 0x3F];
        result[j++] = (bi + 1 < 20) ? b64_table[(n2 >> 6) & 0x3F] : '=';
        result[j++] = (bi + 2 < 20) ? b64_table[n2 & 0x3F] : '=';
    }
    result[j] = '\0';
    ep_gc_register(result, EP_OBJ_STRING);
    return (long long)result;
}

// Read exact N bytes from a socket
#ifdef __wasm__
long long ep_net_recv_bytes(long long fd, long long count) {
    (void)fd; (void)count;
    return (long long)strdup("");
}
#else
long long ep_net_recv_bytes(long long fd, long long count) {
    if (count <= 0) return (long long)strdup("");
    char* buf = (char*)malloc(count + 1);
#ifdef _WIN32
    int total = 0;
    while (total < (int)count) {
        int n = recv((int)fd, buf + total, (int)(count - total), 0);
        if (n <= 0) break;
        total += n;
    }
#else
    ssize_t total = 0;
    while (total < count) {
        ssize_t n = recv((int)fd, buf + total, count - total, 0);
        if (n <= 0) break;
        total += n;
    }
#endif
    buf[total] = '\0';
    ep_gc_register(buf, EP_OBJ_STRING);
    return (long long)buf;
}
#endif

long long ep_get_args(void) {
    long long list_ptr = create_list();
    for (int i = 0; i < ep_argc; i++) {
        char* arg_copy = strdup(ep_argv[i]);
        ep_gc_register(arg_copy, EP_OBJ_STRING);
        append_list(list_ptr, (long long)arg_copy);
    }
    return list_ptr;
}

/* Built-in: string concatenation */
long long concat(long long a, long long b) {
    const char* sa = (const char*)a;
    const char* sb = (const char*)b;
    long long la = strlen(sa);
    long long lb = strlen(sb);
    char* result = malloc(la + lb + 1);
    memcpy(result, sa, la);
    memcpy(result + la, sb, lb);
    result[la + lb] = '\0';
    ep_gc_register(result, EP_OBJ_STRING);
    return (long long)result;
}

long long int_to_string(long long val) {
    char* buf = malloc(32);
    snprintf(buf, 32, "%lld", val);
    ep_gc_register(buf, EP_OBJ_STRING);
    return (long long)buf;
}

long long ep_int_to_str(long long val) { return int_to_string(val); }

typedef struct { char* data; long long len; long long cap; } EpStringBuilder;

long long ep_sb_create(long long dummy) {
    (void)dummy;
    EpStringBuilder* sb = (EpStringBuilder*)malloc(sizeof(EpStringBuilder));
    sb->cap = 256;
    sb->len = 0;
    sb->data = (char*)malloc(sb->cap);
    sb->data[0] = '\0';
    return (long long)sb;
}

long long ep_sb_append(long long sb_ptr, long long str_ptr) {
    EpStringBuilder* sb = (EpStringBuilder*)sb_ptr;
    const char* s = (const char*)str_ptr;
    if (!s) return sb_ptr;
    long long slen = strlen(s);
    while (sb->len + slen + 1 > sb->cap) {
        sb->cap *= 2;
        sb->data = (char*)realloc(sb->data, sb->cap);
    }
    memcpy(sb->data + sb->len, s, slen);
    sb->len += slen;
    sb->data[sb->len] = '\0';
    return sb_ptr;
}

long long ep_sb_append_int(long long sb_ptr, long long val) {
    char buf[32];
    snprintf(buf, sizeof(buf), "%lld", val);
    return ep_sb_append(sb_ptr, (long long)buf);
}

long long ep_sb_to_string(long long sb_ptr) {
    EpStringBuilder* sb = (EpStringBuilder*)sb_ptr;
    char* result = (char*)malloc(sb->len + 1);
    memcpy(result, sb->data, sb->len + 1);
    ep_gc_register(result, EP_OBJ_STRING);
    free(sb->data);
    free(sb);
    return (long long)result;
}

long long ep_sb_length(long long sb_ptr) {
    return ((EpStringBuilder*)sb_ptr)->len;
}

long long str_to_ptr(long long s) { return s; }
long long ptr_to_str(long long p) {
    if (p == 0) return (long long)strdup("");
    char* copy = strdup((const char*)p);
    ep_gc_register(copy, EP_OBJ_STRING);
    return (long long)copy;
}

long long peek_byte(long long ptr, long long offset) {
    return (long long)((unsigned char*)ptr)[offset];
}
long long poke_byte(long long ptr, long long offset, long long value) {
    ((unsigned char*)ptr)[offset] = (unsigned char)value;
    return 0;
}
long long alloc_bytes(long long size) {
    return (long long)calloc((size_t)size, 1);
}
long long free_bytes(long long ptr) {
    free((void*)ptr);
    return 0;
}
long long list_to_bytes(long long list_ptr) {
    long long len = length_list(list_ptr);
    unsigned char* buf = (unsigned char*)malloc(len);
    for (long long i = 0; i < len; i++) {
        buf[i] = (unsigned char)get_list(list_ptr, i);
    }
    return (long long)buf;
}
long long bytes_to_list(long long ptr, long long len) {
    long long list = create_list();
    unsigned char* buf = (unsigned char*)ptr;
    for (long long i = 0; i < len; i++) {
        append_list(list, (long long)buf[i]);
    }
    return list;
}

long long ep_gc_get_minor_count() {
    return ep_gc_minor_count;
}
long long ep_gc_get_major_count() {
    return ep_gc_major_count;
}
long long ep_gc_get_nursery_count() {
    return ep_gc_nursery_count;
}

long long string_to_int(long long s) {
    if (s == 0) return 0;
    return atoll((const char*)s);
}

long long read_line() {
    char buf[4096];
    if (fgets(buf, sizeof(buf), stdin) == NULL) { buf[0] = '\0'; }
    size_t len = strlen(buf);
    if (len > 0 && buf[len-1] == '\n') buf[len-1] = '\0';
    char* result = strdup(buf);
    ep_gc_register(result, EP_OBJ_STRING);
    return (long long)result;
}

long long read_int() {
    long long val = 0;
    scanf("%lld", &val);
    while(getchar() != '\n');
    return val;
}

/* Read ONE key immediately: no echo, no waiting for Enter. Returns the key's
   number code (one byte at a time — escape sequences such as arrow keys
   arrive as successive codes), or -1 at end of input. When stdin is a pipe
   or a file (scripted tests), it simply reads the next byte. */
long long read_key() {
#if defined(__wasm__)
    return -1;
#elif defined(_WIN32)
    if (!_isatty(_fileno(stdin))) {
        return (long long)fgetc(stdin);
    }
    return (long long)_getch();
#else
    if (!isatty(STDIN_FILENO)) {
        return (long long)fgetc(stdin);
    }
    struct termios old_state, raw_state;
    if (tcgetattr(STDIN_FILENO, &old_state) != 0) {
        return (long long)fgetc(stdin);
    }
    raw_state = old_state;
    raw_state.c_lflag &= ~(ICANON | ECHO);
    raw_state.c_cc[VMIN] = 1;
    raw_state.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &raw_state);
    unsigned char ch = 0;
    long long got = (long long)read(STDIN_FILENO, &ch, 1);
    tcsetattr(STDIN_FILENO, TCSANOW, &old_state);
    if (got <= 0) return -1;
    return (long long)ch;
#endif
}

/* How wide the terminal window is, in characters. 80 when unknown. */
long long terminal_columns() {
#if defined(__wasm__)
    return 80;
#elif defined(_WIN32)
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info)) {
        long long cols = (long long)(info.srWindow.Right - info.srWindow.Left + 1);
        if (cols > 0) return cols;
    }
    return 80;
#else
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0 && ws.ws_col > 0) {
        return (long long)ws.ws_col;
    }
    return 80;
#endif
}

/* How tall the terminal window is, in lines. 24 when unknown. */
long long terminal_rows() {
#if defined(__wasm__)
    return 24;
#elif defined(_WIN32)
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info)) {
        long long rows = (long long)(info.srWindow.Bottom - info.srWindow.Top + 1);
        if (rows > 0) return rows;
    }
    return 24;
#else
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0 && ws.ws_row > 0) {
        return (long long)ws.ws_row;
    }
    return 24;
#endif
}

long long read_float() {
    double val = 0.0;
    scanf("%lf", &val);
    while(getchar() != '\n');
    long long result; memcpy(&result, &val, sizeof(double));
    return result;
}

long long int_to_float(long long val) {
    double d = (double)val;
    long long result; memcpy(&result, &d, sizeof(double));
    return result;
}

long long float_to_int(long long val) {
    double d; memcpy(&d, &val, sizeof(double));
    return (long long)d;
}

#define EP_STRUCT_MAX_SLOTS 8
static void __ep_mark_globals_major(void) {
}
static void __ep_mark_globals_minor(void) {
}
void __ep_init_constants(void) {
    ep_gc_mark_globals_major = __ep_mark_globals_major;
    ep_gc_mark_globals_minor = __ep_mark_globals_minor;
}

/* External Function Prototypes (FFI) */


/* User Function Prototypes */
long long get_file_stem(long long);
long long get_file_dir(long long);
long long contains_string(long long, long long);
long long resolve_import_path(long long, long long);
long long canonical_module_path(long long);
long long parse_all_modules(long long, long long, long long, long long, long long, long long, long long, long long, long long, long long);
long long _main();
long long create_token(long long, long long, long long, long long);
long long get_token_type(long long);
long long get_token_value(long long);
long long get_token_line(long long);
long long get_token_col(long long);
long long match_next_word(long long, long long, long long);
long long lex_string_body(long long, long long, long long, long long, long long, long long, long long);
long long lex_is_phrase2(long long, long long, long long, long long);
long long tokenize_source(long long);
long long parse_int(long long);
long long make_node_int(long long);
long long make_node_str(long long);
long long make_node_ident(long long);
long long make_node_binary(long long, long long, long long);
long long make_node_comp(long long, long long, long long);
long long make_node_call(long long, long long);
long long make_node_set(long long, long long);
long long make_node_return(long long);
long long make_node_display(long long);
long long make_node_if(long long, long long, long long);
long long make_node_repeat_while(long long, long long);
long long make_node_func(long long, long long, long long, long long);
long long make_node_program(long long, long long, long long, long long, long long, long long, long long, long long);
long long make_node_spawn(long long, long long);
long long make_node_send(long long, long long);
long long make_node_channel();
long long make_node_receive(long long);
long long make_node_external(long long, long long, long long);
long long make_node_borrow(long long);
long long make_node_await(long long);
long long make_node_logical(long long, long long, long long);
long long make_node_field_access(long long, long long);
long long make_node_field_set(long long, long long, long long);
long long make_node_struct_create(long long, long long);
long long make_node_method_call(long long, long long, long long);
long long make_node_enum_create(long long, long long);
long long make_node_match(long long, long long);
long long make_node_for_each(long long, long long, long long);
long long make_node_break();
long long make_node_continue();
long long make_node_bool(long long);
long long make_node_unary_not(long long);
long long make_node_try(long long);
long long make_node_closure(long long, long long);
long long make_node_list_lit(long long);
long long make_node_expr_stmt(long long);
long long make_node_struct_def(long long, long long);
long long make_node_enum_def(long long, long long);
long long make_node_method_def(long long, long long, long long, long long);
long long make_node_trait_def(long long, long long);
long long make_node_trait_impl(long long, long long, long long);
long long create_parser_state(long long);
long long get_in_condition(long long);
long long set_in_condition(long long, long long);
long long get_call_depth(long long);
long long enter_call_args(long long);
long long leave_call_args(long long);
long long set_parser_error(long long);
long long get_parser_error(long long);
long long get_state_tokens(long long);
long long get_state_pos(long long);
long long set_state_pos(long long, long long);
long long get_eof_token();
long long peek_token(long long);
long long peek_token_at(long long, long long);
long long advance_token(long long);
long long expect_token_type(long long, long long);
long long get_token_precedence(long long);
long long skip_newlines(long long);
long long is_uppercase_start(long long);
long long parse_param_list(long long);
long long parse_program(long long);
long long parse_struct_def(long long);
long long parse_enum_def(long long);
long long parse_method_def(long long);
long long parse_trait_def(long long);
long long parse_trait_impl(long long);
long long parse_function_async(long long, long long);
long long parse_block(long long);
long long parse_statement(long long);
long long parse_if_statement(long long);
long long parse_match_statement(long long);
long long parse_for_each_statement(long long);
long long parse_expr(long long, long long);
long long parse_prefix(long long);
long long parse_closure(long long);
long long parse_struct_create(long long);
long long parse_list_literal(long long);
long long check_lit_category(long long);
long long check_expr(long long, long long, long long);
long long check_stmts(long long, long long);
long long check_function(long long, long long);
long long en_arg_type(long long, long long, long long);
long long en_field_type_at(long long, long long, long long, long long);
long long en_type_conflict(long long, long long, long long, long long);
long long en_check_expr(long long, long long, long long, long long, long long, long long, long long);
long long en_check_stmts(long long, long long, long long, long long, long long, long long, long long, long long);
long long check_program(long long);
long long opt_fold_expr(long long);
long long opt_fold_stmts(long long);
long long optimize_program(long long);
long long map_get(long long, long long, long long);
long long map_contains_key(long long, long long);
long long collect_idents_expr(long long, long long);
long long collect_idents_stmts(long long, long long);
long long map_put(long long, long long, long long, long long);
long long field_slot_index(long long, long long);
long long string_concat(long long, long long);
long long cg_sanitize_name(long long);
long long contains_string_val(long long, long long);
long long get_fn_c_name(long long);
long long cg_int_to_str(long long);
long long escape_string(long long);
long long join_strings(long long);
long long create_codegen_state();
long long count_awaits_expr(long long);
long long count_awaits_stmts(long long);
long long emit_async_yields_expr(long long, long long, long long, long long);
long long emit_async_yields_stmt(long long, long long, long long, long long);
long long type_name_to_code(long long);
long long param_ann_to_code(long long);
long long seed_param_types(long long, long long, long long);
long long collect_prim_param_flags(long long, long long);
long long get_prim_param_flags(long long, long long);
long long cg_expr_has_var(long long, long long);
long long cg_is_prim_expr(long long, long long, long long);
long long cg_stmts_have_nonprim(long long, long long, long long);
long long usage_promote_call(long long, long long, long long, long long);
long long infer_usage_types_expr(long long, long long, long long);
long long infer_usage_types_stmts(long long, long long, long long);
long long is_builtin_c_func(long long, long long);
long long get_codegen_borrowed_keys(long long);
long long set_codegen_borrowed_keys(long long, long long);
long long get_codegen_borrowed_values(long long);
long long set_codegen_borrowed_values(long long, long long);
long long get_codegen_spawn_list(long long);
long long set_codegen_spawn_list(long long, long long);
long long get_codegen_spawn_index(long long);
long long set_codegen_spawn_index(long long, long long);
long long emit(long long, long long);
long long add_string_literal(long long, long long);
long long get_new_label(long long, long long);
long long analyze_return_types(long long, long long);
long long collect_var_types(long long, long long, long long, long long);
long long determine_ret_type(long long, long long, long long, long long);
long long infer_type(long long, long long, long long, long long);
long long is_global_var(long long);
long long cg_string_contains(long long, long long);
long long str_starts_with(long long, long long);
long long str_ends_with(long long, long long);
long long is_accessor_name(long long);
long long is_borrow_expr(long long, long long, long long);
long long scan_stmts_for_borrows(long long, long long, long long);
long long collect_borrowed_vars(long long, long long, long long, long long);
long long var_returned_in_stmts(long long, long long);
long long gen_function(long long, long long);
long long gen_statement(long long, long long, long long, long long);
long long gen_expr(long long, long long, long long, long long);
long long get_c_runtime_source();
long long get_c_main_source();
long long get_c_test_main_source(long long);
long long collect_spawns_in_stmts(long long, long long);
long long collect_all_spawns(long long);
long long clone_list(long long);
long long check_expr_reads(long long, long long, long long, long long, long long);
long long dec_borrow_count(long long, long long, long long);
long long inc_borrow_count(long long, long long, long long);
long long check_safety_stmts(long long, long long, long long, long long, long long, long long, long long, long long, long long, long long);
long long analyze_safety(long long, long long);
long long validate_program_safety(long long);
long long generate_c(long long, long long);
long long ep_rt_core_0();
long long ep_rt_core_1();
long long ep_rt_core_2();
long long ep_rt_core_3();
long long ep_rt_core_4();
long long ep_rt_core_5();
long long ep_rt_core_6();
long long ep_rt_core_7();
long long ep_rt_core_8();
long long ep_rt_core_9();
long long ep_rt_core_10();
long long ep_rt_core_11();
long long ep_rt_core_12();
long long ep_rt_core_13();
long long ep_rt_core_14();
long long ep_rt_core_15();
long long ep_rt_core_16();
long long ep_rt_core_17();
long long ep_rt_core_18();
long long ep_rt_core_19();
long long ep_rt_core_20();
long long ep_rt_core_21();
long long ep_rt_core_22();
long long ep_rt_core_23();
long long ep_rt_core_24();
long long ep_rt_core_25();
long long ep_rt_core_26();
long long ep_rt_core_27();
long long ep_rt_core_28();
long long ep_rt_core_29();
long long ep_rt_core_30();
long long ep_rt_core_31();
long long ep_rt_core_32();
long long ep_rt_builtins_0();
long long ep_rt_builtins_1();
long long get_shared_runtime_source();


/* Thread Spawn Wrappers */


/* EP_CLOSURE_BODIES */
long long get_file_stem(long long path) {
    long long len = 0;
    long long last_slash = 0;
    long long idx = 0;
    long long ch = 0;
    long long start = 0;
    long long dot_pos = 0;
    long long idx2 = 0;
    long long stem_len = 0;
    long long stem = 0;
    long long ret_val = 0;

    ep_gc_push_root(&idx);
    ep_gc_push_root(&start);
    ep_gc_push_root(&idx2);
    ep_gc_push_root(&stem_len);
    ep_gc_push_root(&stem);
    ep_gc_push_root(&path);
    ep_gc_maybe_collect();

    len = string_length((char*)path);
    last_slash = -1LL;
    idx = 0LL;
    while (idx < len) {
    ch = get_character((char*)path, idx);
    if (ch == 47LL) {
    last_slash = idx;
    }
    idx = (idx + 1LL);
    }
    start = (last_slash + 1LL);
    dot_pos = len;
    idx2 = start;
    while (idx2 < len) {
    ch = get_character((char*)path, idx2);
    if (ch == 46LL) {
    dot_pos = idx2;
    }
    idx2 = (idx2 + 1LL);
    }
    stem_len = (dot_pos - start);
    stem = (long long)substring((char*)path, start, stem_len);
    ret_val = stem;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(6);
    return ret_val;
}

long long get_file_dir(long long path) {
    long long len = 0;
    long long last_slash = 0;
    long long idx = 0;
    long long ch = 0;
    long long ret_val = 0;

    ep_gc_push_root(&last_slash);
    ep_gc_push_root(&idx);
    ep_gc_push_root(&path);
    ep_gc_maybe_collect();

    len = string_length((char*)path);
    last_slash = -1LL;
    idx = 0LL;
    while (idx < len) {
    ch = get_character((char*)path, idx);
    if (ch == 47LL) {
    last_slash = idx;
    }
    idx = (idx + 1LL);
    }
    if (last_slash < 0LL) {
    ret_val = (long long)"./";
    goto L_cleanup;
    }
    ret_val = (long long)substring((char*)path, 0LL, (last_slash + 1LL));
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(3);
    return ret_val;
}

long long contains_string(long long list, long long s) {
    long long len = 0;
    long long idx = 0;
    long long item = 0;
    long long ret_val = 0;

    ep_gc_push_root(&idx);
    ep_gc_push_root(&list);
    ep_gc_push_root(&s);
    ep_gc_maybe_collect();

    len = length_list(list);
    idx = 0LL;
    while (idx < len) {
    item = get_list(list, idx);
    if ((strcmp((char*)string_concat(s, (long long)""), (char*)item) == 0)) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    idx = (idx + 1LL);
    }
    ret_val = 0LL;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(3);
    return ret_val;
}

long long resolve_import_path(long long current_file, long long import_path) {
    long long p = 0;
    long long is_std = 0;
    long long std_path = 0;
    long long std_path_ep = 0;
    long long dir = 0;
    long long resolved = 0;
    long long len = 0;
    long long ext = 0;
    long long ret_val = 0;

    ep_gc_push_root(&p);
    ep_gc_push_root(&std_path);
    ep_gc_push_root(&std_path_ep);
    ep_gc_push_root(&dir);
    ep_gc_push_root(&resolved);
    ep_gc_push_root(&len);
    ep_gc_push_root(&ext);
    ep_gc_push_root(&current_file);
    ep_gc_push_root(&import_path);
    ep_gc_maybe_collect();

    p = string_concat(import_path, (long long)"");
    is_std = 0LL;
    if ((((((((((((strcmp((char*)(long long)"math", (char*)p) == 0) || (strcmp((char*)(long long)"hash", (char*)p) == 0)) || (strcmp((char*)(long long)"net", (char*)p) == 0)) || (strcmp((char*)(long long)"json", (char*)p) == 0)) || (strcmp((char*)(long long)"string", (char*)p) == 0)) || (strcmp((char*)(long long)"sql", (char*)p) == 0)) || (strcmp((char*)(long long)"gui", (char*)p) == 0)) || (strcmp((char*)(long long)"crypto", (char*)p) == 0)) || (strcmp((char*)(long long)"fs", (char*)p) == 0)) || (strcmp((char*)(long long)"http", (char*)p) == 0)) || (strcmp((char*)(long long)"collections", (char*)p) == 0))) {
    is_std = 1LL;
    }
    if ((((((((((((((strcmp((char*)(long long)"sort", (char*)p) == 0) || (strcmp((char*)(long long)"datetime", (char*)p) == 0)) || (strcmp((char*)(long long)"os", (char*)p) == 0)) || (strcmp((char*)(long long)"test", (char*)p) == 0)) || (strcmp((char*)(long long)"log", (char*)p) == 0)) || (strcmp((char*)(long long)"sync", (char*)p) == 0)) || (strcmp((char*)(long long)"regex", (char*)p) == 0)) || (strcmp((char*)(long long)"csv", (char*)p) == 0)) || (strcmp((char*)(long long)"websocket", (char*)p) == 0)) || (strcmp((char*)(long long)"static_server", (char*)p) == 0)) || (strcmp((char*)(long long)"toml", (char*)p) == 0)) || (strcmp((char*)(long long)"select", (char*)p) == 0)) || (strcmp((char*)(long long)"structured", (char*)p) == 0))) {
    is_std = 1LL;
    }
    if (is_std == 1LL) {
    std_path = string_concat((long long)"stdlib/", import_path);
    std_path_ep = string_concat(std_path, (long long)".ep");
    ret_val = std_path_ep;
    goto L_cleanup;
    }
    dir = get_file_dir(current_file);
    resolved = string_concat(dir, import_path);
    len = string_length((char*)resolved);
    if (len > 3LL) {
    ext = (long long)substring((char*)resolved, (len - 3LL), 3LL);
    if ((strcmp((char*)(long long)".ep", (char*)ext) == 0)) {
    ret_val = resolved;
    goto L_cleanup;
    }
    }
    ret_val = string_concat(resolved, (long long)".ep");
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(9);
    return ret_val;
}

long long canonical_module_path(long long path) {
    long long library = 0;
    long long resolve_function = 0;
    long long free_function = 0;
    long long closed = 0;
    long long resolved = 0;
    long long canonical = 0;
    long long released = 0;
    long long ret_val = 0;

    ep_gc_push_root(&library);
    ep_gc_push_root(&resolve_function);
    ep_gc_push_root(&free_function);
    ep_gc_push_root(&resolved);
    ep_gc_push_root(&canonical);
    ep_gc_push_root(&path);
    ep_gc_maybe_collect();

    library = ep_dlopen((long long)"/usr/lib/libSystem.B.dylib");
    if (library == 0LL) {
    library = ep_dlopen((long long)"libc.so.6");
    }
    if (library == 0LL) {
    printf("%s\n", (char*)(long long)"Compiler Error: Cannot load the native path resolver.");
    ret_val = (long long)"";
    goto L_cleanup;
    }
    resolve_function = ep_dlsym(library, (long long)"realpath");
    free_function = ep_dlsym(library, (long long)"free");
    if ((resolve_function == 0LL || free_function == 0LL)) {
    closed = ep_dlclose(library);
    ret_val = (long long)"";
    goto L_cleanup;
    }
    resolved = ep_dlcall2(resolve_function, str_to_ptr(path), 0LL);
    if (resolved == 0LL) {
    closed = ep_dlclose(library);
    ret_val = (long long)"";
    goto L_cleanup;
    }
    canonical = string_concat(ptr_to_str(resolved), (long long)"");
    released = ep_dlcall1(free_function, resolved);
    closed = ep_dlclose(library);
    ret_val = canonical;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(6);
    return ret_val;
}

long long parse_all_modules(long long current_file, long long parsed_files, long long all_functions, long long all_externals, long long all_struct_defs, long long all_enum_defs, long long all_method_defs, long long all_trait_defs, long long all_trait_impls, long long all_constants) {
    long long original_file = 0;
    long long shown = 0;
    long long has_parsed = 0;
    long long ok = 0;
    long long source = 0;
    long long tokens = 0;
    long long state = 0;
    long long program_ast = 0;
    long long imports = 0;
    long long externals = 0;
    long long funcs = 0;
    long long externals_len = 0;
    long long e_idx = 0;
    long long ext = 0;
    long long funcs_len = 0;
    long long f_idx = 0;
    long long func = 0;
    long long prog_len = 0;
    long long sd = 0;
    long long sd_len = 0;
    long long sd_idx = 0;
    long long ed = 0;
    long long ed_len = 0;
    long long ed_idx = 0;
    long long md = 0;
    long long md_len = 0;
    long long md_idx = 0;
    long long td = 0;
    long long td_len = 0;
    long long td_idx = 0;
    long long ti = 0;
    long long ti_len = 0;
    long long ti_idx = 0;
    long long tc = 0;
    long long tc_len = 0;
    long long tc_idx = 0;
    long long imp_len = 0;
    long long i_idx = 0;
    long long imp_pair = 0;
    long long imp = 0;
    long long imp_alias = 0;
    long long resolved_path = 0;
    long long status = 0;
    long long mod_funcs = 0;
    long long mod_externals = 0;
    long long mf_len = 0;
    long long mf_i = 0;
    long long mfunc = 0;
    long long acopy = 0;
    long long mfl = 0;
    long long mc_i = 0;
    long long ok2 = 0;
    long long aname = 0;
    long long ok3 = 0;
    long long ok4 = 0;
    long long me_len = 0;
    long long me_i = 0;
    long long mext = 0;
    long long ok5 = 0;
    long long ecopy = 0;
    long long mel = 0;
    long long me_ci = 0;
    long long ok6 = 0;
    long long ename = 0;
    long long ok7 = 0;
    long long ok8 = 0;
    long long ret_val = 0;

    ep_gc_push_root(&original_file);
    ep_gc_push_root(&source);
    ep_gc_push_root(&tokens);
    ep_gc_push_root(&state);
    ep_gc_push_root(&program_ast);
    ep_gc_push_root(&imports);
    ep_gc_push_root(&externals);
    ep_gc_push_root(&funcs);
    ep_gc_push_root(&e_idx);
    ep_gc_push_root(&ext);
    ep_gc_push_root(&f_idx);
    ep_gc_push_root(&func);
    ep_gc_push_root(&sd);
    ep_gc_push_root(&sd_idx);
    ep_gc_push_root(&ed);
    ep_gc_push_root(&ed_idx);
    ep_gc_push_root(&md);
    ep_gc_push_root(&md_idx);
    ep_gc_push_root(&td);
    ep_gc_push_root(&td_idx);
    ep_gc_push_root(&ti);
    ep_gc_push_root(&ti_idx);
    ep_gc_push_root(&tc);
    ep_gc_push_root(&tc_idx);
    ep_gc_push_root(&i_idx);
    ep_gc_push_root(&imp_pair);
    ep_gc_push_root(&imp);
    ep_gc_push_root(&imp_alias);
    ep_gc_push_root(&resolved_path);
    ep_gc_push_root(&mod_funcs);
    ep_gc_push_root(&mod_externals);
    ep_gc_push_root(&mf_i);
    ep_gc_push_root(&mfunc);
    ep_gc_push_root(&acopy);
    ep_gc_push_root(&mc_i);
    ep_gc_push_root(&aname);
    ep_gc_push_root(&me_i);
    ep_gc_push_root(&mext);
    ep_gc_push_root(&ecopy);
    ep_gc_push_root(&me_ci);
    ep_gc_push_root(&ename);
    ep_gc_push_root(&current_file);
    ep_gc_push_root(&parsed_files);
    ep_gc_push_root(&all_functions);
    ep_gc_push_root(&all_externals);
    ep_gc_push_root(&all_struct_defs);
    ep_gc_push_root(&all_enum_defs);
    ep_gc_push_root(&all_method_defs);
    ep_gc_push_root(&all_trait_defs);
    ep_gc_push_root(&all_trait_impls);
    ep_gc_push_root(&all_constants);
    ep_gc_maybe_collect();

    original_file = current_file;
    current_file = canonical_module_path(current_file);
    if (string_length((char*)current_file) == 0LL) {
    printf("%s\n", (char*)(long long)"Compiler Error: Cannot resolve source file:");
    shown = display_string((char*)original_file);
    ret_val = 1LL;
    goto L_cleanup;
    }
    has_parsed = contains_string(parsed_files, current_file);
    if (has_parsed == 1LL) {
    ret_val = 0LL;
    goto L_cleanup;
    }
    ok = append_list(parsed_files, current_file);
    source = (long long)read_file_content((char*)current_file);
    if (string_length((char*)source) == 0LL) {
    printf("%s\n", (char*)(long long)"Compiler Error: Failed to read file or file is empty:");
    ok = display_string((char*)current_file);
    ret_val = 1LL;
    goto L_cleanup;
    }
    tokens = tokenize_source(source);
    if (tokens == 0LL) {
    printf("%s\n", (char*)(long long)"Compiler Error: Lexing failed (unterminated string or invalid token) in:");
    ok = display_string((char*)current_file);
    ret_val = 1LL;
    goto L_cleanup;
    }
    state = create_parser_state(tokens);
    program_ast = parse_program(state);
    if (get_parser_error(state) == 1LL) {
    printf("%s\n", (char*)(long long)"Compiler Error: Parsing failed in:");
    ok = display_string((char*)current_file);
    ret_val = 1LL;
    goto L_cleanup;
    }
    imports = get_list(program_ast, 1LL);
    externals = get_list(program_ast, 2LL);
    funcs = get_list(program_ast, 3LL);
    externals_len = length_list(externals);
    e_idx = 0LL;
    while (e_idx < externals_len) {
    ext = get_list(externals, e_idx);
    ok = append_list(all_externals, ext);
    e_idx = (e_idx + 1LL);
    }
    funcs_len = length_list(funcs);
    f_idx = 0LL;
    while (f_idx < funcs_len) {
    func = get_list(funcs, f_idx);
    ok = append_list(all_functions, func);
    f_idx = (f_idx + 1LL);
    }
    prog_len = length_list(program_ast);
    if (prog_len > 4LL) {
    sd = get_list(program_ast, 4LL);
    sd_len = length_list(sd);
    sd_idx = 0LL;
    while (sd_idx < sd_len) {
    ok = append_list(all_struct_defs, get_list(sd, sd_idx));
    sd_idx = (sd_idx + 1LL);
    }
    }
    if (prog_len > 5LL) {
    ed = get_list(program_ast, 5LL);
    ed_len = length_list(ed);
    ed_idx = 0LL;
    while (ed_idx < ed_len) {
    ok = append_list(all_enum_defs, get_list(ed, ed_idx));
    ed_idx = (ed_idx + 1LL);
    }
    }
    if (prog_len > 6LL) {
    md = get_list(program_ast, 6LL);
    md_len = length_list(md);
    md_idx = 0LL;
    while (md_idx < md_len) {
    ok = append_list(all_method_defs, get_list(md, md_idx));
    md_idx = (md_idx + 1LL);
    }
    }
    if (prog_len > 7LL) {
    td = get_list(program_ast, 7LL);
    td_len = length_list(td);
    td_idx = 0LL;
    while (td_idx < td_len) {
    ok = append_list(all_trait_defs, get_list(td, td_idx));
    td_idx = (td_idx + 1LL);
    }
    }
    if (prog_len > 8LL) {
    ti = get_list(program_ast, 8LL);
    ti_len = length_list(ti);
    ti_idx = 0LL;
    while (ti_idx < ti_len) {
    ok = append_list(all_trait_impls, get_list(ti, ti_idx));
    ti_idx = (ti_idx + 1LL);
    }
    }
    if (prog_len > 9LL) {
    tc = get_list(program_ast, 9LL);
    tc_len = length_list(tc);
    tc_idx = 0LL;
    while (tc_idx < tc_len) {
    ok = append_list(all_constants, get_list(tc, tc_idx));
    tc_idx = (tc_idx + 1LL);
    }
    }
    imp_len = length_list(imports);
    i_idx = 0LL;
    while (i_idx < imp_len) {
    imp_pair = get_list(imports, i_idx);
    imp = get_list(imp_pair, 0LL);
    imp_alias = string_concat(get_list(imp_pair, 1LL), (long long)"");
    resolved_path = resolve_import_path(current_file, imp);
    if (string_length((char*)imp_alias) == 0LL) {
    status = parse_all_modules(resolved_path, parsed_files, all_functions, all_externals, all_struct_defs, all_enum_defs, all_method_defs, all_trait_defs, all_trait_impls, all_constants);
    if (status != 0LL) {
    ret_val = status;
    goto L_cleanup;
    }
    } else {
    mod_funcs = create_list();
    mod_externals = create_list();
    status = parse_all_modules(resolved_path, parsed_files, mod_funcs, mod_externals, all_struct_defs, all_enum_defs, all_method_defs, all_trait_defs, all_trait_impls, all_constants);
    if (status != 0LL) {
    ret_val = status;
    goto L_cleanup;
    }
    mf_len = length_list(mod_funcs);
    mf_i = 0LL;
    while (mf_i < mf_len) {
    mfunc = get_list(mod_funcs, mf_i);
    ok = append_list(all_functions, mfunc);
    acopy = (create_list() + 0LL);
    mfl = length_list(mfunc);
    mc_i = 0LL;
    while (mc_i < mfl) {
    ok2 = append_list(acopy, get_list(mfunc, mc_i));
    mc_i = (mc_i + 1LL);
    }
    aname = string_concat(imp_alias, (long long)"_");
    aname = string_concat(aname, get_list(mfunc, 1LL));
    ok3 = set_list(acopy, 1LL, aname);
    ok4 = append_list(all_functions, acopy);
    mf_i = (mf_i + 1LL);
    }
    me_len = length_list(mod_externals);
    me_i = 0LL;
    while (me_i < me_len) {
    mext = get_list(mod_externals, me_i);
    ok5 = append_list(all_externals, mext);
    ecopy = (create_list() + 0LL);
    mel = length_list(mext);
    me_ci = 0LL;
    while (me_ci < mel) {
    ok6 = append_list(ecopy, get_list(mext, me_ci));
    me_ci = (me_ci + 1LL);
    }
    ename = string_concat(imp_alias, (long long)"_");
    ename = string_concat(ename, get_list(mext, 1LL));
    ok7 = set_list(ecopy, 1LL, ename);
    ok8 = append_list(all_externals, ecopy);
    me_i = (me_i + 1LL);
    }
    }
    i_idx = (i_idx + 1LL);
    }
    ret_val = 0LL;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(51);
    return ret_val;
}

long long _main() {
    long long arg_count = 0;
    long long first_arg = 0;
    long long is_test_mode = 0;
    long long check_only = 0;
    long long input_path = 0;
    long long stem = 0;
    long long all_functions = 0;
    long long all_externals = 0;
    long long all_struct_defs = 0;
    long long all_enum_defs = 0;
    long long all_method_defs = 0;
    long long all_trait_defs = 0;
    long long all_trait_impls = 0;
    long long all_constants = 0;
    long long parsed_files = 0;
    long long status = 0;
    long long f_names = 0;
    long long all_len = 0;
    long long idx = 0;
    long long duplicate_found = 0;
    long long func = 0;
    long long name = 0;
    long long ok = 0;
    long long empty_imports = 0;
    long long program_ast = 0;
    long long check_ok = 0;
    long long safety_ok = 0;
    long long opt_ok = 0;
    long long c_code = 0;
    long long c_path = 0;
    long long c_compiler = 0;
    long long compile_cmd = 0;
    long long pf_len = 0;
    long long pf_idx = 0;
    long long pf = 0;
    long long pf_str = 0;
    long long pf_len_str = 0;
    long long ext_sql = 0;
    long long ext_crypto = 0;
    long long ext_gui = 0;
    long long ext_cry = 0;
    long long ret_val = 0;

    ep_gc_push_root(&first_arg);
    ep_gc_push_root(&is_test_mode);
    ep_gc_push_root(&input_path);
    ep_gc_push_root(&stem);
    ep_gc_push_root(&all_functions);
    ep_gc_push_root(&all_externals);
    ep_gc_push_root(&all_struct_defs);
    ep_gc_push_root(&all_enum_defs);
    ep_gc_push_root(&all_method_defs);
    ep_gc_push_root(&all_trait_defs);
    ep_gc_push_root(&all_trait_impls);
    ep_gc_push_root(&all_constants);
    ep_gc_push_root(&parsed_files);
    ep_gc_push_root(&f_names);
    ep_gc_push_root(&idx);
    ep_gc_push_root(&func);
    ep_gc_push_root(&name);
    ep_gc_push_root(&empty_imports);
    ep_gc_push_root(&program_ast);
    ep_gc_push_root(&c_code);
    ep_gc_push_root(&c_path);
    ep_gc_push_root(&c_compiler);
    ep_gc_push_root(&compile_cmd);
    ep_gc_push_root(&pf_idx);
    ep_gc_push_root(&pf);
    ep_gc_push_root(&pf_str);
    ep_gc_push_root(&pf_len_str);
    ep_gc_push_root(&ext_sql);
    ep_gc_push_root(&ext_crypto);
    ep_gc_push_root(&ext_gui);
    ep_gc_push_root(&ext_cry);
    ep_gc_maybe_collect();

    arg_count = get_argument_count();
    if (arg_count < 2LL) {
    printf("%s\n", (char*)(long long)"Usage: epc <filename.ep> or epc test <filename.ep>");
    ret_val = 1LL;
    goto L_cleanup;
    }
    first_arg = (long long)get_argument(1LL);
    is_test_mode = 0LL;
    check_only = 0LL;
    input_path = (long long)get_argument(1LL);
    if ((strcmp((char*)(long long)"test", (char*)first_arg) == 0)) {
    if (arg_count < 3LL) {
    printf("%s\n", (char*)(long long)"Usage: epc test <filename.ep>");
    ret_val = 1LL;
    goto L_cleanup;
    }
    is_test_mode = 1LL;
    input_path = (long long)get_argument(2LL);
    }
    if ((strcmp((char*)(long long)"check", (char*)first_arg) == 0)) {
    if (arg_count < 3LL) {
    printf("%s\n", (char*)(long long)"Usage: epc check <filename.ep>");
    ret_val = 1LL;
    goto L_cleanup;
    }
    check_only = 1LL;
    input_path = (long long)get_argument(2LL);
    }
    stem = get_file_stem(input_path);
    printf("%s\n", (char*)(long long)"[1/3] Tokenizing and Parsing...");
    all_functions = create_list();
    all_externals = create_list();
    all_struct_defs = create_list();
    all_enum_defs = create_list();
    all_method_defs = create_list();
    all_trait_defs = create_list();
    all_trait_impls = create_list();
    all_constants = create_list();
    parsed_files = create_list();
    status = parse_all_modules(input_path, parsed_files, all_functions, all_externals, all_struct_defs, all_enum_defs, all_method_defs, all_trait_defs, all_trait_impls, all_constants);
    if (status != 0LL) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    f_names = create_list();
    all_len = length_list(all_functions);
    idx = 0LL;
    duplicate_found = 0LL;
    while ((idx < all_len && duplicate_found == 0LL)) {
    func = get_list(all_functions, idx);
    name = get_list(func, 1LL);
    if (contains_string(f_names, name) == 1LL) {
    printf("%s\n", (char*)(long long)"Compiler Error: Function is defined multiple times:");
    ok = display_string((char*)name);
    duplicate_found = 1LL;
    } else {
    ok = append_list(f_names, name);
    }
    idx = (idx + 1LL);
    }
    if (duplicate_found == 1LL) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    empty_imports = create_list();
    program_ast = create_list();
    ok = append_list(program_ast, 13LL);
    ok = append_list(program_ast, empty_imports);
    ok = append_list(program_ast, all_externals);
    ok = append_list(program_ast, all_functions);
    ok = append_list(program_ast, all_struct_defs);
    ok = append_list(program_ast, all_enum_defs);
    ok = append_list(program_ast, all_method_defs);
    ok = append_list(program_ast, all_trait_defs);
    ok = append_list(program_ast, all_trait_impls);
    ok = append_list(program_ast, all_constants);
    check_ok = check_program(program_ast);
    if (check_ok == 0LL) {
    printf("%s\n", (char*)(long long)"Compilation failed: semantic errors.");
    ret_val = 1LL;
    goto L_cleanup;
    }
    if (check_only == 1LL) {
    safety_ok = validate_program_safety(program_ast);
    if (safety_ok == 0LL) {
    printf("%s\n", (char*)(long long)"Compilation failed: ownership/safety errors.");
    ret_val = 1LL;
    goto L_cleanup;
    }
    printf("%s\n", (char*)(long long)"Check passed: no errors.");
    ret_val = 0LL;
    goto L_cleanup;
    }
    opt_ok = optimize_program(program_ast);
    printf("%s\n", (char*)(long long)"[2/3] Generating C Source...");
    c_code = generate_c(program_ast, is_test_mode);
    if (string_length((char*)c_code) == 0LL) {
    printf("%s\n", (char*)(long long)"Compilation failed due to errors.");
    ret_val = 1LL;
    goto L_cleanup;
    }
    c_path = string_concat(stem, (long long)"_compiled.c");
    ok = write_file_content((char*)c_path, (char*)c_code);
    c_compiler = (long long)"clang";
    if (ep_system((long long)"command -v clang >/dev/null 2>&1") != 0LL) {
    c_compiler = (long long)"gcc";
    }
    printf("%s\n", (char*)concat((long long)"[3/3] Compiling and Linking via ", concat(ep_auto_to_string(c_compiler), (long long)"...")));
    compile_cmd = string_concat(c_compiler, (long long)" ");
    compile_cmd = string_concat(compile_cmd, c_path);
    compile_cmd = string_concat(compile_cmd, (long long)" -o ");
    compile_cmd = string_concat(compile_cmd, stem);
    compile_cmd = string_concat(compile_cmd, (long long)" -lpthread");
    pf_len = length_list(parsed_files);
    pf_idx = 0LL;
    while (pf_idx < pf_len) {
    pf = get_list(parsed_files, pf_idx);
    pf_str = string_concat(pf, (long long)"");
    pf_len_str = string_length((char*)pf_str);
    if (pf_len_str > 5LL) {
    ext_sql = (long long)substring((char*)pf_str, (pf_len_str - 6LL), 6LL);
    if ((strcmp((char*)(long long)"sql.ep", (char*)ext_sql) == 0)) {
    compile_cmd = string_concat(compile_cmd, (long long)" -DEP_HAS_SQLITE -lsqlite3");
    }
    }
    if (pf_len_str > 8LL) {
    ext_crypto = (long long)substring((char*)pf_str, (pf_len_str - 9LL), 9LL);
    if ((strcmp((char*)(long long)"crypto.ep", (char*)ext_crypto) == 0)) {
    compile_cmd = string_concat(compile_cmd, (long long)" -L/opt/homebrew/opt/openssl/lib -lcrypto");
    }
    }
    if (pf_len_str > 5LL) {
    ext_gui = (long long)substring((char*)pf_str, (pf_len_str - 6LL), 6LL);
    if ((strcmp((char*)(long long)"gui.ep", (char*)ext_gui) == 0)) {
    compile_cmd = string_concat(compile_cmd, (long long)" -lraylib");
    }
    }
    if (pf_len_str > 8LL) {
    ext_cry = (long long)substring((char*)pf_str, (pf_len_str - 9LL), 9LL);
    if ((strcmp((char*)(long long)"crypto.ep", (char*)ext_cry) == 0)) {
    compile_cmd = string_concat(compile_cmd, (long long)" -L/opt/homebrew/opt/openssl/lib -lcrypto");
    }
    }
    pf_idx = (pf_idx + 1LL);
    }
    status = run_command((char*)compile_cmd);
    if (status == 0LL) {
    printf("%s\n", (char*)(long long)"Self-hosted compilation successful!");
    ret_val = 0LL;
    goto L_cleanup;
    } else {
    printf("%s\n", (char*)(long long)"Compilation failed.");
    ret_val = 1LL;
    goto L_cleanup;
    }
L_cleanup:
    ep_gc_pop_roots(31);
    return ret_val;
}

long long create_token(long long type, long long value, long long line, long long col) {
    long long tok = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&tok);
    ep_gc_push_root(&type);
    ep_gc_push_root(&value);
    ep_gc_push_root(&line);
    ep_gc_push_root(&col);
    ep_gc_maybe_collect();

    tok = create_list();
    ok = append_list(tok, type);
    ok = append_list(tok, value);
    ok = append_list(tok, line);
    ok = append_list(tok, col);
    ret_val = tok;
    tok = 0;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(5);
    return ret_val;
}

long long get_token_type(long long tok) {
    long long ret_val = 0;

    ep_gc_push_root(&tok);
    ep_gc_maybe_collect();

    ret_val = get_list(tok, 0LL);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(1);
    return ret_val;
}

long long get_token_value(long long tok) {
    long long ret_val = 0;

    ep_gc_push_root(&tok);
    ep_gc_maybe_collect();

    ret_val = get_list(tok, 1LL);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(1);
    return ret_val;
}

long long get_token_line(long long tok) {
    long long ret_val = 0;

    ep_gc_push_root(&tok);
    ep_gc_maybe_collect();

    ret_val = get_list(tok, 2LL);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(1);
    return ret_val;
}

long long get_token_col(long long tok) {
    long long ret_val = 0;

    ep_gc_push_root(&tok);
    ep_gc_maybe_collect();

    ret_val = get_list(tok, 3LL);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(1);
    return ret_val;
}

long long match_next_word(long long source, long long start_pos, long long next_word) {
    long long p = 0;
    long long s_len = 0;
    long long loop = 0;
    long long ch = 0;
    long long nw_len = 0;
    long long idx = 0;
    long long matches = 0;
    long long ch1 = 0;
    long long ch2 = 0;
    long long next_ch = 0;
    long long is_id_part = 0;
    long long ret_val = 0;

    ep_gc_push_root(&p);
    ep_gc_push_root(&nw_len);
    ep_gc_push_root(&idx);
    ep_gc_push_root(&source);
    ep_gc_push_root(&next_word);
    ep_gc_maybe_collect();

    p = start_pos;
    s_len = string_length((char*)source);
    loop = 1LL;
    while ((p < s_len && loop == 1LL)) {
    ch = get_character((char*)source, p);
    if ((ch == 32LL || ch == 9LL)) {
    p = (p + 1LL);
    } else {
    loop = 0LL;
    }
    }
    nw_len = string_length((char*)next_word);
    if ((p + nw_len) > s_len) {
    ret_val = 0LL;
    goto L_cleanup;
    }
    idx = 0LL;
    matches = 1LL;
    while ((idx < nw_len && matches == 1LL)) {
    ch1 = get_character((char*)source, (p + idx));
    ch2 = get_character((char*)next_word, idx);
    if (ch1 != ch2) {
    matches = 0LL;
    }
    idx = (idx + 1LL);
    }
    if (matches == 1LL) {
    next_ch = get_character((char*)source, (p + nw_len));
    is_id_part = ((((next_ch > 96LL && next_ch < 123LL) || (next_ch > 64LL && next_ch < 91LL)) || (next_ch > 47LL && next_ch < 58LL)) || next_ch == 95LL);
    if (is_id_part == 0LL) {
    ret_val = (p + nw_len);
    goto L_cleanup;
    }
    }
    ret_val = 0LL;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(5);
    return ret_val;
}

long long lex_string_body(long long source, long long pos0, long long source_len, long long tokens, long long current_line, long long start_col, long long is_fstring) {
    long long pos = 0;
    long long col = 0;
    long long opens = 0;
    long long str_chars = 0;
    long long closed = 0;
    long long err = 0;
    long long looping = 0;
    long long ch = 0;
    long long lit = 0;
    long long t1 = 0;
    long long ok1 = 0;
    long long t2 = 0;
    long long ok2 = 0;
    long long t3 = 0;
    long long ok3 = 0;
    long long t4 = 0;
    long long ok4 = 0;
    long long t5 = 0;
    long long ok5 = 0;
    long long t6 = 0;
    long long ok6 = 0;
    long long t7 = 0;
    long long ok7 = 0;
    long long t8 = 0;
    long long ok8 = 0;
    long long depth = 0;
    long long expr_chars = 0;
    long long eloop = 0;
    long long ec = 0;
    long long okq = 0;
    long long sloop = 0;
    long long sc = 0;
    long long okq2 = 0;
    long long qc = 0;
    long long okq3 = 0;
    long long oka = 0;
    long long okb = 0;
    long long expr_src = 0;
    long long expr_tokens = 0;
    long long et_len = 0;
    long long ei = 0;
    long long et = 0;
    long long ett = 0;
    long long okc = 0;
    long long t9 = 0;
    long long ok9 = 0;
    long long t10 = 0;
    long long ok10 = 0;
    long long esc_ch = 0;
    long long oke = 0;
    long long oke2 = 0;
    long long okp = 0;
    long long lit2 = 0;
    long long tokf = 0;
    long long okf = 0;
    long long ci = 0;
    long long tcp = 0;
    long long okcp = 0;
    long long res = 0;
    long long okr1 = 0;
    long long okr2 = 0;
    long long okr3 = 0;
    long long ret_val = 0;

    ep_gc_push_root(&pos);
    ep_gc_push_root(&col);
    ep_gc_push_root(&str_chars);
    ep_gc_push_root(&err);
    ep_gc_push_root(&ch);
    ep_gc_push_root(&lit);
    ep_gc_push_root(&t1);
    ep_gc_push_root(&t2);
    ep_gc_push_root(&t3);
    ep_gc_push_root(&t4);
    ep_gc_push_root(&t5);
    ep_gc_push_root(&t6);
    ep_gc_push_root(&t7);
    ep_gc_push_root(&t8);
    ep_gc_push_root(&expr_chars);
    ep_gc_push_root(&ec);
    ep_gc_push_root(&sc);
    ep_gc_push_root(&qc);
    ep_gc_push_root(&expr_src);
    ep_gc_push_root(&expr_tokens);
    ep_gc_push_root(&ei);
    ep_gc_push_root(&et);
    ep_gc_push_root(&t9);
    ep_gc_push_root(&t10);
    ep_gc_push_root(&esc_ch);
    ep_gc_push_root(&lit2);
    ep_gc_push_root(&tokf);
    ep_gc_push_root(&tcp);
    ep_gc_push_root(&res);
    ep_gc_push_root(&source);
    ep_gc_push_root(&tokens);
    ep_gc_push_root(&current_line);
    ep_gc_push_root(&start_col);
    ep_gc_maybe_collect();

    pos = (pos0 + 1LL);
    col = (start_col + 1LL);
    opens = 0LL;
    str_chars = create_list();
    closed = 0LL;
    err = 0LL;
    looping = 1LL;
    while ((pos < source_len && looping == 1LL)) {
    ch = get_character((char*)source, pos);
    if (ch == 34LL) {
    closed = 1LL;
    looping = 0LL;
    pos = (pos + 1LL);
    col = (col + 1LL);
    } else {
    if ((ch == 123LL && is_fstring == 1LL)) {
    pos = (pos + 1LL);
    col = (col + 1LL);
    lit = (long long)string_from_list(str_chars);
    str_chars = create_list();
    if (string_length((char*)lit) != 0LL) {
    t1 = (create_token(27LL, (long long)"concat", current_line, col) + 0LL);
    ok1 = append_list(tokens, t1);
    t2 = (create_token(23LL, (long long)"(", current_line, col) + 0LL);
    ok2 = append_list(tokens, t2);
    t3 = (create_token(26LL, lit, current_line, col) + 0LL);
    ok3 = append_list(tokens, t3);
    t4 = (create_token(11LL, (long long)"and", current_line, col) + 0LL);
    ok4 = append_list(tokens, t4);
    opens = (opens + 1LL);
    }
    t5 = (create_token(27LL, (long long)"concat", current_line, col) + 0LL);
    ok5 = append_list(tokens, t5);
    t6 = (create_token(23LL, (long long)"(", current_line, col) + 0LL);
    ok6 = append_list(tokens, t6);
    t7 = (create_token(27LL, (long long)"ep_auto_to_string", current_line, col) + 0LL);
    ok7 = append_list(tokens, t7);
    t8 = (create_token(23LL, (long long)"(", current_line, col) + 0LL);
    ok8 = append_list(tokens, t8);
    depth = 1LL;
    expr_chars = create_list();
    eloop = 1LL;
    while ((pos < source_len && eloop == 1LL)) {
    ec = get_character((char*)source, pos);
    if (ec == 34LL) {
    okq = append_list(expr_chars, ec);
    pos = (pos + 1LL);
    col = (col + 1LL);
    sloop = 1LL;
    while ((pos < source_len && sloop == 1LL)) {
    sc = get_character((char*)source, pos);
    okq2 = append_list(expr_chars, sc);
    pos = (pos + 1LL);
    col = (col + 1LL);
    if (sc == 34LL) {
    sloop = 0LL;
    } else {
    if (sc == 92LL) {
    if (pos < source_len) {
    qc = get_character((char*)source, pos);
    okq3 = append_list(expr_chars, qc);
    pos = (pos + 1LL);
    col = (col + 1LL);
    }
    }
    }
    }
    } else {
    if (ec == 125LL) {
    depth = (depth - 1LL);
    if (depth == 0LL) {
    pos = (pos + 1LL);
    col = (col + 1LL);
    eloop = 0LL;
    } else {
    oka = append_list(expr_chars, ec);
    pos = (pos + 1LL);
    col = (col + 1LL);
    }
    } else {
    if (ec == 123LL) {
    depth = (depth + 1LL);
    }
    okb = append_list(expr_chars, ec);
    pos = (pos + 1LL);
    col = (col + 1LL);
    }
    }
    }
    if (eloop == 1LL) {
    printf("%s\n", (char*)(long long)"Lexer Error: Unterminated interpolation in f-string");
    err = 1LL;
    looping = 0LL;
    } else {
    expr_src = (long long)string_from_list(expr_chars);
    expr_tokens = (tokenize_source(expr_src) + 0LL);
    if (expr_tokens == 0LL) {
    printf("%s\n", (char*)(long long)"Lexer Error: Invalid expression inside f-string interpolation");
    err = 1LL;
    looping = 0LL;
    } else {
    et_len = length_list(expr_tokens);
    ei = 0LL;
    while (ei < et_len) {
    et = get_list(expr_tokens, ei);
    ett = get_token_type(et);
    if ((((ett != 28LL && ett != 29LL) && ett != 30LL) && ett != 31LL)) {
    okc = append_list(tokens, et);
    }
    ei = (ei + 1LL);
    }
    t9 = (create_token(24LL, (long long)")", current_line, col) + 0LL);
    ok9 = append_list(tokens, t9);
    t10 = (create_token(11LL, (long long)"and", current_line, col) + 0LL);
    ok10 = append_list(tokens, t10);
    opens = (opens + 1LL);
    }
    }
    } else {
    if (ch == 92LL) {
    pos = (pos + 1LL);
    col = (col + 1LL);
    if (pos < source_len) {
    esc_ch = get_character((char*)source, pos);
    if (esc_ch == 110LL) {
    oke = append_list(str_chars, 10LL);
    } else {
    if (esc_ch == 116LL) {
    oke = append_list(str_chars, 9LL);
    } else {
    if (esc_ch == 114LL) {
    oke = append_list(str_chars, 13LL);
    } else {
    if (esc_ch == 34LL) {
    oke = append_list(str_chars, 34LL);
    } else {
    if (esc_ch == 92LL) {
    oke = append_list(str_chars, 92LL);
    } else {
    if (esc_ch == 123LL) {
    oke = append_list(str_chars, 123LL);
    } else {
    if (esc_ch == 125LL) {
    oke = append_list(str_chars, 125LL);
    } else {
    oke = append_list(str_chars, 92LL);
    oke2 = append_list(str_chars, esc_ch);
    }
    }
    }
    }
    }
    }
    }
    pos = (pos + 1LL);
    col = (col + 1LL);
    } else {
    printf("%s\n", (char*)(long long)"Lexer Error: Unterminated string literal at escape sequence");
    err = 1LL;
    looping = 0LL;
    }
    } else {
    if ((ch == 10LL || ch == 13LL)) {
    printf("%s\n", (char*)(long long)"Lexer Error: Unterminated string literal");
    err = 1LL;
    looping = 0LL;
    } else {
    okp = append_list(str_chars, ch);
    pos = (pos + 1LL);
    col = (col + 1LL);
    }
    }
    }
    }
    }
    if (closed == 0LL) {
    if (err == 0LL) {
    printf("%s\n", (char*)(long long)"Lexer Error: Unterminated string literal");
    err = 1LL;
    }
    }
    if (err == 0LL) {
    lit2 = (long long)string_from_list(str_chars);
    tokf = (create_token(26LL, lit2, current_line, start_col) + 0LL);
    okf = append_list(tokens, tokf);
    if (is_fstring == 1LL) {
    ci = 0LL;
    while (ci < opens) {
    tcp = (create_token(24LL, (long long)")", current_line, col) + 0LL);
    okcp = append_list(tokens, tcp);
    ci = (ci + 1LL);
    }
    }
    }
    res = create_list();
    okr1 = append_list(res, pos);
    okr2 = append_list(res, col);
    okr3 = append_list(res, err);
    ret_val = res;
    res = 0;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(33);
    return ret_val;
}

long long lex_is_phrase2(long long source, long long pos, long long w1, long long w2) {
    long long p1 = 0;
    long long p2 = 0;
    long long ret_val = 0;

    ep_gc_push_root(&p1);
    ep_gc_push_root(&source);
    ep_gc_push_root(&pos);
    ep_gc_push_root(&w1);
    ep_gc_push_root(&w2);
    ep_gc_maybe_collect();

    p1 = match_next_word(source, pos, w1);
    if (p1 == 0LL) {
    ret_val = 0LL;
    goto L_cleanup;
    }
    p2 = match_next_word(source, p1, w2);
    ret_val = p2;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(5);
    return ret_val;
}

long long tokenize_source(long long source) {
    long long tokens = 0;
    long long source_len = 0;
    long long pos = 0;
    long long current_line = 0;
    long long current_col = 0;
    long long indent_stack = 0;
    long long ok = 0;
    long long at_line_start = 0;
    long long spaces = 0;
    long long space_loop = 0;
    long long ch = 0;
    long long next_ch = 0;
    long long stack_len = 0;
    long long last_indent = 0;
    long long tok = 0;
    long long loop_dedent = 0;
    long long s_len = 0;
    long long top_indent = 0;
    long long popped = 0;
    long long dummy = 0;
    long long c = 0;
    long long tokens_len = 0;
    long long should_emit_nl = 0;
    long long last_tok = 0;
    long long num_start = 0;
    long long num_type = 0;
    long long frac_ch = 0;
    long long num_len = 0;
    long long num_str = 0;
    long long is_id_start = 0;
    long long id_start = 0;
    long long id_loop = 0;
    long long is_id_part = 0;
    long long id_len = 0;
    long long id_str = 0;
    long long tok_type = 0;
    long long is_multi_phrase = 0;
    long long next_p = 0;
    long long next_p2 = 0;
    long long next_p3 = 0;
    long long mp2 = 0;
    long long mp_the = 0;
    long long mp3 = 0;
    long long dn = 0;
    long long de = 0;
    long long start_col = 0;
    long long is_fstring = 0;
    long long tok_count = 0;
    long long sres = 0;
    long long sym_type = 0;
    long long sym_val = 0;
    long long sym_len = 0;
    long long next_c = 0;
    long long ret_val = 0;

    ep_gc_push_root(&tokens);
    ep_gc_push_root(&source_len);
    ep_gc_push_root(&pos);
    ep_gc_push_root(&current_line);
    ep_gc_push_root(&current_col);
    ep_gc_push_root(&indent_stack);
    ep_gc_push_root(&spaces);
    ep_gc_push_root(&stack_len);
    ep_gc_push_root(&tok);
    ep_gc_push_root(&s_len);
    ep_gc_push_root(&tokens_len);
    ep_gc_push_root(&last_tok);
    ep_gc_push_root(&num_start);
    ep_gc_push_root(&num_type);
    ep_gc_push_root(&num_len);
    ep_gc_push_root(&num_str);
    ep_gc_push_root(&id_start);
    ep_gc_push_root(&id_len);
    ep_gc_push_root(&id_str);
    ep_gc_push_root(&tok_type);
    ep_gc_push_root(&next_p);
    ep_gc_push_root(&next_p2);
    ep_gc_push_root(&mp_the);
    ep_gc_push_root(&dn);
    ep_gc_push_root(&start_col);
    ep_gc_push_root(&is_fstring);
    ep_gc_push_root(&tok_count);
    ep_gc_push_root(&sres);
    ep_gc_push_root(&sym_type);
    ep_gc_push_root(&sym_val);
    ep_gc_push_root(&source);
    ep_gc_maybe_collect();

    tokens = create_list();
    source_len = string_length((char*)source);
    pos = 0LL;
    current_line = 1LL;
    current_col = 1LL;
    indent_stack = create_list();
    ok = append_list(indent_stack, 0LL);
    at_line_start = 1LL;
    while (pos < source_len) {
    if (at_line_start == 1LL) {
    spaces = 0LL;
    space_loop = 1LL;
    while ((pos < source_len && space_loop == 1LL)) {
    ch = get_character((char*)source, pos);
    if (ch == 32LL) {
    spaces = (spaces + 1LL);
    pos = (pos + 1LL);
    current_col = (current_col + 1LL);
    } else {
    if (ch == 9LL) {
    spaces = (spaces + 4LL);
    pos = (pos + 1LL);
    current_col = (current_col + 4LL);
    } else {
    space_loop = 0LL;
    }
    }
    }
    next_ch = get_character((char*)source, pos);
    if ((((next_ch != 10LL && next_ch != 13LL) && next_ch != 35LL) && pos < source_len)) {
    stack_len = length_list(indent_stack);
    last_indent = get_list(indent_stack, (stack_len - 1LL));
    if (spaces > last_indent) {
    ok = append_list(indent_stack, spaces);
    tok = (create_token(29LL, (long long)"INDENT", current_line, current_col) + 0LL);
    ok = append_list(tokens, tok);
    } else {
    if (spaces < last_indent) {
    loop_dedent = 1LL;
    while (loop_dedent == 1LL) {
    s_len = length_list(indent_stack);
    top_indent = get_list(indent_stack, (s_len - 1LL));
    if (spaces < top_indent) {
    popped = pop_list(indent_stack);
    tok = (create_token(30LL, (long long)"DEDENT", current_line, current_col) + 0LL);
    ok = append_list(tokens, tok);
    } else {
    loop_dedent = 0LL;
    }
    }
    }
    }
    }
    at_line_start = 0LL;
    }
    if (pos > (source_len - 1LL)) {
    dummy = 0LL;
    } else {
    c = get_character((char*)source, pos);
    if ((c == 32LL || c == 9LL)) {
    pos = (pos + 1LL);
    current_col = (current_col + 1LL);
    } else {
    if ((c == 10LL || c == 13LL)) {
    pos = (pos + 1LL);
    if ((c == 13LL && get_character((char*)source, pos) == 10LL)) {
    pos = (pos + 1LL);
    }
    tokens_len = length_list(tokens);
    should_emit_nl = 1LL;
    if (tokens_len > 0LL) {
    last_tok = get_list(tokens, (tokens_len - 1LL));
    if (get_token_type(last_tok) == 28LL) {
    should_emit_nl = 0LL;
    }
    }
    if ((should_emit_nl == 1LL && tokens_len > 0LL)) {
    tok = (create_token(28LL, (long long)"\n", current_line, current_col) + 0LL);
    ok = append_list(tokens, tok);
    }
    current_line = (current_line + 1LL);
    current_col = 1LL;
    at_line_start = 1LL;
    } else {
    if (c == 35LL) {
    pos = (pos + 1LL);
    while (((pos < source_len && get_character((char*)source, pos) != 10LL) && get_character((char*)source, pos) != 13LL)) {
    pos = (pos + 1LL);
    }
    } else {
    if ((c > 47LL && c < 58LL)) {
    num_start = pos;
    while (((pos < source_len && get_character((char*)source, pos) > 47LL) && get_character((char*)source, pos) < 58LL)) {
    pos = (pos + 1LL);
    current_col = (current_col + 1LL);
    }
    num_type = 25LL;
    if ((pos + 1LL) < source_len) {
    if (get_character((char*)source, pos) == 46LL) {
    frac_ch = get_character((char*)source, (pos + 1LL));
    if ((frac_ch > 47LL && frac_ch < 58LL)) {
    num_type = 70LL;
    pos = (pos + 1LL);
    current_col = (current_col + 1LL);
    while (((pos < source_len && get_character((char*)source, pos) > 47LL) && get_character((char*)source, pos) < 58LL)) {
    pos = (pos + 1LL);
    current_col = (current_col + 1LL);
    }
    }
    }
    }
    num_len = (pos - num_start);
    num_str = (long long)substring((char*)source, num_start, num_len);
    tok = (create_token(num_type, num_str, current_line, (current_col - num_len)) + 0LL);
    ok = append_list(tokens, tok);
    } else {
    is_id_start = (((c > 96LL && c < 123LL) || (c > 64LL && c < 91LL)) || c == 95LL);
    if (is_id_start) {
    id_start = pos;
    id_loop = 1LL;
    while ((pos < source_len && id_loop == 1LL)) {
    ch = get_character((char*)source, pos);
    is_id_part = ((((ch > 96LL && ch < 123LL) || (ch > 64LL && ch < 91LL)) || (ch > 47LL && ch < 58LL)) || ch == 95LL);
    if (is_id_part) {
    pos = (pos + 1LL);
    current_col = (current_col + 1LL);
    } else {
    id_loop = 0LL;
    }
    }
    id_len = (pos - id_start);
    id_str = (long long)substring((char*)source, id_start, id_len);
    tok_type = 27LL;
    is_multi_phrase = 0LL;
    if ((strcmp((char*)(long long)"multiplied", (char*)id_str) == 0)) {
    next_p = match_next_word(source, pos, (long long)"by");
    if (next_p > 0LL) {
    tok_type = 14LL;
    id_str = (long long)"*";
    current_col = (current_col + (next_p - pos));
    pos = next_p;
    is_multi_phrase = 1LL;
    }
    }
    if (is_multi_phrase == 0LL) {
    if ((strcmp((char*)(long long)"divided", (char*)id_str) == 0)) {
    next_p = match_next_word(source, pos, (long long)"by");
    if (next_p > 0LL) {
    tok_type = 15LL;
    id_str = (long long)"/";
    current_col = (current_col + (next_p - pos));
    pos = next_p;
    is_multi_phrase = 1LL;
    }
    }
    }
    if (is_multi_phrase == 0LL) {
    if ((strcmp((char*)(long long)"is", (char*)id_str) == 0)) {
    next_p = match_next_word(source, pos, (long long)"not");
    if (next_p > 0LL) {
    next_p2 = match_next_word(source, next_p, (long long)"equal");
    if (next_p2 > 0LL) {
    next_p3 = match_next_word(source, next_p2, (long long)"to");
    if (next_p3 > 0LL) {
    tok_type = 19LL;
    id_str = (long long)"!=";
    current_col = (current_col + (next_p3 - pos));
    pos = next_p3;
    is_multi_phrase = 1LL;
    }
    }
    }
    if (is_multi_phrase == 0LL) {
    next_p = match_next_word(source, pos, (long long)"less");
    if (next_p > 0LL) {
    next_p2 = match_next_word(source, next_p, (long long)"than");
    if (next_p2 > 0LL) {
    tok_type = 16LL;
    id_str = (long long)"<";
    current_col = (current_col + (next_p2 - pos));
    pos = next_p2;
    is_multi_phrase = 1LL;
    }
    }
    }
    if (is_multi_phrase == 0LL) {
    next_p = match_next_word(source, pos, (long long)"greater");
    if (next_p > 0LL) {
    next_p2 = match_next_word(source, next_p, (long long)"than");
    if (next_p2 > 0LL) {
    tok_type = 17LL;
    id_str = (long long)">";
    current_col = (current_col + (next_p2 - pos));
    pos = next_p2;
    is_multi_phrase = 1LL;
    }
    }
    }
    if (is_multi_phrase == 0LL) {
    next_p = match_next_word(source, pos, (long long)"equal");
    if (next_p > 0LL) {
    next_p2 = match_next_word(source, next_p, (long long)"to");
    if (next_p2 > 0LL) {
    tok_type = 18LL;
    id_str = (long long)"==";
    current_col = (current_col + (next_p2 - pos));
    pos = next_p2;
    is_multi_phrase = 1LL;
    }
    }
    }
    if (is_multi_phrase == 0LL) {
    mp2 = lex_is_phrase2(source, pos, (long long)"more", (long long)"than");
    if (mp2 > 0LL) {
    tok_type = 17LL;
    id_str = (long long)">";
    current_col = (current_col + (mp2 - pos));
    pos = mp2;
    is_multi_phrase = 1LL;
    }
    }
    if (is_multi_phrase == 0LL) {
    mp2 = lex_is_phrase2(source, pos, (long long)"fewer", (long long)"than");
    if (mp2 > 0LL) {
    tok_type = 16LL;
    id_str = (long long)"<";
    current_col = (current_col + (mp2 - pos));
    pos = mp2;
    is_multi_phrase = 1LL;
    }
    }
    if (is_multi_phrase == 0LL) {
    mp2 = lex_is_phrase2(source, pos, (long long)"smaller", (long long)"than");
    if (mp2 > 0LL) {
    tok_type = 16LL;
    id_str = (long long)"<";
    current_col = (current_col + (mp2 - pos));
    pos = mp2;
    is_multi_phrase = 1LL;
    }
    }
    if (is_multi_phrase == 0LL) {
    mp2 = lex_is_phrase2(source, pos, (long long)"bigger", (long long)"than");
    if (mp2 > 0LL) {
    tok_type = 17LL;
    id_str = (long long)">";
    current_col = (current_col + (mp2 - pos));
    pos = mp2;
    is_multi_phrase = 1LL;
    }
    }
    if (is_multi_phrase == 0LL) {
    mp2 = lex_is_phrase2(source, pos, (long long)"larger", (long long)"than");
    if (mp2 > 0LL) {
    tok_type = 17LL;
    id_str = (long long)">";
    current_col = (current_col + (mp2 - pos));
    pos = mp2;
    is_multi_phrase = 1LL;
    }
    }
    if (is_multi_phrase == 0LL) {
    mp2 = lex_is_phrase2(source, pos, (long long)"at", (long long)"least");
    if (mp2 > 0LL) {
    tok_type = 68LL;
    id_str = (long long)">=";
    current_col = (current_col + (mp2 - pos));
    pos = mp2;
    is_multi_phrase = 1LL;
    }
    }
    if (is_multi_phrase == 0LL) {
    mp2 = lex_is_phrase2(source, pos, (long long)"at", (long long)"most");
    if (mp2 > 0LL) {
    tok_type = 67LL;
    id_str = (long long)"<=";
    current_col = (current_col + (mp2 - pos));
    pos = mp2;
    is_multi_phrase = 1LL;
    }
    }
    if (is_multi_phrase == 0LL) {
    mp2 = lex_is_phrase2(source, pos, (long long)"different", (long long)"from");
    if (mp2 > 0LL) {
    tok_type = 19LL;
    id_str = (long long)"!=";
    current_col = (current_col + (mp2 - pos));
    pos = mp2;
    is_multi_phrase = 1LL;
    }
    }
    if (is_multi_phrase == 0LL) {
    mp_the = match_next_word(source, pos, (long long)"the");
    if (mp_the > 0LL) {
    mp3 = lex_is_phrase2(source, mp_the, (long long)"same", (long long)"as");
    if (mp3 > 0LL) {
    tok_type = 18LL;
    id_str = (long long)"==";
    current_col = (current_col + (mp3 - pos));
    pos = mp3;
    is_multi_phrase = 1LL;
    }
    }
    }
    if (is_multi_phrase == 0LL) {
    tok_type = 72LL;
    is_multi_phrase = 1LL;
    }
    }
    if ((strcmp((char*)(long long)"does", (char*)id_str) == 0)) {
    dn = match_next_word(source, pos, (long long)"not");
    if (dn > 0LL) {
    de = match_next_word(source, dn, (long long)"equal");
    if (de > 0LL) {
    tok_type = 19LL;
    id_str = (long long)"!=";
    current_col = (current_col + (de - pos));
    pos = de;
    is_multi_phrase = 1LL;
    }
    }
    }
    }
    if (is_multi_phrase == 0LL) {
    if ((strcmp((char*)(long long)"and", (char*)id_str) == 0)) {
    next_p = match_next_word(source, pos, (long long)"also");
    if (next_p > 0LL) {
    tok_type = 20LL;
    id_str = (long long)"&&";
    current_col = (current_col + (next_p - pos));
    pos = next_p;
    is_multi_phrase = 1LL;
    }
    }
    }
    if (is_multi_phrase == 0LL) {
    if ((strcmp((char*)(long long)"give", (char*)id_str) == 0)) {
    next_p = match_next_word(source, pos, (long long)"back");
    if (next_p > 0LL) {
    tok_type = 6LL;
    id_str = (long long)"return";
    current_col = (current_col + (next_p - pos));
    pos = next_p;
    is_multi_phrase = 1LL;
    }
    }
    }
    if (is_multi_phrase == 0LL) {
    if ((strcmp((char*)(long long)"or", (char*)id_str) == 0)) {
    next_p = match_next_word(source, pos, (long long)"else");
    if (next_p > 0LL) {
    tok_type = 21LL;
    id_str = (long long)"||";
    current_col = (current_col + (next_p - pos));
    pos = next_p;
    is_multi_phrase = 1LL;
    }
    }
    }
    if (is_multi_phrase == 0LL) {
    if ((strcmp((char*)(long long)"define", (char*)id_str) == 0)) {
    tok_type = 1LL;
    }
    if ((strcmp((char*)(long long)"set", (char*)id_str) == 0)) {
    tok_type = 2LL;
    }
    if ((strcmp((char*)(long long)"to", (char*)id_str) == 0)) {
    tok_type = 3LL;
    }
    if ((strcmp((char*)(long long)"if", (char*)id_str) == 0)) {
    tok_type = 4LL;
    }
    if ((strcmp((char*)(long long)"else", (char*)id_str) == 0)) {
    tok_type = 5LL;
    }
    if ((strcmp((char*)(long long)"return", (char*)id_str) == 0)) {
    tok_type = 6LL;
    }
    if ((strcmp((char*)(long long)"display", (char*)id_str) == 0)) {
    tok_type = 7LL;
    }
    if ((strcmp((char*)(long long)"repeat", (char*)id_str) == 0)) {
    tok_type = 8LL;
    }
    if ((strcmp((char*)(long long)"while", (char*)id_str) == 0)) {
    tok_type = 9LL;
    }
    if ((strcmp((char*)(long long)"with", (char*)id_str) == 0)) {
    tok_type = 10LL;
    }
    if ((strcmp((char*)(long long)"and", (char*)id_str) == 0)) {
    tok_type = 11LL;
    }
    if ((strcmp((char*)(long long)"plus", (char*)id_str) == 0)) {
    tok_type = 12LL;
    }
    if ((strcmp((char*)(long long)"minus", (char*)id_str) == 0)) {
    tok_type = 13LL;
    }
    if ((strcmp((char*)(long long)"equals", (char*)id_str) == 0)) {
    tok_type = 18LL;
    }
    if ((strcmp((char*)(long long)"import", (char*)id_str) == 0)) {
    tok_type = 32LL;
    }
    if ((strcmp((char*)(long long)"spawn", (char*)id_str) == 0)) {
    tok_type = 33LL;
    }
    if ((strcmp((char*)(long long)"channel", (char*)id_str) == 0)) {
    tok_type = 34LL;
    }
    if ((strcmp((char*)(long long)"send", (char*)id_str) == 0)) {
    tok_type = 35LL;
    }
    if ((strcmp((char*)(long long)"receive", (char*)id_str) == 0)) {
    tok_type = 36LL;
    }
    if ((strcmp((char*)(long long)"from", (char*)id_str) == 0)) {
    tok_type = 37LL;
    }
    if ((strcmp((char*)(long long)"external", (char*)id_str) == 0)) {
    tok_type = 38LL;
    }
    if ((strcmp((char*)(long long)"borrow", (char*)id_str) == 0)) {
    tok_type = 39LL;
    }
    if ((strcmp((char*)(long long)"structure", (char*)id_str) == 0)) {
    tok_type = 44LL;
    }
    if ((strcmp((char*)(long long)"field", (char*)id_str) == 0)) {
    tok_type = 45LL;
    }
    if ((strcmp((char*)(long long)"create", (char*)id_str) == 0)) {
    tok_type = 46LL;
    }
    if ((strcmp((char*)(long long)"as", (char*)id_str) == 0)) {
    tok_type = 42LL;
    }
    if ((strcmp((char*)(long long)"returning", (char*)id_str) == 0)) {
    tok_type = 43LL;
    }
    if ((strcmp((char*)(long long)"for", (char*)id_str) == 0)) {
    tok_type = 53LL;
    }
    if ((strcmp((char*)(long long)"each", (char*)id_str) == 0)) {
    tok_type = 54LL;
    }
    if ((strcmp((char*)(long long)"in", (char*)id_str) == 0)) {
    tok_type = 55LL;
    }
    if ((strcmp((char*)(long long)"range", (char*)id_str) == 0)) {
    tok_type = 60LL;
    }
    if ((strcmp((char*)(long long)"choice", (char*)id_str) == 0)) {
    tok_type = 47LL;
    }
    if ((strcmp((char*)(long long)"variant", (char*)id_str) == 0)) {
    tok_type = 48LL;
    }
    if ((strcmp((char*)(long long)"check", (char*)id_str) == 0)) {
    tok_type = 49LL;
    }
    if ((strcmp((char*)(long long)"on", (char*)id_str) == 0)) {
    tok_type = 56LL;
    }
    if ((strcmp((char*)(long long)"trait", (char*)id_str) == 0)) {
    tok_type = 57LL;
    }
    if ((strcmp((char*)(long long)"implement", (char*)id_str) == 0)) {
    tok_type = 58LL;
    }
    if ((strcmp((char*)(long long)"not", (char*)id_str) == 0)) {
    tok_type = 52LL;
    }
    if ((strcmp((char*)(long long)"break", (char*)id_str) == 0)) {
    tok_type = 61LL;
    }
    if ((strcmp((char*)(long long)"continue", (char*)id_str) == 0)) {
    tok_type = 62LL;
    }
    if ((strcmp((char*)(long long)"of", (char*)id_str) == 0)) {
    tok_type = 59LL;
    }
    if ((strcmp((char*)(long long)"try", (char*)id_str) == 0)) {
    tok_type = 51LL;
    }
    if ((strcmp((char*)(long long)"given", (char*)id_str) == 0)) {
    tok_type = 50LL;
    }
    if ((strcmp((char*)(long long)"true", (char*)id_str) == 0)) {
    tok_type = 63LL;
    }
    if ((strcmp((char*)(long long)"false", (char*)id_str) == 0)) {
    tok_type = 64LL;
    }
    if ((strcmp((char*)(long long)"async", (char*)id_str) == 0)) {
    tok_type = 65LL;
    }
    if ((strcmp((char*)(long long)"await", (char*)id_str) == 0)) {
    tok_type = 66LL;
    }
    if ((strcmp((char*)(long long)"modulo", (char*)id_str) == 0)) {
    tok_type = 41LL;
    }
    if ((strcmp((char*)(long long)"describe", (char*)id_str) == 0)) {
    tok_type = 1LL;
    }
    if ((strcmp((char*)(long long)"let", (char*)id_str) == 0)) {
    tok_type = 2LL;
    }
    if ((strcmp((char*)(long long)"be", (char*)id_str) == 0)) {
    tok_type = 3LL;
    }
    if ((strcmp((char*)(long long)"show", (char*)id_str) == 0)) {
    tok_type = 7LL;
    }
    if ((strcmp((char*)(long long)"print", (char*)id_str) == 0)) {
    tok_type = 7LL;
    }
    if ((strcmp((char*)(long long)"loop", (char*)id_str) == 0)) {
    tok_type = 8LL;
    }
    if ((strcmp((char*)(long long)"every", (char*)id_str) == 0)) {
    tok_type = 54LL;
    }
    if ((strcmp((char*)(long long)"returns", (char*)id_str) == 0)) {
    tok_type = 43LL;
    }
    if ((strcmp((char*)(long long)"stop", (char*)id_str) == 0)) {
    tok_type = 61LL;
    }
    if ((strcmp((char*)(long long)"skip", (char*)id_str) == 0)) {
    tok_type = 62LL;
    }
    if ((strcmp((char*)(long long)"times", (char*)id_str) == 0)) {
    tok_type = 14LL;
    }
    }
    tok = (create_token(tok_type, id_str, current_line, (current_col - id_len)) + 0LL);
    ok = append_list(tokens, tok);
    } else {
    if (c == 34LL) {
    start_col = current_col;
    is_fstring = 0LL;
    tok_count = length_list(tokens);
    if (tok_count > 0LL) {
    last_tok = get_list(tokens, (tok_count - 1LL));
    if (get_token_type(last_tok) == 27LL) {
    if ((strcmp((char*)(long long)"f", (char*)get_token_value(last_tok)) == 0)) {
    is_fstring = 1LL;
    popped = pop_list(tokens);
    }
    }
    }
    sres = (lex_string_body(source, pos, source_len, tokens, current_line, start_col, is_fstring) + 0LL);
    pos = get_list(sres, 0LL);
    current_col = get_list(sres, 1LL);
    if (get_list(sres, 2LL) == 1LL) {
    ret_val = 0LL;
    goto L_cleanup;
    }
    } else {
    sym_type = 0LL;
    sym_val = (long long)"";
    sym_len = 1LL;
    next_c = get_character((char*)source, (pos + 1LL));
    if ((c == 61LL && next_c == 61LL)) {
    sym_type = 18LL;
    sym_val = (long long)"==";
    sym_len = 2LL;
    }
    if ((c == 33LL && next_c == 61LL)) {
    sym_type = 19LL;
    sym_val = (long long)"!=";
    sym_len = 2LL;
    }
    if ((c == 38LL && next_c == 38LL)) {
    sym_type = 20LL;
    sym_val = (long long)"&&";
    sym_len = 2LL;
    }
    if ((c == 124LL && next_c == 124LL)) {
    sym_type = 21LL;
    sym_val = (long long)"||";
    sym_len = 2LL;
    }
    if ((c == 60LL && next_c == 61LL)) {
    sym_type = 67LL;
    sym_val = (long long)"<=";
    sym_len = 2LL;
    }
    if ((c == 62LL && next_c == 61LL)) {
    sym_type = 68LL;
    sym_val = (long long)">=";
    sym_len = 2LL;
    }
    if (sym_type == 0LL) {
    if (c == 58LL) {
    sym_type = 22LL;
    sym_val = (long long)":";
    }
    if (c == 40LL) {
    sym_type = 23LL;
    sym_val = (long long)"(";
    }
    if (c == 41LL) {
    sym_type = 24LL;
    sym_val = (long long)")";
    }
    if (c == 43LL) {
    sym_type = 12LL;
    sym_val = (long long)"+";
    }
    if (c == 45LL) {
    sym_type = 13LL;
    sym_val = (long long)"-";
    }
    if (c == 42LL) {
    sym_type = 14LL;
    sym_val = (long long)"*";
    }
    if (c == 47LL) {
    sym_type = 15LL;
    sym_val = (long long)"/";
    }
    if (c == 60LL) {
    sym_type = 16LL;
    sym_val = (long long)"<";
    }
    if (c == 62LL) {
    sym_type = 17LL;
    sym_val = (long long)">";
    }
    if (c == 46LL) {
    sym_type = 40LL;
    sym_val = (long long)".";
    }
    if (c == 37LL) {
    sym_type = 41LL;
    sym_val = (long long)"%";
    }
    if (c == 91LL) {
    sym_type = 69LL;
    sym_val = (long long)"[";
    }
    if (c == 93LL) {
    sym_type = 70LL;
    sym_val = (long long)"]";
    }
    if (c == 44LL) {
    sym_type = 71LL;
    sym_val = (long long)",";
    }
    }
    if (sym_type > 0LL) {
    tok = (create_token(sym_type, sym_val, current_line, current_col) + 0LL);
    ok = append_list(tokens, tok);
    pos = (pos + sym_len);
    current_col = (current_col + sym_len);
    } else {
    printf("%s\n", (char*)(long long)"Lexer Error: Unknown symbol character code:");
    printf("%s\n", (char*)ep_auto_to_string(c));
    pos = (pos + 1LL);
    current_col = (current_col + 1LL);
    }
    }
    }
    }
    }
    }
    }
    }
    }
    stack_len = length_list(indent_stack);
    while (stack_len > 1LL) {
    tok = (create_token(30LL, (long long)"DEDENT", current_line, current_col) + 0LL);
    ok = append_list(tokens, tok);
    popped = pop_list(indent_stack);
    stack_len = (stack_len - 1LL);
    }
    tok = (create_token(31LL, (long long)"EOF", current_line, current_col) + 0LL);
    ok = append_list(tokens, tok);
    ret_val = tokens;
    tokens = 0;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(31);
    return ret_val;
}

long long parse_int(long long s) {
    long long val = 0;
    long long len = 0;
    long long idx = 0;
    long long ch = 0;
    long long digit = 0;
    long long ret_val = 0;

    ep_gc_push_root(&idx);
    ep_gc_push_root(&s);
    ep_gc_maybe_collect();

    val = 0LL;
    len = string_length((char*)s);
    idx = 0LL;
    while (idx < len) {
    ch = get_character((char*)s, idx);
    digit = (ch - 48LL);
    val = ((val * 10LL) + digit);
    idx = (idx + 1LL);
    }
    ret_val = val;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(2);
    return ret_val;
}

long long make_node_int(long long val) {
    long long node = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&node);
    ep_gc_push_root(&val);
    ep_gc_maybe_collect();

    node = create_list();
    ok = append_list(node, 1LL);
    ok = append_list(node, val);
    ret_val = node;
    node = 0;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(2);
    return ret_val;
}

long long make_node_str(long long val) {
    long long node = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&node);
    ep_gc_push_root(&val);
    ep_gc_maybe_collect();

    node = create_list();
    ok = append_list(node, 2LL);
    ok = append_list(node, val);
    ret_val = node;
    node = 0;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(2);
    return ret_val;
}

long long make_node_ident(long long name) {
    long long node = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&node);
    ep_gc_push_root(&name);
    ep_gc_maybe_collect();

    node = create_list();
    ok = append_list(node, 3LL);
    ok = append_list(node, name);
    ret_val = node;
    node = 0;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(2);
    return ret_val;
}

long long make_node_binary(long long left, long long op, long long right) {
    long long node = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&node);
    ep_gc_push_root(&left);
    ep_gc_push_root(&op);
    ep_gc_push_root(&right);
    ep_gc_maybe_collect();

    node = create_list();
    ok = append_list(node, 4LL);
    ok = append_list(node, left);
    ok = append_list(node, op);
    ok = append_list(node, right);
    ret_val = node;
    node = 0;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(4);
    return ret_val;
}

long long make_node_comp(long long left, long long op, long long right) {
    long long node = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&node);
    ep_gc_push_root(&left);
    ep_gc_push_root(&op);
    ep_gc_push_root(&right);
    ep_gc_maybe_collect();

    node = create_list();
    ok = append_list(node, 5LL);
    ok = append_list(node, left);
    ok = append_list(node, op);
    ok = append_list(node, right);
    ret_val = node;
    node = 0;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(4);
    return ret_val;
}

long long make_node_call(long long name, long long args) {
    long long node = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&node);
    ep_gc_push_root(&name);
    ep_gc_push_root(&args);
    ep_gc_maybe_collect();

    node = create_list();
    ok = append_list(node, 6LL);
    ok = append_list(node, name);
    ok = append_list(node, args);
    ret_val = node;
    node = 0;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(3);
    return ret_val;
}

long long make_node_set(long long var, long long expr) {
    long long node = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&node);
    ep_gc_push_root(&var);
    ep_gc_push_root(&expr);
    ep_gc_maybe_collect();

    node = create_list();
    ok = append_list(node, 7LL);
    ok = append_list(node, var);
    ok = append_list(node, expr);
    ret_val = node;
    node = 0;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(3);
    return ret_val;
}

long long make_node_return(long long expr) {
    long long node = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&node);
    ep_gc_push_root(&expr);
    ep_gc_maybe_collect();

    node = create_list();
    ok = append_list(node, 8LL);
    ok = append_list(node, expr);
    ret_val = node;
    node = 0;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(2);
    return ret_val;
}

long long make_node_display(long long expr) {
    long long node = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&node);
    ep_gc_push_root(&expr);
    ep_gc_maybe_collect();

    node = create_list();
    ok = append_list(node, 9LL);
    ok = append_list(node, expr);
    ret_val = node;
    node = 0;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(2);
    return ret_val;
}

long long make_node_if(long long cond, long long then_b, long long else_b) {
    long long node = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&node);
    ep_gc_push_root(&cond);
    ep_gc_push_root(&then_b);
    ep_gc_push_root(&else_b);
    ep_gc_maybe_collect();

    node = create_list();
    ok = append_list(node, 10LL);
    ok = append_list(node, cond);
    ok = append_list(node, then_b);
    ok = append_list(node, else_b);
    ret_val = node;
    node = 0;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(4);
    return ret_val;
}

long long make_node_repeat_while(long long cond, long long body) {
    long long node = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&node);
    ep_gc_push_root(&cond);
    ep_gc_push_root(&body);
    ep_gc_maybe_collect();

    node = create_list();
    ok = append_list(node, 11LL);
    ok = append_list(node, cond);
    ok = append_list(node, body);
    ret_val = node;
    node = 0;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(3);
    return ret_val;
}

long long make_node_func(long long name, long long params, long long body, long long is_async) {
    long long node = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&node);
    ep_gc_push_root(&name);
    ep_gc_push_root(&params);
    ep_gc_push_root(&body);
    ep_gc_push_root(&is_async);
    ep_gc_maybe_collect();

    node = create_list();
    ok = append_list(node, 12LL);
    ok = append_list(node, name);
    ok = append_list(node, params);
    ok = append_list(node, body);
    ok = append_list(node, is_async);
    ret_val = node;
    node = 0;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(5);
    return ret_val;
}

long long make_node_program(long long imports, long long externals, long long funcs, long long struct_defs, long long enum_defs, long long method_defs, long long trait_defs, long long trait_impls) {
    long long node = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&node);
    ep_gc_push_root(&imports);
    ep_gc_push_root(&externals);
    ep_gc_push_root(&funcs);
    ep_gc_push_root(&struct_defs);
    ep_gc_push_root(&enum_defs);
    ep_gc_push_root(&method_defs);
    ep_gc_push_root(&trait_defs);
    ep_gc_push_root(&trait_impls);
    ep_gc_maybe_collect();

    node = create_list();
    ok = append_list(node, 13LL);
    ok = append_list(node, imports);
    ok = append_list(node, externals);
    ok = append_list(node, funcs);
    ok = append_list(node, struct_defs);
    ok = append_list(node, enum_defs);
    ok = append_list(node, method_defs);
    ok = append_list(node, trait_defs);
    ok = append_list(node, trait_impls);
    ok = append_list(node, (create_list() + 0LL));
    ret_val = node;
    node = 0;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(9);
    return ret_val;
}

long long make_node_spawn(long long func_name, long long args) {
    long long node = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&node);
    ep_gc_push_root(&func_name);
    ep_gc_push_root(&args);
    ep_gc_maybe_collect();

    node = create_list();
    ok = append_list(node, 15LL);
    ok = append_list(node, func_name);
    ok = append_list(node, args);
    ret_val = node;
    node = 0;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(3);
    return ret_val;
}

long long make_node_send(long long chan, long long val) {
    long long node = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&node);
    ep_gc_push_root(&chan);
    ep_gc_push_root(&val);
    ep_gc_maybe_collect();

    node = create_list();
    ok = append_list(node, 16LL);
    ok = append_list(node, chan);
    ok = append_list(node, val);
    ret_val = node;
    node = 0;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(3);
    return ret_val;
}

long long make_node_channel() {
    long long node = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&node);
    ep_gc_maybe_collect();

    node = create_list();
    ok = append_list(node, 17LL);
    ret_val = node;
    node = 0;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(1);
    return ret_val;
}

long long make_node_receive(long long chan) {
    long long node = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&node);
    ep_gc_push_root(&chan);
    ep_gc_maybe_collect();

    node = create_list();
    ok = append_list(node, 18LL);
    ok = append_list(node, chan);
    ret_val = node;
    node = 0;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(2);
    return ret_val;
}

long long make_node_external(long long name, long long params, long long ret_type) {
    long long node = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&node);
    ep_gc_push_root(&name);
    ep_gc_push_root(&params);
    ep_gc_push_root(&ret_type);
    ep_gc_maybe_collect();

    node = create_list();
    ok = append_list(node, 19LL);
    ok = append_list(node, name);
    ok = append_list(node, params);
    ok = append_list(node, ret_type);
    ret_val = node;
    node = 0;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(4);
    return ret_val;
}

long long make_node_borrow(long long target) {
    long long node = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&node);
    ep_gc_push_root(&target);
    ep_gc_maybe_collect();

    node = create_list();
    ok = append_list(node, 20LL);
    ok = append_list(node, target);
    ret_val = node;
    node = 0;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(2);
    return ret_val;
}

long long make_node_await(long long target) {
    long long node = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&node);
    ep_gc_push_root(&target);
    ep_gc_maybe_collect();

    node = create_list();
    ok = append_list(node, 21LL);
    ok = append_list(node, target);
    ret_val = node;
    node = 0;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(2);
    return ret_val;
}

long long make_node_logical(long long left, long long op, long long right) {
    long long node = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&node);
    ep_gc_push_root(&left);
    ep_gc_push_root(&op);
    ep_gc_push_root(&right);
    ep_gc_maybe_collect();

    node = create_list();
    ok = append_list(node, 14LL);
    ok = append_list(node, left);
    ok = append_list(node, op);
    ok = append_list(node, right);
    ret_val = node;
    node = 0;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(4);
    return ret_val;
}

long long make_node_field_access(long long obj, long long field_name) {
    long long node = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&node);
    ep_gc_push_root(&obj);
    ep_gc_push_root(&field_name);
    ep_gc_maybe_collect();

    node = create_list();
    ok = append_list(node, 22LL);
    ok = append_list(node, obj);
    ok = append_list(node, field_name);
    ret_val = node;
    node = 0;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(3);
    return ret_val;
}

long long make_node_field_set(long long obj, long long field_name, long long val) {
    long long node = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&node);
    ep_gc_push_root(&obj);
    ep_gc_push_root(&field_name);
    ep_gc_push_root(&val);
    ep_gc_maybe_collect();

    node = create_list();
    ok = append_list(node, 23LL);
    ok = append_list(node, obj);
    ok = append_list(node, field_name);
    ok = append_list(node, val);
    ret_val = node;
    node = 0;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(4);
    return ret_val;
}

long long make_node_struct_create(long long struct_name, long long fields) {
    long long node = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&node);
    ep_gc_push_root(&struct_name);
    ep_gc_push_root(&fields);
    ep_gc_maybe_collect();

    node = create_list();
    ok = append_list(node, 24LL);
    ok = append_list(node, struct_name);
    ok = append_list(node, fields);
    ret_val = node;
    node = 0;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(3);
    return ret_val;
}

long long make_node_method_call(long long obj, long long method_name, long long args) {
    long long node = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&node);
    ep_gc_push_root(&obj);
    ep_gc_push_root(&method_name);
    ep_gc_push_root(&args);
    ep_gc_maybe_collect();

    node = create_list();
    ok = append_list(node, 25LL);
    ok = append_list(node, obj);
    ok = append_list(node, method_name);
    ok = append_list(node, args);
    ret_val = node;
    node = 0;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(4);
    return ret_val;
}

long long make_node_enum_create(long long variant_name, long long args) {
    long long node = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&node);
    ep_gc_push_root(&variant_name);
    ep_gc_push_root(&args);
    ep_gc_maybe_collect();

    node = create_list();
    ok = append_list(node, 26LL);
    ok = append_list(node, variant_name);
    ok = append_list(node, args);
    ret_val = node;
    node = 0;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(3);
    return ret_val;
}

long long make_node_match(long long expr, long long arms) {
    long long node = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&node);
    ep_gc_push_root(&expr);
    ep_gc_push_root(&arms);
    ep_gc_maybe_collect();

    node = create_list();
    ok = append_list(node, 27LL);
    ok = append_list(node, expr);
    ok = append_list(node, arms);
    ret_val = node;
    node = 0;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(3);
    return ret_val;
}

long long make_node_for_each(long long var_name, long long iter_expr, long long body) {
    long long node = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&node);
    ep_gc_push_root(&var_name);
    ep_gc_push_root(&iter_expr);
    ep_gc_push_root(&body);
    ep_gc_maybe_collect();

    node = create_list();
    ok = append_list(node, 28LL);
    ok = append_list(node, var_name);
    ok = append_list(node, iter_expr);
    ok = append_list(node, body);
    ret_val = node;
    node = 0;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(4);
    return ret_val;
}

long long make_node_break() {
    long long node = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&node);
    ep_gc_maybe_collect();

    node = create_list();
    ok = append_list(node, 29LL);
    ret_val = node;
    node = 0;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(1);
    return ret_val;
}

long long make_node_continue() {
    long long node = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&node);
    ep_gc_maybe_collect();

    node = create_list();
    ok = append_list(node, 30LL);
    ret_val = node;
    node = 0;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(1);
    return ret_val;
}

long long make_node_bool(long long val) {
    long long node = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&node);
    ep_gc_push_root(&val);
    ep_gc_maybe_collect();

    node = create_list();
    ok = append_list(node, 31LL);
    ok = append_list(node, val);
    ret_val = node;
    node = 0;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(2);
    return ret_val;
}

long long make_node_unary_not(long long expr) {
    long long node = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&node);
    ep_gc_push_root(&expr);
    ep_gc_maybe_collect();

    node = create_list();
    ok = append_list(node, 32LL);
    ok = append_list(node, expr);
    ret_val = node;
    node = 0;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(2);
    return ret_val;
}

long long make_node_try(long long expr) {
    long long node = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&node);
    ep_gc_push_root(&expr);
    ep_gc_maybe_collect();

    node = create_list();
    ok = append_list(node, 33LL);
    ok = append_list(node, expr);
    ret_val = node;
    node = 0;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(2);
    return ret_val;
}

long long make_node_closure(long long params, long long body) {
    long long node = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&node);
    ep_gc_push_root(&params);
    ep_gc_push_root(&body);
    ep_gc_maybe_collect();

    node = create_list();
    ok = append_list(node, 34LL);
    ok = append_list(node, params);
    ok = append_list(node, body);
    ret_val = node;
    node = 0;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(3);
    return ret_val;
}

long long make_node_list_lit(long long elements) {
    long long node = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&node);
    ep_gc_push_root(&elements);
    ep_gc_maybe_collect();

    node = create_list();
    ok = append_list(node, 35LL);
    ok = append_list(node, elements);
    ret_val = node;
    node = 0;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(2);
    return ret_val;
}

long long make_node_expr_stmt(long long expr) {
    long long node = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&node);
    ep_gc_push_root(&expr);
    ep_gc_maybe_collect();

    node = create_list();
    ok = append_list(node, 36LL);
    ok = append_list(node, expr);
    ret_val = node;
    node = 0;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(2);
    return ret_val;
}

long long make_node_struct_def(long long name, long long fields) {
    long long node = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&node);
    ep_gc_push_root(&name);
    ep_gc_push_root(&fields);
    ep_gc_maybe_collect();

    node = create_list();
    ok = append_list(node, 37LL);
    ok = append_list(node, name);
    ok = append_list(node, fields);
    ret_val = node;
    node = 0;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(3);
    return ret_val;
}

long long make_node_enum_def(long long name, long long variants) {
    long long node = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&node);
    ep_gc_push_root(&name);
    ep_gc_push_root(&variants);
    ep_gc_maybe_collect();

    node = create_list();
    ok = append_list(node, 38LL);
    ok = append_list(node, name);
    ok = append_list(node, variants);
    ret_val = node;
    node = 0;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(3);
    return ret_val;
}

long long make_node_method_def(long long method_name, long long struct_name, long long params, long long body) {
    long long node = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&node);
    ep_gc_push_root(&method_name);
    ep_gc_push_root(&struct_name);
    ep_gc_push_root(&params);
    ep_gc_push_root(&body);
    ep_gc_maybe_collect();

    node = create_list();
    ok = append_list(node, 39LL);
    ok = append_list(node, method_name);
    ok = append_list(node, struct_name);
    ok = append_list(node, params);
    ok = append_list(node, body);
    ret_val = node;
    node = 0;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(5);
    return ret_val;
}

long long make_node_trait_def(long long name, long long method_sigs) {
    long long node = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&node);
    ep_gc_push_root(&name);
    ep_gc_push_root(&method_sigs);
    ep_gc_maybe_collect();

    node = create_list();
    ok = append_list(node, 40LL);
    ok = append_list(node, name);
    ok = append_list(node, method_sigs);
    ret_val = node;
    node = 0;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(3);
    return ret_val;
}

long long make_node_trait_impl(long long trait_name, long long type_name, long long methods) {
    long long node = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&node);
    ep_gc_push_root(&trait_name);
    ep_gc_push_root(&type_name);
    ep_gc_push_root(&methods);
    ep_gc_maybe_collect();

    node = create_list();
    ok = append_list(node, 41LL);
    ok = append_list(node, trait_name);
    ok = append_list(node, type_name);
    ok = append_list(node, methods);
    ret_val = node;
    node = 0;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(4);
    return ret_val;
}

long long create_parser_state(long long tokens) {
    long long state = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&state);
    ep_gc_push_root(&tokens);
    ep_gc_maybe_collect();

    state = create_list();
    ok = append_list(state, tokens);
    ok = append_list(state, 0LL);
    ok = append_list(state, 0LL);
    ok = append_list(state, 0LL);
    ok = append_list(state, 0LL);
    ret_val = state;
    state = 0;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(2);
    return ret_val;
}

long long get_in_condition(long long state) {
    long long ret_val = 0;

    ep_gc_push_root(&state);
    ep_gc_maybe_collect();

    ret_val = get_list(state, 3LL);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(1);
    return ret_val;
}

long long set_in_condition(long long state, long long value) {
    long long ret_val = 0;

    ep_gc_push_root(&state);
    ep_gc_push_root(&value);
    ep_gc_maybe_collect();

    ret_val = set_list(state, 3LL, value);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(2);
    return ret_val;
}

long long get_call_depth(long long state) {
    long long ret_val = 0;

    ep_gc_push_root(&state);
    ep_gc_maybe_collect();

    ret_val = get_list(state, 4LL);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(1);
    return ret_val;
}

long long enter_call_args(long long state) {
    long long ret_val = 0;

    ep_gc_push_root(&state);
    ep_gc_maybe_collect();

    ret_val = set_list(state, 4LL, (get_list(state, 4LL) + 1LL));
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(1);
    return ret_val;
}

long long leave_call_args(long long state) {
    long long ret_val = 0;

    ep_gc_push_root(&state);
    ep_gc_maybe_collect();

    ret_val = set_list(state, 4LL, (get_list(state, 4LL) - 1LL));
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(1);
    return ret_val;
}

long long set_parser_error(long long state) {
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&state);
    ep_gc_maybe_collect();

    ok = set_list(state, 2LL, 1LL);
    ret_val = 0LL;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(1);
    return ret_val;
}

long long get_parser_error(long long state) {
    long long ret_val = 0;

    ep_gc_push_root(&state);
    ep_gc_maybe_collect();

    ret_val = get_list(state, 2LL);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(1);
    return ret_val;
}

long long get_state_tokens(long long state) {
    long long ret_val = 0;

    ep_gc_push_root(&state);
    ep_gc_maybe_collect();

    ret_val = get_list(state, 0LL);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(1);
    return ret_val;
}

long long get_state_pos(long long state) {
    long long ret_val = 0;

    ep_gc_push_root(&state);
    ep_gc_maybe_collect();

    ret_val = get_list(state, 1LL);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(1);
    return ret_val;
}

long long set_state_pos(long long state, long long new_pos) {
    long long ret_val = 0;

    ep_gc_push_root(&state);
    ep_gc_push_root(&new_pos);
    ep_gc_maybe_collect();

    ret_val = set_list(state, 1LL, new_pos);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(2);
    return ret_val;
}

long long get_eof_token() {
    long long tok = 0;
    long long ret_val = 0;

    ep_gc_maybe_collect();

    tok = (create_token(31LL, (long long)"EOF", 1LL, 1LL) + 0LL);
    ret_val = tok;
    goto L_cleanup;
L_cleanup:
    return ret_val;
}

long long peek_token(long long state) {
    long long tokens = 0;
    long long pos = 0;
    long long tokens_len = 0;
    long long ret_val = 0;

    ep_gc_push_root(&tokens);
    ep_gc_push_root(&pos);
    ep_gc_push_root(&state);
    ep_gc_maybe_collect();

    tokens = get_state_tokens(state);
    pos = get_state_pos(state);
    tokens_len = length_list(tokens);
    if (pos < tokens_len) {
    ret_val = get_list(tokens, pos);
    goto L_cleanup;
    }
    ret_val = get_eof_token();
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(3);
    return ret_val;
}

long long peek_token_at(long long state, long long offset) {
    long long tokens = 0;
    long long pos = 0;
    long long tokens_len = 0;
    long long ret_val = 0;

    ep_gc_push_root(&tokens);
    ep_gc_push_root(&pos);
    ep_gc_push_root(&state);
    ep_gc_maybe_collect();

    tokens = get_state_tokens(state);
    pos = (get_state_pos(state) + offset);
    tokens_len = length_list(tokens);
    if (pos < tokens_len) {
    ret_val = get_list(tokens, pos);
    goto L_cleanup;
    }
    ret_val = get_eof_token();
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(3);
    return ret_val;
}

long long advance_token(long long state) {
    long long tokens = 0;
    long long pos = 0;
    long long tokens_len = 0;
    long long tok = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&tokens);
    ep_gc_push_root(&pos);
    ep_gc_push_root(&state);
    ep_gc_maybe_collect();

    tokens = get_state_tokens(state);
    pos = get_state_pos(state);
    tokens_len = length_list(tokens);
    if (pos < tokens_len) {
    tok = get_list(tokens, pos);
    ok = set_state_pos(state, (pos + 1LL));
    ret_val = tok;
    goto L_cleanup;
    }
    ret_val = get_eof_token();
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(3);
    return ret_val;
}

long long expect_token_type(long long state, long long expected_type) {
    long long tok = 0;
    long long actual_type = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&tok);
    ep_gc_push_root(&state);
    ep_gc_maybe_collect();

    tok = advance_token(state);
    actual_type = get_token_type(tok);
    if (actual_type == expected_type) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    printf("%s\n", (char*)(long long)"Parser Error: Expected token type:");
    printf("%s\n", (char*)ep_auto_to_string(expected_type));
    printf("%s\n", (char*)(long long)"Found type:");
    printf("%s\n", (char*)ep_auto_to_string(actual_type));
    printf("%s\n", (char*)(long long)"At line:");
    printf("%s\n", (char*)ep_auto_to_string(get_token_line(tok)));
    ok = set_parser_error(state);
    ret_val = 0LL;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(2);
    return ret_val;
}

long long get_token_precedence(long long tok) {
    long long t = 0;
    long long ret_val = 0;

    ep_gc_push_root(&tok);
    ep_gc_maybe_collect();

    t = get_token_type(tok);
    if (t == 21LL) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    if (t == 20LL) {
    ret_val = 2LL;
    goto L_cleanup;
    }
    if ((((((t == 16LL || t == 17LL) || t == 18LL) || t == 19LL) || t == 67LL) || t == 68LL)) {
    ret_val = 3LL;
    goto L_cleanup;
    }
    if ((t == 12LL || t == 13LL)) {
    ret_val = 4LL;
    goto L_cleanup;
    }
    if (((t == 14LL || t == 15LL) || t == 41LL)) {
    ret_val = 5LL;
    goto L_cleanup;
    }
    if (t == 23LL) {
    ret_val = 6LL;
    goto L_cleanup;
    }
    if (t == 40LL) {
    ret_val = 7LL;
    goto L_cleanup;
    }
    ret_val = 0LL;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(1);
    return ret_val;
}

long long skip_newlines(long long state) {
    long long loop = 0;
    long long tok = 0;
    long long dummy = 0;
    long long ret_val = 0;

    ep_gc_push_root(&tok);
    ep_gc_push_root(&state);
    ep_gc_maybe_collect();

    loop = 1LL;
    while (loop == 1LL) {
    tok = peek_token(state);
    if (get_token_type(tok) == 28LL) {
    dummy = advance_token(state);
    } else {
    loop = 0LL;
    }
    }
    ret_val = 0LL;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(2);
    return ret_val;
}

long long is_uppercase_start(long long name) {
    long long ch = 0;
    long long ret_val = 0;

    ep_gc_push_root(&name);
    ep_gc_maybe_collect();

    ch = get_character((char*)name, 0LL);
    if ((ch > 64LL && ch < 91LL)) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    ret_val = 0LL;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(1);
    return ret_val;
}

long long parse_param_list(long long state) {
    long long params = 0;
    long long next_tok = 0;
    long long dummy = 0;
    long long next_tok2 = 0;
    long long is_borrow = 0;
    long long tok_param = 0;
    long long param_name = 0;
    long long ptype = 0;
    long long next_as = 0;
    long long tok_ptype = 0;
    long long param_node = 0;
    long long ok = 0;
    long long loop = 0;
    long long tok_and = 0;
    long long next_tok3 = 0;
    long long is_borrow3 = 0;
    long long ptype3 = 0;
    long long tok_ptype3 = 0;
    long long ret_val = 0;

    ep_gc_push_root(&params);
    ep_gc_push_root(&next_tok);
    ep_gc_push_root(&next_tok2);
    ep_gc_push_root(&is_borrow);
    ep_gc_push_root(&tok_param);
    ep_gc_push_root(&param_name);
    ep_gc_push_root(&ptype);
    ep_gc_push_root(&next_as);
    ep_gc_push_root(&tok_ptype);
    ep_gc_push_root(&param_node);
    ep_gc_push_root(&tok_and);
    ep_gc_push_root(&next_tok3);
    ep_gc_push_root(&is_borrow3);
    ep_gc_push_root(&ptype3);
    ep_gc_push_root(&tok_ptype3);
    ep_gc_push_root(&state);
    ep_gc_maybe_collect();

    params = (create_list() + 0LL);
    next_tok = peek_token(state);
    if (get_token_type(next_tok) == 10LL) {
    dummy = advance_token(state);
    next_tok2 = peek_token(state);
    is_borrow = 0LL;
    if (get_token_type(next_tok2) == 39LL) {
    dummy = advance_token(state);
    is_borrow = 1LL;
    }
    tok_param = advance_token(state);
    param_name = get_token_value(tok_param);
    ptype = (long long)"";
    next_as = peek_token(state);
    if (get_token_type(next_as) == 42LL) {
    dummy = advance_token(state);
    tok_ptype = advance_token(state);
    ptype = get_token_value(tok_ptype);
    }
    param_node = (create_list() + 0LL);
    ok = append_list(param_node, param_name);
    ok = append_list(param_node, is_borrow);
    ok = append_list(param_node, ptype);
    ok = append_list(params, param_node);
    loop = 1LL;
    while (loop == 1LL) {
    tok_and = peek_token(state);
    if (get_token_type(tok_and) == 11LL) {
    dummy = advance_token(state);
    next_tok3 = peek_token(state);
    is_borrow3 = 0LL;
    if (get_token_type(next_tok3) == 39LL) {
    dummy = advance_token(state);
    is_borrow3 = 1LL;
    }
    tok_param = advance_token(state);
    param_name = get_token_value(tok_param);
    ptype3 = (long long)"";
    next_as = peek_token(state);
    if (get_token_type(next_as) == 42LL) {
    dummy = advance_token(state);
    tok_ptype3 = advance_token(state);
    ptype3 = get_token_value(tok_ptype3);
    }
    param_node = (create_list() + 0LL);
    ok = append_list(param_node, param_name);
    ok = append_list(param_node, is_borrow3);
    ok = append_list(param_node, ptype3);
    ok = append_list(params, param_node);
    } else {
    loop = 0LL;
    }
    }
    }
    ret_val = params;
    params = 0;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(16);
    return ret_val;
}

long long parse_program(long long state) {
    long long imports = 0;
    long long externals = 0;
    long long funcs = 0;
    long long struct_defs = 0;
    long long enum_defs = 0;
    long long method_defs = 0;
    long long trait_defs = 0;
    long long trait_impls = 0;
    long long top_constants = 0;
    long long loop = 0;
    long long tok = 0;
    long long t = 0;
    long long dummy = 0;
    long long tok_path = 0;
    long long path_type = 0;
    long long path_val = 0;
    long long alias_val = 0;
    long long peek_as = 0;
    long long dummy2 = 0;
    long long tok_alias = 0;
    long long imp_pair = 0;
    long long ok = 0;
    long long tok_name = 0;
    long long name = 0;
    long long params = 0;
    long long ret_type = 0;
    long long next_tok = 0;
    long long ret_tok = 0;
    long long ext_node = 0;
    long long func = 0;
    long long tok2 = 0;
    long long t2 = 0;
    long long struct_def = 0;
    long long enum_def = 0;
    long long trait_def = 0;
    long long tok3 = 0;
    long long t3 = 0;
    long long method_def = 0;
    long long impl_node = 0;
    long long const_stmt = 0;
    long long prog = 0;
    long long ret_val = 0;

    ep_gc_push_root(&imports);
    ep_gc_push_root(&externals);
    ep_gc_push_root(&funcs);
    ep_gc_push_root(&struct_defs);
    ep_gc_push_root(&enum_defs);
    ep_gc_push_root(&method_defs);
    ep_gc_push_root(&trait_defs);
    ep_gc_push_root(&trait_impls);
    ep_gc_push_root(&top_constants);
    ep_gc_push_root(&tok);
    ep_gc_push_root(&tok_path);
    ep_gc_push_root(&path_val);
    ep_gc_push_root(&alias_val);
    ep_gc_push_root(&peek_as);
    ep_gc_push_root(&tok_alias);
    ep_gc_push_root(&imp_pair);
    ep_gc_push_root(&tok_name);
    ep_gc_push_root(&name);
    ep_gc_push_root(&params);
    ep_gc_push_root(&ret_type);
    ep_gc_push_root(&next_tok);
    ep_gc_push_root(&ret_tok);
    ep_gc_push_root(&ext_node);
    ep_gc_push_root(&func);
    ep_gc_push_root(&tok2);
    ep_gc_push_root(&struct_def);
    ep_gc_push_root(&enum_def);
    ep_gc_push_root(&trait_def);
    ep_gc_push_root(&tok3);
    ep_gc_push_root(&method_def);
    ep_gc_push_root(&impl_node);
    ep_gc_push_root(&const_stmt);
    ep_gc_push_root(&prog);
    ep_gc_push_root(&state);
    ep_gc_maybe_collect();

    imports = (create_list() + 0LL);
    externals = (create_list() + 0LL);
    funcs = (create_list() + 0LL);
    struct_defs = (create_list() + 0LL);
    enum_defs = (create_list() + 0LL);
    method_defs = (create_list() + 0LL);
    trait_defs = (create_list() + 0LL);
    trait_impls = (create_list() + 0LL);
    top_constants = (create_list() + 0LL);
    loop = 1LL;
    while (loop == 1LL) {
    tok = peek_token(state);
    t = get_token_type(tok);
    if (t == 31LL) {
    loop = 0LL;
    } else {
    if (t == 28LL) {
    dummy = advance_token(state);
    } else {
    if (t == 32LL) {
    dummy = advance_token(state);
    tok_path = advance_token(state);
    path_type = get_token_type(tok_path);
    if (path_type == 26LL) {
    path_val = get_token_value(tok_path);
    alias_val = (long long)"";
    peek_as = peek_token(state);
    if (get_token_type(peek_as) == 42LL) {
    dummy2 = advance_token(state);
    tok_alias = advance_token(state);
    alias_val = get_token_value(tok_alias);
    }
    imp_pair = (create_list() + 0LL);
    ok = append_list(imp_pair, path_val);
    ok = append_list(imp_pair, alias_val);
    ok = append_list(imports, imp_pair);
    } else {
    printf("%s\n", (char*)(long long)"Parser Error: Expected string literal after import at line:");
    printf("%s\n", (char*)ep_auto_to_string(get_token_line(tok_path)));
    loop = 0LL;
    }
    } else {
    if (t == 38LL) {
    dummy = advance_token(state);
    ok = expect_token_type(state, 1LL);
    tok_name = advance_token(state);
    name = get_token_value(tok_name);
    params = (parse_param_list(state) + 0LL);
    ret_type = (long long)"";
    next_tok = peek_token(state);
    if (get_token_type(next_tok) == 43LL) {
    dummy = advance_token(state);
    ret_tok = advance_token(state);
    ret_type = get_token_value(ret_tok);
    }
    next_tok = peek_token(state);
    if (get_token_type(next_tok) == 22LL) {
    dummy = advance_token(state);
    }
    next_tok = peek_token(state);
    if (get_token_type(next_tok) == 28LL) {
    dummy = advance_token(state);
    }
    ext_node = (make_node_external(name, params, ret_type) + 0LL);
    ok = append_list(externals, ext_node);
    } else {
    if (t == 65LL) {
    dummy = advance_token(state);
    func = (parse_function_async(state, 1LL) + 0LL);
    ok = append_list(funcs, func);
    } else {
    if (t == 1LL) {
    tok2 = peek_token_at(state, 1LL);
    t2 = get_token_type(tok2);
    if (t2 == 44LL) {
    struct_def = (parse_struct_def(state) + 0LL);
    ok = append_list(struct_defs, struct_def);
    } else {
    if (t2 == 47LL) {
    enum_def = (parse_enum_def(state) + 0LL);
    ok = append_list(enum_defs, enum_def);
    } else {
    if (t2 == 57LL) {
    trait_def = (parse_trait_def(state) + 0LL);
    ok = append_list(trait_defs, trait_def);
    } else {
    tok3 = peek_token_at(state, 2LL);
    t3 = get_token_type(tok3);
    if (t3 == 56LL) {
    method_def = (parse_method_def(state) + 0LL);
    ok = append_list(method_defs, method_def);
    } else {
    func = (parse_function_async(state, 0LL) + 0LL);
    ok = append_list(funcs, func);
    }
    }
    }
    }
    } else {
    if (t == 58LL) {
    impl_node = (parse_trait_impl(state) + 0LL);
    ok = append_list(trait_impls, impl_node);
    } else {
    if (t == 2LL) {
    const_stmt = (parse_statement(state) + 0LL);
    ok = append_list(top_constants, const_stmt);
    } else {
    printf("%s\n", (char*)(long long)"Parser Error: Unexpected token at top level:");
    printf("%s\n", (char*)ep_auto_to_string(t));
    ok = set_parser_error(state);
    loop = 0LL;
    }
    }
    }
    }
    }
    }
    }
    }
    }
    prog = (make_node_program(imports, externals, funcs, struct_defs, enum_defs, method_defs, trait_defs, trait_impls) + 0LL);
    ok = set_list(prog, 9LL, top_constants);
    ret_val = prog;
    prog = 0;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(34);
    return ret_val;
}

long long parse_struct_def(long long state) {
    long long ok = 0;
    long long tok_name = 0;
    long long name = 0;
    long long fields = 0;
    long long field_loop = 0;
    long long tok = 0;
    long long t = 0;
    long long dummy = 0;
    long long field_name_tok = 0;
    long long field_name = 0;
    long long field_type = 0;
    long long field_default = 0;
    long long next = 0;
    long long type_tok = 0;
    long long field_node = 0;
    long long tok_ded = 0;
    long long ret_val = 0;

    ep_gc_push_root(&tok_name);
    ep_gc_push_root(&name);
    ep_gc_push_root(&fields);
    ep_gc_push_root(&tok);
    ep_gc_push_root(&field_name_tok);
    ep_gc_push_root(&field_name);
    ep_gc_push_root(&field_type);
    ep_gc_push_root(&field_default);
    ep_gc_push_root(&next);
    ep_gc_push_root(&type_tok);
    ep_gc_push_root(&field_node);
    ep_gc_push_root(&tok_ded);
    ep_gc_push_root(&state);
    ep_gc_maybe_collect();

    ok = expect_token_type(state, 1LL);
    ok = expect_token_type(state, 44LL);
    tok_name = advance_token(state);
    name = get_token_value(tok_name);
    ok = expect_token_type(state, 22LL);
    ok = skip_newlines(state);
    ok = expect_token_type(state, 29LL);
    fields = (create_list() + 0LL);
    field_loop = 1LL;
    while (field_loop == 1LL) {
    tok = peek_token(state);
    t = get_token_type(tok);
    if ((t == 30LL || t == 31LL)) {
    field_loop = 0LL;
    } else {
    if (t == 28LL) {
    dummy = advance_token(state);
    } else {
    if (t == 45LL) {
    dummy = advance_token(state);
    field_name_tok = advance_token(state);
    field_name = get_token_value(field_name_tok);
    field_type = (long long)"Int";
    field_default = 0LL;
    next = peek_token(state);
    if (get_token_type(next) == 42LL) {
    dummy = advance_token(state);
    type_tok = advance_token(state);
    field_type = get_token_value(type_tok);
    }
    next = peek_token(state);
    if (get_token_type(next) == 72LL) {
    dummy = advance_token(state);
    field_default = (parse_expr(state, 0LL) + 0LL);
    }
    field_node = (create_list() + 0LL);
    ok = append_list(field_node, field_name);
    ok = append_list(field_node, field_type);
    ok = append_list(field_node, field_default);
    ok = append_list(fields, field_node);
    } else {
    dummy = advance_token(state);
    }
    }
    }
    }
    tok_ded = peek_token(state);
    if (get_token_type(tok_ded) == 30LL) {
    dummy = advance_token(state);
    }
    ret_val = make_node_struct_def(name, fields);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(13);
    return ret_val;
}

long long parse_enum_def(long long state) {
    long long ok = 0;
    long long tok_name = 0;
    long long name = 0;
    long long variants = 0;
    long long var_loop = 0;
    long long tok = 0;
    long long t = 0;
    long long dummy = 0;
    long long vname_tok = 0;
    long long vname = 0;
    long long vfields = 0;
    long long next = 0;
    long long vf_tok = 0;
    long long vf_name = 0;
    long long vf_type = 0;
    long long next_as = 0;
    long long vt_tok = 0;
    long long vf_node = 0;
    long long vf_loop = 0;
    long long next_and = 0;
    long long v_node = 0;
    long long tok_ded = 0;
    long long ret_val = 0;

    ep_gc_push_root(&tok_name);
    ep_gc_push_root(&name);
    ep_gc_push_root(&variants);
    ep_gc_push_root(&tok);
    ep_gc_push_root(&vname_tok);
    ep_gc_push_root(&vname);
    ep_gc_push_root(&vfields);
    ep_gc_push_root(&next);
    ep_gc_push_root(&vf_tok);
    ep_gc_push_root(&vf_name);
    ep_gc_push_root(&vf_type);
    ep_gc_push_root(&next_as);
    ep_gc_push_root(&vt_tok);
    ep_gc_push_root(&vf_node);
    ep_gc_push_root(&next_and);
    ep_gc_push_root(&v_node);
    ep_gc_push_root(&tok_ded);
    ep_gc_push_root(&state);
    ep_gc_maybe_collect();

    ok = expect_token_type(state, 1LL);
    ok = expect_token_type(state, 47LL);
    tok_name = advance_token(state);
    name = get_token_value(tok_name);
    ok = expect_token_type(state, 22LL);
    ok = skip_newlines(state);
    ok = expect_token_type(state, 29LL);
    variants = (create_list() + 0LL);
    var_loop = 1LL;
    while (var_loop == 1LL) {
    tok = peek_token(state);
    t = get_token_type(tok);
    if ((t == 30LL || t == 31LL)) {
    var_loop = 0LL;
    } else {
    if (t == 28LL) {
    dummy = advance_token(state);
    } else {
    if (t == 48LL) {
    dummy = advance_token(state);
    vname_tok = advance_token(state);
    vname = get_token_value(vname_tok);
    vfields = (create_list() + 0LL);
    next = peek_token(state);
    if (get_token_type(next) == 10LL) {
    dummy = advance_token(state);
    vf_tok = advance_token(state);
    vf_name = get_token_value(vf_tok);
    vf_type = (long long)"Int";
    next_as = peek_token(state);
    if (get_token_type(next_as) == 42LL) {
    dummy = advance_token(state);
    vt_tok = advance_token(state);
    vf_type = get_token_value(vt_tok);
    }
    vf_node = (create_list() + 0LL);
    ok = append_list(vf_node, vf_name);
    ok = append_list(vf_node, vf_type);
    ok = append_list(vfields, vf_node);
    vf_loop = 1LL;
    while (vf_loop == 1LL) {
    next_and = peek_token(state);
    if (get_token_type(next_and) == 11LL) {
    dummy = advance_token(state);
    vf_tok = advance_token(state);
    vf_name = get_token_value(vf_tok);
    vf_type = (long long)"Int";
    next_as = peek_token(state);
    if (get_token_type(next_as) == 42LL) {
    dummy = advance_token(state);
    vt_tok = advance_token(state);
    vf_type = get_token_value(vt_tok);
    }
    vf_node = (create_list() + 0LL);
    ok = append_list(vf_node, vf_name);
    ok = append_list(vf_node, vf_type);
    ok = append_list(vfields, vf_node);
    } else {
    vf_loop = 0LL;
    }
    }
    }
    v_node = (create_list() + 0LL);
    ok = append_list(v_node, vname);
    ok = append_list(v_node, vfields);
    ok = append_list(variants, v_node);
    } else {
    dummy = advance_token(state);
    }
    }
    }
    }
    tok_ded = peek_token(state);
    if (get_token_type(tok_ded) == 30LL) {
    dummy = advance_token(state);
    }
    ret_val = make_node_enum_def(name, variants);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(18);
    return ret_val;
}

long long parse_method_def(long long state) {
    long long ok = 0;
    long long tok_method = 0;
    long long method_name = 0;
    long long tok_struct = 0;
    long long struct_name = 0;
    long long params = 0;
    long long next_ret = 0;
    long long dummy = 0;
    long long body = 0;
    long long ret_val = 0;

    ep_gc_push_root(&tok_method);
    ep_gc_push_root(&method_name);
    ep_gc_push_root(&tok_struct);
    ep_gc_push_root(&struct_name);
    ep_gc_push_root(&params);
    ep_gc_push_root(&next_ret);
    ep_gc_push_root(&body);
    ep_gc_push_root(&state);
    ep_gc_maybe_collect();

    ok = expect_token_type(state, 1LL);
    tok_method = advance_token(state);
    method_name = get_token_value(tok_method);
    ok = expect_token_type(state, 56LL);
    tok_struct = advance_token(state);
    struct_name = get_token_value(tok_struct);
    params = (parse_param_list(state) + 0LL);
    next_ret = peek_token(state);
    if (get_token_type(next_ret) == 43LL) {
    dummy = advance_token(state);
    dummy = advance_token(state);
    }
    ok = expect_token_type(state, 22LL);
    ok = skip_newlines(state);
    body = (parse_block(state) + 0LL);
    ret_val = make_node_method_def(method_name, struct_name, params, body);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(8);
    return ret_val;
}

long long parse_trait_def(long long state) {
    long long ok = 0;
    long long tok_name = 0;
    long long name = 0;
    long long method_sigs = 0;
    long long t_loop = 0;
    long long tok = 0;
    long long t = 0;
    long long dummy = 0;
    long long sig_tok = 0;
    long long sig_name = 0;
    long long sig_params = 0;
    long long next_ret = 0;
    long long next = 0;
    long long sig_node = 0;
    long long tok_ded = 0;
    long long ret_val = 0;

    ep_gc_push_root(&tok_name);
    ep_gc_push_root(&name);
    ep_gc_push_root(&method_sigs);
    ep_gc_push_root(&tok);
    ep_gc_push_root(&sig_tok);
    ep_gc_push_root(&sig_name);
    ep_gc_push_root(&sig_params);
    ep_gc_push_root(&next_ret);
    ep_gc_push_root(&next);
    ep_gc_push_root(&sig_node);
    ep_gc_push_root(&tok_ded);
    ep_gc_push_root(&state);
    ep_gc_maybe_collect();

    ok = expect_token_type(state, 1LL);
    ok = expect_token_type(state, 57LL);
    tok_name = advance_token(state);
    name = get_token_value(tok_name);
    ok = expect_token_type(state, 22LL);
    ok = skip_newlines(state);
    ok = expect_token_type(state, 29LL);
    method_sigs = (create_list() + 0LL);
    t_loop = 1LL;
    while (t_loop == 1LL) {
    tok = peek_token(state);
    t = get_token_type(tok);
    if ((t == 30LL || t == 31LL)) {
    t_loop = 0LL;
    } else {
    if (t == 28LL) {
    dummy = advance_token(state);
    } else {
    if (t == 1LL) {
    dummy = advance_token(state);
    sig_tok = advance_token(state);
    sig_name = get_token_value(sig_tok);
    sig_params = (parse_param_list(state) + 0LL);
    next_ret = peek_token(state);
    if (get_token_type(next_ret) == 43LL) {
    dummy = advance_token(state);
    dummy = advance_token(state);
    }
    next = peek_token(state);
    if (get_token_type(next) == 22LL) {
    dummy = advance_token(state);
    }
    sig_node = (create_list() + 0LL);
    ok = append_list(sig_node, sig_name);
    ok = append_list(sig_node, sig_params);
    ok = append_list(method_sigs, sig_node);
    } else {
    dummy = advance_token(state);
    }
    }
    }
    }
    tok_ded = peek_token(state);
    if (get_token_type(tok_ded) == 30LL) {
    dummy = advance_token(state);
    }
    ret_val = make_node_trait_def(name, method_sigs);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(12);
    return ret_val;
}

long long parse_trait_impl(long long state) {
    long long ok = 0;
    long long tok_trait = 0;
    long long trait_name = 0;
    long long tok_type = 0;
    long long type_name = 0;
    long long methods = 0;
    long long impl_loop = 0;
    long long tok = 0;
    long long t = 0;
    long long dummy = 0;
    long long method = 0;
    long long tok_ded = 0;
    long long ret_val = 0;

    ep_gc_push_root(&tok_trait);
    ep_gc_push_root(&trait_name);
    ep_gc_push_root(&tok_type);
    ep_gc_push_root(&type_name);
    ep_gc_push_root(&methods);
    ep_gc_push_root(&tok);
    ep_gc_push_root(&method);
    ep_gc_push_root(&tok_ded);
    ep_gc_push_root(&state);
    ep_gc_maybe_collect();

    ok = expect_token_type(state, 58LL);
    tok_trait = advance_token(state);
    trait_name = get_token_value(tok_trait);
    ok = expect_token_type(state, 53LL);
    tok_type = advance_token(state);
    type_name = get_token_value(tok_type);
    ok = expect_token_type(state, 22LL);
    ok = skip_newlines(state);
    ok = expect_token_type(state, 29LL);
    methods = (create_list() + 0LL);
    impl_loop = 1LL;
    while (impl_loop == 1LL) {
    tok = peek_token(state);
    t = get_token_type(tok);
    if ((t == 30LL || t == 31LL)) {
    impl_loop = 0LL;
    } else {
    if (t == 28LL) {
    dummy = advance_token(state);
    } else {
    if (t == 1LL) {
    method = (parse_function_async(state, 0LL) + 0LL);
    ok = append_list(methods, method);
    } else {
    dummy = advance_token(state);
    }
    }
    }
    }
    tok_ded = peek_token(state);
    if (get_token_type(tok_ded) == 30LL) {
    dummy = advance_token(state);
    }
    ret_val = make_node_trait_impl(trait_name, type_name, methods);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(9);
    return ret_val;
}

long long parse_function_async(long long state, long long is_async) {
    long long ok = 0;
    long long tok_name = 0;
    long long name = 0;
    long long params = 0;
    long long ret_type = 0;
    long long next_ret = 0;
    long long dummy = 0;
    long long ret_tok = 0;
    long long tok_nl = 0;
    long long body = 0;
    long long fnode = 0;
    long long ret_val = 0;

    ep_gc_push_root(&tok_name);
    ep_gc_push_root(&name);
    ep_gc_push_root(&params);
    ep_gc_push_root(&ret_type);
    ep_gc_push_root(&next_ret);
    ep_gc_push_root(&ret_tok);
    ep_gc_push_root(&tok_nl);
    ep_gc_push_root(&body);
    ep_gc_push_root(&fnode);
    ep_gc_push_root(&state);
    ep_gc_push_root(&is_async);
    ep_gc_maybe_collect();

    ok = expect_token_type(state, 1LL);
    tok_name = advance_token(state);
    name = get_token_value(tok_name);
    params = (parse_param_list(state) + 0LL);
    ret_type = (long long)"";
    next_ret = peek_token(state);
    if (get_token_type(next_ret) == 43LL) {
    dummy = advance_token(state);
    ret_tok = advance_token(state);
    ret_type = get_token_value(ret_tok);
    }
    ok = expect_token_type(state, 22LL);
    tok_nl = peek_token(state);
    if (get_token_type(tok_nl) == 28LL) {
    dummy = advance_token(state);
    }
    body = (parse_block(state) + 0LL);
    fnode = (make_node_func(name, params, body, is_async) + 0LL);
    ok = append_list(fnode, ret_type);
    ret_val = fnode;
    fnode = 0;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(11);
    return ret_val;
}

long long parse_block(long long state) {
    long long ok = 0;
    long long statements = 0;
    long long loop = 0;
    long long tok = 0;
    long long t = 0;
    long long dummy = 0;
    long long stmt = 0;
    long long tok_dedent = 0;
    long long ret_val = 0;

    ep_gc_push_root(&statements);
    ep_gc_push_root(&tok);
    ep_gc_push_root(&stmt);
    ep_gc_push_root(&tok_dedent);
    ep_gc_push_root(&state);
    ep_gc_maybe_collect();

    ok = expect_token_type(state, 29LL);
    statements = (create_list() + 0LL);
    loop = 1LL;
    while (loop == 1LL) {
    tok = peek_token(state);
    t = get_token_type(tok);
    if ((t == 30LL || t == 31LL)) {
    loop = 0LL;
    } else {
    if (t == 28LL) {
    dummy = advance_token(state);
    } else {
    stmt = (parse_statement(state) + 0LL);
    ok = append_list(statements, stmt);
    }
    }
    }
    tok_dedent = peek_token(state);
    if (get_token_type(tok_dedent) == 30LL) {
    dummy = advance_token(state);
    } else {
    printf("%s\n", (char*)(long long)"Parser Error: Expected DEDENT, found:");
    printf("%s\n", (char*)ep_auto_to_string(get_token_type(tok_dedent)));
    }
    ret_val = statements;
    statements = 0;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(5);
    return ret_val;
}

long long parse_statement(long long state) {
    long long tok = 0;
    long long t = 0;
    long long dummy = 0;
    long long tok_var = 0;
    long long var_name = 0;
    long long next = 0;
    long long field_tok = 0;
    long long field_name = 0;
    long long ok = 0;
    long long val = 0;
    long long obj = 0;
    long long expr = 0;
    long long next_as = 0;
    long long was_cond = 0;
    long long cond = 0;
    long long body = 0;
    long long tok_func = 0;
    long long func_name = 0;
    long long args = 0;
    long long next_tok = 0;
    long long arg = 0;
    long long loop_args = 0;
    long long next_tok3 = 0;
    long long chan = 0;
    long long ret_val = 0;

    ep_gc_push_root(&tok);
    ep_gc_push_root(&tok_var);
    ep_gc_push_root(&var_name);
    ep_gc_push_root(&next);
    ep_gc_push_root(&field_tok);
    ep_gc_push_root(&field_name);
    ep_gc_push_root(&val);
    ep_gc_push_root(&obj);
    ep_gc_push_root(&expr);
    ep_gc_push_root(&next_as);
    ep_gc_push_root(&was_cond);
    ep_gc_push_root(&cond);
    ep_gc_push_root(&body);
    ep_gc_push_root(&tok_func);
    ep_gc_push_root(&func_name);
    ep_gc_push_root(&args);
    ep_gc_push_root(&next_tok);
    ep_gc_push_root(&arg);
    ep_gc_push_root(&next_tok3);
    ep_gc_push_root(&chan);
    ep_gc_push_root(&state);
    ep_gc_maybe_collect();

    tok = peek_token(state);
    t = get_token_type(tok);
    if (t == 2LL) {
    dummy = advance_token(state);
    tok_var = advance_token(state);
    var_name = get_token_value(tok_var);
    next = peek_token(state);
    if (get_token_type(next) == 40LL) {
    dummy = advance_token(state);
    field_tok = advance_token(state);
    field_name = get_token_value(field_tok);
    ok = expect_token_type(state, 3LL);
    val = (parse_expr(state, 0LL) + 0LL);
    obj = (make_node_ident(var_name) + 0LL);
    ok = skip_newlines(state);
    ret_val = make_node_field_set(obj, field_name, val);
    goto L_cleanup;
    } else {
    ok = expect_token_type(state, 3LL);
    expr = (parse_expr(state, 0LL) + 0LL);
    next_as = peek_token(state);
    if (get_token_type(next_as) == 42LL) {
    dummy = advance_token(state);
    dummy = advance_token(state);
    }
    ok = skip_newlines(state);
    ret_val = make_node_set(var_name, expr);
    goto L_cleanup;
    }
    } else {
    if (t == 4LL) {
    ret_val = parse_if_statement(state);
    goto L_cleanup;
    } else {
    if ((t == 8LL || t == 9LL)) {
    dummy = advance_token(state);
    if (t == 8LL) {
    ok = expect_token_type(state, 9LL);
    }
    was_cond = get_in_condition(state);
    ok = set_in_condition(state, 1LL);
    cond = (parse_expr(state, 0LL) + 0LL);
    ok = set_in_condition(state, was_cond);
    ok = expect_token_type(state, 22LL);
    ok = skip_newlines(state);
    body = (parse_block(state) + 0LL);
    ret_val = make_node_repeat_while(cond, body);
    goto L_cleanup;
    } else {
    if (t == 6LL) {
    dummy = advance_token(state);
    expr = (parse_expr(state, 0LL) + 0LL);
    ok = skip_newlines(state);
    ret_val = make_node_return(expr);
    goto L_cleanup;
    } else {
    if (t == 7LL) {
    dummy = advance_token(state);
    expr = (parse_expr(state, 0LL) + 0LL);
    ok = skip_newlines(state);
    ret_val = make_node_display(expr);
    goto L_cleanup;
    } else {
    if (t == 33LL) {
    dummy = advance_token(state);
    tok_func = advance_token(state);
    func_name = get_token_value(tok_func);
    ok = expect_token_type(state, 23LL);
    args = (create_list() + 0LL);
    next_tok = peek_token(state);
    if (get_token_type(next_tok) != 24LL) {
    arg = (parse_expr(state, 0LL) + 0LL);
    ok = append_list(args, arg);
    loop_args = 1LL;
    while (loop_args == 1LL) {
    next_tok3 = peek_token(state);
    if (get_token_type(next_tok3) == 11LL) {
    dummy = advance_token(state);
    arg = (parse_expr(state, 0LL) + 0LL);
    ok = append_list(args, arg);
    } else {
    loop_args = 0LL;
    }
    }
    }
    ok = expect_token_type(state, 24LL);
    ok = skip_newlines(state);
    ret_val = make_node_spawn(func_name, args);
    goto L_cleanup;
    } else {
    if (t == 35LL) {
    dummy = advance_token(state);
    val = (parse_expr(state, 0LL) + 0LL);
    ok = expect_token_type(state, 3LL);
    chan = (parse_expr(state, 0LL) + 0LL);
    ok = skip_newlines(state);
    ret_val = make_node_send(chan, val);
    goto L_cleanup;
    } else {
    if (t == 49LL) {
    ret_val = parse_match_statement(state);
    goto L_cleanup;
    } else {
    if (t == 53LL) {
    ret_val = parse_for_each_statement(state);
    goto L_cleanup;
    } else {
    if (t == 61LL) {
    dummy = advance_token(state);
    ok = skip_newlines(state);
    ret_val = make_node_break();
    goto L_cleanup;
    } else {
    if (t == 62LL) {
    dummy = advance_token(state);
    ok = skip_newlines(state);
    ret_val = make_node_continue();
    goto L_cleanup;
    } else {
    expr = (parse_expr(state, 0LL) + 0LL);
    ok = skip_newlines(state);
    ret_val = make_node_expr_stmt(expr);
    goto L_cleanup;
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
L_cleanup:
    ep_gc_pop_roots(21);
    return ret_val;
}

long long parse_if_statement(long long state) {
    long long dummy = 0;
    long long was_cond = 0;
    long long ok = 0;
    long long cond = 0;
    long long then_branch = 0;
    long long else_branch = 0;
    long long next = 0;
    long long next2 = 0;
    long long chained_if = 0;
    long long ret_val = 0;

    ep_gc_push_root(&was_cond);
    ep_gc_push_root(&cond);
    ep_gc_push_root(&then_branch);
    ep_gc_push_root(&else_branch);
    ep_gc_push_root(&next);
    ep_gc_push_root(&next2);
    ep_gc_push_root(&chained_if);
    ep_gc_push_root(&state);
    ep_gc_maybe_collect();

    dummy = advance_token(state);
    was_cond = get_in_condition(state);
    ok = set_in_condition(state, 1LL);
    cond = (parse_expr(state, 0LL) + 0LL);
    ok = set_in_condition(state, was_cond);
    ok = expect_token_type(state, 22LL);
    ok = skip_newlines(state);
    then_branch = (parse_block(state) + 0LL);
    else_branch = 0LL;
    next = peek_token(state);
    if (get_token_type(next) == 5LL) {
    dummy = advance_token(state);
    next2 = peek_token(state);
    if (get_token_type(next2) == 4LL) {
    chained_if = (parse_if_statement(state) + 0LL);
    else_branch = (create_list() + 0LL);
    ok = append_list(else_branch, chained_if);
    } else {
    ok = expect_token_type(state, 22LL);
    ok = skip_newlines(state);
    else_branch = (parse_block(state) + 0LL);
    }
    }
    ret_val = make_node_if(cond, then_branch, else_branch);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(8);
    return ret_val;
}

long long parse_match_statement(long long state) {
    long long dummy = 0;
    long long expr = 0;
    long long ok = 0;
    long long arms = 0;
    long long arm_loop = 0;
    long long tok = 0;
    long long tok_t = 0;
    long long variant_tok = 0;
    long long variant_name = 0;
    long long pat_kind = 0;
    long long bindings = 0;
    long long next = 0;
    long long bind_tok = 0;
    long long b_loop = 0;
    long long next_and = 0;
    long long arm_body = 0;
    long long arm_node = 0;
    long long tok_ded = 0;
    long long ret_val = 0;

    ep_gc_push_root(&expr);
    ep_gc_push_root(&arms);
    ep_gc_push_root(&tok);
    ep_gc_push_root(&variant_tok);
    ep_gc_push_root(&variant_name);
    ep_gc_push_root(&pat_kind);
    ep_gc_push_root(&bindings);
    ep_gc_push_root(&next);
    ep_gc_push_root(&bind_tok);
    ep_gc_push_root(&next_and);
    ep_gc_push_root(&arm_body);
    ep_gc_push_root(&arm_node);
    ep_gc_push_root(&tok_ded);
    ep_gc_push_root(&state);
    ep_gc_maybe_collect();

    dummy = advance_token(state);
    expr = (parse_expr(state, 0LL) + 0LL);
    ok = expect_token_type(state, 22LL);
    ok = skip_newlines(state);
    ok = expect_token_type(state, 29LL);
    arms = (create_list() + 0LL);
    arm_loop = 1LL;
    while (arm_loop == 1LL) {
    tok = peek_token(state);
    tok_t = get_token_type(tok);
    if ((tok_t == 30LL || tok_t == 31LL)) {
    arm_loop = 0LL;
    } else {
    if (tok_t == 28LL) {
    dummy = advance_token(state);
    } else {
    if (tok_t == 4LL) {
    dummy = advance_token(state);
    variant_tok = advance_token(state);
    variant_name = get_token_value(variant_tok);
    pat_kind = get_token_type(variant_tok);
    bindings = (create_list() + 0LL);
    next = peek_token(state);
    if (get_token_type(next) == 10LL) {
    dummy = advance_token(state);
    bind_tok = advance_token(state);
    ok = append_list(bindings, get_token_value(bind_tok));
    b_loop = 1LL;
    while (b_loop == 1LL) {
    next_and = peek_token(state);
    if (get_token_type(next_and) == 11LL) {
    dummy = advance_token(state);
    bind_tok = advance_token(state);
    ok = append_list(bindings, get_token_value(bind_tok));
    } else {
    b_loop = 0LL;
    }
    }
    }
    ok = expect_token_type(state, 22LL);
    ok = skip_newlines(state);
    arm_body = (parse_block(state) + 0LL);
    arm_node = (create_list() + 0LL);
    ok = append_list(arm_node, variant_name);
    ok = append_list(arm_node, bindings);
    ok = append_list(arm_node, arm_body);
    ok = append_list(arm_node, pat_kind);
    ok = append_list(arms, arm_node);
    } else {
    dummy = advance_token(state);
    }
    }
    }
    }
    tok_ded = peek_token(state);
    if (get_token_type(tok_ded) == 30LL) {
    dummy = advance_token(state);
    }
    ret_val = make_node_match(expr, arms);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(14);
    return ret_val;
}

long long parse_for_each_statement(long long state) {
    long long dummy = 0;
    long long ok = 0;
    long long var_tok = 0;
    long long var_name = 0;
    long long iter_expr = 0;
    long long body = 0;
    long long ret_val = 0;

    ep_gc_push_root(&var_tok);
    ep_gc_push_root(&var_name);
    ep_gc_push_root(&iter_expr);
    ep_gc_push_root(&body);
    ep_gc_push_root(&state);
    ep_gc_maybe_collect();

    dummy = advance_token(state);
    ok = expect_token_type(state, 54LL);
    var_tok = advance_token(state);
    var_name = get_token_value(var_tok);
    ok = expect_token_type(state, 55LL);
    iter_expr = (parse_expr(state, 0LL) + 0LL);
    ok = expect_token_type(state, 22LL);
    ok = skip_newlines(state);
    body = (parse_block(state) + 0LL);
    ret_val = make_node_for_each(var_name, iter_expr, body);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(5);
    return ret_val;
}

long long parse_expr(long long state, long long precedence) {
    long long left = 0;
    long long climbing = 0;
    long long next_tok = 0;
    long long next_t = 0;
    long long next_prec = 0;
    long long dummy = 0;
    long long member_tok = 0;
    long long member_name = 0;
    long long next2 = 0;
    long long entered = 0;
    long long args = 0;
    long long next3 = 0;
    long long arg = 0;
    long long ok = 0;
    long long arg_loop = 0;
    long long next4 = 0;
    long long left_call = 0;
    long long op_tok = 0;
    long long op_type = 0;
    long long is_math = 0;
    long long op = 0;
    long long right_prec = 0;
    long long right = 0;
    long long is_comp = 0;
    long long is_logical = 0;
    long long a_loop = 0;
    long long left_type = 0;
    long long call_name = 0;
    long long ret_val = 0;

    ep_gc_push_root(&left);
    ep_gc_push_root(&next_tok);
    ep_gc_push_root(&member_tok);
    ep_gc_push_root(&member_name);
    ep_gc_push_root(&next2);
    ep_gc_push_root(&args);
    ep_gc_push_root(&next3);
    ep_gc_push_root(&arg);
    ep_gc_push_root(&next4);
    ep_gc_push_root(&op_tok);
    ep_gc_push_root(&op);
    ep_gc_push_root(&right_prec);
    ep_gc_push_root(&right);
    ep_gc_push_root(&call_name);
    ep_gc_push_root(&state);
    ep_gc_maybe_collect();

    left = (parse_prefix(state) + 0LL);
    climbing = 1LL;
    while (climbing == 1LL) {
    next_tok = peek_token(state);
    next_t = get_token_type(next_tok);
    next_prec = get_token_precedence(next_tok);
    if (((next_t == 11LL && get_in_condition(state) == 1LL) && get_call_depth(state) == 0LL)) {
    next_prec = 2LL;
    }
    if ((next_t == 40LL && precedence < 7LL)) {
    dummy = advance_token(state);
    member_tok = advance_token(state);
    member_name = get_token_value(member_tok);
    next2 = peek_token(state);
    if (get_token_type(next2) == 23LL) {
    dummy = advance_token(state);
    entered = enter_call_args(state);
    args = (create_list() + 0LL);
    next3 = peek_token(state);
    if (get_token_type(next3) != 24LL) {
    arg = (parse_expr(state, 0LL) + 0LL);
    ok = append_list(args, arg);
    arg_loop = 1LL;
    while (arg_loop == 1LL) {
    next4 = peek_token(state);
    if (get_token_type(next4) == 11LL) {
    dummy = advance_token(state);
    arg = (parse_expr(state, 0LL) + 0LL);
    ok = append_list(args, arg);
    } else {
    if (get_token_type(next4) == 71LL) {
    dummy = advance_token(state);
    arg = (parse_expr(state, 0LL) + 0LL);
    ok = append_list(args, arg);
    } else {
    arg_loop = 0LL;
    }
    }
    }
    }
    left_call = leave_call_args(state);
    ok = expect_token_type(state, 24LL);
    left = (make_node_method_call(left, member_name, args) + 0LL);
    } else {
    left = (make_node_field_access(left, member_name) + 0LL);
    }
    } else {
    if (precedence < next_prec) {
    op_tok = advance_token(state);
    op_type = get_token_type(op_tok);
    is_math = ((((op_type == 12LL || op_type == 13LL) || op_type == 14LL) || op_type == 15LL) || op_type == 41LL);
    if (is_math == 1LL) {
    op = 0LL;
    if (op_type == 12LL) {
    op = 1LL;
    }
    if (op_type == 13LL) {
    op = 2LL;
    }
    if (op_type == 14LL) {
    op = 3LL;
    }
    if (op_type == 15LL) {
    op = 4LL;
    }
    if (op_type == 41LL) {
    op = 5LL;
    }
    right_prec = get_token_precedence(op_tok);
    right = (parse_expr(state, right_prec) + 0LL);
    left = (make_node_binary(left, op, right) + 0LL);
    } else {
    is_comp = (((((op_type == 16LL || op_type == 17LL) || op_type == 18LL) || op_type == 19LL) || op_type == 67LL) || op_type == 68LL);
    if (is_comp == 1LL) {
    op = 0LL;
    if (op_type == 16LL) {
    op = 1LL;
    }
    if (op_type == 17LL) {
    op = 2LL;
    }
    if (op_type == 18LL) {
    op = 3LL;
    }
    if (op_type == 19LL) {
    op = 4LL;
    }
    if (op_type == 67LL) {
    op = 5LL;
    }
    if (op_type == 68LL) {
    op = 6LL;
    }
    right_prec = get_token_precedence(op_tok);
    right = (parse_expr(state, right_prec) + 0LL);
    left = (make_node_comp(left, op, right) + 0LL);
    } else {
    is_logical = ((op_type == 20LL || op_type == 21LL) || op_type == 11LL);
    if (is_logical == 1LL) {
    op = 0LL;
    if (op_type == 20LL) {
    op = 1LL;
    }
    if (op_type == 21LL) {
    op = 2LL;
    }
    if (op_type == 11LL) {
    op = 1LL;
    }
    right_prec = 2LL;
    if (op_type == 21LL) {
    right_prec = 1LL;
    }
    right = (parse_expr(state, right_prec) + 0LL);
    left = (make_node_logical(left, op, right) + 0LL);
    } else {
    if (op_type == 23LL) {
    entered = enter_call_args(state);
    args = (create_list() + 0LL);
    next3 = peek_token(state);
    if (get_token_type(next3) != 24LL) {
    arg = (parse_expr(state, 0LL) + 0LL);
    ok = append_list(args, arg);
    a_loop = 1LL;
    while (a_loop == 1LL) {
    next4 = peek_token(state);
    if ((get_token_type(next4) == 11LL || get_token_type(next4) == 71LL)) {
    dummy = advance_token(state);
    arg = (parse_expr(state, 0LL) + 0LL);
    ok = append_list(args, arg);
    } else {
    a_loop = 0LL;
    }
    }
    }
    left_call = leave_call_args(state);
    ok = expect_token_type(state, 24LL);
    left_type = get_list(left, 0LL);
    if (left_type == 3LL) {
    call_name = get_list(left, 1LL);
    left = (make_node_call(call_name, args) + 0LL);
    } else {
    printf("%s\n", (char*)(long long)"Parser Error: Expected function name before (");
    }
    }
    }
    }
    }
    } else {
    climbing = 0LL;
    }
    }
    }
    ret_val = left;
    left = 0;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(15);
    return ret_val;
}

long long parse_prefix(long long state) {
    long long tok = 0;
    long long t = 0;
    long long val = 0;
    long long fnode = 0;
    long long ok = 0;
    long long chan = 0;
    long long target = 0;
    long long operand = 0;
    long long try_expr = 0;
    long long name = 0;
    long long next_tok = 0;
    long long dummy = 0;
    long long entered = 0;
    long long args = 0;
    long long next_tok2 = 0;
    long long arg = 0;
    long long loop_args = 0;
    long long next_tok3 = 0;
    long long nt3_type = 0;
    long long left_call = 0;
    long long next2 = 0;
    long long nt2 = 0;
    long long vargs = 0;
    long long varg = 0;
    long long va_loop = 0;
    long long next3 = 0;
    long long left_enum = 0;
    long long expr = 0;
    long long start_expr = 0;
    long long end_expr = 0;
    long long range_args = 0;
    long long ret_val = 0;

    ep_gc_push_root(&tok);
    ep_gc_push_root(&val);
    ep_gc_push_root(&fnode);
    ep_gc_push_root(&chan);
    ep_gc_push_root(&target);
    ep_gc_push_root(&operand);
    ep_gc_push_root(&try_expr);
    ep_gc_push_root(&name);
    ep_gc_push_root(&next_tok);
    ep_gc_push_root(&args);
    ep_gc_push_root(&next_tok2);
    ep_gc_push_root(&arg);
    ep_gc_push_root(&next_tok3);
    ep_gc_push_root(&next2);
    ep_gc_push_root(&vargs);
    ep_gc_push_root(&varg);
    ep_gc_push_root(&next3);
    ep_gc_push_root(&start_expr);
    ep_gc_push_root(&end_expr);
    ep_gc_push_root(&range_args);
    ep_gc_push_root(&state);
    ep_gc_maybe_collect();

    tok = advance_token(state);
    t = get_token_type(tok);
    if (t == 25LL) {
    val = parse_int(get_token_value(tok));
    ret_val = (make_node_int(val) + 0LL);
    goto L_cleanup;
    }
    if (t == 70LL) {
    fnode = (create_list() + 0LL);
    ok = append_list(fnode, 42LL);
    ok = append_list(fnode, get_token_value(tok));
    ret_val = (fnode + 0LL);
    goto L_cleanup;
    }
    if (t == 26LL) {
    ret_val = (make_node_str(get_token_value(tok)) + 0LL);
    goto L_cleanup;
    }
    if (t == 34LL) {
    ret_val = (make_node_channel() + 0LL);
    goto L_cleanup;
    }
    if (t == 36LL) {
    ok = expect_token_type(state, 37LL);
    chan = (parse_expr(state, 0LL) + 0LL);
    ret_val = (make_node_receive(chan) + 0LL);
    goto L_cleanup;
    }
    if (t == 39LL) {
    target = (parse_expr(state, 0LL) + 0LL);
    ret_val = (make_node_borrow(target) + 0LL);
    goto L_cleanup;
    }
    if (t == 66LL) {
    target = (parse_expr(state, 0LL) + 0LL);
    ret_val = (make_node_await(target) + 0LL);
    goto L_cleanup;
    }
    if (t == 63LL) {
    ret_val = (make_node_bool(1LL) + 0LL);
    goto L_cleanup;
    }
    if (t == 64LL) {
    ret_val = (make_node_bool(0LL) + 0LL);
    goto L_cleanup;
    }
    if (t == 52LL) {
    operand = (parse_expr(state, 6LL) + 0LL);
    ret_val = (make_node_unary_not(operand) + 0LL);
    goto L_cleanup;
    }
    if (t == 51LL) {
    try_expr = (parse_expr(state, 0LL) + 0LL);
    ret_val = (make_node_try(try_expr) + 0LL);
    goto L_cleanup;
    }
    if (t == 13LL) {
    operand = (parse_expr(state, 6LL) + 0LL);
    ret_val = (make_node_binary(make_node_int(0LL), 2LL, operand) + 0LL);
    goto L_cleanup;
    }
    if (t == 50LL) {
    ret_val = (parse_closure(state) + 0LL);
    goto L_cleanup;
    }
    if (t == 46LL) {
    ret_val = (parse_struct_create(state) + 0LL);
    goto L_cleanup;
    }
    if (t == 69LL) {
    ret_val = (parse_list_literal(state) + 0LL);
    goto L_cleanup;
    }
    if (((((((((((((((t == 1LL || t == 7LL) || t == 8LL) || t == 43LL) || t == 44LL) || t == 45LL) || t == 47LL) || t == 48LL) || t == 49LL) || t == 54LL) || t == 57LL) || t == 58LL) || t == 60LL) || t == 61LL) || t == 62LL)) {
    t = 27LL;
    }
    if (t == 27LL) {
    name = get_token_value(tok);
    next_tok = peek_token(state);
    if (get_token_type(next_tok) == 23LL) {
    dummy = advance_token(state);
    entered = enter_call_args(state);
    args = (create_list() + 0LL);
    next_tok2 = peek_token(state);
    if (get_token_type(next_tok2) != 24LL) {
    arg = (parse_expr(state, 0LL) + 0LL);
    ok = append_list(args, arg);
    loop_args = 1LL;
    while (loop_args == 1LL) {
    next_tok3 = peek_token(state);
    nt3_type = get_token_type(next_tok3);
    if ((nt3_type == 11LL || nt3_type == 71LL)) {
    dummy = advance_token(state);
    arg = (parse_expr(state, 0LL) + 0LL);
    ok = append_list(args, arg);
    } else {
    loop_args = 0LL;
    }
    }
    }
    left_call = leave_call_args(state);
    ok = expect_token_type(state, 24LL);
    ret_val = (make_node_call(name, args) + 0LL);
    goto L_cleanup;
    } else {
    if (is_uppercase_start(name) == 1LL) {
    next2 = peek_token(state);
    nt2 = get_token_type(next2);
    if (nt2 == 10LL) {
    dummy = advance_token(state);
    entered = enter_call_args(state);
    vargs = (create_list() + 0LL);
    varg = (parse_expr(state, 0LL) + 0LL);
    ok = append_list(vargs, varg);
    va_loop = 1LL;
    while (va_loop == 1LL) {
    next3 = peek_token(state);
    if (get_token_type(next3) == 11LL) {
    dummy = advance_token(state);
    varg = (parse_expr(state, 0LL) + 0LL);
    ok = append_list(vargs, varg);
    } else {
    va_loop = 0LL;
    }
    }
    left_enum = leave_call_args(state);
    ret_val = (make_node_enum_create(name, vargs) + 0LL);
    goto L_cleanup;
    }
    }
    ret_val = (make_node_ident(name) + 0LL);
    goto L_cleanup;
    }
    }
    if (t == 23LL) {
    expr = (parse_expr(state, 0LL) + 0LL);
    ok = expect_token_type(state, 24LL);
    ret_val = expr;
    goto L_cleanup;
    }
    if (t == 60LL) {
    ok = expect_token_type(state, 23LL);
    start_expr = (parse_expr(state, 0LL) + 0LL);
    ok = expect_token_type(state, 71LL);
    end_expr = (parse_expr(state, 0LL) + 0LL);
    ok = expect_token_type(state, 24LL);
    range_args = (create_list() + 0LL);
    ok = append_list(range_args, start_expr);
    ok = append_list(range_args, end_expr);
    ret_val = (make_node_call((long long)"range", range_args) + 0LL);
    goto L_cleanup;
    }
    printf("%s\n", (char*)(long long)"Parser Error: Expected expression, found token type:");
    printf("%s\n", (char*)ep_auto_to_string(t));
    ok = set_parser_error(state);
    ret_val = (make_node_int(0LL) + 0LL);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(21);
    return ret_val;
}

long long parse_closure(long long state) {
    long long params = 0;
    long long next = 0;
    long long p_tok = 0;
    long long p_name = 0;
    long long p_node = 0;
    long long ok = 0;
    long long p_loop = 0;
    long long next_and = 0;
    long long dummy = 0;
    long long body = 0;
    long long single = 0;
    long long ret_val = 0;

    ep_gc_push_root(&params);
    ep_gc_push_root(&next);
    ep_gc_push_root(&p_tok);
    ep_gc_push_root(&p_name);
    ep_gc_push_root(&p_node);
    ep_gc_push_root(&next_and);
    ep_gc_push_root(&body);
    ep_gc_push_root(&single);
    ep_gc_push_root(&state);
    ep_gc_maybe_collect();

    params = (create_list() + 0LL);
    next = peek_token(state);
    if (get_token_type(next) != 22LL) {
    p_tok = advance_token(state);
    p_name = get_token_value(p_tok);
    p_node = (create_list() + 0LL);
    ok = append_list(p_node, p_name);
    ok = append_list(p_node, 0LL);
    ok = append_list(params, p_node);
    p_loop = 1LL;
    while (p_loop == 1LL) {
    next_and = peek_token(state);
    if (get_token_type(next_and) == 11LL) {
    dummy = advance_token(state);
    p_tok = advance_token(state);
    p_name = get_token_value(p_tok);
    p_node = (create_list() + 0LL);
    ok = append_list(p_node, p_name);
    ok = append_list(p_node, 0LL);
    ok = append_list(params, p_node);
    } else {
    p_loop = 0LL;
    }
    }
    }
    ok = expect_token_type(state, 22LL);
    ok = skip_newlines(state);
    next = peek_token(state);
    if (get_token_type(next) == 29LL) {
    body = (parse_block(state) + 0LL);
    } else {
    single = (parse_statement(state) + 0LL);
    body = (create_list() + 0LL);
    ok = append_list(body, single);
    }
    ret_val = make_node_closure(params, body);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(9);
    return ret_val;
}

long long parse_struct_create(long long state) {
    long long tok_name = 0;
    long long struct_name = 0;
    long long ok = 0;
    long long fields = 0;
    long long next = 0;
    long long cf_loop = 0;
    long long tok = 0;
    long long tok_t = 0;
    long long dummy = 0;
    long long fname_tok = 0;
    long long fname = 0;
    long long fval = 0;
    long long fpair = 0;
    long long tok_ded = 0;
    long long ret_val = 0;

    ep_gc_push_root(&tok_name);
    ep_gc_push_root(&struct_name);
    ep_gc_push_root(&fields);
    ep_gc_push_root(&next);
    ep_gc_push_root(&tok);
    ep_gc_push_root(&fname_tok);
    ep_gc_push_root(&fname);
    ep_gc_push_root(&fval);
    ep_gc_push_root(&fpair);
    ep_gc_push_root(&tok_ded);
    ep_gc_push_root(&state);
    ep_gc_maybe_collect();

    tok_name = advance_token(state);
    struct_name = get_token_value(tok_name);
    ok = expect_token_type(state, 22LL);
    ok = skip_newlines(state);
    fields = (create_list() + 0LL);
    next = peek_token(state);
    if (get_token_type(next) == 29LL) {
    ok = expect_token_type(state, 29LL);
    cf_loop = 1LL;
    while (cf_loop == 1LL) {
    tok = peek_token(state);
    tok_t = get_token_type(tok);
    if ((tok_t == 30LL || tok_t == 31LL)) {
    cf_loop = 0LL;
    } else {
    if (tok_t == 28LL) {
    dummy = advance_token(state);
    } else {
    fname_tok = advance_token(state);
    fname = get_token_value(fname_tok);
    ok = expect_token_type(state, 72LL);
    fval = (parse_expr(state, 0LL) + 0LL);
    fpair = (create_list() + 0LL);
    ok = append_list(fpair, fname);
    ok = append_list(fpair, fval);
    ok = append_list(fields, fpair);
    }
    }
    }
    tok_ded = peek_token(state);
    if (get_token_type(tok_ded) == 30LL) {
    dummy = advance_token(state);
    }
    }
    ret_val = make_node_struct_create(struct_name, fields);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(11);
    return ret_val;
}

long long parse_list_literal(long long state) {
    long long entered = 0;
    long long elements = 0;
    long long next = 0;
    long long elem = 0;
    long long ok = 0;
    long long el_loop = 0;
    long long nt = 0;
    long long dummy = 0;
    long long left_list = 0;
    long long ret_val = 0;

    ep_gc_push_root(&elements);
    ep_gc_push_root(&next);
    ep_gc_push_root(&elem);
    ep_gc_push_root(&state);
    ep_gc_maybe_collect();

    entered = enter_call_args(state);
    elements = (create_list() + 0LL);
    next = peek_token(state);
    if (get_token_type(next) != 70LL) {
    elem = (parse_expr(state, 0LL) + 0LL);
    ok = append_list(elements, elem);
    el_loop = 1LL;
    while (el_loop == 1LL) {
    next = peek_token(state);
    nt = get_token_type(next);
    if (nt == 71LL) {
    dummy = advance_token(state);
    elem = (parse_expr(state, 0LL) + 0LL);
    ok = append_list(elements, elem);
    } else {
    if (nt == 11LL) {
    dummy = advance_token(state);
    elem = (parse_expr(state, 0LL) + 0LL);
    ok = append_list(elements, elem);
    } else {
    el_loop = 0LL;
    }
    }
    }
    }
    left_list = leave_call_args(state);
    ok = expect_token_type(state, 70LL);
    ret_val = make_node_list_lit(elements);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(4);
    return ret_val;
}

long long check_lit_category(long long expr) {
    long long t = 0;
    long long ret_val = 0;

    ep_gc_push_root(&expr);
    ep_gc_maybe_collect();

    t = get_list(expr, 0LL);
    if (t == 1LL) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    if (t == 42LL) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    if (t == 31LL) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    if (t == 2LL) {
    ret_val = 2LL;
    goto L_cleanup;
    }
    ret_val = 0LL;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(1);
    return ret_val;
}

long long check_expr(long long expr, long long errs, long long in_spawn_arg) {
    long long t = 0;
    long long ok = 0;
    long long elems = 0;
    long long n = 0;
    long long saw_str = 0;
    long long saw_num = 0;
    long long i = 0;
    long long el = 0;
    long long cat = 0;
    long long oke = 0;
    long long okl = 0;
    long long okr = 0;
    long long args = 0;
    long long an = 0;
    long long ai = 0;
    long long oka = 0;
    long long ret_val = 0;

    ep_gc_push_root(&elems);
    ep_gc_push_root(&i);
    ep_gc_push_root(&el);
    ep_gc_push_root(&args);
    ep_gc_push_root(&ai);
    ep_gc_push_root(&expr);
    ep_gc_push_root(&errs);
    ep_gc_maybe_collect();

    if (expr == 0LL) {
    ret_val = 0LL;
    goto L_cleanup;
    }
    t = get_list(expr, 0LL);
    if (t == 20LL) {
    if (in_spawn_arg == 1LL) {
    ok = append_list(errs, (long long)"Send safety (E0036): a borrowed reference is not Send and cannot be sent to a spawned thread");
    }
    ret_val = check_expr(get_list(expr, 1LL), errs, 0LL);
    goto L_cleanup;
    }
    if (t == 35LL) {
    elems = get_list(expr, 1LL);
    n = length_list(elems);
    saw_str = 0LL;
    saw_num = 0LL;
    i = 0LL;
    while (i < n) {
    el = get_list(elems, i);
    cat = check_lit_category(el);
    if (cat == 1LL) {
    saw_num = 1LL;
    }
    if (cat == 2LL) {
    saw_str = 1LL;
    }
    oke = check_expr(el, errs, 0LL);
    i = (i + 1LL);
    }
    if (saw_str == 1LL) {
    if (saw_num == 1LL) {
    ok = append_list(errs, (long long)"list elements have conflicting types (string mixed with non-string)");
    }
    }
    ret_val = 0LL;
    goto L_cleanup;
    }
    if (((t == 4LL || t == 5LL) || t == 14LL)) {
    okl = check_expr(get_list(expr, 1LL), errs, 0LL);
    okr = check_expr(get_list(expr, 3LL), errs, 0LL);
    ret_val = 0LL;
    goto L_cleanup;
    }
    if (t == 6LL) {
    args = get_list(expr, 2LL);
    an = length_list(args);
    ai = 0LL;
    while (ai < an) {
    oka = check_expr(get_list(args, ai), errs, 0LL);
    ai = (ai + 1LL);
    }
    ret_val = 0LL;
    goto L_cleanup;
    }
    if ((((t == 18LL || t == 21LL) || t == 32LL) || t == 33LL)) {
    ret_val = check_expr(get_list(expr, 1LL), errs, 0LL);
    goto L_cleanup;
    }
    ret_val = 0LL;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(7);
    return ret_val;
}

long long check_stmts(long long stmts, long long errs) {
    long long n = 0;
    long long idx = 0;
    long long stmt = 0;
    long long t = 0;
    long long tgt = 0;
    long long ok = 0;
    long long okv = 0;
    long long okr = 0;
    long long okc = 0;
    long long okt = 0;
    long long eb = 0;
    long long oke = 0;
    long long okw = 0;
    long long okwb = 0;
    long long sargs = 0;
    long long sn = 0;
    long long si = 0;
    long long oks = 0;
    long long okn = 0;
    long long okfe = 0;
    long long okfeb = 0;
    long long arms = 0;
    long long arn = 0;
    long long ari = 0;
    long long okam = 0;
    long long ret_val = 0;

    ep_gc_push_root(&idx);
    ep_gc_push_root(&stmt);
    ep_gc_push_root(&tgt);
    ep_gc_push_root(&eb);
    ep_gc_push_root(&sargs);
    ep_gc_push_root(&si);
    ep_gc_push_root(&arms);
    ep_gc_push_root(&ari);
    ep_gc_push_root(&stmts);
    ep_gc_push_root(&errs);
    ep_gc_maybe_collect();

    n = length_list(stmts);
    idx = 0LL;
    while (idx < n) {
    stmt = get_list(stmts, idx);
    t = get_list(stmt, 0LL);
    if (t == 7LL) {
    tgt = string_concat(get_list(stmt, 1LL), (long long)"");
    if ((strcmp((char*)tgt, (char*)(long long)"channel") == 0)) {
    ok = append_list(errs, (long long)"cannot shadow the reserved keyword 'channel' (use it as a channel, not a variable name)");
    }
    okv = check_expr(get_list(stmt, 2LL), errs, 0LL);
    }
    if (((t == 8LL || t == 9LL) || t == 36LL)) {
    okr = check_expr(get_list(stmt, 1LL), errs, 0LL);
    }
    if (t == 10LL) {
    okc = check_expr(get_list(stmt, 1LL), errs, 0LL);
    okt = check_stmts(get_list(stmt, 2LL), errs);
    eb = get_list(stmt, 3LL);
    if (eb != 0LL) {
    oke = check_stmts(eb, errs);
    }
    }
    if (t == 11LL) {
    okw = check_expr(get_list(stmt, 1LL), errs, 0LL);
    okwb = check_stmts(get_list(stmt, 2LL), errs);
    }
    if (t == 15LL) {
    sargs = get_list(stmt, 2LL);
    sn = length_list(sargs);
    si = 0LL;
    while (si < sn) {
    oks = check_expr(get_list(sargs, si), errs, 1LL);
    si = (si + 1LL);
    }
    }
    if (t == 16LL) {
    okn = check_expr(get_list(stmt, 2LL), errs, 1LL);
    }
    if (t == 28LL) {
    okfe = check_expr(get_list(stmt, 2LL), errs, 0LL);
    okfeb = check_stmts(get_list(stmt, 3LL), errs);
    }
    if (t == 27LL) {
    arms = get_list(stmt, 2LL);
    arn = length_list(arms);
    ari = 0LL;
    while (ari < arn) {
    okam = check_stmts(get_list(get_list(arms, ari), 2LL), errs);
    ari = (ari + 1LL);
    }
    }
    idx = (idx + 1LL);
    }
    ret_val = 0LL;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(10);
    return ret_val;
}

long long check_function(long long func, long long errs) {
    long long params = 0;
    long long pn = 0;
    long long pi = 0;
    long long pname = 0;
    long long ok = 0;
    long long okb = 0;
    long long ret_val = 0;

    ep_gc_push_root(&params);
    ep_gc_push_root(&pi);
    ep_gc_push_root(&pname);
    ep_gc_push_root(&func);
    ep_gc_push_root(&errs);
    ep_gc_maybe_collect();

    params = get_list(func, 2LL);
    pn = length_list(params);
    pi = 0LL;
    while (pi < pn) {
    pname = string_concat(get_list(get_list(params, pi), 0LL), (long long)"");
    if ((strcmp((char*)pname, (char*)(long long)"channel") == 0)) {
    ok = append_list(errs, (long long)"cannot use the reserved keyword 'channel' as a parameter name");
    }
    pi = (pi + 1LL);
    }
    okb = check_stmts(get_list(func, 3LL), errs);
    ret_val = 0LL;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(5);
    return ret_val;
}

long long en_arg_type(long long arg, long long vk, long long vo) {
    long long t = 0;
    long long vn = 0;
    long long i = 0;
    long long ret_val = 0;

    ep_gc_push_root(&vn);
    ep_gc_push_root(&i);
    ep_gc_push_root(&arg);
    ep_gc_push_root(&vk);
    ep_gc_push_root(&vo);
    ep_gc_maybe_collect();

    if (arg == 0LL) {
    ret_val = (long long)"";
    goto L_cleanup;
    }
    t = get_list(arg, 0LL);
    if (t == 26LL) {
    vn = string_concat(get_list(arg, 1LL), (long long)"");
    i = 0LL;
    while (i < length_list(vk)) {
    if ((strcmp((char*)vn, (char*)get_list(vk, i)) == 0)) {
    ret_val = get_list(vo, i);
    goto L_cleanup;
    }
    i = (i + 1LL);
    }
    ret_val = (long long)"";
    goto L_cleanup;
    }
    if (t == 1LL) {
    ret_val = (long long)"Int";
    goto L_cleanup;
    }
    if (t == 2LL) {
    ret_val = (long long)"Str";
    goto L_cleanup;
    }
    if (t == 42LL) {
    ret_val = (long long)"Float";
    goto L_cleanup;
    }
    if (t == 31LL) {
    ret_val = (long long)"Bool";
    goto L_cleanup;
    }
    ret_val = (long long)"";
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(5);
    return ret_val;
}

long long en_field_type_at(long long variant, long long ai, long long vk, long long vf) {
    long long vn = 0;
    long long i = 0;
    long long ret_val = 0;

    ep_gc_push_root(&vn);
    ep_gc_push_root(&i);
    ep_gc_push_root(&variant);
    ep_gc_push_root(&ai);
    ep_gc_push_root(&vk);
    ep_gc_push_root(&vf);
    ep_gc_maybe_collect();

    vn = string_concat(variant, (long long)"");
    i = 0LL;
    while (i < length_list(vk)) {
    if ((strcmp((char*)vn, (char*)get_list(vk, i)) == 0)) {
    if (ai < length_list(get_list(vf, i))) {
    ret_val = string_concat(get_list(get_list(vf, i), ai), (long long)"");
    goto L_cleanup;
    }
    ret_val = (long long)"";
    goto L_cleanup;
    }
    i = (i + 1LL);
    }
    ret_val = (long long)"";
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(6);
    return ret_val;
}

long long en_type_conflict(long long dt, long long arg, long long vk, long long vo) {
    long long dts = 0;
    long long at = 0;
    long long ret_val = 0;

    ep_gc_push_root(&dts);
    ep_gc_push_root(&at);
    ep_gc_push_root(&dt);
    ep_gc_push_root(&arg);
    ep_gc_push_root(&vk);
    ep_gc_push_root(&vo);
    ep_gc_maybe_collect();

    dts = string_concat(dt, (long long)"");
    if (string_length((char*)dts) == 0LL) {
    ret_val = 0LL;
    goto L_cleanup;
    }
    at = string_concat(en_arg_type(arg, vk, vo), (long long)"");
    if (string_length((char*)at) == 0LL) {
    ret_val = 0LL;
    goto L_cleanup;
    }
    if ((strcmp((char*)dts, (char*)(long long)"Str") == 0)) {
    if ((strcmp((char*)at, (char*)(long long)"Int") == 0)) {
    if (get_list(arg, 0LL) == 1LL) {
    if (get_list(arg, 1LL) == 0LL) {
    ret_val = 0LL;
    goto L_cleanup;
    }
    }
    ret_val = 1LL;
    goto L_cleanup;
    }
    if ((strcmp((char*)at, (char*)(long long)"Float") == 0)) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    if ((strcmp((char*)at, (char*)(long long)"Bool") == 0)) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    ret_val = 0LL;
    goto L_cleanup;
    }
    if ((strcmp((char*)at, (char*)(long long)"Str") == 0)) {
    if ((strcmp((char*)dts, (char*)(long long)"Int") == 0)) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    if ((strcmp((char*)dts, (char*)(long long)"Float") == 0)) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    if ((strcmp((char*)dts, (char*)(long long)"Bool") == 0)) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    }
    ret_val = 0LL;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(6);
    return ret_val;
}

long long en_check_expr(long long expr, long long errs, long long vk, long long vo, long long vf, long long fk, long long fp) {
    long long t = 0;
    long long variant = 0;
    long long args = 0;
    long long ai = 0;
    long long arg = 0;
    long long ft = 0;
    long long at = 0;
    long long noop = 0;
    long long msg = 0;
    long long ok = 0;
    long long oka = 0;
    long long bl = 0;
    long long bop = 0;
    long long br = 0;
    long long blt = 0;
    long long brt = 0;
    long long castok = 0;
    long long castok2 = 0;
    long long okl = 0;
    long long okr = 0;
    long long cname = 0;
    long long cargs = 0;
    long long fi = 0;
    long long pi = 0;
    long long dt = 0;
    long long amsg = 0;
    long long ci = 0;
    long long okc = 0;
    long long ret_val = 0;

    ep_gc_push_root(&variant);
    ep_gc_push_root(&args);
    ep_gc_push_root(&ai);
    ep_gc_push_root(&arg);
    ep_gc_push_root(&ft);
    ep_gc_push_root(&at);
    ep_gc_push_root(&msg);
    ep_gc_push_root(&bl);
    ep_gc_push_root(&br);
    ep_gc_push_root(&blt);
    ep_gc_push_root(&brt);
    ep_gc_push_root(&cname);
    ep_gc_push_root(&cargs);
    ep_gc_push_root(&fi);
    ep_gc_push_root(&pi);
    ep_gc_push_root(&dt);
    ep_gc_push_root(&amsg);
    ep_gc_push_root(&ci);
    ep_gc_push_root(&expr);
    ep_gc_push_root(&errs);
    ep_gc_push_root(&vk);
    ep_gc_push_root(&vo);
    ep_gc_push_root(&vf);
    ep_gc_push_root(&fk);
    ep_gc_push_root(&fp);
    ep_gc_maybe_collect();

    if (expr == 0LL) {
    ret_val = 0LL;
    goto L_cleanup;
    }
    t = get_list(expr, 0LL);
    if (t == 26LL) {
    variant = get_list(expr, 1LL);
    args = get_list(expr, 2LL);
    ai = 0LL;
    while (ai < length_list(args)) {
    arg = get_list(args, ai);
    ft = string_concat(en_field_type_at(variant, ai, vk, vf), (long long)"");
    if (string_length((char*)ft) > 0LL) {
    at = string_concat(en_arg_type(arg, vk, vo), (long long)"");
    if (string_length((char*)at) > 0LL) {
    if ((strcmp((char*)at, (char*)ft) == 0)) {
    noop = 0LL;
    } else {
    msg = string_concat((long long)"enum variant field type mismatch: expected ", ft);
    msg = string_concat(msg, (long long)" but got ");
    msg = string_concat(msg, at);
    ok = append_list(errs, msg);
    }
    }
    }
    oka = en_check_expr(arg, errs, vk, vo, vf, fk, fp);
    ai = (ai + 1LL);
    }
    ret_val = 0LL;
    goto L_cleanup;
    }
    if (t == 4LL) {
    bl = get_list(expr, 1LL);
    bop = get_list(expr, 2LL);
    br = get_list(expr, 3LL);
    blt = string_concat(en_arg_type(bl, vk, vo), (long long)"");
    brt = string_concat(en_arg_type(br, vk, vo), (long long)"");
    if ((strcmp((char*)blt, (char*)(long long)"Str") == 0)) {
    castok = 0LL;
    if (bop == 1LL) {
    if (get_list(br, 0LL) == 1LL) {
    if (get_list(br, 1LL) == 0LL) {
    castok = 1LL;
    }
    }
    }
    if (castok == 0LL) {
    ok = append_list(errs, (long long)"arithmetic on a string: a Str operand is only allowed in the '+ 0' int-cast idiom");
    }
    }
    if ((strcmp((char*)brt, (char*)(long long)"Str") == 0)) {
    castok2 = 0LL;
    if (bop == 1LL) {
    if (get_list(bl, 0LL) == 1LL) {
    if (get_list(bl, 1LL) == 0LL) {
    castok2 = 1LL;
    }
    }
    }
    if (castok2 == 0LL) {
    ok = append_list(errs, (long long)"arithmetic on a string: a Str operand is only allowed in the '+ 0' int-cast idiom");
    }
    }
    okl = en_check_expr(bl, errs, vk, vo, vf, fk, fp);
    okr = en_check_expr(br, errs, vk, vo, vf, fk, fp);
    ret_val = 0LL;
    goto L_cleanup;
    }
    if ((t == 5LL || t == 14LL)) {
    okl = en_check_expr(get_list(expr, 1LL), errs, vk, vo, vf, fk, fp);
    okr = en_check_expr(get_list(expr, 3LL), errs, vk, vo, vf, fk, fp);
    ret_val = 0LL;
    goto L_cleanup;
    }
    if (t == 6LL) {
    cname = string_concat(get_list(expr, 1LL), (long long)"");
    cargs = get_list(expr, 2LL);
    fi = 0LL;
    while (fi < length_list(fk)) {
    if ((strcmp((char*)cname, (char*)get_list(fk, fi)) == 0)) {
    pi = 0LL;
    while (pi < length_list(cargs)) {
    if (pi < length_list(get_list(fp, fi))) {
    dt = string_concat(get_list(get_list(fp, fi), pi), (long long)"");
    if (en_type_conflict(dt, get_list(cargs, pi), vk, vo) == 1LL) {
    amsg = string_concat((long long)"argument type mismatch in call to '", cname);
    amsg = string_concat(amsg, (long long)"': expected ");
    amsg = string_concat(amsg, dt);
    amsg = string_concat(amsg, (long long)" but got ");
    amsg = string_concat(amsg, en_arg_type(get_list(cargs, pi), vk, vo));
    ok = append_list(errs, amsg);
    }
    }
    pi = (pi + 1LL);
    }
    fi = length_list(fk);
    } else {
    fi = (fi + 1LL);
    }
    }
    ci = 0LL;
    while (ci < length_list(cargs)) {
    okc = en_check_expr(get_list(cargs, ci), errs, vk, vo, vf, fk, fp);
    ci = (ci + 1LL);
    }
    ret_val = 0LL;
    goto L_cleanup;
    }
    if (((((t == 18LL || t == 20LL) || t == 21LL) || t == 32LL) || t == 33LL)) {
    ret_val = en_check_expr(get_list(expr, 1LL), errs, vk, vo, vf, fk, fp);
    goto L_cleanup;
    }
    ret_val = 0LL;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(25);
    return ret_val;
}

long long en_check_stmts(long long stmts, long long errs, long long vk, long long vo, long long vf, long long fk, long long fp, long long drt) {
    long long i = 0;
    long long stmt = 0;
    long long t = 0;
    long long ok = 0;
    long long rmsg = 0;
    long long eb = 0;
    long long arms = 0;
    long long ari = 0;
    long long ret_val = 0;

    ep_gc_push_root(&i);
    ep_gc_push_root(&stmt);
    ep_gc_push_root(&rmsg);
    ep_gc_push_root(&eb);
    ep_gc_push_root(&arms);
    ep_gc_push_root(&ari);
    ep_gc_push_root(&stmts);
    ep_gc_push_root(&errs);
    ep_gc_push_root(&vk);
    ep_gc_push_root(&vo);
    ep_gc_push_root(&vf);
    ep_gc_push_root(&fk);
    ep_gc_push_root(&fp);
    ep_gc_push_root(&drt);
    ep_gc_maybe_collect();

    i = 0LL;
    while (i < length_list(stmts)) {
    stmt = get_list(stmts, i);
    t = get_list(stmt, 0LL);
    if (t == 7LL) {
    ok = en_check_expr(get_list(stmt, 2LL), errs, vk, vo, vf, fk, fp);
    }
    if (t == 8LL) {
    if (en_type_conflict(drt, get_list(stmt, 1LL), vk, vo) == 1LL) {
    rmsg = string_concat((long long)"return type mismatch: function is declared to return ", drt);
    rmsg = string_concat(rmsg, (long long)" but returns ");
    rmsg = string_concat(rmsg, en_arg_type(get_list(stmt, 1LL), vk, vo));
    ok = append_list(errs, rmsg);
    }
    }
    if (((t == 8LL || t == 9LL) || t == 36LL)) {
    ok = en_check_expr(get_list(stmt, 1LL), errs, vk, vo, vf, fk, fp);
    }
    if (t == 10LL) {
    ok = en_check_expr(get_list(stmt, 1LL), errs, vk, vo, vf, fk, fp);
    ok = en_check_stmts(get_list(stmt, 2LL), errs, vk, vo, vf, fk, fp, drt);
    eb = get_list(stmt, 3LL);
    if (eb != 0LL) {
    ok = en_check_stmts(eb, errs, vk, vo, vf, fk, fp, drt);
    }
    }
    if (t == 11LL) {
    ok = en_check_expr(get_list(stmt, 1LL), errs, vk, vo, vf, fk, fp);
    ok = en_check_stmts(get_list(stmt, 2LL), errs, vk, vo, vf, fk, fp, drt);
    }
    if (t == 28LL) {
    ok = en_check_expr(get_list(stmt, 2LL), errs, vk, vo, vf, fk, fp);
    ok = en_check_stmts(get_list(stmt, 3LL), errs, vk, vo, vf, fk, fp, drt);
    }
    if (t == 27LL) {
    arms = get_list(stmt, 2LL);
    ari = 0LL;
    while (ari < length_list(arms)) {
    ok = en_check_stmts(get_list(get_list(arms, ari), 2LL), errs, vk, vo, vf, fk, fp, drt);
    ari = (ari + 1LL);
    }
    }
    i = (i + 1LL);
    }
    ret_val = 0LL;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(14);
    return ret_val;
}

long long check_program(long long program) {
    long long errs = 0;
    long long en_vk = 0;
    long long en_vo = 0;
    long long en_vf = 0;
    long long enums = 0;
    long long ei = 0;
    long long edef = 0;
    long long ename = 0;
    long long evs = 0;
    long long evi = 0;
    long long ev = 0;
    long long fts = 0;
    long long fields = 0;
    long long fi = 0;
    long long ok = 0;
    long long en_fk = 0;
    long long en_fp = 0;
    long long en_funcs = 0;
    long long bfi = 0;
    long long bfn = 0;
    long long pts = 0;
    long long bpl = 0;
    long long bpi = 0;
    long long ptname = 0;
    long long efi = 0;
    long long efn = 0;
    long long edrt = 0;
    long long en_methods = 0;
    long long emi = 0;
    long long funcs = 0;
    long long n = 0;
    long long idx = 0;
    long long okf = 0;
    long long methods = 0;
    long long mn = 0;
    long long mi = 0;
    long long mdef = 0;
    long long okm = 0;
    long long e_len = 0;
    long long ret_val = 0;

    ep_gc_push_root(&errs);
    ep_gc_push_root(&en_vk);
    ep_gc_push_root(&en_vo);
    ep_gc_push_root(&en_vf);
    ep_gc_push_root(&enums);
    ep_gc_push_root(&ei);
    ep_gc_push_root(&edef);
    ep_gc_push_root(&ename);
    ep_gc_push_root(&evs);
    ep_gc_push_root(&evi);
    ep_gc_push_root(&ev);
    ep_gc_push_root(&fts);
    ep_gc_push_root(&fields);
    ep_gc_push_root(&fi);
    ep_gc_push_root(&en_fk);
    ep_gc_push_root(&en_fp);
    ep_gc_push_root(&en_funcs);
    ep_gc_push_root(&bfi);
    ep_gc_push_root(&bfn);
    ep_gc_push_root(&pts);
    ep_gc_push_root(&bpl);
    ep_gc_push_root(&bpi);
    ep_gc_push_root(&ptname);
    ep_gc_push_root(&efi);
    ep_gc_push_root(&efn);
    ep_gc_push_root(&edrt);
    ep_gc_push_root(&en_methods);
    ep_gc_push_root(&emi);
    ep_gc_push_root(&funcs);
    ep_gc_push_root(&idx);
    ep_gc_push_root(&methods);
    ep_gc_push_root(&mi);
    ep_gc_push_root(&mdef);
    ep_gc_push_root(&program);
    ep_gc_maybe_collect();

    errs = create_list();
    en_vk = create_list();
    en_vo = create_list();
    en_vf = create_list();
    if (length_list(program) > 5LL) {
    enums = get_list(program, 5LL);
    ei = 0LL;
    while (ei < length_list(enums)) {
    edef = get_list(enums, ei);
    ename = get_list(edef, 1LL);
    evs = get_list(edef, 2LL);
    evi = 0LL;
    while (evi < length_list(evs)) {
    ev = get_list(evs, evi);
    fts = create_list();
    fields = get_list(ev, 1LL);
    fi = 0LL;
    while (fi < length_list(fields)) {
    ok = append_list(fts, get_list(get_list(fields, fi), 1LL));
    fi = (fi + 1LL);
    }
    ok = append_list(en_vk, get_list(ev, 0LL));
    ok = append_list(en_vo, ename);
    ok = append_list(en_vf, fts);
    evi = (evi + 1LL);
    }
    ei = (ei + 1LL);
    }
    }
    en_fk = create_list();
    en_fp = create_list();
    en_funcs = get_list(program, 3LL);
    bfi = 0LL;
    while (bfi < length_list(en_funcs)) {
    bfn = get_list(en_funcs, bfi);
    pts = create_list();
    bpl = get_list(bfn, 2LL);
    bpi = 0LL;
    while (bpi < length_list(bpl)) {
    ptname = (long long)"";
    if (length_list(get_list(bpl, bpi)) > 2LL) {
    ptname = get_list(get_list(bpl, bpi), 2LL);
    }
    ok = append_list(pts, ptname);
    bpi = (bpi + 1LL);
    }
    ok = append_list(en_fk, get_list(bfn, 1LL));
    ok = append_list(en_fp, pts);
    bfi = (bfi + 1LL);
    }
    efi = 0LL;
    while (efi < length_list(en_funcs)) {
    efn = get_list(en_funcs, efi);
    edrt = (long long)"";
    if (length_list(efn) > 5LL) {
    edrt = string_concat(get_list(efn, 5LL), (long long)"");
    }
    ok = en_check_stmts(get_list(efn, 3LL), errs, en_vk, en_vo, en_vf, en_fk, en_fp, edrt);
    efi = (efi + 1LL);
    }
    if (length_list(program) > 6LL) {
    en_methods = get_list(program, 6LL);
    emi = 0LL;
    while (emi < length_list(en_methods)) {
    ok = en_check_stmts(get_list(get_list(en_methods, emi), 4LL), errs, en_vk, en_vo, en_vf, en_fk, en_fp, (long long)"");
    emi = (emi + 1LL);
    }
    }
    funcs = get_list(program, 3LL);
    n = length_list(funcs);
    idx = 0LL;
    while (idx < n) {
    okf = check_function(get_list(funcs, idx), errs);
    idx = (idx + 1LL);
    }
    if (length_list(program) > 6LL) {
    methods = get_list(program, 6LL);
    mn = length_list(methods);
    mi = 0LL;
    while (mi < mn) {
    mdef = get_list(methods, mi);
    okm = check_stmts(get_list(mdef, 4LL), errs);
    mi = (mi + 1LL);
    }
    }
    e_len = length_list(errs);
    if (e_len == 0LL) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    ei = 0LL;
    while (ei < e_len) {
    printf("%s\n", (char*)concat((long long)"Type Error: ", get_list(errs, ei)));
    ei = (ei + 1LL);
    }
    ret_val = 0LL;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(34);
    return ret_val;
}

long long opt_fold_expr(long long expr) {
    long long t = 0;
    long long left = 0;
    long long right = 0;
    long long ok1 = 0;
    long long ok3 = 0;
    long long op = 0;
    long long lv = 0;
    long long rv = 0;
    long long folded = 0;
    long long res = 0;
    long long ok5l = 0;
    long long ok5r = 0;
    long long args = 0;
    long long an = 0;
    long long ai = 0;
    long long okca = 0;
    long long ok1c = 0;
    long long elems = 0;
    long long en = 0;
    long long ei = 0;
    long long okle = 0;
    long long ret_val = 0;

    ep_gc_push_root(&left);
    ep_gc_push_root(&right);
    ep_gc_push_root(&res);
    ep_gc_push_root(&args);
    ep_gc_push_root(&ai);
    ep_gc_push_root(&elems);
    ep_gc_push_root(&ei);
    ep_gc_push_root(&expr);
    ep_gc_maybe_collect();

    if (expr == 0LL) {
    ret_val = 0LL;
    goto L_cleanup;
    }
    t = get_list(expr, 0LL);
    if (t == 4LL) {
    left = opt_fold_expr(get_list(expr, 1LL));
    right = opt_fold_expr(get_list(expr, 3LL));
    ok1 = set_list(expr, 1LL, left);
    ok3 = set_list(expr, 3LL, right);
    op = get_list(expr, 2LL);
    if (get_list(left, 0LL) == 1LL) {
    if (get_list(right, 0LL) == 1LL) {
    lv = get_list(left, 1LL);
    rv = get_list(right, 1LL);
    folded = 0LL;
    res = 0LL;
    if (op == 1LL) {
    res = (lv + rv);
    folded = 1LL;
    }
    if (op == 2LL) {
    res = (lv - rv);
    folded = 1LL;
    }
    if (op == 3LL) {
    res = (lv * rv);
    folded = 1LL;
    }
    if (op == 4LL) {
    if (rv != 0LL) {
    res = (lv / rv);
    folded = 1LL;
    }
    }
    if (op == 5LL) {
    if (rv != 0LL) {
    res = (lv - ((lv / rv) * rv));
    folded = 1LL;
    }
    }
    if (folded == 1LL) {
    ret_val = (make_node_int(res) + 0LL);
    goto L_cleanup;
    }
    }
    }
    ret_val = expr;
    expr = 0;
    goto L_cleanup;
    }
    if ((t == 5LL || t == 14LL)) {
    ok5l = set_list(expr, 1LL, opt_fold_expr(get_list(expr, 1LL)));
    ok5r = set_list(expr, 3LL, opt_fold_expr(get_list(expr, 3LL)));
    ret_val = expr;
    expr = 0;
    goto L_cleanup;
    }
    if (t == 6LL) {
    args = get_list(expr, 2LL);
    an = length_list(args);
    ai = 0LL;
    while (ai < an) {
    okca = set_list(args, ai, opt_fold_expr(get_list(args, ai)));
    ai = (ai + 1LL);
    }
    ret_val = expr;
    expr = 0;
    goto L_cleanup;
    }
    if ((((t == 18LL || t == 21LL) || t == 32LL) || t == 33LL)) {
    ok1c = set_list(expr, 1LL, opt_fold_expr(get_list(expr, 1LL)));
    ret_val = expr;
    expr = 0;
    goto L_cleanup;
    }
    if (t == 35LL) {
    elems = get_list(expr, 1LL);
    en = length_list(elems);
    ei = 0LL;
    while (ei < en) {
    okle = set_list(elems, ei, opt_fold_expr(get_list(elems, ei)));
    ei = (ei + 1LL);
    }
    ret_val = expr;
    expr = 0;
    goto L_cleanup;
    }
    ret_val = expr;
    expr = 0;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(8);
    return ret_val;
}

long long opt_fold_stmts(long long stmts) {
    long long n = 0;
    long long idx = 0;
    long long stmt = 0;
    long long t = 0;
    long long oks = 0;
    long long okr = 0;
    long long okc = 0;
    long long okt = 0;
    long long eb = 0;
    long long oke = 0;
    long long okw = 0;
    long long okwb = 0;
    long long okse = 0;
    long long okfe = 0;
    long long okfeb = 0;
    long long arms = 0;
    long long arn = 0;
    long long ari = 0;
    long long okam = 0;
    long long ret_val = 0;

    ep_gc_push_root(&idx);
    ep_gc_push_root(&stmt);
    ep_gc_push_root(&eb);
    ep_gc_push_root(&arms);
    ep_gc_push_root(&ari);
    ep_gc_push_root(&stmts);
    ep_gc_maybe_collect();

    n = length_list(stmts);
    idx = 0LL;
    while (idx < n) {
    stmt = get_list(stmts, idx);
    t = get_list(stmt, 0LL);
    if (t == 7LL) {
    oks = set_list(stmt, 2LL, opt_fold_expr(get_list(stmt, 2LL)));
    }
    if (((t == 8LL || t == 9LL) || t == 36LL)) {
    okr = set_list(stmt, 1LL, opt_fold_expr(get_list(stmt, 1LL)));
    }
    if (t == 10LL) {
    okc = set_list(stmt, 1LL, opt_fold_expr(get_list(stmt, 1LL)));
    okt = opt_fold_stmts(get_list(stmt, 2LL));
    eb = get_list(stmt, 3LL);
    if (eb != 0LL) {
    oke = opt_fold_stmts(eb);
    }
    }
    if (t == 11LL) {
    okw = set_list(stmt, 1LL, opt_fold_expr(get_list(stmt, 1LL)));
    okwb = opt_fold_stmts(get_list(stmt, 2LL));
    }
    if (t == 16LL) {
    okse = set_list(stmt, 2LL, opt_fold_expr(get_list(stmt, 2LL)));
    }
    if (t == 28LL) {
    okfe = set_list(stmt, 2LL, opt_fold_expr(get_list(stmt, 2LL)));
    okfeb = opt_fold_stmts(get_list(stmt, 3LL));
    }
    if (t == 27LL) {
    arms = get_list(stmt, 2LL);
    arn = length_list(arms);
    ari = 0LL;
    while (ari < arn) {
    okam = opt_fold_stmts(get_list(get_list(arms, ari), 2LL));
    ari = (ari + 1LL);
    }
    }
    idx = (idx + 1LL);
    }
    ret_val = 0LL;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(6);
    return ret_val;
}

long long optimize_program(long long program) {
    long long funcs = 0;
    long long n = 0;
    long long idx = 0;
    long long okf = 0;
    long long methods = 0;
    long long mn = 0;
    long long mi = 0;
    long long okm = 0;
    long long ret_val = 0;

    ep_gc_push_root(&funcs);
    ep_gc_push_root(&idx);
    ep_gc_push_root(&methods);
    ep_gc_push_root(&mi);
    ep_gc_push_root(&program);
    ep_gc_maybe_collect();

    funcs = get_list(program, 3LL);
    n = length_list(funcs);
    idx = 0LL;
    while (idx < n) {
    okf = opt_fold_stmts(get_list(get_list(funcs, idx), 3LL));
    idx = (idx + 1LL);
    }
    if (length_list(program) > 6LL) {
    methods = get_list(program, 6LL);
    mn = length_list(methods);
    mi = 0LL;
    while (mi < mn) {
    okm = opt_fold_stmts(get_list(get_list(methods, mi), 4LL));
    mi = (mi + 1LL);
    }
    }
    ret_val = 0LL;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(5);
    return ret_val;
}

long long map_get(long long keys, long long values, long long key) {
    long long key_str = 0;
    long long len = 0;
    long long idx = 0;
    long long k = 0;
    long long ret_val = 0;

    ep_gc_push_root(&key_str);
    ep_gc_push_root(&idx);
    ep_gc_push_root(&keys);
    ep_gc_push_root(&values);
    ep_gc_push_root(&key);
    ep_gc_maybe_collect();

    key_str = string_concat(key, (long long)"");
    len = length_list(keys);
    idx = 0LL;
    while (idx < len) {
    k = get_list(keys, idx);
    if ((strcmp((char*)key_str, (char*)k) == 0)) {
    ret_val = get_list(values, idx);
    goto L_cleanup;
    }
    idx = (idx + 1LL);
    }
    ret_val = 0LL;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(5);
    return ret_val;
}

long long map_contains_key(long long keys, long long key) {
    long long key_str = 0;
    long long len = 0;
    long long idx = 0;
    long long ret_val = 0;

    ep_gc_push_root(&key_str);
    ep_gc_push_root(&idx);
    ep_gc_push_root(&keys);
    ep_gc_push_root(&key);
    ep_gc_maybe_collect();

    key_str = string_concat(key, (long long)"");
    len = length_list(keys);
    idx = 0LL;
    while (idx < len) {
    if ((strcmp((char*)key_str, (char*)get_list(keys, idx)) == 0)) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    idx = (idx + 1LL);
    }
    ret_val = 0LL;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(4);
    return ret_val;
}

long long collect_idents_expr(long long expr, long long out_names) {
    long long t = 0;
    long long ok = 0;
    long long okl = 0;
    long long okr = 0;
    long long args = 0;
    long long a_len = 0;
    long long a_i = 0;
    long long oka = 0;
    long long fields = 0;
    long long f_len = 0;
    long long f_i = 0;
    long long fpair = 0;
    long long okf = 0;
    long long okm = 0;
    long long margs = 0;
    long long m_len = 0;
    long long m_i = 0;
    long long okma = 0;
    long long eargs = 0;
    long long e_len = 0;
    long long e_i = 0;
    long long oke = 0;
    long long elems = 0;
    long long l_len = 0;
    long long l_i = 0;
    long long okl2 = 0;
    long long ret_val = 0;

    ep_gc_push_root(&args);
    ep_gc_push_root(&a_i);
    ep_gc_push_root(&fields);
    ep_gc_push_root(&f_i);
    ep_gc_push_root(&fpair);
    ep_gc_push_root(&margs);
    ep_gc_push_root(&m_i);
    ep_gc_push_root(&eargs);
    ep_gc_push_root(&e_i);
    ep_gc_push_root(&elems);
    ep_gc_push_root(&l_i);
    ep_gc_push_root(&expr);
    ep_gc_push_root(&out_names);
    ep_gc_maybe_collect();

    if (expr == 0LL) {
    ret_val = 0LL;
    goto L_cleanup;
    }
    t = get_list(expr, 0LL);
    if (t == 3LL) {
    ok = append_list(out_names, get_list(expr, 1LL));
    ret_val = 0LL;
    goto L_cleanup;
    }
    if (((t == 4LL || t == 5LL) || t == 14LL)) {
    okl = collect_idents_expr(get_list(expr, 1LL), out_names);
    okr = collect_idents_expr(get_list(expr, 3LL), out_names);
    ret_val = 0LL;
    goto L_cleanup;
    }
    if (t == 6LL) {
    ok = append_list(out_names, get_list(expr, 1LL));
    args = create_list();
    args = get_list(expr, 2LL);
    a_len = length_list(args);
    a_i = 0LL;
    while (a_i < a_len) {
    oka = collect_idents_expr(get_list(args, a_i), out_names);
    a_i = (a_i + 1LL);
    }
    ret_val = 0LL;
    goto L_cleanup;
    }
    if (((((t == 18LL || t == 20LL) || t == 21LL) || t == 32LL) || t == 33LL)) {
    ret_val = collect_idents_expr(get_list(expr, 1LL), out_names);
    goto L_cleanup;
    }
    if (t == 22LL) {
    ret_val = collect_idents_expr(get_list(expr, 1LL), out_names);
    goto L_cleanup;
    }
    if (t == 24LL) {
    fields = get_list(expr, 2LL);
    f_len = length_list(fields);
    f_i = 0LL;
    while (f_i < f_len) {
    fpair = get_list(fields, f_i);
    okf = collect_idents_expr(get_list(fpair, 1LL), out_names);
    f_i = (f_i + 1LL);
    }
    ret_val = 0LL;
    goto L_cleanup;
    }
    if (t == 25LL) {
    okm = collect_idents_expr(get_list(expr, 1LL), out_names);
    margs = get_list(expr, 3LL);
    m_len = length_list(margs);
    m_i = 0LL;
    while (m_i < m_len) {
    okma = collect_idents_expr(get_list(margs, m_i), out_names);
    m_i = (m_i + 1LL);
    }
    ret_val = 0LL;
    goto L_cleanup;
    }
    if (t == 26LL) {
    eargs = get_list(expr, 2LL);
    e_len = length_list(eargs);
    e_i = 0LL;
    while (e_i < e_len) {
    oke = collect_idents_expr(get_list(eargs, e_i), out_names);
    e_i = (e_i + 1LL);
    }
    ret_val = 0LL;
    goto L_cleanup;
    }
    if (t == 34LL) {
    ret_val = collect_idents_stmts(get_list(expr, 2LL), out_names);
    goto L_cleanup;
    }
    if (t == 35LL) {
    elems = get_list(expr, 1LL);
    l_len = length_list(elems);
    l_i = 0LL;
    while (l_i < l_len) {
    okl2 = collect_idents_expr(get_list(elems, l_i), out_names);
    l_i = (l_i + 1LL);
    }
    ret_val = 0LL;
    goto L_cleanup;
    }
    ret_val = 0LL;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(13);
    return ret_val;
}

long long collect_idents_stmts(long long stmts, long long out_names) {
    long long len = 0;
    long long idx = 0;
    long long stmt = 0;
    long long t = 0;
    long long ok7 = 0;
    long long ok7b = 0;
    long long ok8 = 0;
    long long okc = 0;
    long long okt = 0;
    long long else_b = 0;
    long long oke2 = 0;
    long long okw = 0;
    long long okwb = 0;
    long long oks = 0;
    long long sargs = 0;
    long long s_len = 0;
    long long s_i = 0;
    long long oksa = 0;
    long long okn1 = 0;
    long long okn2 = 0;
    long long okf1 = 0;
    long long okf2 = 0;
    long long okm1 = 0;
    long long arms = 0;
    long long ar_len = 0;
    long long ar_i = 0;
    long long arm = 0;
    long long okab = 0;
    long long okfe1 = 0;
    long long okfe2 = 0;
    long long ret_val = 0;

    ep_gc_push_root(&idx);
    ep_gc_push_root(&stmt);
    ep_gc_push_root(&else_b);
    ep_gc_push_root(&sargs);
    ep_gc_push_root(&s_i);
    ep_gc_push_root(&arms);
    ep_gc_push_root(&ar_i);
    ep_gc_push_root(&arm);
    ep_gc_push_root(&stmts);
    ep_gc_push_root(&out_names);
    ep_gc_maybe_collect();

    len = length_list(stmts);
    idx = 0LL;
    while (idx < len) {
    stmt = get_list(stmts, idx);
    t = get_list(stmt, 0LL);
    if (t == 7LL) {
    ok7 = append_list(out_names, get_list(stmt, 1LL));
    ok7b = collect_idents_expr(get_list(stmt, 2LL), out_names);
    }
    if (((t == 8LL || t == 9LL) || t == 36LL)) {
    ok8 = collect_idents_expr(get_list(stmt, 1LL), out_names);
    }
    if (t == 10LL) {
    okc = collect_idents_expr(get_list(stmt, 1LL), out_names);
    okt = collect_idents_stmts(get_list(stmt, 2LL), out_names);
    else_b = get_list(stmt, 3LL);
    if (else_b != 0LL) {
    oke2 = collect_idents_stmts(else_b, out_names);
    }
    }
    if (t == 11LL) {
    okw = collect_idents_expr(get_list(stmt, 1LL), out_names);
    okwb = collect_idents_stmts(get_list(stmt, 2LL), out_names);
    }
    if (t == 15LL) {
    oks = append_list(out_names, get_list(stmt, 1LL));
    sargs = get_list(stmt, 2LL);
    s_len = length_list(sargs);
    s_i = 0LL;
    while (s_i < s_len) {
    oksa = collect_idents_expr(get_list(sargs, s_i), out_names);
    s_i = (s_i + 1LL);
    }
    }
    if (t == 16LL) {
    okn1 = collect_idents_expr(get_list(stmt, 1LL), out_names);
    okn2 = collect_idents_expr(get_list(stmt, 2LL), out_names);
    }
    if (t == 23LL) {
    okf1 = collect_idents_expr(get_list(stmt, 1LL), out_names);
    okf2 = collect_idents_expr(get_list(stmt, 3LL), out_names);
    }
    if (t == 27LL) {
    okm1 = collect_idents_expr(get_list(stmt, 1LL), out_names);
    arms = get_list(stmt, 2LL);
    ar_len = length_list(arms);
    ar_i = 0LL;
    while (ar_i < ar_len) {
    arm = get_list(arms, ar_i);
    okab = collect_idents_stmts(get_list(arm, 2LL), out_names);
    ar_i = (ar_i + 1LL);
    }
    }
    if (t == 28LL) {
    okfe1 = collect_idents_expr(get_list(stmt, 2LL), out_names);
    okfe2 = collect_idents_stmts(get_list(stmt, 3LL), out_names);
    }
    idx = (idx + 1LL);
    }
    ret_val = 0LL;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(10);
    return ret_val;
}

long long map_put(long long keys, long long values, long long key, long long val) {
    long long key_str = 0;
    long long len = 0;
    long long idx = 0;
    long long found = 0;
    long long k = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&key_str);
    ep_gc_push_root(&idx);
    ep_gc_push_root(&keys);
    ep_gc_push_root(&values);
    ep_gc_push_root(&key);
    ep_gc_push_root(&val);
    ep_gc_maybe_collect();

    key_str = string_concat(key, (long long)"");
    len = length_list(keys);
    idx = 0LL;
    found = 0LL;
    while ((idx < len && found == 0LL)) {
    k = get_list(keys, idx);
    if ((strcmp((char*)key_str, (char*)k) == 0)) {
    ok = set_list(values, idx, val);
    found = 1LL;
    }
    idx = (idx + 1LL);
    }
    if (found == 0LL) {
    ok = append_list(keys, key);
    ok = append_list(values, val);
    }
    ret_val = 0LL;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(6);
    return ret_val;
}

long long field_slot_index(long long seen, long long name) {
    long long name_str = 0;
    long long len = 0;
    long long idx = 0;
    long long k = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&name_str);
    ep_gc_push_root(&idx);
    ep_gc_push_root(&seen);
    ep_gc_push_root(&name);
    ep_gc_maybe_collect();

    name_str = string_concat(name, (long long)"");
    len = length_list(seen);
    idx = 0LL;
    while (idx < len) {
    k = get_list(seen, idx);
    if ((strcmp((char*)name_str, (char*)k) == 0)) {
    ret_val = idx;
    goto L_cleanup;
    }
    idx = (idx + 1LL);
    }
    ok = append_list(seen, name_str);
    ret_val = len;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(4);
    return ret_val;
}

long long string_concat(long long s1, long long s2) {
    long long lst = 0;
    long long len1 = 0;
    long long idx = 0;
    long long ok = 0;
    long long len2 = 0;
    long long idx2 = 0;
    long long res = 0;
    long long ret_val = 0;

    ep_gc_push_root(&lst);
    ep_gc_push_root(&idx);
    ep_gc_push_root(&idx2);
    ep_gc_push_root(&res);
    ep_gc_push_root(&s1);
    ep_gc_push_root(&s2);
    ep_gc_maybe_collect();

    lst = create_list();
    len1 = string_length((char*)s1);
    idx = 0LL;
    while (idx < len1) {
    ok = append_list(lst, get_character((char*)s1, idx));
    idx = (idx + 1LL);
    }
    len2 = string_length((char*)s2);
    idx2 = 0LL;
    while (idx2 < len2) {
    ok = append_list(lst, get_character((char*)s2, idx2));
    idx2 = (idx2 + 1LL);
    }
    res = (long long)string_from_list(lst);
    ret_val = res;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(6);
    return ret_val;
}

long long cg_sanitize_name(long long name) {
    long long n = 0;
    long long kws = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&n);
    ep_gc_push_root(&kws);
    ep_gc_push_root(&name);
    ep_gc_maybe_collect();

    n = string_concat(name, (long long)"");
    if ((strcmp((char*)(long long)"main", (char*)n) == 0)) {
    ret_val = (long long)"_main";
    goto L_cleanup;
    }
    kws = create_list();
    ok = append_list(kws, (long long)"auto");
    ok = append_list(kws, (long long)"break");
    ok = append_list(kws, (long long)"case");
    ok = append_list(kws, (long long)"char");
    ok = append_list(kws, (long long)"const");
    ok = append_list(kws, (long long)"continue");
    ok = append_list(kws, (long long)"default");
    ok = append_list(kws, (long long)"do");
    ok = append_list(kws, (long long)"double");
    ok = append_list(kws, (long long)"else");
    ok = append_list(kws, (long long)"enum");
    ok = append_list(kws, (long long)"extern");
    ok = append_list(kws, (long long)"float");
    ok = append_list(kws, (long long)"for");
    ok = append_list(kws, (long long)"goto");
    ok = append_list(kws, (long long)"if");
    ok = append_list(kws, (long long)"int");
    ok = append_list(kws, (long long)"long");
    ok = append_list(kws, (long long)"register");
    ok = append_list(kws, (long long)"return");
    ok = append_list(kws, (long long)"short");
    ok = append_list(kws, (long long)"signed");
    ok = append_list(kws, (long long)"sizeof");
    ok = append_list(kws, (long long)"static");
    ok = append_list(kws, (long long)"struct");
    ok = append_list(kws, (long long)"switch");
    ok = append_list(kws, (long long)"typedef");
    ok = append_list(kws, (long long)"union");
    ok = append_list(kws, (long long)"unsigned");
    ok = append_list(kws, (long long)"void");
    ok = append_list(kws, (long long)"volatile");
    ok = append_list(kws, (long long)"while");
    ok = append_list(kws, (long long)"inline");
    ok = append_list(kws, (long long)"restrict");
    ok = append_list(kws, (long long)"printf");
    ok = append_list(kws, (long long)"scanf");
    ok = append_list(kws, (long long)"malloc");
    ok = append_list(kws, (long long)"free");
    ok = append_list(kws, (long long)"exit");
    ok = append_list(kws, (long long)"read");
    ok = append_list(kws, (long long)"write");
    ok = append_list(kws, (long long)"open");
    ok = append_list(kws, (long long)"close");
    ok = append_list(kws, (long long)"time");
    ok = append_list(kws, (long long)"sleep");
    ok = append_list(kws, (long long)"select");
    ok = append_list(kws, (long long)"remove");
    if (contains_string_val(kws, n) == 1LL) {
    ret_val = string_concat((long long)"ep_", n);
    goto L_cleanup;
    }
    ret_val = n;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(3);
    return ret_val;
}

long long contains_string_val(long long list, long long s) {
    long long key = 0;
    long long n = 0;
    long long idx = 0;
    long long ret_val = 0;

    ep_gc_push_root(&key);
    ep_gc_push_root(&idx);
    ep_gc_push_root(&list);
    ep_gc_push_root(&s);
    ep_gc_maybe_collect();

    key = string_concat(s, (long long)"");
    n = length_list(list);
    idx = 0LL;
    while (idx < n) {
    if ((strcmp((char*)key, (char*)get_list(list, idx)) == 0)) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    idx = (idx + 1LL);
    }
    ret_val = 0LL;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(4);
    return ret_val;
}

long long get_fn_c_name(long long func) {
    long long ret_val = 0;

    ep_gc_push_root(&func);
    ep_gc_maybe_collect();

    ret_val = cg_sanitize_name(get_list(func, 1LL));
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(1);
    return ret_val;
}

long long cg_int_to_str(long long n) {
    long long neg = 0;
    long long lst = 0;
    long long temp = 0;
    long long digits = 0;
    long long digit = 0;
    long long ok = 0;
    long long len = 0;
    long long d = 0;
    long long res = 0;
    long long ret_val = 0;

    ep_gc_push_root(&lst);
    ep_gc_push_root(&digits);
    ep_gc_push_root(&digit);
    ep_gc_push_root(&d);
    ep_gc_push_root(&res);
    ep_gc_maybe_collect();

    if (n == 0LL) {
    ret_val = (long long)"0";
    goto L_cleanup;
    }
    neg = 0LL;
    if (n < 0LL) {
    neg = 1LL;
    n = (0LL - n);
    }
    lst = create_list();
    temp = n;
    digits = create_list();
    while (temp > 0LL) {
    digit = (temp - ((temp / 10LL) * 10LL));
    ok = append_list(digits, (digit + 48LL));
    temp = (temp / 10LL);
    }
    len = length_list(digits);
    while (len > 0LL) {
    d = pop_list(digits);
    ok = append_list(lst, d);
    len = (len - 1LL);
    }
    res = (long long)string_from_list(lst);
    if (neg == 1LL) {
    ret_val = string_concat((long long)"-", res);
    goto L_cleanup;
    }
    ret_val = res;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(5);
    return ret_val;
}

long long escape_string(long long s) {
    long long lst = 0;
    long long len = 0;
    long long idx = 0;
    long long ch = 0;
    long long ok = 0;
    long long res = 0;
    long long ret_val = 0;

    ep_gc_push_root(&lst);
    ep_gc_push_root(&idx);
    ep_gc_push_root(&ch);
    ep_gc_push_root(&res);
    ep_gc_push_root(&s);
    ep_gc_maybe_collect();

    lst = create_list();
    len = string_length((char*)s);
    idx = 0LL;
    while (idx < len) {
    ch = get_character((char*)s, idx);
    if (ch == 92LL) {
    ok = append_list(lst, 92LL);
    ok = append_list(lst, 92LL);
    } else {
    if (ch == 34LL) {
    ok = append_list(lst, 92LL);
    ok = append_list(lst, 34LL);
    } else {
    if (ch == 10LL) {
    ok = append_list(lst, 92LL);
    ok = append_list(lst, 110LL);
    } else {
    if (ch == 9LL) {
    ok = append_list(lst, 92LL);
    ok = append_list(lst, 116LL);
    } else {
    if (ch == 13LL) {
    ok = append_list(lst, 92LL);
    ok = append_list(lst, 114LL);
    } else {
    ok = append_list(lst, ch);
    }
    }
    }
    }
    }
    idx = (idx + 1LL);
    }
    res = (long long)string_from_list(lst);
    ret_val = res;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(5);
    return ret_val;
}

long long join_strings(long long lines) {
    long long lst = 0;
    long long len = 0;
    long long idx = 0;
    long long line = 0;
    long long line_len = 0;
    long long c_idx = 0;
    long long ok = 0;
    long long res = 0;
    long long ret_val = 0;

    ep_gc_push_root(&lst);
    ep_gc_push_root(&idx);
    ep_gc_push_root(&line);
    ep_gc_push_root(&c_idx);
    ep_gc_push_root(&res);
    ep_gc_push_root(&lines);
    ep_gc_maybe_collect();

    lst = create_list();
    len = length_list(lines);
    idx = 0LL;
    while (idx < len) {
    line = get_list(lines, idx);
    line_len = string_length((char*)line);
    c_idx = 0LL;
    while (c_idx < line_len) {
    ok = append_list(lst, get_character((char*)line, c_idx));
    c_idx = (c_idx + 1LL);
    }
    idx = (idx + 1LL);
    }
    res = (long long)string_from_list(lst);
    ret_val = res;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(6);
    return ret_val;
}

long long create_codegen_state() {
    long long state = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&state);
    ep_gc_maybe_collect();

    state = create_list();
    ok = append_list(state, (create_list() + 0LL));
    ok = append_list(state, (create_list() + 0LL));
    ok = append_list(state, 0LL);
    ok = append_list(state, (create_list() + 0LL));
    ok = append_list(state, (create_list() + 0LL));
    ok = append_list(state, 0LL);
    ok = append_list(state, (create_list() + 0LL));
    ok = append_list(state, 0LL);
    ok = append_list(state, (create_list() + 0LL));
    ok = append_list(state, (create_list() + 0LL));
    ok = append_list(state, 0LL);
    ok = append_list(state, (create_list() + 0LL));
    ok = append_list(state, (create_list() + 0LL));
    ok = append_list(state, 0LL);
    ok = append_list(state, (create_list() + 0LL));
    ok = append_list(state, (create_list() + 0LL));
    ok = append_list(state, (create_list() + 0LL));
    ok = append_list(state, (create_list() + 0LL));
    ok = append_list(state, (create_list() + 0LL));
    ok = append_list(state, 0LL);
    ok = append_list(state, 0LL);
    ok = append_list(state, (create_list() + 0LL));
    ok = append_list(state, (create_list() + 0LL));
    ok = append_list(state, (create_list() + 0LL));
    ret_val = state;
    state = 0;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(1);
    return ret_val;
}

long long count_awaits_expr(long long expr) {
    long long t = 0;
    long long args = 0;
    long long c = 0;
    long long i = 0;
    long long ret_val = 0;

    ep_gc_push_root(&args);
    ep_gc_push_root(&i);
    ep_gc_push_root(&expr);
    ep_gc_maybe_collect();

    if (expr == 0LL) {
    ret_val = 0LL;
    goto L_cleanup;
    }
    t = get_list(expr, 0LL);
    if (t == 21LL) {
    ret_val = (1LL + count_awaits_expr(get_list(expr, 1LL)));
    goto L_cleanup;
    }
    if (((t == 4LL || t == 5LL) || t == 14LL)) {
    ret_val = (count_awaits_expr(get_list(expr, 1LL)) + count_awaits_expr(get_list(expr, 3LL)));
    goto L_cleanup;
    }
    if ((((t == 20LL || t == 32LL) || t == 33LL) || t == 18LL)) {
    ret_val = count_awaits_expr(get_list(expr, 1LL));
    goto L_cleanup;
    }
    if (t == 6LL) {
    args = get_list(expr, 2LL);
    c = 0LL;
    i = 0LL;
    while (i < length_list(args)) {
    c = (c + count_awaits_expr(get_list(args, i)));
    i = (i + 1LL);
    }
    ret_val = c;
    goto L_cleanup;
    }
    ret_val = 0LL;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(3);
    return ret_val;
}

long long count_awaits_stmts(long long stmts) {
    long long c = 0;
    long long i = 0;
    long long stmt = 0;
    long long t = 0;
    long long eb = 0;
    long long ret_val = 0;

    ep_gc_push_root(&i);
    ep_gc_push_root(&stmt);
    ep_gc_push_root(&eb);
    ep_gc_push_root(&stmts);
    ep_gc_maybe_collect();

    c = 0LL;
    i = 0LL;
    while (i < length_list(stmts)) {
    stmt = get_list(stmts, i);
    t = get_list(stmt, 0LL);
    if (t == 7LL) {
    c = (c + count_awaits_expr(get_list(stmt, 2LL)));
    }
    if (((t == 8LL || t == 9LL) || t == 36LL)) {
    c = (c + count_awaits_expr(get_list(stmt, 1LL)));
    }
    if (t == 10LL) {
    c = (c + count_awaits_expr(get_list(stmt, 1LL)));
    c = (c + count_awaits_stmts(get_list(stmt, 2LL)));
    eb = get_list(stmt, 3LL);
    if (eb != 0LL) {
    c = (c + count_awaits_stmts(eb));
    }
    }
    if (t == 11LL) {
    c = (c + count_awaits_expr(get_list(stmt, 1LL)));
    c = (c + count_awaits_stmts(get_list(stmt, 2LL)));
    }
    if (t == 28LL) {
    c = (c + count_awaits_stmts(get_list(stmt, 3LL)));
    }
    i = (i + 1LL);
    }
    ret_val = c;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(4);
    return ret_val;
}

long long emit_async_yields_expr(long long state, long long expr, long long var_keys, long long var_values) {
    long long t = 0;
    long long ok = 0;
    long long n = 0;
    long long ns = 0;
    long long inner_str = 0;
    long long line = 0;
    long long args = 0;
    long long i = 0;
    long long ret_val = 0;

    ep_gc_push_root(&n);
    ep_gc_push_root(&ns);
    ep_gc_push_root(&inner_str);
    ep_gc_push_root(&line);
    ep_gc_push_root(&args);
    ep_gc_push_root(&i);
    ep_gc_push_root(&state);
    ep_gc_push_root(&expr);
    ep_gc_push_root(&var_keys);
    ep_gc_push_root(&var_values);
    ep_gc_maybe_collect();

    if (expr == 0LL) {
    ret_val = 0LL;
    goto L_cleanup;
    }
    t = get_list(expr, 0LL);
    if (t == 21LL) {
    ok = emit_async_yields_expr(state, get_list(expr, 1LL), var_keys, var_values);
    n = (get_list(state, 20LL) + 1LL);
    ok = set_list(state, 20LL, n);
    ns = cg_int_to_str(n);
    inner_str = gen_expr(state, get_list(expr, 1LL), var_keys, var_values);
    line = (long long)"            { EpFuture* _f = (EpFuture*)(";
    line = string_concat(line, inner_str);
    line = string_concat(line, (long long)"); args->awaited_fut_");
    line = string_concat(line, ns);
    line = string_concat(line, (long long)" = _f; if (_f && !_f->completed) { args->state = ");
    line = string_concat(line, ns);
    line = string_concat(line, (long long)"; _f->waiting_task = ep_current_task; return -999999; } }\n            case ");
    line = string_concat(line, ns);
    line = string_concat(line, (long long)":\n");
    ok = emit(state, line);
    ret_val = 0LL;
    goto L_cleanup;
    }
    if (((t == 4LL || t == 5LL) || t == 14LL)) {
    ok = emit_async_yields_expr(state, get_list(expr, 1LL), var_keys, var_values);
    ok = emit_async_yields_expr(state, get_list(expr, 3LL), var_keys, var_values);
    ret_val = 0LL;
    goto L_cleanup;
    }
    if ((((t == 20LL || t == 32LL) || t == 33LL) || t == 18LL)) {
    ret_val = emit_async_yields_expr(state, get_list(expr, 1LL), var_keys, var_values);
    goto L_cleanup;
    }
    if (t == 6LL) {
    args = get_list(expr, 2LL);
    i = 0LL;
    while (i < length_list(args)) {
    ok = emit_async_yields_expr(state, get_list(args, i), var_keys, var_values);
    i = (i + 1LL);
    }
    ret_val = 0LL;
    goto L_cleanup;
    }
    ret_val = 0LL;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(10);
    return ret_val;
}

long long emit_async_yields_stmt(long long state, long long stmt, long long var_keys, long long var_values) {
    long long t = 0;
    long long ret_val = 0;

    ep_gc_push_root(&state);
    ep_gc_push_root(&stmt);
    ep_gc_push_root(&var_keys);
    ep_gc_push_root(&var_values);
    ep_gc_maybe_collect();

    t = get_list(stmt, 0LL);
    if (t == 7LL) {
    ret_val = emit_async_yields_expr(state, get_list(stmt, 2LL), var_keys, var_values);
    goto L_cleanup;
    }
    if (((t == 8LL || t == 9LL) || t == 36LL)) {
    ret_val = emit_async_yields_expr(state, get_list(stmt, 1LL), var_keys, var_values);
    goto L_cleanup;
    }
    ret_val = 0LL;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(4);
    return ret_val;
}

long long type_name_to_code(long long tname) {
    long long t = 0;
    long long ret_val = 0;

    ep_gc_push_root(&t);
    ep_gc_push_root(&tname);
    ep_gc_maybe_collect();

    t = string_concat(tname, (long long)"");
    if ((strcmp((char*)(long long)"Str", (char*)t) == 0)) {
    ret_val = 2LL;
    goto L_cleanup;
    }
    if ((strcmp((char*)(long long)"Float", (char*)t) == 0)) {
    ret_val = 8LL;
    goto L_cleanup;
    }
    if ((strcmp((char*)(long long)"Bool", (char*)t) == 0)) {
    ret_val = 7LL;
    goto L_cleanup;
    }
    ret_val = 1LL;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(2);
    return ret_val;
}

long long param_ann_to_code(long long tname) {
    long long t = 0;
    long long ret_val = 0;

    ep_gc_push_root(&t);
    ep_gc_push_root(&tname);
    ep_gc_maybe_collect();

    t = string_concat(tname, (long long)"");
    if ((strcmp((char*)t, (char*)(long long)"Int") == 0)) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    if ((strcmp((char*)t, (char*)(long long)"Str") == 0)) {
    ret_val = 2LL;
    goto L_cleanup;
    }
    if ((strcmp((char*)t, (char*)(long long)"List") == 0)) {
    ret_val = 4LL;
    goto L_cleanup;
    }
    if ((strcmp((char*)t, (char*)(long long)"Bool") == 0)) {
    ret_val = 7LL;
    goto L_cleanup;
    }
    if ((strcmp((char*)t, (char*)(long long)"Float") == 0)) {
    ret_val = 8LL;
    goto L_cleanup;
    }
    ret_val = 0LL;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(2);
    return ret_val;
}

long long seed_param_types(long long params, long long var_keys, long long var_values) {
    long long p_len = 0;
    long long p_idx = 0;
    long long p_node = 0;
    long long p_name = 0;
    long long is_borrow = 0;
    long long param_type = 0;
    long long ann_code = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&p_idx);
    ep_gc_push_root(&p_node);
    ep_gc_push_root(&p_name);
    ep_gc_push_root(&param_type);
    ep_gc_push_root(&params);
    ep_gc_push_root(&var_keys);
    ep_gc_push_root(&var_values);
    ep_gc_maybe_collect();

    p_len = length_list(params);
    p_idx = 0LL;
    while (p_idx < p_len) {
    p_node = get_list(params, p_idx);
    p_name = get_list(p_node, 0LL);
    is_borrow = get_list(p_node, 1LL);
    param_type = 1LL;
    if (is_borrow == 1LL) {
    param_type = 5LL;
    }
    if (length_list(p_node) > 2LL) {
    ann_code = param_ann_to_code(get_list(p_node, 2LL));
    if (ann_code != 0LL) {
    param_type = ann_code;
    }
    }
    ok = map_put(var_keys, var_values, p_name, param_type);
    p_idx = (p_idx + 1LL);
    }
    ret_val = 0LL;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(7);
    return ret_val;
}

long long collect_prim_param_flags(long long state, long long program) {
    long long funcs = 0;
    long long names = 0;
    long long flaglists = 0;
    long long f_len = 0;
    long long f_i = 0;
    long long func = 0;
    long long params = 0;
    long long flags = 0;
    long long p_len = 0;
    long long p_i = 0;
    long long p_node = 0;
    long long fl = 0;
    long long c = 0;
    long long okf = 0;
    long long okn = 0;
    long long okl = 0;
    long long ret_val = 0;

    ep_gc_push_root(&funcs);
    ep_gc_push_root(&names);
    ep_gc_push_root(&flaglists);
    ep_gc_push_root(&f_i);
    ep_gc_push_root(&func);
    ep_gc_push_root(&params);
    ep_gc_push_root(&flags);
    ep_gc_push_root(&p_i);
    ep_gc_push_root(&p_node);
    ep_gc_push_root(&fl);
    ep_gc_push_root(&state);
    ep_gc_push_root(&program);
    ep_gc_maybe_collect();

    funcs = get_list(program, 3LL);
    names = get_list(state, 22LL);
    flaglists = get_list(state, 23LL);
    f_len = length_list(funcs);
    f_i = 0LL;
    while (f_i < f_len) {
    func = get_list(funcs, f_i);
    params = get_list(func, 2LL);
    flags = (create_list() + 0LL);
    p_len = length_list(params);
    p_i = 0LL;
    while (p_i < p_len) {
    p_node = get_list(params, p_i);
    fl = 0LL;
    if (length_list(p_node) > 2LL) {
    c = param_ann_to_code(get_list(p_node, 2LL));
    if (((c == 1LL || c == 7LL) || c == 8LL)) {
    fl = 1LL;
    }
    }
    okf = append_list(flags, fl);
    p_i = (p_i + 1LL);
    }
    okn = append_list(names, get_list(func, 1LL));
    okl = append_list(flaglists, flags);
    f_i = (f_i + 1LL);
    }
    ret_val = 0LL;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(12);
    return ret_val;
}

long long get_prim_param_flags(long long state, long long name) {
    long long n = 0;
    long long names = 0;
    long long len = 0;
    long long idx = 0;
    long long ret_val = 0;

    ep_gc_push_root(&n);
    ep_gc_push_root(&names);
    ep_gc_push_root(&idx);
    ep_gc_push_root(&state);
    ep_gc_push_root(&name);
    ep_gc_maybe_collect();

    n = string_concat(name, (long long)"");
    names = get_list(state, 22LL);
    len = length_list(names);
    idx = 0LL;
    while (idx < len) {
    if ((strcmp((char*)n, (char*)get_list(names, idx)) == 0)) {
    ret_val = get_list(get_list(state, 23LL), idx);
    goto L_cleanup;
    }
    idx = (idx + 1LL);
    }
    ret_val = 0LL;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(5);
    return ret_val;
}

long long cg_expr_has_var(long long expr, long long vname) {
    long long t = 0;
    long long n = 0;
    long long cn = 0;
    long long args = 0;
    long long a_len = 0;
    long long a_i = 0;
    long long fields = 0;
    long long f_len = 0;
    long long f_i = 0;
    long long margs = 0;
    long long m_len = 0;
    long long m_i = 0;
    long long eargs = 0;
    long long e_len = 0;
    long long e_i = 0;
    long long elems = 0;
    long long l_len = 0;
    long long l_i = 0;
    long long ret_val = 0;

    ep_gc_push_root(&n);
    ep_gc_push_root(&cn);
    ep_gc_push_root(&args);
    ep_gc_push_root(&a_i);
    ep_gc_push_root(&fields);
    ep_gc_push_root(&f_i);
    ep_gc_push_root(&margs);
    ep_gc_push_root(&m_i);
    ep_gc_push_root(&eargs);
    ep_gc_push_root(&e_i);
    ep_gc_push_root(&elems);
    ep_gc_push_root(&l_i);
    ep_gc_push_root(&expr);
    ep_gc_push_root(&vname);
    ep_gc_maybe_collect();

    if (expr == 0LL) {
    ret_val = 0LL;
    goto L_cleanup;
    }
    t = get_list(expr, 0LL);
    if (t == 3LL) {
    n = string_concat(get_list(expr, 1LL), (long long)"");
    if ((strcmp((char*)n, (char*)vname) == 0)) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    ret_val = 0LL;
    goto L_cleanup;
    }
    if (((t == 4LL || t == 5LL) || t == 14LL)) {
    if (cg_expr_has_var(get_list(expr, 1LL), vname) == 1LL) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    ret_val = cg_expr_has_var(get_list(expr, 3LL), vname);
    goto L_cleanup;
    }
    if (t == 6LL) {
    cn = string_concat(get_list(expr, 1LL), (long long)"");
    if ((strcmp((char*)cn, (char*)vname) == 0)) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    args = get_list(expr, 2LL);
    a_len = length_list(args);
    a_i = 0LL;
    while (a_i < a_len) {
    if (cg_expr_has_var(get_list(args, a_i), vname) == 1LL) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    a_i = (a_i + 1LL);
    }
    ret_val = 0LL;
    goto L_cleanup;
    }
    if (((((t == 18LL || t == 20LL) || t == 21LL) || t == 32LL) || t == 33LL)) {
    ret_val = cg_expr_has_var(get_list(expr, 1LL), vname);
    goto L_cleanup;
    }
    if (t == 22LL) {
    ret_val = cg_expr_has_var(get_list(expr, 1LL), vname);
    goto L_cleanup;
    }
    if (t == 24LL) {
    fields = get_list(expr, 2LL);
    f_len = length_list(fields);
    f_i = 0LL;
    while (f_i < f_len) {
    if (cg_expr_has_var(get_list(get_list(fields, f_i), 1LL), vname) == 1LL) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    f_i = (f_i + 1LL);
    }
    ret_val = 0LL;
    goto L_cleanup;
    }
    if (t == 25LL) {
    if (cg_expr_has_var(get_list(expr, 1LL), vname) == 1LL) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    margs = get_list(expr, 3LL);
    m_len = length_list(margs);
    m_i = 0LL;
    while (m_i < m_len) {
    if (cg_expr_has_var(get_list(margs, m_i), vname) == 1LL) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    m_i = (m_i + 1LL);
    }
    ret_val = 0LL;
    goto L_cleanup;
    }
    if (t == 26LL) {
    eargs = get_list(expr, 2LL);
    e_len = length_list(eargs);
    e_i = 0LL;
    while (e_i < e_len) {
    if (cg_expr_has_var(get_list(eargs, e_i), vname) == 1LL) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    e_i = (e_i + 1LL);
    }
    ret_val = 0LL;
    goto L_cleanup;
    }
    if (t == 34LL) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    if (t == 35LL) {
    elems = get_list(expr, 1LL);
    l_len = length_list(elems);
    l_i = 0LL;
    while (l_i < l_len) {
    if (cg_expr_has_var(get_list(elems, l_i), vname) == 1LL) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    l_i = (l_i + 1LL);
    }
    ret_val = 0LL;
    goto L_cleanup;
    }
    ret_val = 0LL;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(14);
    return ret_val;
}

long long cg_is_prim_expr(long long state, long long expr, long long pname) {
    long long t = 0;
    long long name = 0;
    long long args = 0;
    long long a_len = 0;
    long long is_prim_builtin = 0;
    long long a_i = 0;
    long long flags = 0;
    long long arg = 0;
    long long fl = 0;
    long long ret_val = 0;

    ep_gc_push_root(&name);
    ep_gc_push_root(&args);
    ep_gc_push_root(&a_i);
    ep_gc_push_root(&flags);
    ep_gc_push_root(&arg);
    ep_gc_push_root(&state);
    ep_gc_push_root(&expr);
    ep_gc_push_root(&pname);
    ep_gc_maybe_collect();

    if (expr == 0LL) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    t = get_list(expr, 0LL);
    if ((((t == 1LL || t == 2LL) || t == 31LL) || t == 42LL)) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    if (t == 3LL) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    if (((t == 4LL || t == 5LL) || t == 14LL)) {
    if (cg_is_prim_expr(state, get_list(expr, 1LL), pname) == 0LL) {
    ret_val = 0LL;
    goto L_cleanup;
    }
    ret_val = cg_is_prim_expr(state, get_list(expr, 3LL), pname);
    goto L_cleanup;
    }
    if (t == 32LL) {
    ret_val = cg_is_prim_expr(state, get_list(expr, 1LL), pname);
    goto L_cleanup;
    }
    if (t == 6LL) {
    name = string_concat(get_list(expr, 1LL), (long long)"");
    args = get_list(expr, 2LL);
    a_len = length_list(args);
    is_prim_builtin = 0LL;
    if ((strcmp((char*)name, (char*)(long long)"int_to_string") == 0)) {
    is_prim_builtin = 1LL;
    }
    if ((strcmp((char*)name, (char*)(long long)"int_to_float") == 0)) {
    is_prim_builtin = 1LL;
    }
    if ((strcmp((char*)name, (char*)(long long)"float_to_int") == 0)) {
    is_prim_builtin = 1LL;
    }
    if ((strcmp((char*)name, (char*)(long long)"char_from_code") == 0)) {
    is_prim_builtin = 1LL;
    }
    if ((strcmp((char*)name, (char*)(long long)"ep_abs") == 0)) {
    is_prim_builtin = 1LL;
    }
    if ((strcmp((char*)name, (char*)(long long)"ep_sleep_ms") == 0)) {
    is_prim_builtin = 1LL;
    }
    if ((strcmp((char*)name, (char*)(long long)"sleep_ms") == 0)) {
    is_prim_builtin = 1LL;
    }
    if ((strcmp((char*)name, (char*)(long long)"ep_time_ms") == 0)) {
    is_prim_builtin = 1LL;
    }
    if ((strcmp((char*)name, (char*)(long long)"ep_random_int") == 0)) {
    is_prim_builtin = 1LL;
    }
    if (is_prim_builtin == 1LL) {
    a_i = 0LL;
    while (a_i < a_len) {
    if (cg_is_prim_expr(state, get_list(args, a_i), pname) == 0LL) {
    ret_val = 0LL;
    goto L_cleanup;
    }
    a_i = (a_i + 1LL);
    }
    ret_val = 1LL;
    goto L_cleanup;
    }
    flags = get_prim_param_flags(state, name);
    if (flags != 0LL) {
    a_i = 0LL;
    while (a_i < a_len) {
    arg = get_list(args, a_i);
    if (cg_expr_has_var(arg, pname) == 1LL) {
    fl = 0LL;
    if (a_i < length_list(flags)) {
    fl = get_list(flags, a_i);
    }
    if (fl == 0LL) {
    ret_val = 0LL;
    goto L_cleanup;
    }
    if (cg_is_prim_expr(state, arg, pname) == 0LL) {
    ret_val = 0LL;
    goto L_cleanup;
    }
    }
    a_i = (a_i + 1LL);
    }
    ret_val = 1LL;
    goto L_cleanup;
    }
    if (cg_expr_has_var(expr, pname) == 1LL) {
    ret_val = 0LL;
    goto L_cleanup;
    }
    ret_val = 1LL;
    goto L_cleanup;
    }
    if (cg_expr_has_var(expr, pname) == 1LL) {
    ret_val = 0LL;
    goto L_cleanup;
    }
    ret_val = 1LL;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(8);
    return ret_val;
}

long long cg_stmts_have_nonprim(long long state, long long stmts, long long pname) {
    long long len = 0;
    long long idx = 0;
    long long stmt = 0;
    long long t = 0;
    long long eb = 0;
    long long lv = 0;
    long long arms = 0;
    long long a_len = 0;
    long long a_i = 0;
    long long arm = 0;
    long long ret_val = 0;

    ep_gc_push_root(&idx);
    ep_gc_push_root(&stmt);
    ep_gc_push_root(&eb);
    ep_gc_push_root(&lv);
    ep_gc_push_root(&arms);
    ep_gc_push_root(&a_i);
    ep_gc_push_root(&arm);
    ep_gc_push_root(&state);
    ep_gc_push_root(&stmts);
    ep_gc_push_root(&pname);
    ep_gc_maybe_collect();

    len = length_list(stmts);
    idx = 0LL;
    while (idx < len) {
    stmt = get_list(stmts, idx);
    t = get_list(stmt, 0LL);
    if (t == 7LL) {
    if (cg_is_prim_expr(state, get_list(stmt, 2LL), pname) == 0LL) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    }
    if (((t == 8LL || t == 9LL) || t == 36LL)) {
    if (cg_is_prim_expr(state, get_list(stmt, 1LL), pname) == 0LL) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    }
    if (t == 10LL) {
    if (cg_is_prim_expr(state, get_list(stmt, 1LL), pname) == 0LL) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    if (cg_stmts_have_nonprim(state, get_list(stmt, 2LL), pname) == 1LL) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    eb = get_list(stmt, 3LL);
    if (eb != 0LL) {
    if (cg_stmts_have_nonprim(state, eb, pname) == 1LL) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    }
    }
    if (t == 11LL) {
    if (cg_is_prim_expr(state, get_list(stmt, 1LL), pname) == 0LL) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    if (cg_stmts_have_nonprim(state, get_list(stmt, 2LL), pname) == 1LL) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    }
    if (t == 28LL) {
    lv = string_concat(get_list(stmt, 1LL), (long long)"");
    if ((strcmp((char*)lv, (char*)pname) == 0)) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    if (cg_is_prim_expr(state, get_list(stmt, 2LL), pname) == 0LL) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    if (cg_stmts_have_nonprim(state, get_list(stmt, 3LL), pname) == 1LL) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    }
    if (t == 16LL) {
    if (cg_expr_has_var(get_list(stmt, 1LL), pname) == 1LL) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    if (cg_expr_has_var(get_list(stmt, 2LL), pname) == 1LL) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    }
    if (t == 23LL) {
    if (cg_expr_has_var(get_list(stmt, 1LL), pname) == 1LL) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    if (cg_expr_has_var(get_list(stmt, 3LL), pname) == 1LL) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    }
    if (t == 27LL) {
    if (cg_expr_has_var(get_list(stmt, 1LL), pname) == 1LL) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    arms = get_list(stmt, 2LL);
    a_len = length_list(arms);
    a_i = 0LL;
    while (a_i < a_len) {
    arm = get_list(arms, a_i);
    if (contains_string_val(get_list(arm, 1LL), pname) == 1LL) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    if (cg_stmts_have_nonprim(state, get_list(arm, 2LL), pname) == 1LL) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    a_i = (a_i + 1LL);
    }
    }
    idx = (idx + 1LL);
    }
    ret_val = 0LL;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(10);
    return ret_val;
}

long long usage_promote_call(long long name, long long args, long long var_keys, long long var_values) {
    long long a0 = 0;
    long long vname = 0;
    long long cur = 0;
    long long n = 0;
    long long new_t = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&a0);
    ep_gc_push_root(&vname);
    ep_gc_push_root(&n);
    ep_gc_push_root(&new_t);
    ep_gc_push_root(&name);
    ep_gc_push_root(&args);
    ep_gc_push_root(&var_keys);
    ep_gc_push_root(&var_values);
    ep_gc_maybe_collect();

    if (length_list(args) == 0LL) {
    ret_val = 0LL;
    goto L_cleanup;
    }
    a0 = get_list(args, 0LL);
    if (get_list(a0, 0LL) != 3LL) {
    ret_val = 0LL;
    goto L_cleanup;
    }
    vname = get_list(a0, 1LL);
    cur = map_get(var_keys, var_values, vname);
    if (cur != 1LL) {
    ret_val = 0LL;
    goto L_cleanup;
    }
    n = string_concat(name, (long long)"");
    new_t = 0LL;
    if ((strcmp((char*)n, (char*)(long long)"length_list") == 0)) {
    new_t = 4LL;
    }
    if ((strcmp((char*)n, (char*)(long long)"append_list") == 0)) {
    new_t = 4LL;
    }
    if ((strcmp((char*)n, (char*)(long long)"get_list") == 0)) {
    new_t = 4LL;
    }
    if ((strcmp((char*)n, (char*)(long long)"set_list") == 0)) {
    new_t = 4LL;
    }
    if ((strcmp((char*)n, (char*)(long long)"remove_list") == 0)) {
    new_t = 4LL;
    }
    if ((strcmp((char*)n, (char*)(long long)"pop_list") == 0)) {
    new_t = 4LL;
    }
    if ((strcmp((char*)n, (char*)(long long)"map_insert") == 0)) {
    new_t = 4LL;
    }
    if ((strcmp((char*)n, (char*)(long long)"map_get_val") == 0)) {
    new_t = 4LL;
    }
    if ((strcmp((char*)n, (char*)(long long)"map_contains") == 0)) {
    new_t = 4LL;
    }
    if ((strcmp((char*)n, (char*)(long long)"map_delete") == 0)) {
    new_t = 4LL;
    }
    if ((strcmp((char*)n, (char*)(long long)"map_keys") == 0)) {
    new_t = 4LL;
    }
    if ((strcmp((char*)n, (char*)(long long)"map_size") == 0)) {
    new_t = 4LL;
    }
    if ((strcmp((char*)n, (char*)(long long)"map_values") == 0)) {
    new_t = 4LL;
    }
    if ((strcmp((char*)n, (char*)(long long)"map_set_str") == 0)) {
    new_t = 4LL;
    }
    if ((strcmp((char*)n, (char*)(long long)"map_get_str") == 0)) {
    new_t = 4LL;
    }
    if ((strcmp((char*)n, (char*)(long long)"string_length") == 0)) {
    new_t = 2LL;
    }
    if ((strcmp((char*)n, (char*)(long long)"get_character") == 0)) {
    new_t = 2LL;
    }
    if ((strcmp((char*)n, (char*)(long long)"char_at") == 0)) {
    new_t = 2LL;
    }
    if ((strcmp((char*)n, (char*)(long long)"string_contains") == 0)) {
    new_t = 2LL;
    }
    if ((strcmp((char*)n, (char*)(long long)"string_index_of") == 0)) {
    new_t = 2LL;
    }
    if ((strcmp((char*)n, (char*)(long long)"string_replace") == 0)) {
    new_t = 2LL;
    }
    if ((strcmp((char*)n, (char*)(long long)"string_upper") == 0)) {
    new_t = 2LL;
    }
    if ((strcmp((char*)n, (char*)(long long)"string_lower") == 0)) {
    new_t = 2LL;
    }
    if ((strcmp((char*)n, (char*)(long long)"string_trim") == 0)) {
    new_t = 2LL;
    }
    if ((strcmp((char*)n, (char*)(long long)"string_split") == 0)) {
    new_t = 2LL;
    }
    if ((strcmp((char*)n, (char*)(long long)"string_to_list") == 0)) {
    new_t = 2LL;
    }
    if ((strcmp((char*)n, (char*)(long long)"json_get_string") == 0)) {
    new_t = 2LL;
    }
    if ((strcmp((char*)n, (char*)(long long)"json_get_int") == 0)) {
    new_t = 2LL;
    }
    if ((strcmp((char*)n, (char*)(long long)"json_get_bool") == 0)) {
    new_t = 2LL;
    }
    if (new_t != 0LL) {
    ok = map_put(var_keys, var_values, vname, new_t);
    }
    ret_val = 0LL;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(8);
    return ret_val;
}

long long infer_usage_types_expr(long long expr, long long var_keys, long long var_values) {
    long long t = 0;
    long long ok = 0;
    long long args = 0;
    long long a_len = 0;
    long long a_i = 0;
    long long oka = 0;
    long long okl = 0;
    long long okr = 0;
    long long fields = 0;
    long long f_len = 0;
    long long f_i = 0;
    long long okf = 0;
    long long oko = 0;
    long long margs = 0;
    long long m_len = 0;
    long long m_i = 0;
    long long okm = 0;
    long long eargs = 0;
    long long e_len = 0;
    long long e_i = 0;
    long long oke = 0;
    long long elems = 0;
    long long l_len = 0;
    long long l_i = 0;
    long long okl2 = 0;
    long long ret_val = 0;

    ep_gc_push_root(&args);
    ep_gc_push_root(&a_i);
    ep_gc_push_root(&fields);
    ep_gc_push_root(&f_i);
    ep_gc_push_root(&margs);
    ep_gc_push_root(&m_i);
    ep_gc_push_root(&eargs);
    ep_gc_push_root(&e_i);
    ep_gc_push_root(&elems);
    ep_gc_push_root(&l_i);
    ep_gc_push_root(&expr);
    ep_gc_push_root(&var_keys);
    ep_gc_push_root(&var_values);
    ep_gc_maybe_collect();

    if (expr == 0LL) {
    ret_val = 0LL;
    goto L_cleanup;
    }
    t = get_list(expr, 0LL);
    if (t == 6LL) {
    ok = usage_promote_call(get_list(expr, 1LL), get_list(expr, 2LL), var_keys, var_values);
    args = get_list(expr, 2LL);
    a_len = length_list(args);
    a_i = 0LL;
    while (a_i < a_len) {
    oka = infer_usage_types_expr(get_list(args, a_i), var_keys, var_values);
    a_i = (a_i + 1LL);
    }
    ret_val = 0LL;
    goto L_cleanup;
    }
    if (((t == 4LL || t == 5LL) || t == 14LL)) {
    okl = infer_usage_types_expr(get_list(expr, 1LL), var_keys, var_values);
    okr = infer_usage_types_expr(get_list(expr, 3LL), var_keys, var_values);
    ret_val = 0LL;
    goto L_cleanup;
    }
    if (((((t == 18LL || t == 20LL) || t == 21LL) || t == 32LL) || t == 33LL)) {
    ret_val = infer_usage_types_expr(get_list(expr, 1LL), var_keys, var_values);
    goto L_cleanup;
    }
    if (t == 24LL) {
    fields = get_list(expr, 2LL);
    f_len = length_list(fields);
    f_i = 0LL;
    while (f_i < f_len) {
    okf = infer_usage_types_expr(get_list(get_list(fields, f_i), 1LL), var_keys, var_values);
    f_i = (f_i + 1LL);
    }
    ret_val = 0LL;
    goto L_cleanup;
    }
    if (t == 25LL) {
    oko = infer_usage_types_expr(get_list(expr, 1LL), var_keys, var_values);
    margs = get_list(expr, 3LL);
    m_len = length_list(margs);
    m_i = 0LL;
    while (m_i < m_len) {
    okm = infer_usage_types_expr(get_list(margs, m_i), var_keys, var_values);
    m_i = (m_i + 1LL);
    }
    ret_val = 0LL;
    goto L_cleanup;
    }
    if (t == 26LL) {
    eargs = get_list(expr, 2LL);
    e_len = length_list(eargs);
    e_i = 0LL;
    while (e_i < e_len) {
    oke = infer_usage_types_expr(get_list(eargs, e_i), var_keys, var_values);
    e_i = (e_i + 1LL);
    }
    ret_val = 0LL;
    goto L_cleanup;
    }
    if (t == 35LL) {
    elems = get_list(expr, 1LL);
    l_len = length_list(elems);
    l_i = 0LL;
    while (l_i < l_len) {
    okl2 = infer_usage_types_expr(get_list(elems, l_i), var_keys, var_values);
    l_i = (l_i + 1LL);
    }
    ret_val = 0LL;
    goto L_cleanup;
    }
    ret_val = 0LL;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(13);
    return ret_val;
}

long long infer_usage_types_stmts(long long stmts, long long var_keys, long long var_values) {
    long long len = 0;
    long long idx = 0;
    long long stmt = 0;
    long long t = 0;
    long long ok7 = 0;
    long long ok8 = 0;
    long long okc = 0;
    long long okt = 0;
    long long eb = 0;
    long long oke = 0;
    long long okw = 0;
    long long okb = 0;
    long long okn1 = 0;
    long long okn2 = 0;
    long long okf1 = 0;
    long long okf2 = 0;
    long long okm1 = 0;
    long long arms = 0;
    long long ar_len = 0;
    long long ar_i = 0;
    long long okab = 0;
    long long okfe1 = 0;
    long long okfe2 = 0;
    long long ret_val = 0;

    ep_gc_push_root(&idx);
    ep_gc_push_root(&stmt);
    ep_gc_push_root(&eb);
    ep_gc_push_root(&arms);
    ep_gc_push_root(&ar_i);
    ep_gc_push_root(&stmts);
    ep_gc_push_root(&var_keys);
    ep_gc_push_root(&var_values);
    ep_gc_maybe_collect();

    len = length_list(stmts);
    idx = 0LL;
    while (idx < len) {
    stmt = get_list(stmts, idx);
    t = get_list(stmt, 0LL);
    if (t == 7LL) {
    ok7 = infer_usage_types_expr(get_list(stmt, 2LL), var_keys, var_values);
    }
    if (((t == 8LL || t == 9LL) || t == 36LL)) {
    ok8 = infer_usage_types_expr(get_list(stmt, 1LL), var_keys, var_values);
    }
    if (t == 10LL) {
    okc = infer_usage_types_expr(get_list(stmt, 1LL), var_keys, var_values);
    okt = infer_usage_types_stmts(get_list(stmt, 2LL), var_keys, var_values);
    eb = get_list(stmt, 3LL);
    if (eb != 0LL) {
    oke = infer_usage_types_stmts(eb, var_keys, var_values);
    }
    }
    if (t == 11LL) {
    okw = infer_usage_types_expr(get_list(stmt, 1LL), var_keys, var_values);
    okb = infer_usage_types_stmts(get_list(stmt, 2LL), var_keys, var_values);
    }
    if (t == 16LL) {
    okn1 = infer_usage_types_expr(get_list(stmt, 1LL), var_keys, var_values);
    okn2 = infer_usage_types_expr(get_list(stmt, 2LL), var_keys, var_values);
    }
    if (t == 23LL) {
    okf1 = infer_usage_types_expr(get_list(stmt, 1LL), var_keys, var_values);
    okf2 = infer_usage_types_expr(get_list(stmt, 3LL), var_keys, var_values);
    }
    if (t == 27LL) {
    okm1 = infer_usage_types_expr(get_list(stmt, 1LL), var_keys, var_values);
    arms = get_list(stmt, 2LL);
    ar_len = length_list(arms);
    ar_i = 0LL;
    while (ar_i < ar_len) {
    okab = infer_usage_types_stmts(get_list(get_list(arms, ar_i), 2LL), var_keys, var_values);
    ar_i = (ar_i + 1LL);
    }
    }
    if (t == 28LL) {
    okfe1 = infer_usage_types_expr(get_list(stmt, 2LL), var_keys, var_values);
    okfe2 = infer_usage_types_stmts(get_list(stmt, 3LL), var_keys, var_values);
    }
    idx = (idx + 1LL);
    }
    ret_val = 0LL;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(8);
    return ret_val;
}

long long is_builtin_c_func(long long state, long long name) {
    long long name_str = 0;
    long long keys = 0;
    long long n = 0;
    long long idx = 0;
    long long ret_val = 0;

    ep_gc_push_root(&name_str);
    ep_gc_push_root(&keys);
    ep_gc_push_root(&idx);
    ep_gc_push_root(&state);
    ep_gc_push_root(&name);
    ep_gc_maybe_collect();

    name_str = string_concat(name, (long long)"");
    keys = get_list(state, 3LL);
    n = get_list(state, 10LL);
    idx = 0LL;
    while (idx < n) {
    if ((strcmp((char*)name_str, (char*)get_list(keys, idx)) == 0)) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    idx = (idx + 1LL);
    }
    ret_val = 0LL;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(5);
    return ret_val;
}

long long get_codegen_borrowed_keys(long long state) {
    long long ret_val = 0;

    ep_gc_push_root(&state);
    ep_gc_maybe_collect();

    ret_val = get_list(state, 8LL);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(1);
    return ret_val;
}

long long set_codegen_borrowed_keys(long long state, long long keys) {
    long long ret_val = 0;

    ep_gc_push_root(&state);
    ep_gc_push_root(&keys);
    ep_gc_maybe_collect();

    ret_val = set_list(state, 8LL, keys);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(2);
    return ret_val;
}

long long get_codegen_borrowed_values(long long state) {
    long long ret_val = 0;

    ep_gc_push_root(&state);
    ep_gc_maybe_collect();

    ret_val = get_list(state, 9LL);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(1);
    return ret_val;
}

long long set_codegen_borrowed_values(long long state, long long values) {
    long long ret_val = 0;

    ep_gc_push_root(&state);
    ep_gc_push_root(&values);
    ep_gc_maybe_collect();

    ret_val = set_list(state, 9LL, values);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(2);
    return ret_val;
}

long long get_codegen_spawn_list(long long state) {
    long long ret_val = 0;

    ep_gc_push_root(&state);
    ep_gc_maybe_collect();

    ret_val = get_list(state, 6LL);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(1);
    return ret_val;
}

long long set_codegen_spawn_list(long long state, long long spawn_list) {
    long long ret_val = 0;

    ep_gc_push_root(&state);
    ep_gc_push_root(&spawn_list);
    ep_gc_maybe_collect();

    ret_val = set_list(state, 6LL, spawn_list);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(2);
    return ret_val;
}

long long get_codegen_spawn_index(long long state) {
    long long ret_val = 0;

    ep_gc_push_root(&state);
    ep_gc_maybe_collect();

    ret_val = get_list(state, 7LL);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(1);
    return ret_val;
}

long long set_codegen_spawn_index(long long state, long long spawn_index) {
    long long ret_val = 0;

    ep_gc_push_root(&state);
    ep_gc_push_root(&spawn_index);
    ep_gc_maybe_collect();

    ret_val = set_list(state, 7LL, spawn_index);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(2);
    return ret_val;
}

long long emit(long long state, long long line) {
    long long lines = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&lines);
    ep_gc_push_root(&state);
    ep_gc_push_root(&line);
    ep_gc_maybe_collect();

    lines = get_list(state, 0LL);
    ok = append_list(lines, line);
    ret_val = 0LL;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(3);
    return ret_val;
}

long long add_string_literal(long long state, long long s) {
    long long s_str = 0;
    long long lits = 0;
    long long len = 0;
    long long idx = 0;
    long long val = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&s_str);
    ep_gc_push_root(&lits);
    ep_gc_push_root(&idx);
    ep_gc_push_root(&state);
    ep_gc_push_root(&s);
    ep_gc_maybe_collect();

    s_str = string_concat(s, (long long)"");
    lits = get_list(state, 1LL);
    len = length_list(lits);
    idx = 0LL;
    while (idx < len) {
    val = get_list(lits, idx);
    if ((strcmp((char*)s_str, (char*)val) == 0)) {
    ret_val = idx;
    goto L_cleanup;
    }
    idx = (idx + 1LL);
    }
    ok = append_list(lits, s);
    ret_val = len;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(5);
    return ret_val;
}

long long get_new_label(long long state, long long prefix) {
    long long count = 0;
    long long next_count = 0;
    long long ok = 0;
    long long num_str = 0;
    long long label_half = 0;
    long long label = 0;
    long long final_label = 0;
    long long ret_val = 0;

    ep_gc_push_root(&count);
    ep_gc_push_root(&next_count);
    ep_gc_push_root(&num_str);
    ep_gc_push_root(&label_half);
    ep_gc_push_root(&label);
    ep_gc_push_root(&final_label);
    ep_gc_push_root(&state);
    ep_gc_push_root(&prefix);
    ep_gc_maybe_collect();

    count = get_list(state, 2LL);
    next_count = (count + 1LL);
    ok = set_list(state, 2LL, next_count);
    num_str = cg_int_to_str(count);
    label_half = string_concat((long long)"L_", prefix);
    label = string_concat(label_half, (long long)"_");
    final_label = string_concat(label, num_str);
    ret_val = final_label;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(8);
    return ret_val;
}

long long analyze_return_types(long long state, long long program) {
    long long keys = 0;
    long long values = 0;
    long long ok = 0;
    long long externals = 0;
    long long ext_len = 0;
    long long idx = 0;
    long long ext_node = 0;
    long long ext_name = 0;
    long long existing = 0;
    long long funcs = 0;
    long long funcs_len = 0;
    long long pass = 0;
    long long func = 0;
    long long name = 0;
    long long params = 0;
    long long body = 0;
    long long var_keys = 0;
    long long var_values = 0;
    long long ret_t = 0;
    long long ret_val = 0;

    ep_gc_push_root(&keys);
    ep_gc_push_root(&values);
    ep_gc_push_root(&externals);
    ep_gc_push_root(&idx);
    ep_gc_push_root(&ext_node);
    ep_gc_push_root(&ext_name);
    ep_gc_push_root(&funcs);
    ep_gc_push_root(&func);
    ep_gc_push_root(&name);
    ep_gc_push_root(&params);
    ep_gc_push_root(&body);
    ep_gc_push_root(&var_keys);
    ep_gc_push_root(&var_values);
    ep_gc_push_root(&ret_t);
    ep_gc_push_root(&state);
    ep_gc_push_root(&program);
    ep_gc_maybe_collect();

    keys = get_list(state, 3LL);
    values = get_list(state, 4LL);
    ok = map_put(keys, values, (long long)"read_file_content", 3LL);
    ok = map_put(keys, values, (long long)"create_list", 4LL);
    ok = map_put(keys, values, (long long)"ep_md5", 3LL);
    ok = map_put(keys, values, (long long)"ep_sha256", 3LL);
    ok = map_put(keys, values, (long long)"ep_net_connect", 1LL);
    ok = map_put(keys, values, (long long)"ep_net_listen", 1LL);
    ok = map_put(keys, values, (long long)"ep_net_accept", 1LL);
    ok = map_put(keys, values, (long long)"ep_net_send", 1LL);
    ok = map_put(keys, values, (long long)"ep_net_send_raw", 1LL);
    ok = map_put(keys, values, (long long)"ep_net_recv", 3LL);
    ok = map_put(keys, values, (long long)"ep_net_close", 1LL);
    ok = map_put(keys, values, (long long)"append_list", 1LL);
    ok = map_put(keys, values, (long long)"get_list", 1LL);
    ok = map_put(keys, values, (long long)"set_list", 1LL);
    ok = map_put(keys, values, (long long)"length_list", 1LL);
    ok = map_put(keys, values, (long long)"string_length", 1LL);
    ok = map_put(keys, values, (long long)"get_character", 1LL);
    ok = map_put(keys, values, (long long)"display_string", 1LL);
    ok = map_put(keys, values, (long long)"screen_write", 1LL);
    ok = map_put(keys, values, (long long)"get_argument_count", 1LL);
    ok = map_put(keys, values, (long long)"get_argument", 2LL);
    ok = map_put(keys, values, (long long)"write_file_content", 1LL);
    ok = map_put(keys, values, (long long)"run_command", 1LL);
    ok = map_put(keys, values, (long long)"substring", 3LL);
    ok = map_put(keys, values, (long long)"string_from_list", 3LL);
    ok = map_put(keys, values, (long long)"pop_list", 1LL);
    ok = map_put(keys, values, (long long)"get_list_data_ptr", 1LL);
    ok = map_put(keys, values, (long long)"sqlite_get_callback_ptr", 1LL);
    ok = map_put(keys, values, (long long)"free_list", 1LL);
    ok = map_put(keys, values, (long long)"create_map", 1LL);
    ok = map_put(keys, values, (long long)"map_insert", 1LL);
    ok = map_put(keys, values, (long long)"map_get_val", 1LL);
    ok = map_put(keys, values, (long long)"map_contains", 1LL);
    ok = map_put(keys, values, (long long)"map_delete", 1LL);
    ok = map_put(keys, values, (long long)"free_map", 1LL);
    ok = map_put(keys, values, (long long)"create_deque", 1LL);
    ok = map_put(keys, values, (long long)"deque_push_back", 1LL);
    ok = map_put(keys, values, (long long)"deque_push_front", 1LL);
    ok = map_put(keys, values, (long long)"deque_pop_back", 1LL);
    ok = map_put(keys, values, (long long)"deque_pop_front", 1LL);
    ok = map_put(keys, values, (long long)"deque_length", 1LL);
    ok = map_put(keys, values, (long long)"free_deque", 1LL);
    ok = map_put(keys, values, (long long)"fs_scan_dir", 4LL);
    ok = map_put(keys, values, (long long)"fs_copy_file", 1LL);
    ok = map_put(keys, values, (long long)"fs_delete_file", 1LL);
    ok = map_put(keys, values, (long long)"fs_move_file", 1LL);
    ok = map_put(keys, values, (long long)"fs_exists", 1LL);
    ok = map_put(keys, values, (long long)"fs_is_dir", 1LL);
    ok = map_put(keys, values, (long long)"fs_is_file", 1LL);
    ok = map_put(keys, values, (long long)"fs_get_size", 1LL);
    ok = map_put(keys, values, (long long)"ep_http_request", 3LL);
    ok = map_put(keys, values, (long long)"ep_sleep_ms", 1LL);
    ok = map_put(keys, values, (long long)"concat", 3LL);
    ok = map_put(keys, values, (long long)"ep_auto_to_string", 3LL);
    ok = map_put(keys, values, (long long)"ep_float_to_string", 3LL);
    ok = map_put(keys, values, (long long)"int_to_string", 3LL);
    ok = map_put(keys, values, (long long)"ep_int_to_str", 3LL);
    ok = map_put(keys, values, (long long)"string_upper", 3LL);
    ok = map_put(keys, values, (long long)"string_lower", 3LL);
    ok = map_put(keys, values, (long long)"string_trim", 3LL);
    ok = map_put(keys, values, (long long)"string_replace", 3LL);
    ok = map_put(keys, values, (long long)"string_split", 4LL);
    ok = map_put(keys, values, (long long)"string_contains", 1LL);
    ok = map_put(keys, values, (long long)"string_index_of", 1LL);
    ok = map_put(keys, values, (long long)"string_to_int", 1LL);
    ok = map_put(keys, values, (long long)"char_at", 3LL);
    ok = map_put(keys, values, (long long)"char_from_code", 3LL);
    ok = map_put(keys, values, (long long)"ptr_to_str", 3LL);
    ok = map_put(keys, values, (long long)"str_to_ptr", 1LL);
    ok = map_put(keys, values, (long long)"ep_sb_to_string", 3LL);
    ok = map_put(keys, values, (long long)"ep_sb_create", 1LL);
    ok = map_put(keys, values, (long long)"ep_sb_append", 1LL);
    ok = map_put(keys, values, (long long)"ep_sb_append_int", 1LL);
    ok = map_put(keys, values, (long long)"ep_random_int", 1LL);
    ok = map_put(keys, values, (long long)"ep_abs", 1LL);
    ok = map_put(keys, values, (long long)"ep_time_now_ms", 1LL);
    ok = map_put(keys, values, (long long)"ep_time_now_sec", 1LL);
    ok = map_put(keys, values, (long long)"ep_time_day", 1LL);
    ok = map_put(keys, values, (long long)"ep_time_month", 1LL);
    ok = map_put(keys, values, (long long)"ep_time_year", 1LL);
    ok = map_put(keys, values, (long long)"sleep_ms", 1LL);
    ok = map_put(keys, values, (long long)"ep_system", 1LL);
    ok = map_put(keys, values, (long long)"int_to_float", 8LL);
    ok = map_put(keys, values, (long long)"ep_dlcall_f0", 8LL);
    ok = map_put(keys, values, (long long)"ep_dlcall_f1", 8LL);
    ok = map_put(keys, values, (long long)"ep_dlcall_f2", 8LL);
    ok = map_put(keys, values, (long long)"ep_dlcall_f3", 8LL);
    ok = map_put(keys, values, (long long)"ep_dlcall_f4", 8LL);
    ok = map_put(keys, values, (long long)"ep_dlcall_f5", 8LL);
    ok = map_put(keys, values, (long long)"ep_dlcall_f6", 8LL);
    ok = map_put(keys, values, (long long)"float_to_int", 1LL);
    ok = map_put(keys, values, (long long)"ep_hmac_sha256", 3LL);
    ok = map_put(keys, values, (long long)"ep_base64_encode", 3LL);
    ok = map_put(keys, values, (long long)"ep_uuid_v4", 3LL);
    ok = map_put(keys, values, (long long)"file_read", 3LL);
    ok = map_put(keys, values, (long long)"file_write", 1LL);
    ok = map_put(keys, values, (long long)"file_append", 1LL);
    ok = map_put(keys, values, (long long)"file_exists", 1LL);
    ok = map_put(keys, values, (long long)"read_line", 3LL);
    ok = map_put(keys, values, (long long)"read_int", 1LL);
    ok = map_put(keys, values, (long long)"read_key", 1LL);
    ok = map_put(keys, values, (long long)"terminal_columns", 1LL);
    ok = map_put(keys, values, (long long)"terminal_rows", 1LL);
    ok = map_put(keys, values, (long long)"ep_dlopen", 1LL);
    ok = map_put(keys, values, (long long)"ep_dlsym", 1LL);
    ok = map_put(keys, values, (long long)"ep_dlclose", 1LL);
    ok = map_put(keys, values, (long long)"ep_dlcall0", 1LL);
    ok = map_put(keys, values, (long long)"ep_dlcall1", 1LL);
    ok = map_put(keys, values, (long long)"ep_dlcall2", 1LL);
    ok = map_put(keys, values, (long long)"ep_dlcall3", 1LL);
    ok = map_put(keys, values, (long long)"ep_dlcall4", 1LL);
    ok = map_put(keys, values, (long long)"ep_dlcall5", 1LL);
    ok = map_put(keys, values, (long long)"ep_dlcall6", 1LL);
    ok = map_put(keys, values, (long long)"remove_list", 1LL);
    ok = map_put(keys, values, (long long)"map_size", 1LL);
    ok = map_put(keys, values, (long long)"map_get_str", 3LL);
    ok = map_put(keys, values, (long long)"map_set_str", 1LL);
    ok = map_put(keys, values, (long long)"map_keys", 4LL);
    ok = map_put(keys, values, (long long)"map_values", 4LL);
    ok = map_put(keys, values, (long long)"alloc_bytes", 1LL);
    ok = map_put(keys, values, (long long)"free_bytes", 1LL);
    ok = map_put(keys, values, (long long)"peek_byte", 1LL);
    ok = map_put(keys, values, (long long)"poke_byte", 1LL);
    ok = map_put(keys, values, (long long)"list_to_bytes", 1LL);
    ok = map_put(keys, values, (long long)"bytes_to_list", 4LL);
    ok = map_put(keys, values, (long long)"ep_gc_get_minor_count", 1LL);
    ok = map_put(keys, values, (long long)"ep_gc_get_major_count", 1LL);
    ok = map_put(keys, values, (long long)"ep_gc_get_nursery_count", 1LL);
    ok = set_list(state, 10LL, length_list(keys));
    externals = get_list(program, 2LL);
    ext_len = length_list(externals);
    idx = 0LL;
    while (idx < ext_len) {
    ext_node = get_list(externals, idx);
    ext_name = get_list(ext_node, 1LL);
    existing = map_get(keys, values, ext_name);
    if (existing == 0LL) {
    ok = map_put(keys, values, ext_name, 1LL);
    }
    idx = (idx + 1LL);
    }
    funcs = get_list(program, 3LL);
    funcs_len = length_list(funcs);
    pass = 0LL;
    while (pass < 3LL) {
    idx = 0LL;
    while (idx < funcs_len) {
    func = get_list(funcs, idx);
    name = get_list(func, 1LL);
    params = get_list(func, 2LL);
    body = get_list(func, 3LL);
    var_keys = (create_list() + 0LL);
    var_values = (create_list() + 0LL);
    ok = seed_param_types(params, var_keys, var_values);
    ok = collect_var_types(state, body, var_keys, var_values);
    ret_t = determine_ret_type(state, body, var_keys, var_values);
    if (ret_t == 0LL) {
    ret_t = 1LL;
    }
    ok = map_put(keys, values, name, ret_t);
    idx = (idx + 1LL);
    }
    pass = (pass + 1LL);
    }
    ret_val = 0LL;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(16);
    return ret_val;
}

long long collect_var_types(long long state, long long stmts, long long var_keys, long long var_values) {
    long long len = 0;
    long long idx = 0;
    long long stmt = 0;
    long long type = 0;
    long long var_name = 0;
    long long expr = 0;
    long long t = 0;
    long long ok = 0;
    long long then_b = 0;
    long long else_b = 0;
    long long body = 0;
    long long arms = 0;
    long long a_len = 0;
    long long a_idx = 0;
    long long arm = 0;
    long long arm_body = 0;
    long long fe_body = 0;
    long long ret_val = 0;

    ep_gc_push_root(&idx);
    ep_gc_push_root(&stmt);
    ep_gc_push_root(&var_name);
    ep_gc_push_root(&expr);
    ep_gc_push_root(&t);
    ep_gc_push_root(&then_b);
    ep_gc_push_root(&else_b);
    ep_gc_push_root(&body);
    ep_gc_push_root(&arms);
    ep_gc_push_root(&a_idx);
    ep_gc_push_root(&arm);
    ep_gc_push_root(&arm_body);
    ep_gc_push_root(&fe_body);
    ep_gc_push_root(&state);
    ep_gc_push_root(&stmts);
    ep_gc_push_root(&var_keys);
    ep_gc_push_root(&var_values);
    ep_gc_maybe_collect();

    len = length_list(stmts);
    idx = 0LL;
    while (idx < len) {
    stmt = get_list(stmts, idx);
    type = get_list(stmt, 0LL);
    if (type == 7LL) {
    var_name = get_list(stmt, 1LL);
    expr = get_list(stmt, 2LL);
    t = infer_type(state, expr, var_keys, var_values);
    if (get_list(expr, 0LL) == 24LL) {
    if (contains_string_val(get_list(state, 18LL), get_list(expr, 1LL)) == 1LL) {
    t = 9LL;
    }
    }
    ok = map_put(var_keys, var_values, var_name, t);
    } else {
    if (type == 10LL) {
    then_b = get_list(stmt, 2LL);
    ok = collect_var_types(state, then_b, var_keys, var_values);
    else_b = get_list(stmt, 3LL);
    if (else_b != 0LL) {
    ok = collect_var_types(state, else_b, var_keys, var_values);
    }
    } else {
    if (type == 11LL) {
    body = get_list(stmt, 2LL);
    ok = collect_var_types(state, body, var_keys, var_values);
    } else {
    if (type == 27LL) {
    arms = get_list(stmt, 2LL);
    a_len = length_list(arms);
    a_idx = 0LL;
    while (a_idx < a_len) {
    arm = get_list(arms, a_idx);
    arm_body = get_list(arm, 2LL);
    ok = collect_var_types(state, arm_body, var_keys, var_values);
    a_idx = (a_idx + 1LL);
    }
    } else {
    if (type == 28LL) {
    fe_body = get_list(stmt, 3LL);
    ok = collect_var_types(state, fe_body, var_keys, var_values);
    }
    }
    }
    }
    }
    idx = (idx + 1LL);
    }
    ret_val = 0LL;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(17);
    return ret_val;
}

long long determine_ret_type(long long state, long long stmts, long long var_keys, long long var_values) {
    long long len = 0;
    long long idx = 0;
    long long stmt = 0;
    long long type = 0;
    long long expr = 0;
    long long then_b = 0;
    long long ret_t = 0;
    long long else_b = 0;
    long long ret_t2 = 0;
    long long body = 0;
    long long ret_val = 0;

    ep_gc_push_root(&idx);
    ep_gc_push_root(&stmt);
    ep_gc_push_root(&expr);
    ep_gc_push_root(&then_b);
    ep_gc_push_root(&else_b);
    ep_gc_push_root(&body);
    ep_gc_push_root(&state);
    ep_gc_push_root(&stmts);
    ep_gc_push_root(&var_keys);
    ep_gc_push_root(&var_values);
    ep_gc_maybe_collect();

    len = length_list(stmts);
    idx = 0LL;
    while (idx < len) {
    stmt = get_list(stmts, idx);
    type = get_list(stmt, 0LL);
    if (type == 8LL) {
    expr = get_list(stmt, 1LL);
    ret_val = infer_type(state, expr, var_keys, var_values);
    goto L_cleanup;
    } else {
    if (type == 10LL) {
    then_b = get_list(stmt, 2LL);
    ret_t = determine_ret_type(state, then_b, var_keys, var_values);
    if (ret_t != 0LL) {
    ret_val = ret_t;
    goto L_cleanup;
    }
    else_b = get_list(stmt, 3LL);
    if (else_b != 0LL) {
    ret_t2 = determine_ret_type(state, else_b, var_keys, var_values);
    if (ret_t2 != 0LL) {
    ret_val = ret_t2;
    goto L_cleanup;
    }
    }
    } else {
    if (type == 11LL) {
    body = get_list(stmt, 2LL);
    ret_t = determine_ret_type(state, body, var_keys, var_values);
    if (ret_t != 0LL) {
    ret_val = ret_t;
    goto L_cleanup;
    }
    }
    }
    }
    idx = (idx + 1LL);
    }
    ret_val = 0LL;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(10);
    return ret_val;
}

long long infer_type(long long state, long long expr, long long var_keys, long long var_values) {
    long long type = 0;
    long long name = 0;
    long long t = 0;
    long long fl = 0;
    long long fr = 0;
    long long func_keys = 0;
    long long func_values = 0;
    long long inner = 0;
    long long ret_val = 0;

    ep_gc_push_root(&name);
    ep_gc_push_root(&func_keys);
    ep_gc_push_root(&func_values);
    ep_gc_push_root(&inner);
    ep_gc_push_root(&state);
    ep_gc_push_root(&expr);
    ep_gc_push_root(&var_keys);
    ep_gc_push_root(&var_values);
    ep_gc_maybe_collect();

    type = get_list(expr, 0LL);
    if (type == 1LL) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    if (type == 2LL) {
    ret_val = 2LL;
    goto L_cleanup;
    }
    if (type == 3LL) {
    name = get_list(expr, 1LL);
    t = map_get(var_keys, var_values, name);
    if (t != 0LL) {
    ret_val = t;
    goto L_cleanup;
    }
    ret_val = 1LL;
    goto L_cleanup;
    }
    if (type == 42LL) {
    ret_val = 8LL;
    goto L_cleanup;
    }
    if (type == 4LL) {
    fl = infer_type(state, get_list(expr, 1LL), var_keys, var_values);
    fr = infer_type(state, get_list(expr, 3LL), var_keys, var_values);
    if ((fl == 8LL || fr == 8LL)) {
    ret_val = 8LL;
    goto L_cleanup;
    }
    ret_val = 1LL;
    goto L_cleanup;
    }
    if ((type == 5LL || type == 14LL)) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    if ((type == 31LL || type == 32LL)) {
    ret_val = 7LL;
    goto L_cleanup;
    }
    if (type == 6LL) {
    name = get_list(expr, 1LL);
    func_keys = get_list(state, 3LL);
    func_values = get_list(state, 4LL);
    t = map_get(func_keys, func_values, name);
    if (t != 0LL) {
    ret_val = t;
    goto L_cleanup;
    }
    ret_val = 1LL;
    goto L_cleanup;
    }
    if (type == 20LL) {
    inner = get_list(expr, 1LL);
    t = infer_type(state, inner, var_keys, var_values);
    if ((t == 4LL || t == 5LL)) {
    ret_val = 5LL;
    goto L_cleanup;
    }
    if (((t == 2LL || t == 3LL) || t == 6LL)) {
    ret_val = 6LL;
    goto L_cleanup;
    }
    ret_val = 5LL;
    goto L_cleanup;
    }
    if (type == 21LL) {
    inner = get_list(expr, 1LL);
    ret_val = infer_type(state, inner, var_keys, var_values);
    goto L_cleanup;
    }
    if (((type == 24LL || type == 26LL) || type == 35LL)) {
    ret_val = 4LL;
    goto L_cleanup;
    }
    ret_val = 1LL;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(8);
    return ret_val;
}

long long is_global_var(long long name) {
    long long len = 0;
    long long has_upper = 0;
    long long idx = 0;
    long long ch = 0;
    long long ret_val = 0;

    ep_gc_push_root(&idx);
    ep_gc_push_root(&name);
    ep_gc_maybe_collect();

    len = string_length((char*)name);
    if (len == 0LL) {
    ret_val = 0LL;
    goto L_cleanup;
    }
    has_upper = 0LL;
    idx = 0LL;
    while (idx < len) {
    ch = get_character((char*)name, idx);
    if ((ch >= 97LL && ch <= 122LL)) {
    ret_val = 0LL;
    goto L_cleanup;
    }
    if ((ch >= 65LL && ch <= 90LL)) {
    has_upper = 1LL;
    }
    idx = (idx + 1LL);
    }
    ret_val = has_upper;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(2);
    return ret_val;
}

long long cg_string_contains(long long s, long long sub) {
    long long len = 0;
    long long sub_len = 0;
    long long limit = 0;
    long long i = 0;
    long long match = 0;
    long long j = 0;
    long long c1 = 0;
    long long c2 = 0;
    long long ret_val = 0;

    ep_gc_push_root(&i);
    ep_gc_push_root(&j);
    ep_gc_push_root(&s);
    ep_gc_push_root(&sub);
    ep_gc_maybe_collect();

    len = string_length((char*)s);
    sub_len = string_length((char*)sub);
    if (sub_len > len) {
    ret_val = 0LL;
    goto L_cleanup;
    }
    if (sub_len == 0LL) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    limit = ((len - sub_len) + 1LL);
    i = 0LL;
    while (i < limit) {
    match = 1LL;
    j = 0LL;
    while ((j < sub_len && match == 1LL)) {
    c1 = get_character((char*)s, (i + j));
    c2 = get_character((char*)sub, j);
    if (c1 != c2) {
    match = 0LL;
    }
    j = (j + 1LL);
    }
    if (match == 1LL) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    i = (i + 1LL);
    }
    ret_val = 0LL;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(4);
    return ret_val;
}

long long str_starts_with(long long s, long long prefix) {
    long long slen = 0;
    long long plen = 0;
    long long i = 0;
    long long ret_val = 0;

    ep_gc_push_root(&i);
    ep_gc_push_root(&s);
    ep_gc_push_root(&prefix);
    ep_gc_maybe_collect();

    slen = string_length((char*)s);
    plen = string_length((char*)prefix);
    if (plen > slen) {
    ret_val = 0LL;
    goto L_cleanup;
    }
    i = 0LL;
    while (i < plen) {
    if (get_character((char*)s, i) != get_character((char*)prefix, i)) {
    ret_val = 0LL;
    goto L_cleanup;
    }
    i = (i + 1LL);
    }
    ret_val = 1LL;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(3);
    return ret_val;
}

long long str_ends_with(long long s, long long suffix) {
    long long slen = 0;
    long long xlen = 0;
    long long off = 0;
    long long i = 0;
    long long ret_val = 0;

    ep_gc_push_root(&off);
    ep_gc_push_root(&i);
    ep_gc_push_root(&s);
    ep_gc_push_root(&suffix);
    ep_gc_maybe_collect();

    slen = string_length((char*)s);
    xlen = string_length((char*)suffix);
    if (xlen > slen) {
    ret_val = 0LL;
    goto L_cleanup;
    }
    off = (slen - xlen);
    i = 0LL;
    while (i < xlen) {
    if (get_character((char*)s, (off + i)) != get_character((char*)suffix, i)) {
    ret_val = 0LL;
    goto L_cleanup;
    }
    i = (i + 1LL);
    }
    ret_val = 1LL;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(4);
    return ret_val;
}

long long is_accessor_name(long long name) {
    long long ret_val = 0;

    ep_gc_push_root(&name);
    ep_gc_maybe_collect();

    if ((strcmp((char*)(long long)"get", (char*)name) == 0)) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    if ((strcmp((char*)(long long)"peek", (char*)name) == 0)) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    if (str_starts_with(name, (long long)"get_") == 1LL) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    if (str_starts_with(name, (long long)"peek_") == 1LL) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    if (str_ends_with(name, (long long)"_get") == 1LL) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    if (str_ends_with(name, (long long)"_peek") == 1LL) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    ret_val = 0LL;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(1);
    return ret_val;
}

long long is_borrow_expr(long long expr, long long borrowed_keys, long long borrowed_values) {
    long long type = 0;
    long long func_name = 0;
    long long name = 0;
    long long is_borrowed = 0;
    long long ret_val = 0;

    ep_gc_push_root(&func_name);
    ep_gc_push_root(&name);
    ep_gc_push_root(&expr);
    ep_gc_push_root(&borrowed_keys);
    ep_gc_push_root(&borrowed_values);
    ep_gc_maybe_collect();

    type = get_list(expr, 0LL);
    if (type == 20LL) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    if (type == 6LL) {
    func_name = get_list(expr, 1LL);
    if (is_accessor_name(func_name) == 1LL) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    }
    if (type == 3LL) {
    name = get_list(expr, 1LL);
    is_borrowed = map_get(borrowed_keys, borrowed_values, name);
    if (is_borrowed == 1LL) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    }
    ret_val = 0LL;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(5);
    return ret_val;
}

long long scan_stmts_for_borrows(long long stmts, long long borrowed_keys, long long borrowed_values) {
    long long len = 0;
    long long idx = 0;
    long long stmt = 0;
    long long type = 0;
    long long var_name = 0;
    long long expr = 0;
    long long is_borrow = 0;
    long long ok = 0;
    long long then_b = 0;
    long long else_b = 0;
    long long body = 0;
    long long ret_val = 0;

    ep_gc_push_root(&idx);
    ep_gc_push_root(&stmt);
    ep_gc_push_root(&var_name);
    ep_gc_push_root(&expr);
    ep_gc_push_root(&then_b);
    ep_gc_push_root(&else_b);
    ep_gc_push_root(&body);
    ep_gc_push_root(&stmts);
    ep_gc_push_root(&borrowed_keys);
    ep_gc_push_root(&borrowed_values);
    ep_gc_maybe_collect();

    len = length_list(stmts);
    idx = 0LL;
    while (idx < len) {
    stmt = get_list(stmts, idx);
    type = get_list(stmt, 0LL);
    if (type == 7LL) {
    var_name = get_list(stmt, 1LL);
    expr = get_list(stmt, 2LL);
    is_borrow = is_borrow_expr(expr, borrowed_keys, borrowed_values);
    if (is_borrow == 1LL) {
    ok = map_put(borrowed_keys, borrowed_values, var_name, 1LL);
    } else {
    ok = map_put(borrowed_keys, borrowed_values, var_name, 0LL);
    }
    }
    if (type == 10LL) {
    then_b = get_list(stmt, 2LL);
    ok = scan_stmts_for_borrows(then_b, borrowed_keys, borrowed_values);
    else_b = get_list(stmt, 3LL);
    if (else_b != 0LL) {
    ok = scan_stmts_for_borrows(else_b, borrowed_keys, borrowed_values);
    }
    }
    if (type == 11LL) {
    body = get_list(stmt, 2LL);
    ok = scan_stmts_for_borrows(body, borrowed_keys, borrowed_values);
    }
    idx = (idx + 1LL);
    }
    ret_val = 0LL;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(10);
    return ret_val;
}

long long collect_borrowed_vars(long long stmts, long long params, long long borrowed_keys, long long borrowed_values) {
    long long p_len = 0;
    long long idx = 0;
    long long p_node = 0;
    long long p_name = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&idx);
    ep_gc_push_root(&p_node);
    ep_gc_push_root(&p_name);
    ep_gc_push_root(&stmts);
    ep_gc_push_root(&params);
    ep_gc_push_root(&borrowed_keys);
    ep_gc_push_root(&borrowed_values);
    ep_gc_maybe_collect();

    p_len = length_list(params);
    idx = 0LL;
    while (idx < p_len) {
    p_node = get_list(params, idx);
    p_name = get_list(p_node, 0LL);
    ok = map_put(borrowed_keys, borrowed_values, p_name, 1LL);
    idx = (idx + 1LL);
    }
    ok = scan_stmts_for_borrows(stmts, borrowed_keys, borrowed_values);
    ret_val = 0LL;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(7);
    return ret_val;
}

long long var_returned_in_stmts(long long name, long long stmts) {
    long long n = 0;
    long long idx = 0;
    long long stmt = 0;
    long long t = 0;
    long long ids = 0;
    long long ok = 0;
    long long eb = 0;
    long long arms = 0;
    long long an = 0;
    long long ai = 0;
    long long ret_val = 0;

    ep_gc_push_root(&idx);
    ep_gc_push_root(&stmt);
    ep_gc_push_root(&ids);
    ep_gc_push_root(&eb);
    ep_gc_push_root(&arms);
    ep_gc_push_root(&ai);
    ep_gc_push_root(&name);
    ep_gc_push_root(&stmts);
    ep_gc_maybe_collect();

    n = length_list(stmts);
    idx = 0LL;
    while (idx < n) {
    stmt = get_list(stmts, idx);
    t = get_list(stmt, 0LL);
    if (t == 8LL) {
    ids = create_list();
    ok = collect_idents_expr(get_list(stmt, 1LL), ids);
    if (contains_string_val(ids, name) == 1LL) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    }
    if (t == 10LL) {
    if (var_returned_in_stmts(name, get_list(stmt, 2LL)) == 1LL) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    eb = get_list(stmt, 3LL);
    if (eb != 0LL) {
    if (var_returned_in_stmts(name, eb) == 1LL) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    }
    }
    if (t == 11LL) {
    if (var_returned_in_stmts(name, get_list(stmt, 2LL)) == 1LL) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    }
    if (t == 28LL) {
    if (var_returned_in_stmts(name, get_list(stmt, 3LL)) == 1LL) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    }
    if (t == 27LL) {
    arms = get_list(stmt, 2LL);
    an = length_list(arms);
    ai = 0LL;
    while (ai < an) {
    if (var_returned_in_stmts(name, get_list(get_list(arms, ai), 2LL)) == 1LL) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    ai = (ai + 1LL);
    }
    }
    idx = (idx + 1LL);
    }
    ret_val = 0LL;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(8);
    return ret_val;
}

long long gen_function(long long state, long long func) {
    long long name = 0;
    long long params = 0;
    long long body = 0;
    long long is_async = 0;
    long long func_keys = 0;
    long long func_values = 0;
    long long ret_t = 0;
    long long ok = 0;
    long long var_types_keys = 0;
    long long var_types_values = 0;
    long long p_len = 0;
    long long cname = 0;
    long long aw_count = 0;
    long long async_locals = 0;
    long long al_i = 0;
    long long okal = 0;
    long long sd = 0;
    long long vi = 0;
    long long awi = 0;
    long long sf = 0;
    long long bs_i = 0;
    long long bstmt = 0;
    long long saved_ctr = 0;
    long long post_ctr = 0;
    long long pf = 0;
    long long pj = 0;
    long long pnm = 0;
    long long impl_name = 0;
    long long header = 0;
    long long p_idx = 0;
    long long p_node = 0;
    long long p_name = 0;
    long long borrowed_keys = 0;
    long long borrowed_values = 0;
    long long num_vars = 0;
    long long idx = 0;
    long long var_name = 0;
    long long is_param = 0;
    long long p_i = 0;
    long long is_global = 0;
    long long decl = 0;
    long long gc_root_count = 0;
    long long is_p = 0;
    long long t = 0;
    long long should_root = 0;
    long long root_line = 0;
    long long body_len = 0;
    long long stmt = 0;
    long long gc_count_str = 0;
    long long root_pop = 0;
    long long ret_val = 0;

    ep_gc_push_root(&name);
    ep_gc_push_root(&params);
    ep_gc_push_root(&body);
    ep_gc_push_root(&func_keys);
    ep_gc_push_root(&func_values);
    ep_gc_push_root(&ret_t);
    ep_gc_push_root(&var_types_keys);
    ep_gc_push_root(&var_types_values);
    ep_gc_push_root(&cname);
    ep_gc_push_root(&async_locals);
    ep_gc_push_root(&al_i);
    ep_gc_push_root(&sd);
    ep_gc_push_root(&vi);
    ep_gc_push_root(&awi);
    ep_gc_push_root(&sf);
    ep_gc_push_root(&bs_i);
    ep_gc_push_root(&bstmt);
    ep_gc_push_root(&saved_ctr);
    ep_gc_push_root(&post_ctr);
    ep_gc_push_root(&pf);
    ep_gc_push_root(&pj);
    ep_gc_push_root(&pnm);
    ep_gc_push_root(&impl_name);
    ep_gc_push_root(&header);
    ep_gc_push_root(&p_idx);
    ep_gc_push_root(&p_node);
    ep_gc_push_root(&p_name);
    ep_gc_push_root(&borrowed_keys);
    ep_gc_push_root(&borrowed_values);
    ep_gc_push_root(&idx);
    ep_gc_push_root(&var_name);
    ep_gc_push_root(&p_i);
    ep_gc_push_root(&decl);
    ep_gc_push_root(&gc_root_count);
    ep_gc_push_root(&root_line);
    ep_gc_push_root(&stmt);
    ep_gc_push_root(&gc_count_str);
    ep_gc_push_root(&root_pop);
    ep_gc_push_root(&state);
    ep_gc_push_root(&func);
    ep_gc_maybe_collect();

    name = get_list(func, 1LL);
    params = get_list(func, 2LL);
    body = get_list(func, 3LL);
    is_async = 0LL;
    if (length_list(func) > 4LL) {
    is_async = get_list(func, 4LL);
    }
    func_keys = get_list(state, 3LL);
    func_values = get_list(state, 4LL);
    ret_t = map_get(func_keys, func_values, name);
    ok = set_list(state, 5LL, ret_t);
    var_types_keys = (create_list() + 0LL);
    var_types_values = (create_list() + 0LL);
    p_len = length_list(params);
    ok = seed_param_types(params, var_types_keys, var_types_values);
    ok = collect_var_types(state, body, var_types_keys, var_types_values);
    ok = infer_usage_types_stmts(body, var_types_keys, var_types_values);
    if (is_async == 1LL) {
    cname = get_fn_c_name(func);
    aw_count = count_awaits_stmts(body);
    async_locals = create_list();
    al_i = 0LL;
    while (al_i < length_list(var_types_keys)) {
    okal = append_list(async_locals, get_list(var_types_keys, al_i));
    al_i = (al_i + 1LL);
    }
    sd = (long long)"typedef struct {\n    int state;\n    EpFuture* fut;\n";
    vi = 0LL;
    while (vi < length_list(var_types_keys)) {
    sd = string_concat(sd, (long long)"    long long ");
    sd = string_concat(sd, get_list(var_types_keys, vi));
    sd = string_concat(sd, (long long)";\n");
    vi = (vi + 1LL);
    }
    awi = 1LL;
    while (awi <= aw_count) {
    sd = string_concat(sd, (long long)"    EpFuture* awaited_fut_");
    sd = string_concat(sd, cg_int_to_str(awi));
    sd = string_concat(sd, (long long)";\n");
    awi = (awi + 1LL);
    }
    if (length_list(var_types_keys) == 0LL) {
    if (aw_count == 0LL) {
    sd = string_concat(sd, (long long)"    int dummy;\n");
    }
    }
    sd = string_concat(sd, (long long)"} ");
    sd = string_concat(sd, cname);
    sd = string_concat(sd, (long long)"_async_args;\n\n");
    ok = emit(state, sd);
    sf = (long long)"long long ";
    sf = string_concat(sf, cname);
    sf = string_concat(sf, (long long)"_step(void* r) {\n    ");
    sf = string_concat(sf, cname);
    sf = string_concat(sf, (long long)"_async_args* args = (");
    sf = string_concat(sf, cname);
    sf = string_concat(sf, (long long)"_async_args*)r;\n    switch (args->state) {\n        case 0:\n");
    ok = emit(state, sf);
    ok = set_list(state, 19LL, 1LL);
    ok = set_list(state, 21LL, async_locals);
    ok = set_list(state, 20LL, 0LL);
    ok = set_codegen_borrowed_keys(state, (create_list() + 0LL));
    ok = set_codegen_borrowed_values(state, (create_list() + 0LL));
    bs_i = 0LL;
    while (bs_i < length_list(body)) {
    bstmt = get_list(body, bs_i);
    saved_ctr = get_list(state, 20LL);
    ok = emit_async_yields_stmt(state, bstmt, var_types_keys, var_types_values);
    post_ctr = get_list(state, 20LL);
    ok = set_list(state, 20LL, saved_ctr);
    ok = gen_statement(state, bstmt, var_types_keys, var_types_values);
    ok = set_list(state, 20LL, post_ctr);
    bs_i = (bs_i + 1LL);
    }
    ok = emit(state, (long long)"            args->state = -1;\n            return 0;\n    }\n    return 0;\n}\n\n");
    ok = set_list(state, 19LL, 0LL);
    pf = (long long)"long long ";
    pf = string_concat(pf, cname);
    pf = string_concat(pf, (long long)"(");
    pj = 0LL;
    while (pj < p_len) {
    pf = string_concat(pf, (long long)"long long ");
    pf = string_concat(pf, get_list(get_list(params, pj), 0LL));
    if (pj < (p_len - 1LL)) {
    pf = string_concat(pf, (long long)", ");
    }
    pj = (pj + 1LL);
    }
    pf = string_concat(pf, (long long)") {\n");
    pf = string_concat(pf, (long long)"    EpFuture* fut = (EpFuture*)malloc(sizeof(EpFuture));\n");
    pf = string_concat(pf, (long long)"    fut->completed = 0; fut->value = 0; fut->waiting_task = NULL; fut->chan = 0;\n");
    pf = string_concat(pf, (long long)"    { EpGCObject* _go = ep_gc_register(fut, EP_OBJ_STRUCT); if(_go) _go->num_fields = 3; }\n");
    pf = string_concat(pf, (long long)"    ");
    pf = string_concat(pf, cname);
    pf = string_concat(pf, (long long)"_async_args* args = (");
    pf = string_concat(pf, cname);
    pf = string_concat(pf, (long long)"_async_args*)malloc(sizeof(");
    pf = string_concat(pf, cname);
    pf = string_concat(pf, (long long)"_async_args));\n    memset(args, 0, sizeof(");
    pf = string_concat(pf, cname);
    pf = string_concat(pf, (long long)"_async_args));\n    args->state = 0;\n    args->fut = fut;\n");
    pj = 0LL;
    while (pj < p_len) {
    pnm = get_list(get_list(params, pj), 0LL);
    pf = string_concat(pf, (long long)"    args->");
    pf = string_concat(pf, pnm);
    pf = string_concat(pf, (long long)" = ");
    pf = string_concat(pf, pnm);
    pf = string_concat(pf, (long long)";\n");
    pj = (pj + 1LL);
    }
    pf = string_concat(pf, (long long)"    EpTask* task = (EpTask*)malloc(sizeof(EpTask));\n");
    pf = string_concat(pf, (long long)"    task->step = ");
    pf = string_concat(pf, cname);
    pf = string_concat(pf, (long long)"_step;\n    task->args = args;\n    task->args_size_bytes = sizeof(");
    pf = string_concat(pf, cname);
    pf = string_concat(pf, (long long)"_async_args);\n    task->fut = fut;\n    task->state = 0;\n    task->is_cancelled = 0;\n    task->parent = ep_current_task;\n    ep_task_enqueue(task);\n    return (long long)fut;\n}\n\n");
    ok = emit(state, pf);
    ret_val = 0LL;
    goto L_cleanup;
    }
    impl_name = get_fn_c_name(func);
    header = (long long)"long long ";
    header = string_concat(header, impl_name);
    header = string_concat(header, (long long)"(");
    p_idx = 0LL;
    while (p_idx < p_len) {
    p_node = get_list(params, p_idx);
    p_name = get_list(p_node, 0LL);
    header = string_concat(header, (long long)"long long ");
    header = string_concat(header, p_name);
    if (p_idx < (p_len - 1LL)) {
    header = string_concat(header, (long long)", ");
    }
    p_idx = (p_idx + 1LL);
    }
    header = string_concat(header, (long long)") {\n");
    ok = emit(state, header);
    borrowed_keys = (create_list() + 0LL);
    borrowed_values = (create_list() + 0LL);
    ok = collect_borrowed_vars(body, params, borrowed_keys, borrowed_values);
    ok = set_codegen_borrowed_keys(state, borrowed_keys);
    ok = set_codegen_borrowed_values(state, borrowed_values);
    num_vars = length_list(var_types_keys);
    idx = 0LL;
    while (idx < num_vars) {
    var_name = get_list(var_types_keys, idx);
    is_param = 0LL;
    p_i = 0LL;
    while (p_i < p_len) {
    p_node = get_list(params, p_i);
    p_name = get_list(p_node, 0LL);
    if (var_name == p_name) {
    is_param = 1LL;
    }
    p_i = (p_i + 1LL);
    }
    if (is_param == 0LL) {
    is_global = is_global_var(var_name);
    if (is_global == 0LL) {
    decl = (long long)"    long long ";
    decl = string_concat(decl, var_name);
    decl = string_concat(decl, (long long)" = 0;\n");
    ok = emit(state, decl);
    }
    }
    idx = (idx + 1LL);
    }
    ok = emit(state, (long long)"    long long ret_val = 0;\n\n");
    gc_root_count = 0LL;
    idx = 0LL;
    while (idx < num_vars) {
    var_name = get_list(var_types_keys, idx);
    is_p = 0LL;
    p_i = 0LL;
    while (p_i < p_len) {
    p_node = get_list(params, p_i);
    p_name = get_list(p_node, 0LL);
    if (var_name == p_name) {
    is_p = 1LL;
    }
    p_i = (p_i + 1LL);
    }
    if (is_p == 0LL) {
    is_global = is_global_var(var_name);
    if (is_global == 0LL) {
    t = map_get(var_types_keys, var_types_values, var_name);
    should_root = 1LL;
    if (((t == 1LL || t == 7LL) || t == 8LL)) {
    should_root = cg_stmts_have_nonprim(state, body, string_concat(var_name, (long long)""));
    }
    if (should_root == 1LL) {
    root_line = (long long)"    ep_gc_push_root(&";
    root_line = string_concat(root_line, var_name);
    root_line = string_concat(root_line, (long long)");\n");
    ok = emit(state, root_line);
    gc_root_count = (gc_root_count + 1LL);
    }
    }
    }
    idx = (idx + 1LL);
    }
    p_i = 0LL;
    while (p_i < p_len) {
    p_node = get_list(params, p_i);
    p_name = get_list(p_node, 0LL);
    t = map_get(var_types_keys, var_types_values, p_name);
    should_root = 1LL;
    if (((t == 1LL || t == 7LL) || t == 8LL)) {
    should_root = cg_stmts_have_nonprim(state, body, string_concat(p_name, (long long)""));
    }
    if (should_root == 1LL) {
    root_line = (long long)"    ep_gc_push_root(&";
    root_line = string_concat(root_line, p_name);
    root_line = string_concat(root_line, (long long)");\n");
    ok = emit(state, root_line);
    gc_root_count = (gc_root_count + 1LL);
    }
    p_i = (p_i + 1LL);
    }
    ok = emit(state, (long long)"    ep_gc_maybe_collect();\n\n");
    body_len = length_list(body);
    idx = 0LL;
    while (idx < body_len) {
    stmt = get_list(body, idx);
    ok = gen_statement(state, stmt, var_types_keys, var_types_values);
    idx = (idx + 1LL);
    }
    ok = emit(state, (long long)"L_cleanup:\n");
    if (gc_root_count > 0LL) {
    gc_count_str = cg_int_to_str(gc_root_count);
    root_pop = (long long)"    ep_gc_pop_roots(";
    root_pop = string_concat(root_pop, gc_count_str);
    root_pop = string_concat(root_pop, (long long)");\n");
    ok = emit(state, root_pop);
    }
    idx = 0LL;
    while (idx < num_vars) {
    var_name = get_list(var_types_keys, idx);
    is_param = 0LL;
    p_i = 0LL;
    while (p_i < p_len) {
    p_node = get_list(params, p_i);
    p_name = get_list(p_node, 0LL);
    if (var_name == p_name) {
    is_param = 1LL;
    }
    p_i = (p_i + 1LL);
    }
    idx = (idx + 1LL);
    }
    ok = emit(state, (long long)"    return ret_val;\n}\n\n");
    ret_val = 0LL;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(40);
    return ret_val;
}

long long gen_statement(long long state, long long stmt, long long var_keys, long long var_values) {
    long long type = 0;
    long long name = 0;
    long long expr = 0;
    long long t = 0;
    long long expr_str = 0;
    long long al = 0;
    long long ok = 0;
    long long line = 0;
    long long arl = 0;
    long long expr_type = 0;
    long long null_line = 0;
    long long is_any_call = 0;
    long long callee = 0;
    long long cond = 0;
    long long then_b = 0;
    long long else_b = 0;
    long long cond_str = 0;
    long long t_len = 0;
    long long t_idx = 0;
    long long s = 0;
    long long e_len = 0;
    long long e_idx = 0;
    long long body = 0;
    long long b_len = 0;
    long long b_idx = 0;
    long long func_name = 0;
    long long args = 0;
    long long args_len = 0;
    long long idx_val = 0;
    long long j = 0;
    long long arg = 0;
    long long arg_str = 0;
    long long chan = 0;
    long long val = 0;
    long long chan_str = 0;
    long long val_str = 0;
    long long val_type = 0;
    long long obj = 0;
    long long field_name = 0;
    long long obj_str = 0;
    long long arms = 0;
    long long arm_len = 0;
    long long arm_idx = 0;
    long long arm = 0;
    long long vname = 0;
    long long bindings = 0;
    long long arm_body = 0;
    long long pat_kind = 0;
    long long kw = 0;
    long long vp_codes = 0;
    long long vpk = 0;
    long long vpv = 0;
    long long vpk_i = 0;
    long long vpk_len = 0;
    long long bname = 0;
    long long bcode = 0;
    long long ab_len = 0;
    long long ab_idx = 0;
    long long var_name = 0;
    long long iter_expr = 0;
    long long iter_str = 0;
    long long label = 0;
    long long iter_t = 0;
    long long il = 0;
    long long bl = 0;
    long long ib_len = 0;
    long long ib_i = 0;
    long long ret_val = 0;

    ep_gc_push_root(&name);
    ep_gc_push_root(&expr);
    ep_gc_push_root(&expr_str);
    ep_gc_push_root(&al);
    ep_gc_push_root(&line);
    ep_gc_push_root(&arl);
    ep_gc_push_root(&null_line);
    ep_gc_push_root(&callee);
    ep_gc_push_root(&cond);
    ep_gc_push_root(&then_b);
    ep_gc_push_root(&else_b);
    ep_gc_push_root(&cond_str);
    ep_gc_push_root(&t_idx);
    ep_gc_push_root(&s);
    ep_gc_push_root(&e_idx);
    ep_gc_push_root(&body);
    ep_gc_push_root(&b_idx);
    ep_gc_push_root(&args);
    ep_gc_push_root(&idx_val);
    ep_gc_push_root(&j);
    ep_gc_push_root(&arg);
    ep_gc_push_root(&arg_str);
    ep_gc_push_root(&chan);
    ep_gc_push_root(&val);
    ep_gc_push_root(&chan_str);
    ep_gc_push_root(&val_str);
    ep_gc_push_root(&obj);
    ep_gc_push_root(&field_name);
    ep_gc_push_root(&obj_str);
    ep_gc_push_root(&arms);
    ep_gc_push_root(&arm_idx);
    ep_gc_push_root(&arm);
    ep_gc_push_root(&vname);
    ep_gc_push_root(&bindings);
    ep_gc_push_root(&arm_body);
    ep_gc_push_root(&kw);
    ep_gc_push_root(&vp_codes);
    ep_gc_push_root(&vpk);
    ep_gc_push_root(&vpv);
    ep_gc_push_root(&vpk_i);
    ep_gc_push_root(&bname);
    ep_gc_push_root(&bcode);
    ep_gc_push_root(&ab_idx);
    ep_gc_push_root(&var_name);
    ep_gc_push_root(&iter_expr);
    ep_gc_push_root(&iter_str);
    ep_gc_push_root(&label);
    ep_gc_push_root(&il);
    ep_gc_push_root(&bl);
    ep_gc_push_root(&ib_i);
    ep_gc_push_root(&state);
    ep_gc_push_root(&stmt);
    ep_gc_push_root(&var_keys);
    ep_gc_push_root(&var_values);
    ep_gc_maybe_collect();

    type = get_list(stmt, 0LL);
    if (type == 7LL) {
    name = get_list(stmt, 1LL);
    expr = get_list(stmt, 2LL);
    t = map_get(var_keys, var_values, name);
    expr_str = gen_expr(state, expr, var_keys, var_values);
    if (get_list(state, 19LL) == 1LL) {
    if (contains_string_val(get_list(state, 21LL), name) == 1LL) {
    al = (long long)"    args->";
    al = string_concat(al, name);
    al = string_concat(al, (long long)" = ");
    al = string_concat(al, expr_str);
    al = string_concat(al, (long long)";\n");
    ok = emit(state, al);
    ret_val = 0LL;
    goto L_cleanup;
    }
    }
    line = (long long)"    ";
    line = string_concat(line, name);
    line = string_concat(line, (long long)" = ");
    line = string_concat(line, expr_str);
    line = string_concat(line, (long long)";\n");
    ok = emit(state, line);
    ret_val = 0LL;
    goto L_cleanup;
    }
    if (type == 8LL) {
    expr = get_list(stmt, 1LL);
    expr_str = gen_expr(state, expr, var_keys, var_values);
    if (get_list(state, 19LL) == 1LL) {
    arl = (long long)"    return ";
    arl = string_concat(arl, expr_str);
    arl = string_concat(arl, (long long)";\n");
    ok = emit(state, arl);
    ret_val = 0LL;
    goto L_cleanup;
    }
    line = (long long)"    ret_val = ";
    line = string_concat(line, expr_str);
    line = string_concat(line, (long long)";\n");
    ok = emit(state, line);
    expr_type = get_list(expr, 0LL);
    if (expr_type == 3LL) {
    name = get_list(expr, 1LL);
    t = map_get(var_keys, var_values, name);
    if (t == 4LL) {
    null_line = (long long)"    ";
    null_line = string_concat(null_line, name);
    null_line = string_concat(null_line, (long long)" = 0;\n");
    ok = emit(state, null_line);
    }
    }
    ok = emit(state, (long long)"    goto L_cleanup;\n");
    ret_val = 0LL;
    goto L_cleanup;
    }
    if (type == 9LL) {
    expr = get_list(stmt, 1LL);
    t = infer_type(state, expr, var_keys, var_values);
    expr_str = gen_expr(state, expr, var_keys, var_values);
    is_any_call = 0LL;
    if (get_list(expr, 0LL) == 6LL) {
    callee = string_concat(get_list(expr, 1LL), (long long)"");
    if ((strcmp((char*)callee, (char*)(long long)"get_list") == 0)) {
    is_any_call = 1LL;
    }
    if ((strcmp((char*)callee, (char*)(long long)"pop_list") == 0)) {
    is_any_call = 1LL;
    }
    if ((strcmp((char*)callee, (char*)(long long)"map_get_val") == 0)) {
    is_any_call = 1LL;
    }
    if ((strcmp((char*)callee, (char*)(long long)"map_get_str") == 0)) {
    is_any_call = 1LL;
    }
    }
    if (is_any_call == 1LL) {
    line = (long long)"    printf(\"%s\\n\", (char*)ep_auto_to_string(";
    line = string_concat(line, expr_str);
    line = string_concat(line, (long long)"));\n");
    ok = emit(state, line);
    ret_val = 0LL;
    goto L_cleanup;
    }
    if ((t == 2LL || t == 3LL)) {
    line = (long long)"    printf(\"%s\\n\", (char*)";
    line = string_concat(line, expr_str);
    line = string_concat(line, (long long)");\n");
    ok = emit(state, line);
    } else {
    if (t == 8LL) {
    line = (long long)"    { long long _ftmp = ";
    line = string_concat(line, expr_str);
    line = string_concat(line, (long long)"; double _dv; memcpy(&_dv, &_ftmp, sizeof(double)); printf(\"%.15g\\n\", _dv); }\n");
    ok = emit(state, line);
    } else {
    if (t == 7LL) {
    line = (long long)"    printf(\"%s\\n\", (";
    line = string_concat(line, expr_str);
    line = string_concat(line, (long long)") ? \"true\" : \"false\");\n");
    ok = emit(state, line);
    } else {
    line = (long long)"    printf(\"%s\\n\", (char*)ep_auto_to_string(";
    line = string_concat(line, expr_str);
    line = string_concat(line, (long long)"));\n");
    ok = emit(state, line);
    }
    }
    }
    ret_val = 0LL;
    goto L_cleanup;
    }
    if (type == 10LL) {
    cond = get_list(stmt, 1LL);
    then_b = get_list(stmt, 2LL);
    else_b = get_list(stmt, 3LL);
    cond_str = gen_expr(state, cond, var_keys, var_values);
    line = (long long)"    if (";
    line = string_concat(line, cond_str);
    line = string_concat(line, (long long)") {\n");
    ok = emit(state, line);
    t_len = length_list(then_b);
    t_idx = 0LL;
    while (t_idx < t_len) {
    s = get_list(then_b, t_idx);
    ok = gen_statement(state, s, var_keys, var_values);
    t_idx = (t_idx + 1LL);
    }
    if (else_b != 0LL) {
    ok = emit(state, (long long)"    } else {\n");
    e_len = length_list(else_b);
    e_idx = 0LL;
    while (e_idx < e_len) {
    s = get_list(else_b, e_idx);
    ok = gen_statement(state, s, var_keys, var_values);
    e_idx = (e_idx + 1LL);
    }
    ok = emit(state, (long long)"    }\n");
    } else {
    ok = emit(state, (long long)"    }\n");
    }
    ret_val = 0LL;
    goto L_cleanup;
    }
    if (type == 11LL) {
    cond = get_list(stmt, 1LL);
    body = get_list(stmt, 2LL);
    cond_str = gen_expr(state, cond, var_keys, var_values);
    line = (long long)"    while (";
    line = string_concat(line, cond_str);
    line = string_concat(line, (long long)") {\n");
    ok = emit(state, line);
    b_len = length_list(body);
    b_idx = 0LL;
    while (b_idx < b_len) {
    s = get_list(body, b_idx);
    ok = gen_statement(state, s, var_keys, var_values);
    b_idx = (b_idx + 1LL);
    }
    ok = emit(state, (long long)"    }\n");
    ret_val = 0LL;
    goto L_cleanup;
    }
    if (type == 15LL) {
    func_name = get_list(stmt, 1LL);
    args = get_list(stmt, 2LL);
    args_len = length_list(args);
    idx_val = get_codegen_spawn_index(state);
    ok = set_codegen_spawn_index(state, (idx_val + 1LL));
    ok = emit(state, (long long)"    {\n");
    line = (long long)"        spawn_args_";
    line = string_concat(line, cg_int_to_str(idx_val));
    line = string_concat(line, (long long)"* s_args = malloc(sizeof(spawn_args_");
    line = string_concat(line, cg_int_to_str(idx_val));
    line = string_concat(line, (long long)"));\n");
    ok = emit(state, line);
    j = 0LL;
    while (j < args_len) {
    arg = get_list(args, j);
    arg_str = gen_expr(state, arg, var_keys, var_values);
    line = (long long)"        s_args->arg";
    line = string_concat(line, cg_int_to_str(j));
    line = string_concat(line, (long long)" = ");
    line = string_concat(line, arg_str);
    line = string_concat(line, (long long)";\n");
    ok = emit(state, line);
    j = (j + 1LL);
    }
    ok = emit(state, (long long)"        pthread_t t;\n");
    line = (long long)"        pthread_create(&t, NULL, spawn_wrapper_";
    line = string_concat(line, cg_int_to_str(idx_val));
    line = string_concat(line, (long long)", s_args);\n");
    ok = emit(state, line);
    ok = emit(state, (long long)"        pthread_detach(t);\n");
    ok = emit(state, (long long)"    }\n");
    ret_val = 0LL;
    goto L_cleanup;
    }
    if (type == 16LL) {
    chan = get_list(stmt, 1LL);
    val = get_list(stmt, 2LL);
    chan_str = gen_expr(state, chan, var_keys, var_values);
    val_str = gen_expr(state, val, var_keys, var_values);
    line = (long long)"    send_channel(";
    line = string_concat(line, chan_str);
    line = string_concat(line, (long long)", ");
    line = string_concat(line, val_str);
    line = string_concat(line, (long long)");\n");
    ok = emit(state, line);
    val_type = get_list(val, 0LL);
    if (val_type == 3LL) {
    name = get_list(val, 1LL);
    t = map_get(var_keys, var_values, name);
    if (t == 4LL) {
    null_line = (long long)"    ";
    null_line = string_concat(null_line, name);
    null_line = string_concat(null_line, (long long)" = 0;\n");
    ok = emit(state, null_line);
    }
    }
    ret_val = 0LL;
    goto L_cleanup;
    }
    if (type == 23LL) {
    obj = get_list(stmt, 1LL);
    field_name = get_list(stmt, 2LL);
    val = get_list(stmt, 3LL);
    obj_str = gen_expr(state, obj, var_keys, var_values);
    val_str = gen_expr(state, val, var_keys, var_values);
    line = (long long)"    { long long* _ep_fo = (long long*)(";
    line = string_concat(line, obj_str);
    line = string_concat(line, (long long)"); long long _ep_fv = ");
    line = string_concat(line, val_str);
    line = string_concat(line, (long long)"; _ep_fo[EP_FIELD_");
    line = string_concat(line, field_name);
    line = string_concat(line, (long long)"] = _ep_fv; ep_gc_write_barrier((void*)_ep_fo, _ep_fv); }\n");
    ok = emit(state, line);
    ret_val = 0LL;
    goto L_cleanup;
    }
    if (type == 27LL) {
    expr = get_list(stmt, 1LL);
    arms = get_list(stmt, 2LL);
    expr_str = gen_expr(state, expr, var_keys, var_values);
    ok = emit(state, (long long)"    {\n");
    line = (long long)"        long long _match_val = ";
    line = string_concat(line, expr_str);
    line = string_concat(line, (long long)";\n");
    ok = emit(state, line);
    arm_len = length_list(arms);
    arm_idx = 0LL;
    while (arm_idx < arm_len) {
    arm = get_list(arms, arm_idx);
    vname = get_list(arm, 0LL);
    bindings = get_list(arm, 1LL);
    arm_body = get_list(arm, 2LL);
    pat_kind = 0LL;
    if (length_list(arm) > 3LL) {
    pat_kind = get_list(arm, 3LL);
    }
    kw = (long long)"if";
    if (arm_idx > 0LL) {
    kw = (long long)"} else if";
    }
    line = (long long)"        ";
    line = string_concat(line, kw);
    if (pat_kind == 26LL) {
    line = string_concat(line, (long long)" (strcmp((char*)_match_val, \"");
    line = string_concat(line, escape_string(vname));
    line = string_concat(line, (long long)"\") == 0) {\n");
    } else {
    if (pat_kind == 25LL) {
    line = string_concat(line, (long long)" (_match_val == ");
    line = string_concat(line, vname);
    line = string_concat(line, (long long)"LL) {\n");
    } else {
    line = string_concat(line, (long long)" (((long long*)_match_val)[0] == EP_TAG_");
    line = string_concat(line, vname);
    line = string_concat(line, (long long)") {\n");
    }
    }
    ok = emit(state, line);
    b_len = length_list(bindings);
    vp_codes = 0LL;
    vpk = get_list(state, 16LL);
    vpv = get_list(state, 17LL);
    vpk_i = 0LL;
    vpk_len = length_list(vpk);
    while (vpk_i < vpk_len) {
    if ((strcmp((char*)string_concat(vname, (long long)""), (char*)get_list(vpk, vpk_i)) == 0)) {
    vp_codes = get_list(vpv, vpk_i);
    }
    vpk_i = (vpk_i + 1LL);
    }
    b_idx = 0LL;
    while (b_idx < b_len) {
    bname = get_list(bindings, b_idx);
    line = (long long)"            long long ";
    line = string_concat(line, bname);
    line = string_concat(line, (long long)" = ((long long*)_match_val)[");
    line = string_concat(line, cg_int_to_str((b_idx + 1LL)));
    line = string_concat(line, (long long)"];\n");
    ok = emit(state, line);
    bcode = 1LL;
    if (vp_codes != 0LL) {
    if (b_idx < length_list(vp_codes)) {
    bcode = get_list(vp_codes, b_idx);
    }
    }
    ok = map_put(var_keys, var_values, bname, bcode);
    b_idx = (b_idx + 1LL);
    }
    ab_len = length_list(arm_body);
    ab_idx = 0LL;
    while (ab_idx < ab_len) {
    s = get_list(arm_body, ab_idx);
    ok = gen_statement(state, s, var_keys, var_values);
    ab_idx = (ab_idx + 1LL);
    }
    arm_idx = (arm_idx + 1LL);
    }
    ok = emit(state, (long long)"        }\n");
    ok = emit(state, (long long)"    }\n");
    ret_val = 0LL;
    goto L_cleanup;
    }
    if (type == 28LL) {
    var_name = get_list(stmt, 1LL);
    iter_expr = get_list(stmt, 2LL);
    body = get_list(stmt, 3LL);
    iter_str = gen_expr(state, iter_expr, var_keys, var_values);
    label = get_new_label(state, (long long)"foreach");
    iter_t = infer_type(state, iter_expr, var_keys, var_values);
    if (iter_t == 9LL) {
    ok = emit(state, (long long)"    {\n");
    il = (long long)"        long long _it = ";
    il = string_concat(il, iter_str);
    il = string_concat(il, (long long)";\n");
    ok = emit(state, il);
    ok = emit(state, (long long)"        while (1) {\n");
    ok = emit(state, (long long)"            long long _res = next(_it);\n");
    ok = emit(state, (long long)"            if (_res == 0) break;\n");
    ok = emit(state, (long long)"            if (((long long*)_res)[0] == EP_TAG_Done) break;\n");
    bl = (long long)"            long long ";
    bl = string_concat(bl, var_name);
    bl = string_concat(bl, (long long)" = ((long long*)_res)[1];\n");
    ok = emit(state, bl);
    ib_len = length_list(body);
    ib_i = 0LL;
    while (ib_i < ib_len) {
    ok = gen_statement(state, get_list(body, ib_i), var_keys, var_values);
    ib_i = (ib_i + 1LL);
    }
    ok = emit(state, (long long)"        }\n");
    ok = emit(state, (long long)"    }\n");
    ret_val = 0LL;
    goto L_cleanup;
    }
    ok = emit(state, (long long)"    {\n");
    line = (long long)"        long long _iter = ";
    line = string_concat(line, iter_str);
    line = string_concat(line, (long long)";\n");
    ok = emit(state, line);
    ok = emit(state, (long long)"        long long _iter_len = length_list(_iter);\n");
    ok = emit(state, (long long)"        long long _iter_i = 0;\n");
    ok = emit(state, (long long)"        while (_iter_i < _iter_len) {\n");
    line = (long long)"            long long ";
    line = string_concat(line, var_name);
    line = string_concat(line, (long long)" = get_list(_iter, _iter_i);\n");
    ok = emit(state, line);
    b_len = length_list(body);
    b_idx = 0LL;
    while (b_idx < b_len) {
    s = get_list(body, b_idx);
    ok = gen_statement(state, s, var_keys, var_values);
    b_idx = (b_idx + 1LL);
    }
    ok = emit(state, (long long)"            _iter_i++;\n");
    ok = emit(state, (long long)"        }\n");
    ok = emit(state, (long long)"    }\n");
    ret_val = 0LL;
    goto L_cleanup;
    }
    if (type == 29LL) {
    ok = emit(state, (long long)"    break;\n");
    ret_val = 0LL;
    goto L_cleanup;
    }
    if (type == 30LL) {
    ok = emit(state, (long long)"    continue;\n");
    ret_val = 0LL;
    goto L_cleanup;
    }
    if (type == 36LL) {
    expr = get_list(stmt, 1LL);
    expr_str = gen_expr(state, expr, var_keys, var_values);
    line = (long long)"    ";
    line = string_concat(line, expr_str);
    line = string_concat(line, (long long)";\n");
    ok = emit(state, line);
    ret_val = 0LL;
    goto L_cleanup;
    }
    ret_val = 0LL;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(54);
    return ret_val;
}

long long gen_expr(long long state, long long expr, long long var_keys, long long var_values) {
    long long type = 0;
    long long val = 0;
    long long num_str = 0;
    long long fres = 0;
    long long escaped = 0;
    long long res = 0;
    long long name = 0;
    long long eres = 0;
    long long fk = 0;
    long long left = 0;
    long long op = 0;
    long long right = 0;
    long long left_str = 0;
    long long right_str = 0;
    long long flt = 0;
    long long frt = 0;
    long long fop = 0;
    long long lu = 0;
    long long ru = 0;
    long long fr2 = 0;
    long long op_str = 0;
    long long lt = 0;
    long long is_string = 0;
    long long cmp_op = 0;
    long long args = 0;
    long long fexpr = 0;
    long long args_len = 0;
    long long formatted_args = 0;
    long long idx = 0;
    long long arg = 0;
    long long arg_val = 0;
    long long needs_cast = 0;
    long long casted = 0;
    long long ok = 0;
    long long args_str_list = 0;
    long long arg_item = 0;
    long long args_joined = 0;
    long long func_keys = 0;
    long long cl_sig = 0;
    long long rw_sig = 0;
    long long sg_i = 0;
    long long call_str = 0;
    long long chan = 0;
    long long chan_str = 0;
    long long inner = 0;
    long long awn = 0;
    long long awns = 0;
    long long inner_str = 0;
    long long obj = 0;
    long long field_name = 0;
    long long obj_str = 0;
    long long struct_name = 0;
    long long fields = 0;
    long long field_count = 0;
    long long f_idx = 0;
    long long fpair = 0;
    long long fname = 0;
    long long fval = 0;
    long long fval_str = 0;
    long long method_name = 0;
    long long arg_str = 0;
    long long variant_name = 0;
    long long alloc_size = 0;
    long long a_idx = 0;
    long long ok_variant = 0;
    long long fkeys = 0;
    long long fvals = 0;
    long long callee = 0;
    long long fi = 0;
    long long fn = 0;
    long long tid = 0;
    long long dummy = 0;
    long long tv = 0;
    long long r = 0;
    long long params = 0;
    long long body = 0;
    long long cidx = 0;
    long long cname = 0;
    long long raw_names = 0;
    long long okr = 0;
    long long p_len = 0;
    long long captured = 0;
    long long rn_len = 0;
    long long rn_i = 0;
    long long nm = 0;
    long long skip = 0;
    long long pp_i = 0;
    long long p_node = 0;
    long long okc = 0;
    long long n_caps = 0;
    long long saved_lines = 0;
    long long fresh = 0;
    long long dummy2 = 0;
    long long hdr = 0;
    long long hp_i = 0;
    long long okh = 0;
    long long cp_i = 0;
    long long unp = 0;
    long long oku = 0;
    long long c_keys = 0;
    long long c_values = 0;
    long long ov_len = 0;
    long long ov_i = 0;
    long long okov = 0;
    long long pv_i = 0;
    long long okpv = 0;
    long long b_keys = 0;
    long long b_values = 0;
    long long okbv = 0;
    long long bv_len = 0;
    long long bv_i = 0;
    long long bname = 0;
    long long is_p = 0;
    long long bp_i = 0;
    long long dec = 0;
    long long okd = 0;
    long long okbm = 0;
    long long bs_len = 0;
    long long bs_i = 0;
    long long okst = 0;
    long long okft = 0;
    long long closure_text = 0;
    long long dummy3 = 0;
    long long cbodies = 0;
    long long okcb = 0;
    long long ce_i = 0;
    long long elements = 0;
    long long elem_len = 0;
    long long e_idx = 0;
    long long elem = 0;
    long long elem_str = 0;
    long long ret_val = 0;

    ep_gc_push_root(&val);
    ep_gc_push_root(&num_str);
    ep_gc_push_root(&fres);
    ep_gc_push_root(&escaped);
    ep_gc_push_root(&res);
    ep_gc_push_root(&name);
    ep_gc_push_root(&eres);
    ep_gc_push_root(&fk);
    ep_gc_push_root(&left);
    ep_gc_push_root(&right);
    ep_gc_push_root(&left_str);
    ep_gc_push_root(&right_str);
    ep_gc_push_root(&fop);
    ep_gc_push_root(&lu);
    ep_gc_push_root(&ru);
    ep_gc_push_root(&fr2);
    ep_gc_push_root(&op_str);
    ep_gc_push_root(&cmp_op);
    ep_gc_push_root(&args);
    ep_gc_push_root(&fexpr);
    ep_gc_push_root(&formatted_args);
    ep_gc_push_root(&idx);
    ep_gc_push_root(&arg);
    ep_gc_push_root(&arg_val);
    ep_gc_push_root(&casted);
    ep_gc_push_root(&args_str_list);
    ep_gc_push_root(&arg_item);
    ep_gc_push_root(&args_joined);
    ep_gc_push_root(&func_keys);
    ep_gc_push_root(&cl_sig);
    ep_gc_push_root(&rw_sig);
    ep_gc_push_root(&call_str);
    ep_gc_push_root(&chan);
    ep_gc_push_root(&chan_str);
    ep_gc_push_root(&inner);
    ep_gc_push_root(&awn);
    ep_gc_push_root(&awns);
    ep_gc_push_root(&inner_str);
    ep_gc_push_root(&obj);
    ep_gc_push_root(&field_name);
    ep_gc_push_root(&obj_str);
    ep_gc_push_root(&fields);
    ep_gc_push_root(&f_idx);
    ep_gc_push_root(&fpair);
    ep_gc_push_root(&fname);
    ep_gc_push_root(&fval);
    ep_gc_push_root(&fval_str);
    ep_gc_push_root(&arg_str);
    ep_gc_push_root(&variant_name);
    ep_gc_push_root(&alloc_size);
    ep_gc_push_root(&a_idx);
    ep_gc_push_root(&ok_variant);
    ep_gc_push_root(&fkeys);
    ep_gc_push_root(&fvals);
    ep_gc_push_root(&callee);
    ep_gc_push_root(&fi);
    ep_gc_push_root(&tid);
    ep_gc_push_root(&tv);
    ep_gc_push_root(&r);
    ep_gc_push_root(&params);
    ep_gc_push_root(&body);
    ep_gc_push_root(&cidx);
    ep_gc_push_root(&cname);
    ep_gc_push_root(&raw_names);
    ep_gc_push_root(&captured);
    ep_gc_push_root(&rn_i);
    ep_gc_push_root(&nm);
    ep_gc_push_root(&pp_i);
    ep_gc_push_root(&p_node);
    ep_gc_push_root(&n_caps);
    ep_gc_push_root(&saved_lines);
    ep_gc_push_root(&fresh);
    ep_gc_push_root(&hdr);
    ep_gc_push_root(&hp_i);
    ep_gc_push_root(&cp_i);
    ep_gc_push_root(&unp);
    ep_gc_push_root(&c_keys);
    ep_gc_push_root(&c_values);
    ep_gc_push_root(&ov_i);
    ep_gc_push_root(&pv_i);
    ep_gc_push_root(&b_keys);
    ep_gc_push_root(&b_values);
    ep_gc_push_root(&bv_i);
    ep_gc_push_root(&bname);
    ep_gc_push_root(&bp_i);
    ep_gc_push_root(&dec);
    ep_gc_push_root(&bs_i);
    ep_gc_push_root(&closure_text);
    ep_gc_push_root(&cbodies);
    ep_gc_push_root(&ce_i);
    ep_gc_push_root(&elements);
    ep_gc_push_root(&e_idx);
    ep_gc_push_root(&elem);
    ep_gc_push_root(&elem_str);
    ep_gc_push_root(&state);
    ep_gc_push_root(&expr);
    ep_gc_push_root(&var_keys);
    ep_gc_push_root(&var_values);
    ep_gc_maybe_collect();

    type = get_list(expr, 0LL);
    if (type == 1LL) {
    val = get_list(expr, 1LL);
    num_str = cg_int_to_str(val);
    ret_val = string_concat(num_str, (long long)"LL");
    goto L_cleanup;
    }
    if (type == 42LL) {
    fres = (long long)"({ double _fl = ";
    fres = string_concat(fres, get_list(expr, 1LL));
    fres = string_concat(fres, (long long)"; long long _fv; memcpy(&_fv, &_fl, sizeof(double)); _fv; })");
    ret_val = fres;
    goto L_cleanup;
    }
    if (type == 2LL) {
    val = get_list(expr, 1LL);
    escaped = escape_string(val);
    res = (long long)"(long long)\"";
    res = string_concat(res, escaped);
    res = string_concat(res, (long long)"\"");
    ret_val = res;
    goto L_cleanup;
    }
    if (type == 3LL) {
    name = get_list(expr, 1LL);
    if (get_list(state, 19LL) == 1LL) {
    if (contains_string_val(get_list(state, 21LL), name) == 1LL) {
    ret_val = string_concat((long long)"args->", name);
    goto L_cleanup;
    }
    }
    if (map_contains_key(var_keys, name) == 0LL) {
    if (map_contains_key(get_list(state, 12LL), name) == 1LL) {
    eres = (long long)"({ long long* _v = (long long*)malloc(sizeof(long long) * 1); _v[0] = EP_TAG_";
    eres = string_concat(eres, name);
    eres = string_concat(eres, (long long)"; ep_gc_register(_v, EP_OBJ_STRUCT); (long long)_v; })");
    ret_val = eres;
    goto L_cleanup;
    }
    }
    if (map_contains_key(var_keys, name) == 0LL) {
    fk = get_list(state, 3LL);
    if (map_contains_key(fk, name) == 1LL) {
    fres = (long long)"(long long)";
    fres = string_concat(fres, cg_sanitize_name(name));
    ret_val = fres;
    goto L_cleanup;
    }
    }
    ret_val = name;
    goto L_cleanup;
    }
    if (type == 4LL) {
    left = get_list(expr, 1LL);
    op = get_list(expr, 2LL);
    right = get_list(expr, 3LL);
    left_str = gen_expr(state, left, var_keys, var_values);
    right_str = gen_expr(state, right, var_keys, var_values);
    flt = infer_type(state, left, var_keys, var_values);
    frt = infer_type(state, right, var_keys, var_values);
    if ((flt == 8LL || frt == 8LL)) {
    fop = (long long)"+";
    if (op == 2LL) {
    fop = (long long)"-";
    }
    if (op == 3LL) {
    fop = (long long)"*";
    }
    if (op == 4LL) {
    fop = (long long)"/";
    }
    lu = (long long)"";
    if (flt == 8LL) {
    lu = string_concat((long long)"({ long long _lt = ", left_str);
    lu = string_concat(lu, (long long)"; double _d; memcpy(&_d, &_lt, sizeof(double)); _d; })");
    } else {
    lu = string_concat((long long)"(double)(", left_str);
    lu = string_concat(lu, (long long)")");
    }
    ru = (long long)"";
    if (frt == 8LL) {
    ru = string_concat((long long)"({ long long _rt = ", right_str);
    ru = string_concat(ru, (long long)"; double _d; memcpy(&_d, &_rt, sizeof(double)); _d; })");
    } else {
    ru = string_concat((long long)"(double)(", right_str);
    ru = string_concat(ru, (long long)")");
    }
    fr2 = (long long)"({ double _r = ";
    fr2 = string_concat(fr2, lu);
    fr2 = string_concat(fr2, (long long)" ");
    fr2 = string_concat(fr2, fop);
    fr2 = string_concat(fr2, (long long)" ");
    fr2 = string_concat(fr2, ru);
    fr2 = string_concat(fr2, (long long)"; long long _v; memcpy(&_v, &_r, sizeof(double)); _v; })");
    ret_val = fr2;
    goto L_cleanup;
    }
    op_str = (long long)"";
    if (op == 1LL) {
    op_str = (long long)"+";
    }
    if (op == 2LL) {
    op_str = (long long)"-";
    }
    if (op == 3LL) {
    op_str = (long long)"*";
    }
    if (op == 4LL) {
    op_str = (long long)"/";
    }
    if (op == 5LL) {
    op_str = (long long)"%";
    }
    res = (long long)"(";
    res = string_concat(res, left_str);
    res = string_concat(res, (long long)" ");
    res = string_concat(res, op_str);
    res = string_concat(res, (long long)" ");
    res = string_concat(res, right_str);
    res = string_concat(res, (long long)")");
    ret_val = res;
    goto L_cleanup;
    }
    if (type == 5LL) {
    left = get_list(expr, 1LL);
    op = get_list(expr, 2LL);
    right = get_list(expr, 3LL);
    left_str = gen_expr(state, left, var_keys, var_values);
    right_str = gen_expr(state, right, var_keys, var_values);
    lt = infer_type(state, left, var_keys, var_values);
    is_string = 0LL;
    if ((lt == 2LL || lt == 3LL)) {
    is_string = 1LL;
    }
    if (is_string == 1LL) {
    cmp_op = (long long)"";
    if (op == 1LL) {
    cmp_op = (long long)"< 0";
    }
    if (op == 2LL) {
    cmp_op = (long long)"> 0";
    }
    if (op == 3LL) {
    cmp_op = (long long)"== 0";
    }
    if (op == 4LL) {
    cmp_op = (long long)"!= 0";
    }
    if (op == 5LL) {
    cmp_op = (long long)"<= 0";
    }
    if (op == 6LL) {
    cmp_op = (long long)">= 0";
    }
    res = (long long)"(strcmp((char*)";
    res = string_concat(res, left_str);
    res = string_concat(res, (long long)", (char*)");
    res = string_concat(res, right_str);
    res = string_concat(res, (long long)") ");
    res = string_concat(res, cmp_op);
    res = string_concat(res, (long long)")");
    ret_val = res;
    goto L_cleanup;
    } else {
    op_str = (long long)"";
    if (op == 1LL) {
    op_str = (long long)"<";
    }
    if (op == 2LL) {
    op_str = (long long)">";
    }
    if (op == 3LL) {
    op_str = (long long)"==";
    }
    if (op == 4LL) {
    op_str = (long long)"!=";
    }
    if (op == 5LL) {
    op_str = (long long)"<=";
    }
    if (op == 6LL) {
    op_str = (long long)">=";
    }
    res = (long long)"";
    res = string_concat(res, left_str);
    res = string_concat(res, (long long)" ");
    res = string_concat(res, op_str);
    res = string_concat(res, (long long)" ");
    res = string_concat(res, right_str);
    ret_val = res;
    goto L_cleanup;
    }
    }
    if (type == 14LL) {
    left = get_list(expr, 1LL);
    op = get_list(expr, 2LL);
    right = get_list(expr, 3LL);
    left_str = gen_expr(state, left, var_keys, var_values);
    right_str = gen_expr(state, right, var_keys, var_values);
    op_str = (long long)"";
    if (op == 1LL) {
    op_str = (long long)"&&";
    }
    if (op == 2LL) {
    op_str = (long long)"||";
    }
    res = (long long)"(";
    res = string_concat(res, left_str);
    res = string_concat(res, (long long)" ");
    res = string_concat(res, op_str);
    res = string_concat(res, (long long)" ");
    res = string_concat(res, right_str);
    res = string_concat(res, (long long)")");
    ret_val = res;
    goto L_cleanup;
    }
    if (type == 6LL) {
    name = get_list(expr, 1LL);
    args = get_list(expr, 2LL);
    if ((strcmp((char*)(long long)"ep_auto_to_string", (char*)name) == 0)) {
    if (length_list(args) == 1LL) {
    if (infer_type(state, get_list(args, 0LL), var_keys, var_values) == 8LL) {
    fexpr = gen_expr(state, get_list(args, 0LL), var_keys, var_values);
    fres = string_concat((long long)"ep_float_to_string(", fexpr);
    fres = string_concat(fres, (long long)")");
    ret_val = fres;
    goto L_cleanup;
    }
    }
    }
    args_len = length_list(args);
    formatted_args = create_list();
    idx = 0LL;
    while (idx < args_len) {
    arg = get_list(args, idx);
    arg_val = gen_expr(state, arg, var_keys, var_values);
    needs_cast = 0LL;
    if (((((((((strcmp((char*)(long long)"read_file_content", (char*)name) == 0) || (strcmp((char*)(long long)"string_length", (char*)name) == 0)) || (strcmp((char*)(long long)"display_string", (char*)name) == 0)) || (strcmp((char*)(long long)"screen_write", (char*)name) == 0)) || (strcmp((char*)(long long)"run_command", (char*)name) == 0)) || (strcmp((char*)(long long)"ep_md5", (char*)name) == 0)) || (strcmp((char*)(long long)"ep_sha256", (char*)name) == 0)) || (strcmp((char*)(long long)"ep_net_connect", (char*)name) == 0))) {
    if (idx == 0LL) {
    needs_cast = 1LL;
    }
    } else {
    if (((strcmp((char*)(long long)"get_character", (char*)name) == 0) || (strcmp((char*)(long long)"substring", (char*)name) == 0))) {
    if (idx == 0LL) {
    needs_cast = 1LL;
    }
    } else {
    if ((strcmp((char*)(long long)"write_file_content", (char*)name) == 0)) {
    if ((idx == 0LL || idx == 1LL)) {
    needs_cast = 1LL;
    }
    } else {
    if ((strcmp((char*)(long long)"ep_net_send", (char*)name) == 0)) {
    if (idx == 1LL) {
    needs_cast = 1LL;
    }
    }
    }
    }
    }
    casted = (long long)"";
    if (needs_cast == 1LL) {
    casted = string_concat((long long)"(char*)", arg_val);
    } else {
    casted = arg_val;
    }
    ok = append_list(formatted_args, casted);
    idx = (idx + 1LL);
    }
    args_str_list = create_list();
    idx = 0LL;
    while (idx < args_len) {
    arg_item = get_list(formatted_args, idx);
    ok = append_list(args_str_list, arg_item);
    if (idx < (args_len - 1LL)) {
    ok = append_list(args_str_list, (long long)", ");
    }
    idx = (idx + 1LL);
    }
    args_joined = join_strings(args_str_list);
    func_keys = get_list(state, 3LL);
    if (map_contains_key(func_keys, name) == 0LL) {
    if (map_contains_key(var_keys, name) == 1LL) {
    cl_sig = (long long)"long long(*)(long long";
    rw_sig = (long long)"long long(*)(";
    sg_i = 0LL;
    while (sg_i < args_len) {
    cl_sig = string_concat(cl_sig, (long long)", long long");
    if (sg_i > 0LL) {
    rw_sig = string_concat(rw_sig, (long long)", long long");
    } else {
    rw_sig = string_concat(rw_sig, (long long)"long long");
    }
    sg_i = (sg_i + 1LL);
    }
    if (args_len == 0LL) {
    rw_sig = string_concat(rw_sig, (long long)"void");
    }
    cl_sig = string_concat(cl_sig, (long long)")");
    rw_sig = string_concat(rw_sig, (long long)")");
    res = (long long)"({ long long _fv = ";
    res = string_concat(res, name);
    res = string_concat(res, (long long)"; EpClosure* _cl = (EpClosure*)_fv; (_fv != 0 && _cl->magic == EP_CLOSURE_MAGIC) ? ((");
    res = string_concat(res, cl_sig);
    res = string_concat(res, (long long)")_cl->fn_ptr)((long long)_cl->env");
    if (args_len > 0LL) {
    res = string_concat(res, (long long)", ");
    res = string_concat(res, args_joined);
    }
    res = string_concat(res, (long long)") : ((");
    res = string_concat(res, rw_sig);
    res = string_concat(res, (long long)")_fv)(");
    res = string_concat(res, args_joined);
    res = string_concat(res, (long long)"); })");
    ret_val = res;
    goto L_cleanup;
    }
    }
    call_str = cg_sanitize_name(name);
    call_str = string_concat(call_str, (long long)"(");
    call_str = string_concat(call_str, args_joined);
    call_str = string_concat(call_str, (long long)")");
    if ((((((((strcmp((char*)(long long)"read_file_content", (char*)name) == 0) || (strcmp((char*)(long long)"get_argument", (char*)name) == 0)) || (strcmp((char*)(long long)"substring", (char*)name) == 0)) || (strcmp((char*)(long long)"string_from_list", (char*)name) == 0)) || (strcmp((char*)(long long)"ep_net_recv", (char*)name) == 0)) || (strcmp((char*)(long long)"ep_md5", (char*)name) == 0)) || (strcmp((char*)(long long)"ep_sha256", (char*)name) == 0))) {
    res = (long long)"(long long)";
    res = string_concat(res, call_str);
    ret_val = res;
    goto L_cleanup;
    } else {
    ret_val = call_str;
    goto L_cleanup;
    }
    }
    if (type == 17LL) {
    ret_val = (long long)"create_channel()";
    goto L_cleanup;
    }
    if (type == 18LL) {
    chan = get_list(expr, 1LL);
    chan_str = gen_expr(state, chan, var_keys, var_values);
    res = (long long)"receive_channel(";
    res = string_concat(res, chan_str);
    res = string_concat(res, (long long)")");
    ret_val = res;
    goto L_cleanup;
    }
    if (type == 20LL) {
    inner = get_list(expr, 1LL);
    ret_val = gen_expr(state, inner, var_keys, var_values);
    goto L_cleanup;
    }
    if (type == 21LL) {
    inner = get_list(expr, 1LL);
    if (get_list(state, 19LL) == 1LL) {
    awn = (get_list(state, 20LL) + 1LL);
    ok = set_list(state, 20LL, awn);
    awns = cg_int_to_str(awn);
    res = (long long)"(args->awaited_fut_";
    res = string_concat(res, awns);
    res = string_concat(res, (long long)" ? args->awaited_fut_");
    res = string_concat(res, awns);
    res = string_concat(res, (long long)"->value : 0)");
    ret_val = res;
    goto L_cleanup;
    }
    inner_str = gen_expr(state, inner, var_keys, var_values);
    res = (long long)"ep_await_future((EpFuture*)";
    res = string_concat(res, inner_str);
    res = string_concat(res, (long long)")");
    ret_val = res;
    goto L_cleanup;
    }
    if (type == 22LL) {
    obj = get_list(expr, 1LL);
    field_name = get_list(expr, 2LL);
    obj_str = gen_expr(state, obj, var_keys, var_values);
    res = (long long)"((long long*)";
    res = string_concat(res, obj_str);
    res = string_concat(res, (long long)")[EP_FIELD_");
    res = string_concat(res, field_name);
    res = string_concat(res, (long long)"]");
    ret_val = res;
    goto L_cleanup;
    }
    if (type == 24LL) {
    struct_name = get_list(expr, 1LL);
    fields = get_list(expr, 2LL);
    field_count = length_list(fields);
    res = (long long)"({ long long* _s = (long long*)calloc(EP_STRUCT_MAX_SLOTS, sizeof(long long)); ";
    f_idx = 0LL;
    while (f_idx < field_count) {
    fpair = get_list(fields, f_idx);
    fname = get_list(fpair, 0LL);
    fval = get_list(fpair, 1LL);
    fval_str = gen_expr(state, fval, var_keys, var_values);
    res = string_concat(res, (long long)"_s[EP_FIELD_");
    res = string_concat(res, fname);
    res = string_concat(res, (long long)"] = ");
    res = string_concat(res, fval_str);
    res = string_concat(res, (long long)"; ");
    f_idx = (f_idx + 1LL);
    }
    res = string_concat(res, (long long)"{ EpGCObject* _go = ep_gc_register(_s, EP_OBJ_STRUCT); if(_go) _go->num_fields = EP_STRUCT_MAX_SLOTS; } (long long)_s; })");
    ret_val = res;
    goto L_cleanup;
    }
    if (type == 25LL) {
    obj = get_list(expr, 1LL);
    method_name = get_list(expr, 2LL);
    args = get_list(expr, 3LL);
    obj_str = gen_expr(state, obj, var_keys, var_values);
    args_len = length_list(args);
    res = method_name;
    res = string_concat(res, (long long)"(");
    res = string_concat(res, obj_str);
    idx = 0LL;
    while (idx < args_len) {
    arg = get_list(args, idx);
    arg_str = gen_expr(state, arg, var_keys, var_values);
    res = string_concat(res, (long long)", ");
    res = string_concat(res, arg_str);
    idx = (idx + 1LL);
    }
    res = string_concat(res, (long long)")");
    ret_val = res;
    goto L_cleanup;
    }
    if (type == 26LL) {
    variant_name = get_list(expr, 1LL);
    args = get_list(expr, 2LL);
    args_len = length_list(args);
    alloc_size = (args_len + 1LL);
    res = (long long)"({ long long* _v = (long long*)malloc(sizeof(long long) * ";
    res = string_concat(res, cg_int_to_str(alloc_size));
    res = string_concat(res, (long long)"); _v[0] = EP_TAG_");
    res = string_concat(res, variant_name);
    res = string_concat(res, (long long)"; ");
    a_idx = 0LL;
    while (a_idx < args_len) {
    arg = get_list(args, a_idx);
    arg_str = gen_expr(state, arg, var_keys, var_values);
    res = string_concat(res, (long long)"_v[");
    res = string_concat(res, cg_int_to_str((a_idx + 1LL)));
    res = string_concat(res, (long long)"] = ");
    res = string_concat(res, arg_str);
    res = string_concat(res, (long long)"; ");
    a_idx = (a_idx + 1LL);
    }
    res = string_concat(res, (long long)"ep_gc_register(_v, EP_OBJ_STRUCT); (long long)_v; })");
    ret_val = res;
    goto L_cleanup;
    }
    if (type == 31LL) {
    val = get_list(expr, 1LL);
    if (val == 1LL) {
    ret_val = (long long)"1LL";
    goto L_cleanup;
    }
    ret_val = (long long)"0LL";
    goto L_cleanup;
    }
    if (type == 32LL) {
    inner = get_list(expr, 1LL);
    inner_str = gen_expr(state, inner, var_keys, var_values);
    res = (long long)"(!(";
    res = string_concat(res, inner_str);
    res = string_concat(res, (long long)"))");
    ret_val = res;
    goto L_cleanup;
    }
    if (type == 33LL) {
    inner = get_list(expr, 1LL);
    inner_str = gen_expr(state, inner, var_keys, var_values);
    ok_variant = (long long)"";
    if (get_list(inner, 0LL) == 6LL) {
    fkeys = get_list(state, 14LL);
    fvals = get_list(state, 15LL);
    callee = get_list(inner, 1LL);
    fi = 0LL;
    fn = length_list(fkeys);
    while (fi < fn) {
    if ((strcmp((char*)string_concat(callee, (long long)""), (char*)get_list(fkeys, fi)) == 0)) {
    ok_variant = get_list(fvals, fi);
    }
    fi = (fi + 1LL);
    }
    }
    if (string_length((char*)ok_variant) == 0LL) {
    ret_val = inner_str;
    goto L_cleanup;
    }
    tid = get_list(state, 13LL);
    dummy = set_list(state, 13LL, (tid + 1LL));
    tv = string_concat((long long)"_try", cg_int_to_str(tid));
    r = (long long)"({ long long ";
    r = string_concat(r, tv);
    r = string_concat(r, (long long)" = ");
    r = string_concat(r, inner_str);
    r = string_concat(r, (long long)"; if (((long long*)");
    r = string_concat(r, tv);
    r = string_concat(r, (long long)")[0] != EP_TAG_");
    r = string_concat(r, ok_variant);
    r = string_concat(r, (long long)") { ret_val = ");
    r = string_concat(r, tv);
    r = string_concat(r, (long long)"; goto L_cleanup; } ((long long*)");
    r = string_concat(r, tv);
    r = string_concat(r, (long long)")[1]; })");
    ret_val = r;
    goto L_cleanup;
    }
    if (type == 34LL) {
    params = get_list(expr, 1LL);
    body = get_list(expr, 2LL);
    cidx = get_list(state, 13LL);
    dummy = set_list(state, 13LL, (cidx + 1LL));
    cname = string_concat((long long)"_ep_closure_", cg_int_to_str(cidx));
    raw_names = create_list();
    okr = collect_idents_stmts(body, raw_names);
    func_keys = get_list(state, 3LL);
    p_len = length_list(params);
    captured = create_list();
    rn_len = length_list(raw_names);
    rn_i = 0LL;
    while (rn_i < rn_len) {
    nm = string_concat(get_list(raw_names, rn_i), (long long)"");
    skip = 0LL;
    if ((strcmp((char*)nm, (char*)(long long)"ret_val") == 0)) {
    skip = 1LL;
    }
    if (skip == 0LL) {
    if (map_contains_key(captured, nm) == 1LL) {
    skip = 1LL;
    }
    }
    if (skip == 0LL) {
    pp_i = 0LL;
    while (pp_i < p_len) {
    p_node = get_list(params, pp_i);
    if ((strcmp((char*)nm, (char*)get_list(p_node, 0LL)) == 0)) {
    skip = 1LL;
    }
    pp_i = (pp_i + 1LL);
    }
    }
    if (skip == 0LL) {
    if (map_contains_key(var_keys, nm) == 0LL) {
    skip = 1LL;
    }
    }
    if (skip == 0LL) {
    if (map_contains_key(func_keys, nm) == 1LL) {
    skip = 1LL;
    }
    }
    if (skip == 0LL) {
    okc = append_list(captured, nm);
    }
    rn_i = (rn_i + 1LL);
    }
    n_caps = length_list(captured);
    saved_lines = get_list(state, 0LL);
    fresh = (create_list() + 0LL);
    dummy2 = set_list(state, 0LL, fresh);
    hdr = (long long)"long long ";
    hdr = string_concat(hdr, cname);
    hdr = string_concat(hdr, (long long)"(long long _ep_env");
    hp_i = 0LL;
    while (hp_i < p_len) {
    p_node = get_list(params, hp_i);
    hdr = string_concat(hdr, (long long)", long long ");
    hdr = string_concat(hdr, get_list(p_node, 0LL));
    hp_i = (hp_i + 1LL);
    }
    hdr = string_concat(hdr, (long long)") {\n    long long ret_val = 0;\n");
    okh = emit(state, hdr);
    cp_i = 0LL;
    while (cp_i < n_caps) {
    unp = (long long)"    long long ";
    unp = string_concat(unp, get_list(captured, cp_i));
    unp = string_concat(unp, (long long)" = ((long long*)_ep_env)[");
    unp = string_concat(unp, cg_int_to_str(cp_i));
    unp = string_concat(unp, (long long)"];\n");
    oku = emit(state, unp);
    cp_i = (cp_i + 1LL);
    }
    c_keys = create_list();
    c_values = create_list();
    ov_len = length_list(var_keys);
    ov_i = 0LL;
    while (ov_i < ov_len) {
    okov = map_put(c_keys, c_values, get_list(var_keys, ov_i), get_list(var_values, ov_i));
    ov_i = (ov_i + 1LL);
    }
    pv_i = 0LL;
    while (pv_i < p_len) {
    p_node = get_list(params, pv_i);
    okpv = map_put(c_keys, c_values, get_list(p_node, 0LL), 1LL);
    pv_i = (pv_i + 1LL);
    }
    b_keys = create_list();
    b_values = create_list();
    okbv = collect_var_types(state, body, b_keys, b_values);
    bv_len = length_list(b_keys);
    bv_i = 0LL;
    while (bv_i < bv_len) {
    bname = string_concat(get_list(b_keys, bv_i), (long long)"");
    is_p = 0LL;
    bp_i = 0LL;
    while (bp_i < p_len) {
    p_node = get_list(params, bp_i);
    if ((strcmp((char*)bname, (char*)get_list(p_node, 0LL)) == 0)) {
    is_p = 1LL;
    }
    bp_i = (bp_i + 1LL);
    }
    if (is_p == 0LL) {
    if (map_contains_key(captured, bname) == 0LL) {
    if ((strcmp((char*)bname, (char*)(long long)"ret_val") == 0)) {
    is_p = 1LL;
    } else {
    dec = (long long)"    long long ";
    dec = string_concat(dec, bname);
    dec = string_concat(dec, (long long)" = 0;\n");
    okd = emit(state, dec);
    }
    }
    }
    okbm = map_put(c_keys, c_values, bname, get_list(b_values, bv_i));
    bv_i = (bv_i + 1LL);
    }
    bs_len = length_list(body);
    bs_i = 0LL;
    while (bs_i < bs_len) {
    okst = gen_statement(state, get_list(body, bs_i), c_keys, c_values);
    bs_i = (bs_i + 1LL);
    }
    okft = emit(state, (long long)"L_cleanup:\n    return ret_val;\n}\n\n");
    closure_text = join_strings(fresh);
    dummy3 = set_list(state, 0LL, saved_lines);
    cbodies = get_list(state, 11LL);
    okcb = append_list(cbodies, closure_text);
    res = (long long)"({ EpClosure* _cl_";
    res = string_concat(res, cg_int_to_str(cidx));
    res = string_concat(res, (long long)" = (EpClosure*)malloc(sizeof(EpClosure) + ");
    res = string_concat(res, cg_int_to_str(n_caps));
    res = string_concat(res, (long long)" * sizeof(long long)); _cl_");
    res = string_concat(res, cg_int_to_str(cidx));
    res = string_concat(res, (long long)"->magic = EP_CLOSURE_MAGIC; _cl_");
    res = string_concat(res, cg_int_to_str(cidx));
    res = string_concat(res, (long long)"->fn_ptr = (long long)");
    res = string_concat(res, cname);
    res = string_concat(res, (long long)";");
    ce_i = 0LL;
    while (ce_i < n_caps) {
    res = string_concat(res, (long long)" _cl_");
    res = string_concat(res, cg_int_to_str(cidx));
    res = string_concat(res, (long long)"->env[");
    res = string_concat(res, cg_int_to_str(ce_i));
    res = string_concat(res, (long long)"] = ");
    res = string_concat(res, get_list(captured, ce_i));
    res = string_concat(res, (long long)";");
    ce_i = (ce_i + 1LL);
    }
    res = string_concat(res, (long long)" (long long)_cl_");
    res = string_concat(res, cg_int_to_str(cidx));
    res = string_concat(res, (long long)"; })");
    ret_val = res;
    goto L_cleanup;
    }
    if (type == 35LL) {
    elements = get_list(expr, 1LL);
    elem_len = length_list(elements);
    res = (long long)"({ long long _lst = create_list(); ";
    e_idx = 0LL;
    while (e_idx < elem_len) {
    elem = get_list(elements, e_idx);
    elem_str = gen_expr(state, elem, var_keys, var_values);
    res = string_concat(res, (long long)"append_list(_lst, ");
    res = string_concat(res, elem_str);
    res = string_concat(res, (long long)"); ");
    e_idx = (e_idx + 1LL);
    }
    res = string_concat(res, (long long)"_lst; })");
    ret_val = res;
    goto L_cleanup;
    }
    ret_val = (long long)"";
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(98);
    return ret_val;
}

long long get_c_runtime_source() {
    long long ret_val = 0;

    ep_gc_maybe_collect();

    ret_val = get_shared_runtime_source();
    goto L_cleanup;
L_cleanup:
    return ret_val;
}

long long get_c_main_source() {
    long long lines = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&lines);
    ep_gc_maybe_collect();

    lines = create_list();
    ok = append_list(lines, (long long)"\n/* Bootstrapper C main */\n");
    ok = append_list(lines, (long long)"void __ep_init_constants(void);\n");
    ok = append_list(lines, (long long)"int main(int argc, char** argv) {\n");
    ok = append_list(lines, (long long)"    init_ep_args(argc, argv);\n");
    ok = append_list(lines, (long long)"    __ep_init_constants();\n");
    ok = append_list(lines, (long long)"    int result = (int)_main();\n");
    ok = append_list(lines, (long long)"    ep_gc_shutdown();\n");
    ok = append_list(lines, (long long)"    return result;\n");
    ok = append_list(lines, (long long)"}\n");
    ret_val = join_strings(lines);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(1);
    return ret_val;
}

long long get_c_test_main_source(long long program) {
    long long funcs = 0;
    long long funcs_len = 0;
    long long test_cases = 0;
    long long idx = 0;
    long long func = 0;
    long long name = 0;
    long long name_len = 0;
    long long prefix = 0;
    long long ok = 0;
    long long test_count = 0;
    long long lines = 0;
    long long idx2 = 0;
    long long tc_name = 0;
    long long ret_val = 0;

    ep_gc_push_root(&funcs);
    ep_gc_push_root(&test_cases);
    ep_gc_push_root(&idx);
    ep_gc_push_root(&func);
    ep_gc_push_root(&name);
    ep_gc_push_root(&prefix);
    ep_gc_push_root(&test_count);
    ep_gc_push_root(&lines);
    ep_gc_push_root(&idx2);
    ep_gc_push_root(&tc_name);
    ep_gc_push_root(&program);
    ep_gc_maybe_collect();

    funcs = get_list(program, 3LL);
    funcs_len = length_list(funcs);
    test_cases = create_list();
    idx = 0LL;
    while (idx < funcs_len) {
    func = get_list(funcs, idx);
    name = get_list(func, 1LL);
    name_len = string_length((char*)name);
    if (name_len > 4LL) {
    prefix = (long long)substring((char*)name, 0LL, 5LL);
    if ((strcmp((char*)(long long)"test_", (char*)prefix) == 0)) {
    ok = append_list(test_cases, name);
    }
    }
    idx = (idx + 1LL);
    }
    test_count = length_list(test_cases);
    lines = create_list();
    ok = append_list(lines, (long long)"\n/* Test runner C main */\n");
    ok = append_list(lines, (long long)"#include <sys/types.h>\n");
    ok = append_list(lines, (long long)"#include <sys/wait.h>\n");
    ok = append_list(lines, (long long)"#include <unistd.h>\n");
    ok = append_list(lines, (long long)"#include <stdio.h>\n");
    ok = append_list(lines, (long long)"#include <stdlib.h>\n\n");
    ok = append_list(lines, (long long)"int run_test(long long (*test_func)(void), const char* name) {\n");
    ok = append_list(lines, (long long)"    printf(\"test_%s ... \", name);\n");
    ok = append_list(lines, (long long)"    fflush(stdout);\n");
    ok = append_list(lines, (long long)"    pid_t pid = fork();\n");
    ok = append_list(lines, (long long)"    if (pid < 0) {\n");
    ok = append_list(lines, (long long)"        printf(\"FAILED (fork failed)\\n\");\n");
    ok = append_list(lines, (long long)"        return 0;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    if (pid == 0) {\n");
    ok = append_list(lines, (long long)"        exit((int)test_func());\n");
    ok = append_list(lines, (long long)"    } else {\n");
    ok = append_list(lines, (long long)"        int status;\n");
    ok = append_list(lines, (long long)"        waitpid(pid, &status, 0);\n");
    ok = append_list(lines, (long long)"        if (WIFEXITED(status)) {\n");
    ok = append_list(lines, (long long)"            int exit_code = WEXITSTATUS(status);\n");
    ok = append_list(lines, (long long)"            if (exit_code == 0) {\n");
    ok = append_list(lines, (long long)"                printf(\"OK\\n\");\n");
    ok = append_list(lines, (long long)"                return 1;\n");
    ok = append_list(lines, (long long)"            } else {\n");
    ok = append_list(lines, (long long)"                printf(\"FAILED (exit code %d)\\n\", exit_code);\n");
    ok = append_list(lines, (long long)"                return 0;\n");
    ok = append_list(lines, (long long)"            }\n");
    ok = append_list(lines, (long long)"        } else if (WIFSIGNALED(status)) {\n");
    ok = append_list(lines, (long long)"            int sig = WTERMSIG(status);\n");
    ok = append_list(lines, (long long)"            printf(\"FAILED (crashed/signal %d)\\n\", sig);\n");
    ok = append_list(lines, (long long)"            return 0;\n");
    ok = append_list(lines, (long long)"        } else {\n");
    ok = append_list(lines, (long long)"            printf(\"FAILED\\n\");\n");
    ok = append_list(lines, (long long)"            return 0;\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"}\n\n");
    ok = append_list(lines, (long long)"int main(int argc, char** argv) {\n");
    ok = append_list(lines, (long long)"    init_ep_args(argc, argv);\n");
    ok = append_list(lines, (long long)"    printf(\"Running ");
    ok = append_list(lines, cg_int_to_str(test_count));
    ok = append_list(lines, (long long)" tests...\\n\");\n");
    ok = append_list(lines, (long long)"    int passed = 0;\n");
    ok = append_list(lines, (long long)"    int failed = 0;\n");
    ok = append_list(lines, (long long)"    int total = 0;\n\n");
    idx2 = 0LL;
    while (idx2 < test_count) {
    tc_name = get_list(test_cases, idx2);
    ok = append_list(lines, (long long)"    total++;\n");
    ok = append_list(lines, (long long)"    if (run_test(");
    ok = append_list(lines, tc_name);
    ok = append_list(lines, (long long)", \"");
    ok = append_list(lines, tc_name);
    ok = append_list(lines, (long long)"\")) passed++; else failed++;\n");
    idx2 = (idx2 + 1LL);
    }
    ok = append_list(lines, (long long)"\n    printf(\"\\nResult: %d passed; %d failed\\n\", passed, failed);\n");
    ok = append_list(lines, (long long)"    if (failed > 0) return 1;\n");
    ok = append_list(lines, (long long)"    return 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ret_val = join_strings(lines);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(11);
    return ret_val;
}

long long collect_spawns_in_stmts(long long stmts, long long spawn_list) {
    long long len = 0;
    long long idx = 0;
    long long stmt = 0;
    long long type = 0;
    long long ok = 0;
    long long then_b = 0;
    long long else_b = 0;
    long long body = 0;
    long long ret_val = 0;

    ep_gc_push_root(&idx);
    ep_gc_push_root(&stmt);
    ep_gc_push_root(&then_b);
    ep_gc_push_root(&else_b);
    ep_gc_push_root(&body);
    ep_gc_push_root(&stmts);
    ep_gc_push_root(&spawn_list);
    ep_gc_maybe_collect();

    len = length_list(stmts);
    idx = 0LL;
    while (idx < len) {
    stmt = get_list(stmts, idx);
    type = get_list(stmt, 0LL);
    if (type == 15LL) {
    ok = append_list(spawn_list, stmt);
    } else {
    if (type == 10LL) {
    then_b = get_list(stmt, 2LL);
    else_b = get_list(stmt, 3LL);
    ok = collect_spawns_in_stmts(then_b, spawn_list);
    if (else_b != 0LL) {
    ok = collect_spawns_in_stmts(else_b, spawn_list);
    }
    } else {
    if (type == 11LL) {
    body = get_list(stmt, 2LL);
    ok = collect_spawns_in_stmts(body, spawn_list);
    }
    }
    }
    idx = (idx + 1LL);
    }
    ret_val = 0LL;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(7);
    return ret_val;
}

long long collect_all_spawns(long long program) {
    long long spawn_list = 0;
    long long funcs = 0;
    long long len = 0;
    long long idx = 0;
    long long func = 0;
    long long body = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&spawn_list);
    ep_gc_push_root(&funcs);
    ep_gc_push_root(&idx);
    ep_gc_push_root(&func);
    ep_gc_push_root(&body);
    ep_gc_push_root(&program);
    ep_gc_maybe_collect();

    spawn_list = create_list();
    funcs = get_list(program, 3LL);
    len = length_list(funcs);
    idx = 0LL;
    while (idx < len) {
    func = get_list(funcs, idx);
    body = get_list(func, 3LL);
    ok = collect_spawns_in_stmts(body, spawn_list);
    idx = (idx + 1LL);
    }
    ret_val = spawn_list;
    spawn_list = 0;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(6);
    return ret_val;
}

long long clone_list(long long lst) {
    long long new_lst = 0;
    long long len = 0;
    long long idx = 0;
    long long item = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&new_lst);
    ep_gc_push_root(&idx);
    ep_gc_push_root(&item);
    ep_gc_push_root(&lst);
    ep_gc_maybe_collect();

    new_lst = create_list();
    len = length_list(lst);
    idx = 0LL;
    while (idx < len) {
    item = get_list(lst, idx);
    ok = append_list(new_lst, item);
    idx = (idx + 1LL);
    }
    ret_val = new_lst;
    new_lst = 0;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(4);
    return ret_val;
}

long long check_expr_reads(long long expr, long long var_keys, long long var_values, long long state_keys, long long state_values) {
    long long type = 0;
    long long name = 0;
    long long t = 0;
    long long is_tracked = 0;
    long long st = 0;
    long long ok = 0;
    long long inner = 0;
    long long left = 0;
    long long right = 0;
    long long args = 0;
    long long args_len = 0;
    long long idx = 0;
    long long arg = 0;
    long long chan = 0;
    long long ret_val = 0;

    ep_gc_push_root(&name);
    ep_gc_push_root(&inner);
    ep_gc_push_root(&left);
    ep_gc_push_root(&right);
    ep_gc_push_root(&args);
    ep_gc_push_root(&idx);
    ep_gc_push_root(&arg);
    ep_gc_push_root(&chan);
    ep_gc_push_root(&expr);
    ep_gc_push_root(&var_keys);
    ep_gc_push_root(&var_values);
    ep_gc_push_root(&state_keys);
    ep_gc_push_root(&state_values);
    ep_gc_maybe_collect();

    type = get_list(expr, 0LL);
    if (((type == 1LL || type == 2LL) || type == 17LL)) {
    ret_val = 1LL;
    goto L_cleanup;
    }
    if (type == 3LL) {
    name = get_list(expr, 1LL);
    t = map_get(var_keys, var_values, name);
    is_tracked = 0LL;
    if (((((t == 4LL || t == 2LL) || t == 3LL) || t == 5LL) || t == 6LL)) {
    is_tracked = 1LL;
    }
    if (is_tracked == 1LL) {
    st = map_get(state_keys, state_values, name);
    if (st == 2LL) {
    printf("%s\n", (char*)(long long)"Safety Error: Use of moved value:");
    ok = display_string((char*)name);
    ret_val = 0LL;
    goto L_cleanup;
    }
    }
    ret_val = 1LL;
    goto L_cleanup;
    }
    if (type == 20LL) {
    inner = get_list(expr, 1LL);
    ret_val = check_expr_reads(inner, var_keys, var_values, state_keys, state_values);
    goto L_cleanup;
    }
    if (type == 21LL) {
    inner = get_list(expr, 1LL);
    ret_val = check_expr_reads(inner, var_keys, var_values, state_keys, state_values);
    goto L_cleanup;
    }
    if (((type == 4LL || type == 5LL) || type == 14LL)) {
    left = get_list(expr, 1LL);
    right = get_list(expr, 3LL);
    ok = check_expr_reads(left, var_keys, var_values, state_keys, state_values);
    if (ok == 0LL) {
    ret_val = 0LL;
    goto L_cleanup;
    }
    ok = check_expr_reads(right, var_keys, var_values, state_keys, state_values);
    if (ok == 0LL) {
    ret_val = 0LL;
    goto L_cleanup;
    }
    ret_val = 1LL;
    goto L_cleanup;
    }
    if (type == 6LL) {
    name = get_list(expr, 1LL);
    args = get_list(expr, 2LL);
    args_len = length_list(args);
    idx = 0LL;
    while (idx < args_len) {
    arg = get_list(args, idx);
    ok = check_expr_reads(arg, var_keys, var_values, state_keys, state_values);
    if (ok == 0LL) {
    ret_val = 0LL;
    goto L_cleanup;
    }
    idx = (idx + 1LL);
    }
    ret_val = 1LL;
    goto L_cleanup;
    }
    if (type == 18LL) {
    chan = get_list(expr, 1LL);
    ret_val = check_expr_reads(chan, var_keys, var_values, state_keys, state_values);
    goto L_cleanup;
    }
    ret_val = 1LL;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(13);
    return ret_val;
}

long long dec_borrow_count(long long target, long long count_keys, long long count_values) {
    long long val = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&val);
    ep_gc_push_root(&target);
    ep_gc_push_root(&count_keys);
    ep_gc_push_root(&count_values);
    ep_gc_maybe_collect();

    val = map_get(count_keys, count_values, target);
    if (val != 0LL) {
    if (val > 0LL) {
    ok = map_put(count_keys, count_values, target, (val - 1LL));
    }
    }
    ret_val = 0LL;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(4);
    return ret_val;
}

long long inc_borrow_count(long long target, long long count_keys, long long count_values) {
    long long val = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&val);
    ep_gc_push_root(&target);
    ep_gc_push_root(&count_keys);
    ep_gc_push_root(&count_values);
    ep_gc_maybe_collect();

    val = map_get(count_keys, count_values, target);
    ok = map_put(count_keys, count_values, target, (val + 1LL));
    ret_val = 0LL;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(4);
    return ret_val;
}

long long check_safety_stmts(long long func, long long stmts, long long var_keys, long long var_values, long long state_keys, long long state_values, long long borrow_keys, long long borrow_values, long long count_keys, long long count_values) {
    long long len = 0;
    long long idx = 0;
    long long stmt = 0;
    long long type = 0;
    long long name = 0;
    long long expr = 0;
    long long ok = 0;
    long long old_target = 0;
    long long expr_type = 0;
    long long inner = 0;
    long long inner_type = 0;
    long long target = 0;
    long long st = 0;
    long long bc = 0;
    long long t = 0;
    long long is_tracked = 0;
    long long src = 0;
    long long src_t = 0;
    long long src_tracked = 0;
    long long src_bc = 0;
    long long chan = 0;
    long long val = 0;
    long long val_type = 0;
    long long func_name = 0;
    long long args = 0;
    long long args_len = 0;
    long long a_idx = 0;
    long long arg = 0;
    long long arg_type = 0;
    long long params = 0;
    long long p_len = 0;
    long long p_idx = 0;
    long long is_borrowed_param = 0;
    long long p_node = 0;
    long long p_name = 0;
    long long is_borrow = 0;
    long long cond = 0;
    long long then_b = 0;
    long long else_b = 0;
    long long then_state_keys = 0;
    long long then_state_values = 0;
    long long then_borrow_keys = 0;
    long long then_borrow_values = 0;
    long long then_count_keys = 0;
    long long then_count_values = 0;
    long long else_state_keys = 0;
    long long else_state_values = 0;
    long long else_borrow_keys = 0;
    long long else_borrow_values = 0;
    long long else_count_keys = 0;
    long long else_count_values = 0;
    long long st_len = 0;
    long long st_idx = 0;
    long long var_name = 0;
    long long then_val = 0;
    long long else_val = 0;
    long long b_len = 0;
    long long b_idx = 0;
    long long b_k = 0;
    long long b_v = 0;
    long long bc_len = 0;
    long long bc_idx = 0;
    long long bc_k = 0;
    long long bc_v = 0;
    long long cur_v = 0;
    long long body = 0;
    long long start_state_keys = 0;
    long long start_state_values = 0;
    long long start_val = 0;
    long long end_val = 0;
    long long ret_val = 0;

    ep_gc_push_root(&idx);
    ep_gc_push_root(&stmt);
    ep_gc_push_root(&name);
    ep_gc_push_root(&expr);
    ep_gc_push_root(&old_target);
    ep_gc_push_root(&inner);
    ep_gc_push_root(&target);
    ep_gc_push_root(&src);
    ep_gc_push_root(&chan);
    ep_gc_push_root(&val);
    ep_gc_push_root(&args);
    ep_gc_push_root(&a_idx);
    ep_gc_push_root(&arg);
    ep_gc_push_root(&params);
    ep_gc_push_root(&p_idx);
    ep_gc_push_root(&p_node);
    ep_gc_push_root(&p_name);
    ep_gc_push_root(&cond);
    ep_gc_push_root(&then_b);
    ep_gc_push_root(&else_b);
    ep_gc_push_root(&then_state_keys);
    ep_gc_push_root(&then_state_values);
    ep_gc_push_root(&then_borrow_keys);
    ep_gc_push_root(&then_borrow_values);
    ep_gc_push_root(&then_count_keys);
    ep_gc_push_root(&then_count_values);
    ep_gc_push_root(&else_state_keys);
    ep_gc_push_root(&else_state_values);
    ep_gc_push_root(&else_borrow_keys);
    ep_gc_push_root(&else_borrow_values);
    ep_gc_push_root(&else_count_keys);
    ep_gc_push_root(&else_count_values);
    ep_gc_push_root(&st_idx);
    ep_gc_push_root(&var_name);
    ep_gc_push_root(&b_idx);
    ep_gc_push_root(&b_k);
    ep_gc_push_root(&b_v);
    ep_gc_push_root(&bc_idx);
    ep_gc_push_root(&bc_k);
    ep_gc_push_root(&bc_v);
    ep_gc_push_root(&body);
    ep_gc_push_root(&start_state_keys);
    ep_gc_push_root(&start_state_values);
    ep_gc_push_root(&func);
    ep_gc_push_root(&stmts);
    ep_gc_push_root(&var_keys);
    ep_gc_push_root(&var_values);
    ep_gc_push_root(&state_keys);
    ep_gc_push_root(&state_values);
    ep_gc_push_root(&borrow_keys);
    ep_gc_push_root(&borrow_values);
    ep_gc_push_root(&count_keys);
    ep_gc_push_root(&count_values);
    ep_gc_maybe_collect();

    len = length_list(stmts);
    idx = 0LL;
    while (idx < len) {
    stmt = get_list(stmts, idx);
    type = get_list(stmt, 0LL);
    if (type == 7LL) {
    name = get_list(stmt, 1LL);
    expr = get_list(stmt, 2LL);
    ok = check_expr_reads(expr, var_keys, var_values, state_keys, state_values);
    if (ok == 0LL) {
    ret_val = 0LL;
    goto L_cleanup;
    }
    old_target = map_get(borrow_keys, borrow_values, name);
    if (old_target != 0LL) {
    ok = map_put(borrow_keys, borrow_values, name, 0LL);
    ok = dec_borrow_count(old_target, count_keys, count_values);
    }
    expr_type = get_list(expr, 0LL);
    if (expr_type == 20LL) {
    inner = get_list(expr, 1LL);
    inner_type = get_list(inner, 0LL);
    if (inner_type == 3LL) {
    target = get_list(inner, 1LL);
    st = map_get(state_keys, state_values, target);
    if (st == 2LL) {
    printf("%s\n", (char*)(long long)"Safety Error: Cannot borrow moved variable:");
    ok = display_string((char*)target);
    ret_val = 0LL;
    goto L_cleanup;
    }
    ok = map_put(borrow_keys, borrow_values, name, target);
    ok = inc_borrow_count(target, count_keys, count_values);
    } else {
    printf("%s\n", (char*)(long long)"Safety Error: Expected identifier in borrow expression");
    ret_val = 0LL;
    goto L_cleanup;
    }
    } else {
    bc = map_get(count_keys, count_values, name);
    if (bc > 0LL) {
    printf("%s\n", (char*)(long long)"Safety Error: Cannot modify variable because it is currently borrowed:");
    ok = display_string((char*)name);
    ret_val = 0LL;
    goto L_cleanup;
    }
    t = map_get(var_keys, var_values, name);
    is_tracked = 0LL;
    if (((((t == 4LL || t == 2LL) || t == 3LL) || t == 5LL) || t == 6LL)) {
    is_tracked = 1LL;
    }
    if (is_tracked == 1LL) {
    if (expr_type == 3LL) {
    src = get_list(expr, 1LL);
    src_t = map_get(var_keys, var_values, src);
    src_tracked = 0LL;
    if (((((src_t == 4LL || src_t == 2LL) || src_t == 3LL) || src_t == 5LL) || src_t == 6LL)) {
    src_tracked = 1LL;
    }
    if (src_tracked == 1LL) {
    st = map_get(state_keys, state_values, src);
    if (st == 2LL) {
    printf("%s\n", (char*)(long long)"Safety Error: Use of moved value:");
    ok = display_string((char*)src);
    ret_val = 0LL;
    goto L_cleanup;
    }
    src_bc = map_get(count_keys, count_values, src);
    if (src_bc > 0LL) {
    printf("%s\n", (char*)(long long)"Safety Error: Cannot move variable because it is currently borrowed:");
    ok = display_string((char*)src);
    ret_val = 0LL;
    goto L_cleanup;
    }
    }
    }
    ok = map_put(state_keys, state_values, name, 1LL);
    }
    }
    }
    if (type == 16LL) {
    chan = get_list(stmt, 1LL);
    val = get_list(stmt, 2LL);
    ok = check_expr_reads(chan, var_keys, var_values, state_keys, state_values);
    if (ok == 0LL) {
    ret_val = 0LL;
    goto L_cleanup;
    }
    val_type = get_list(val, 0LL);
    if (val_type == 3LL) {
    src = get_list(val, 1LL);
    src_t = map_get(var_keys, var_values, src);
    src_tracked = 0LL;
    if (((((src_t == 4LL || src_t == 2LL) || src_t == 3LL) || src_t == 5LL) || src_t == 6LL)) {
    src_tracked = 1LL;
    }
    if (src_tracked == 1LL) {
    st = map_get(state_keys, state_values, src);
    if (st == 2LL) {
    printf("%s\n", (char*)(long long)"Safety Error: Use of moved value:");
    ok = display_string((char*)src);
    ret_val = 0LL;
    goto L_cleanup;
    }
    src_bc = map_get(count_keys, count_values, src);
    if (src_bc > 0LL) {
    printf("%s\n", (char*)(long long)"Safety Error: Cannot move variable because it is currently borrowed:");
    ok = display_string((char*)src);
    ret_val = 0LL;
    goto L_cleanup;
    }
    if ((src_t != 5LL && src_t != 6LL)) {
    ok = map_put(state_keys, state_values, src, 2LL);
    }
    } else {
    ok = check_expr_reads(val, var_keys, var_values, state_keys, state_values);
    if (ok == 0LL) {
    ret_val = 0LL;
    goto L_cleanup;
    }
    }
    } else {
    ok = check_expr_reads(val, var_keys, var_values, state_keys, state_values);
    if (ok == 0LL) {
    ret_val = 0LL;
    goto L_cleanup;
    }
    }
    }
    if (type == 15LL) {
    func_name = get_list(stmt, 1LL);
    args = get_list(stmt, 2LL);
    args_len = length_list(args);
    a_idx = 0LL;
    while (a_idx < args_len) {
    arg = get_list(args, a_idx);
    arg_type = get_list(arg, 0LL);
    if (arg_type == 3LL) {
    src = get_list(arg, 1LL);
    src_t = map_get(var_keys, var_values, src);
    src_tracked = 0LL;
    if (((((src_t == 4LL || src_t == 2LL) || src_t == 3LL) || src_t == 5LL) || src_t == 6LL)) {
    src_tracked = 1LL;
    }
    if (src_tracked == 1LL) {
    st = map_get(state_keys, state_values, src);
    if (st == 2LL) {
    printf("%s\n", (char*)(long long)"Safety Error: Use of moved value:");
    ok = display_string((char*)src);
    ret_val = 0LL;
    goto L_cleanup;
    }
    src_bc = map_get(count_keys, count_values, src);
    if (src_bc > 0LL) {
    printf("%s\n", (char*)(long long)"Safety Error: Cannot move variable because it is currently borrowed:");
    ok = display_string((char*)src);
    ret_val = 0LL;
    goto L_cleanup;
    }
    if ((src_t != 5LL && src_t != 6LL)) {
    ok = map_put(state_keys, state_values, src, 2LL);
    }
    } else {
    ok = check_expr_reads(arg, var_keys, var_values, state_keys, state_values);
    if (ok == 0LL) {
    ret_val = 0LL;
    goto L_cleanup;
    }
    }
    } else {
    ok = check_expr_reads(arg, var_keys, var_values, state_keys, state_values);
    if (ok == 0LL) {
    ret_val = 0LL;
    goto L_cleanup;
    }
    }
    a_idx = (a_idx + 1LL);
    }
    }
    if (type == 9LL) {
    expr = get_list(stmt, 1LL);
    ok = check_expr_reads(expr, var_keys, var_values, state_keys, state_values);
    if (ok == 0LL) {
    ret_val = 0LL;
    goto L_cleanup;
    }
    }
    if (type == 8LL) {
    expr = get_list(stmt, 1LL);
    ok = check_expr_reads(expr, var_keys, var_values, state_keys, state_values);
    if (ok == 0LL) {
    ret_val = 0LL;
    goto L_cleanup;
    }
    expr_type = get_list(expr, 0LL);
    if (expr_type == 20LL) {
    inner = get_list(expr, 1LL);
    inner_type = get_list(inner, 0LL);
    if (inner_type == 3LL) {
    target = get_list(inner, 1LL);
    params = get_list(func, 2LL);
    p_len = length_list(params);
    p_idx = 0LL;
    is_borrowed_param = 0LL;
    while (p_idx < p_len) {
    p_node = get_list(params, p_idx);
    p_name = get_list(p_node, 0LL);
    is_borrow = get_list(p_node, 1LL);
    if ((strcmp((char*)string_concat(p_name, (long long)""), (char*)string_concat(target, (long long)"")) == 0)) {
    if (is_borrow == 1LL) {
    is_borrowed_param = 1LL;
    }
    }
    p_idx = (p_idx + 1LL);
    }
    if (is_borrowed_param == 0LL) {
    printf("%s\n", (char*)(long long)"Safety Error: Cannot return reference to local variable:");
    ok = display_string((char*)target);
    ret_val = 0LL;
    goto L_cleanup;
    }
    }
    } else {
    if (expr_type == 3LL) {
    name = get_list(expr, 1LL);
    t = map_get(var_keys, var_values, name);
    if ((t == 5LL || t == 6LL)) {
    target = map_get(borrow_keys, borrow_values, name);
    if (target == 0LL) {
    target = name;
    }
    params = get_list(func, 2LL);
    p_len = length_list(params);
    p_idx = 0LL;
    is_borrowed_param = 0LL;
    while (p_idx < p_len) {
    p_node = get_list(params, p_idx);
    p_name = get_list(p_node, 0LL);
    is_borrow = get_list(p_node, 1LL);
    if ((strcmp((char*)string_concat(p_name, (long long)""), (char*)string_concat(target, (long long)"")) == 0)) {
    if (is_borrow == 1LL) {
    is_borrowed_param = 1LL;
    }
    }
    p_idx = (p_idx + 1LL);
    }
    if (is_borrowed_param == 0LL) {
    printf("%s\n", (char*)(long long)"Safety Error: Cannot return reference to local variable:");
    ok = display_string((char*)target);
    ret_val = 0LL;
    goto L_cleanup;
    }
    }
    }
    }
    }
    if (type == 10LL) {
    cond = get_list(stmt, 1LL);
    then_b = get_list(stmt, 2LL);
    else_b = get_list(stmt, 3LL);
    ok = check_expr_reads(cond, var_keys, var_values, state_keys, state_values);
    if (ok == 0LL) {
    ret_val = 0LL;
    goto L_cleanup;
    }
    then_state_keys = clone_list(state_keys);
    then_state_values = clone_list(state_values);
    then_borrow_keys = clone_list(borrow_keys);
    then_borrow_values = clone_list(borrow_values);
    then_count_keys = clone_list(count_keys);
    then_count_values = clone_list(count_values);
    ok = check_safety_stmts(func, then_b, var_keys, var_values, then_state_keys, then_state_values, then_borrow_keys, then_borrow_values, then_count_keys, then_count_values);
    if (ok == 0LL) {
    ret_val = 0LL;
    goto L_cleanup;
    }
    else_state_keys = clone_list(state_keys);
    else_state_values = clone_list(state_values);
    else_borrow_keys = clone_list(borrow_keys);
    else_borrow_values = clone_list(borrow_values);
    else_count_keys = clone_list(count_keys);
    else_count_values = clone_list(count_values);
    if (else_b != 0LL) {
    ok = check_safety_stmts(func, else_b, var_keys, var_values, else_state_keys, else_state_values, else_borrow_keys, else_borrow_values, else_count_keys, else_count_values);
    if (ok == 0LL) {
    ret_val = 0LL;
    goto L_cleanup;
    }
    }
    st_len = length_list(state_keys);
    st_idx = 0LL;
    while (st_idx < st_len) {
    var_name = get_list(state_keys, st_idx);
    then_val = map_get(then_state_keys, then_state_values, var_name);
    else_val = map_get(else_state_keys, else_state_values, var_name);
    if ((then_val == 2LL || else_val == 2LL)) {
    ok = map_put(state_keys, state_values, var_name, 2LL);
    }
    st_idx = (st_idx + 1LL);
    }
    b_len = length_list(then_borrow_keys);
    b_idx = 0LL;
    while (b_idx < b_len) {
    b_k = get_list(then_borrow_keys, b_idx);
    b_v = get_list(then_borrow_values, b_idx);
    ok = map_put(borrow_keys, borrow_values, b_k, b_v);
    b_idx = (b_idx + 1LL);
    }
    b_len = length_list(else_borrow_keys);
    b_idx = 0LL;
    while (b_idx < b_len) {
    b_k = get_list(else_borrow_keys, b_idx);
    b_v = get_list(else_borrow_values, b_idx);
    ok = map_put(borrow_keys, borrow_values, b_k, b_v);
    b_idx = (b_idx + 1LL);
    }
    bc_len = length_list(then_count_keys);
    bc_idx = 0LL;
    while (bc_idx < bc_len) {
    bc_k = get_list(then_count_keys, bc_idx);
    bc_v = get_list(then_count_values, bc_idx);
    cur_v = map_get(count_keys, count_values, bc_k);
    if (bc_v > cur_v) {
    ok = map_put(count_keys, count_values, bc_k, bc_v);
    }
    bc_idx = (bc_idx + 1LL);
    }
    bc_len = length_list(else_count_keys);
    bc_idx = 0LL;
    while (bc_idx < bc_len) {
    bc_k = get_list(else_count_keys, bc_idx);
    bc_v = get_list(else_count_values, bc_idx);
    cur_v = map_get(count_keys, count_values, bc_k);
    if (bc_v > cur_v) {
    ok = map_put(count_keys, count_values, bc_k, bc_v);
    }
    bc_idx = (bc_idx + 1LL);
    }
    }
    if (type == 11LL) {
    cond = get_list(stmt, 1LL);
    body = get_list(stmt, 2LL);
    ok = check_expr_reads(cond, var_keys, var_values, state_keys, state_values);
    if (ok == 0LL) {
    ret_val = 0LL;
    goto L_cleanup;
    }
    start_state_keys = clone_list(state_keys);
    start_state_values = clone_list(state_values);
    ok = check_safety_stmts(func, body, var_keys, var_values, state_keys, state_values, borrow_keys, borrow_values, count_keys, count_values);
    if (ok == 0LL) {
    ret_val = 0LL;
    goto L_cleanup;
    }
    st_len = length_list(start_state_keys);
    st_idx = 0LL;
    while (st_idx < st_len) {
    var_name = get_list(start_state_keys, st_idx);
    start_val = get_list(start_state_values, st_idx);
    end_val = map_get(state_keys, state_values, var_name);
    if ((start_val == 1LL && end_val == 2LL)) {
    printf("%s\n", (char*)(long long)"Safety Error: Variable is moved inside a loop and not reinitialized:");
    ok = display_string((char*)var_name);
    ret_val = 0LL;
    goto L_cleanup;
    }
    st_idx = (st_idx + 1LL);
    }
    ok = check_expr_reads(cond, var_keys, var_values, state_keys, state_values);
    if (ok == 0LL) {
    ret_val = 0LL;
    goto L_cleanup;
    }
    }
    idx = (idx + 1LL);
    }
    ret_val = 1LL;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(53);
    return ret_val;
}

long long analyze_safety(long long state, long long program) {
    long long funcs = 0;
    long long funcs_len = 0;
    long long idx = 0;
    long long func = 0;
    long long name = 0;
    long long params = 0;
    long long body = 0;
    long long var_keys = 0;
    long long var_values = 0;
    long long p_len = 0;
    long long p_idx = 0;
    long long p_node = 0;
    long long p_name = 0;
    long long is_borrow = 0;
    long long param_type = 0;
    long long ok = 0;
    long long state_keys = 0;
    long long state_values = 0;
    long long t = 0;
    long long borrow_keys = 0;
    long long borrow_values = 0;
    long long count_keys = 0;
    long long count_values = 0;
    long long ret_val = 0;

    ep_gc_push_root(&funcs);
    ep_gc_push_root(&idx);
    ep_gc_push_root(&func);
    ep_gc_push_root(&params);
    ep_gc_push_root(&body);
    ep_gc_push_root(&var_keys);
    ep_gc_push_root(&var_values);
    ep_gc_push_root(&p_idx);
    ep_gc_push_root(&p_node);
    ep_gc_push_root(&p_name);
    ep_gc_push_root(&param_type);
    ep_gc_push_root(&state_keys);
    ep_gc_push_root(&state_values);
    ep_gc_push_root(&borrow_keys);
    ep_gc_push_root(&borrow_values);
    ep_gc_push_root(&count_keys);
    ep_gc_push_root(&count_values);
    ep_gc_push_root(&state);
    ep_gc_push_root(&program);
    ep_gc_maybe_collect();

    funcs = get_list(program, 3LL);
    funcs_len = length_list(funcs);
    idx = 0LL;
    while (idx < funcs_len) {
    func = get_list(funcs, idx);
    name = get_list(func, 1LL);
    params = get_list(func, 2LL);
    body = get_list(func, 3LL);
    var_keys = (create_list() + 0LL);
    var_values = (create_list() + 0LL);
    p_len = length_list(params);
    p_idx = 0LL;
    while (p_idx < p_len) {
    p_node = get_list(params, p_idx);
    p_name = get_list(p_node, 0LL);
    is_borrow = get_list(p_node, 1LL);
    param_type = 1LL;
    if (is_borrow == 1LL) {
    param_type = 5LL;
    }
    ok = map_put(var_keys, var_values, p_name, param_type);
    p_idx = (p_idx + 1LL);
    }
    ok = collect_var_types(state, body, var_keys, var_values);
    state_keys = (create_list() + 0LL);
    state_values = (create_list() + 0LL);
    p_idx = 0LL;
    while (p_idx < p_len) {
    p_node = get_list(params, p_idx);
    p_name = get_list(p_node, 0LL);
    t = map_get(var_keys, var_values, p_name);
    if (((((t == 4LL || t == 2LL) || t == 3LL) || t == 5LL) || t == 6LL)) {
    ok = map_put(state_keys, state_values, p_name, 1LL);
    }
    p_idx = (p_idx + 1LL);
    }
    borrow_keys = (create_list() + 0LL);
    borrow_values = (create_list() + 0LL);
    count_keys = (create_list() + 0LL);
    count_values = (create_list() + 0LL);
    ok = check_safety_stmts(func, body, var_keys, var_values, state_keys, state_values, borrow_keys, borrow_values, count_keys, count_values);
    if (ok == 0LL) {
    ret_val = 0LL;
    goto L_cleanup;
    }
    idx = (idx + 1LL);
    }
    ret_val = 1LL;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(19);
    return ret_val;
}

long long validate_program_safety(long long program) {
    long long state = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&state);
    ep_gc_push_root(&program);
    ep_gc_maybe_collect();

    state = create_codegen_state();
    ok = analyze_return_types(state, program);
    ok = collect_prim_param_flags(state, program);
    ret_val = analyze_safety(state, program);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(2);
    return ret_val;
}

long long generate_c(long long program, long long is_test_mode) {
    long long state = 0;
    long long ok = 0;
    long long it_impls = 0;
    long long it_i = 0;
    long long it_len = 0;
    long long it_impl = 0;
    long long safety_ok = 0;
    long long prog_len = 0;
    long long field_slots = 0;
    long long struct_defs = 0;
    long long sd_len = 0;
    long long sd_idx = 0;
    long long sdef = 0;
    long long sfields = 0;
    long long sf_len = 0;
    long long sf_idx = 0;
    long long sf = 0;
    long long sf_name = 0;
    long long slot = 0;
    long long fs_len = 0;
    long long fs_idx = 0;
    long long fname = 0;
    long long line = 0;
    long long slot_line = 0;
    long long tag_slots = 0;
    long long enum_defs = 0;
    long long ed_len = 0;
    long long ed_idx = 0;
    long long edef = 0;
    long long evariants = 0;
    long long ev_len = 0;
    long long ev_idx = 0;
    long long ev = 0;
    long long ev_name = 0;
    long long tslot = 0;
    long long okvn = 0;
    long long ts_len = 0;
    long long ts_idx = 0;
    long long tname = 0;
    long long vpat_keys = 0;
    long long vpat_vals = 0;
    long long vp_enums = 0;
    long long vp_i = 0;
    long long vp_len = 0;
    long long vp_variants = 0;
    long long vpv_i = 0;
    long long vpv_len = 0;
    long long vp_var = 0;
    long long vp_fields = 0;
    long long vp_codes = 0;
    long long vpf_i = 0;
    long long vpf_len = 0;
    long long try_keys = 0;
    long long try_vals = 0;
    long long all_enums = 0;
    long long try_funcs = 0;
    long long tf_len = 0;
    long long tf_i = 0;
    long long tf = 0;
    long long rt = 0;
    long long en_i = 0;
    long long en_len = 0;
    long long evs = 0;
    long long constants = 0;
    long long const_n = 0;
    long long ci = 0;
    long long cstmt = 0;
    long long gline = 0;
    long long gn = 0;
    long long ml = 0;
    long long gk = 0;
    long long gv = 0;
    long long ival = 0;
    long long il = 0;
    long long externals = 0;
    long long ext_len = 0;
    long long idx = 0;
    long long ext = 0;
    long long name = 0;
    long long params = 0;
    long long p_len = 0;
    long long proto = 0;
    long long p_i = 0;
    long long funcs = 0;
    long long len = 0;
    long long func = 0;
    long long is_async = 0;
    long long proto2 = 0;
    long long spawn_list = 0;
    long long dummy = 0;
    long long spawn_len = 0;
    long long spawn_node = 0;
    long long func_name = 0;
    long long args = 0;
    long long args_len = 0;
    long long struct_decl = 0;
    long long j = 0;
    long long wrap_fn = 0;
    long long c_name = 0;
    long long out_lines = 0;
    long long closure_slot = 0;
    long long method_defs = 0;
    long long md_len = 0;
    long long md_idx = 0;
    long long mdef = 0;
    long long mname = 0;
    long long msname = 0;
    long long mparams = 0;
    long long mbody = 0;
    long long full_params = 0;
    long long self_param = 0;
    long long mp_len = 0;
    long long mp_idx = 0;
    long long mp = 0;
    long long method_func = 0;
    long long emitted_methods = 0;
    long long trait_impls = 0;
    long long ti_len = 0;
    long long ti_idx = 0;
    long long timpl = 0;
    long long imethods = 0;
    long long im_len = 0;
    long long im_idx = 0;
    long long imeth = 0;
    long long imeth_name = 0;
    long long tparams = 0;
    long long tbody = 0;
    long long tfull = 0;
    long long tself = 0;
    long long tp_len = 0;
    long long tp_idx = 0;
    long long tfunc = 0;
    long long lines = 0;
    long long cbodies = 0;
    long long marker_line = 0;
    long long spliced = 0;
    long long oksp = 0;
    long long c_code = 0;
    long long ret_val = 0;

    ep_gc_push_root(&state);
    ep_gc_push_root(&it_impls);
    ep_gc_push_root(&it_i);
    ep_gc_push_root(&it_impl);
    ep_gc_push_root(&field_slots);
    ep_gc_push_root(&struct_defs);
    ep_gc_push_root(&sd_idx);
    ep_gc_push_root(&sdef);
    ep_gc_push_root(&sfields);
    ep_gc_push_root(&sf_idx);
    ep_gc_push_root(&sf);
    ep_gc_push_root(&sf_name);
    ep_gc_push_root(&fs_idx);
    ep_gc_push_root(&fname);
    ep_gc_push_root(&line);
    ep_gc_push_root(&slot_line);
    ep_gc_push_root(&tag_slots);
    ep_gc_push_root(&enum_defs);
    ep_gc_push_root(&ed_idx);
    ep_gc_push_root(&edef);
    ep_gc_push_root(&evariants);
    ep_gc_push_root(&ev_idx);
    ep_gc_push_root(&ev);
    ep_gc_push_root(&ev_name);
    ep_gc_push_root(&ts_idx);
    ep_gc_push_root(&tname);
    ep_gc_push_root(&vpat_keys);
    ep_gc_push_root(&vpat_vals);
    ep_gc_push_root(&vp_enums);
    ep_gc_push_root(&vp_i);
    ep_gc_push_root(&vp_variants);
    ep_gc_push_root(&vpv_i);
    ep_gc_push_root(&vp_var);
    ep_gc_push_root(&vp_fields);
    ep_gc_push_root(&vp_codes);
    ep_gc_push_root(&vpf_i);
    ep_gc_push_root(&try_keys);
    ep_gc_push_root(&try_vals);
    ep_gc_push_root(&all_enums);
    ep_gc_push_root(&try_funcs);
    ep_gc_push_root(&tf_i);
    ep_gc_push_root(&tf);
    ep_gc_push_root(&rt);
    ep_gc_push_root(&en_i);
    ep_gc_push_root(&evs);
    ep_gc_push_root(&constants);
    ep_gc_push_root(&ci);
    ep_gc_push_root(&cstmt);
    ep_gc_push_root(&gline);
    ep_gc_push_root(&gn);
    ep_gc_push_root(&ml);
    ep_gc_push_root(&gk);
    ep_gc_push_root(&gv);
    ep_gc_push_root(&ival);
    ep_gc_push_root(&il);
    ep_gc_push_root(&externals);
    ep_gc_push_root(&idx);
    ep_gc_push_root(&ext);
    ep_gc_push_root(&name);
    ep_gc_push_root(&params);
    ep_gc_push_root(&proto);
    ep_gc_push_root(&funcs);
    ep_gc_push_root(&func);
    ep_gc_push_root(&proto2);
    ep_gc_push_root(&spawn_list);
    ep_gc_push_root(&spawn_node);
    ep_gc_push_root(&args);
    ep_gc_push_root(&struct_decl);
    ep_gc_push_root(&j);
    ep_gc_push_root(&wrap_fn);
    ep_gc_push_root(&c_name);
    ep_gc_push_root(&out_lines);
    ep_gc_push_root(&closure_slot);
    ep_gc_push_root(&method_defs);
    ep_gc_push_root(&md_idx);
    ep_gc_push_root(&mdef);
    ep_gc_push_root(&mname);
    ep_gc_push_root(&mparams);
    ep_gc_push_root(&mbody);
    ep_gc_push_root(&full_params);
    ep_gc_push_root(&self_param);
    ep_gc_push_root(&mp_idx);
    ep_gc_push_root(&mp);
    ep_gc_push_root(&method_func);
    ep_gc_push_root(&emitted_methods);
    ep_gc_push_root(&trait_impls);
    ep_gc_push_root(&ti_idx);
    ep_gc_push_root(&timpl);
    ep_gc_push_root(&imethods);
    ep_gc_push_root(&im_idx);
    ep_gc_push_root(&imeth);
    ep_gc_push_root(&imeth_name);
    ep_gc_push_root(&tparams);
    ep_gc_push_root(&tbody);
    ep_gc_push_root(&tfull);
    ep_gc_push_root(&tself);
    ep_gc_push_root(&tp_idx);
    ep_gc_push_root(&tfunc);
    ep_gc_push_root(&lines);
    ep_gc_push_root(&cbodies);
    ep_gc_push_root(&marker_line);
    ep_gc_push_root(&spliced);
    ep_gc_push_root(&c_code);
    ep_gc_push_root(&program);
    ep_gc_maybe_collect();

    state = create_codegen_state();
    ok = analyze_return_types(state, program);
    ok = collect_prim_param_flags(state, program);
    if (length_list(program) > 8LL) {
    it_impls = get_list(program, 8LL);
    it_i = 0LL;
    it_len = length_list(it_impls);
    while (it_i < it_len) {
    it_impl = get_list(it_impls, it_i);
    if ((strcmp((char*)string_concat(get_list(it_impl, 1LL), (long long)""), (char*)(long long)"Iterator") == 0)) {
    ok = append_list(get_list(state, 18LL), get_list(it_impl, 2LL));
    }
    it_i = (it_i + 1LL);
    }
    }
    safety_ok = analyze_safety(state, program);
    if (safety_ok == 0LL) {
    ret_val = (long long)"";
    goto L_cleanup;
    }
    ok = emit(state, get_c_runtime_source());
    prog_len = length_list(program);
    field_slots = create_list();
    if (prog_len > 4LL) {
    struct_defs = get_list(program, 4LL);
    sd_len = length_list(struct_defs);
    if (sd_len > 0LL) {
    ok = emit(state, (long long)"\n/* Struct Field Index Defines (global slots) */\n");
    sd_idx = 0LL;
    while (sd_idx < sd_len) {
    sdef = get_list(struct_defs, sd_idx);
    sfields = get_list(sdef, 2LL);
    sf_len = length_list(sfields);
    sf_idx = 0LL;
    while (sf_idx < sf_len) {
    sf = get_list(sfields, sf_idx);
    sf_name = get_list(sf, 0LL);
    slot = field_slot_index(field_slots, sf_name);
    sf_idx = (sf_idx + 1LL);
    }
    sd_idx = (sd_idx + 1LL);
    }
    fs_len = length_list(field_slots);
    fs_idx = 0LL;
    while (fs_idx < fs_len) {
    fname = get_list(field_slots, fs_idx);
    line = (long long)"#define EP_FIELD_";
    line = string_concat(line, fname);
    line = string_concat(line, (long long)" ");
    line = string_concat(line, cg_int_to_str(fs_idx));
    line = string_concat(line, (long long)"\n");
    ok = emit(state, line);
    fs_idx = (fs_idx + 1LL);
    }
    }
    }
    slot_line = (long long)"#define EP_STRUCT_MAX_SLOTS ";
    slot_line = string_concat(slot_line, cg_int_to_str((length_list(field_slots) + 8LL)));
    slot_line = string_concat(slot_line, (long long)"\n");
    ok = emit(state, slot_line);
    tag_slots = create_list();
    if (prog_len > 5LL) {
    enum_defs = get_list(program, 5LL);
    ed_len = length_list(enum_defs);
    if (ed_len > 0LL) {
    ok = emit(state, (long long)"\n/* Enum Tag Defines (global slots) */\n");
    ed_idx = 0LL;
    while (ed_idx < ed_len) {
    edef = get_list(enum_defs, ed_idx);
    evariants = get_list(edef, 2LL);
    ev_len = length_list(evariants);
    ev_idx = 0LL;
    while (ev_idx < ev_len) {
    ev = get_list(evariants, ev_idx);
    ev_name = get_list(ev, 0LL);
    tslot = field_slot_index(tag_slots, ev_name);
    okvn = append_list(get_list(state, 12LL), ev_name);
    ev_idx = (ev_idx + 1LL);
    }
    ed_idx = (ed_idx + 1LL);
    }
    ts_len = length_list(tag_slots);
    ts_idx = 0LL;
    while (ts_idx < ts_len) {
    tname = get_list(tag_slots, ts_idx);
    line = (long long)"#define EP_TAG_";
    line = string_concat(line, tname);
    line = string_concat(line, (long long)" ");
    line = string_concat(line, cg_int_to_str(ts_idx));
    line = string_concat(line, (long long)"\n");
    ok = emit(state, line);
    ts_idx = (ts_idx + 1LL);
    }
    }
    }
    vpat_keys = get_list(state, 16LL);
    vpat_vals = get_list(state, 17LL);
    if (prog_len > 5LL) {
    vp_enums = get_list(program, 5LL);
    vp_i = 0LL;
    vp_len = length_list(vp_enums);
    while (vp_i < vp_len) {
    vp_variants = get_list(get_list(vp_enums, vp_i), 2LL);
    vpv_i = 0LL;
    vpv_len = length_list(vp_variants);
    while (vpv_i < vpv_len) {
    vp_var = get_list(vp_variants, vpv_i);
    vp_fields = get_list(vp_var, 1LL);
    vp_codes = create_list();
    vpf_i = 0LL;
    vpf_len = length_list(vp_fields);
    while (vpf_i < vpf_len) {
    ok = append_list(vp_codes, type_name_to_code(get_list(get_list(vp_fields, vpf_i), 1LL)));
    vpf_i = (vpf_i + 1LL);
    }
    ok = append_list(vpat_keys, get_list(vp_var, 0LL));
    ok = append_list(vpat_vals, vp_codes);
    vpv_i = (vpv_i + 1LL);
    }
    vp_i = (vp_i + 1LL);
    }
    }
    try_keys = get_list(state, 14LL);
    try_vals = get_list(state, 15LL);
    if (prog_len > 5LL) {
    all_enums = get_list(program, 5LL);
    try_funcs = get_list(program, 3LL);
    tf_len = length_list(try_funcs);
    tf_i = 0LL;
    while (tf_i < tf_len) {
    tf = get_list(try_funcs, tf_i);
    if (length_list(tf) > 5LL) {
    rt = string_concat(get_list(tf, 5LL), (long long)"");
    if (string_length((char*)rt) > 0LL) {
    en_i = 0LL;
    en_len = length_list(all_enums);
    while (en_i < en_len) {
    edef = get_list(all_enums, en_i);
    if ((strcmp((char*)rt, (char*)get_list(edef, 1LL)) == 0)) {
    evs = get_list(edef, 2LL);
    if (length_list(evs) > 0LL) {
    ok = append_list(try_keys, get_list(tf, 1LL));
    ok = append_list(try_vals, get_list(get_list(evs, 0LL), 0LL));
    }
    }
    en_i = (en_i + 1LL);
    }
    }
    }
    tf_i = (tf_i + 1LL);
    }
    }
    constants = create_list();
    if (length_list(program) > 9LL) {
    constants = get_list(program, 9LL);
    }
    const_n = length_list(constants);
    ci = 0LL;
    while (ci < const_n) {
    cstmt = get_list(constants, ci);
    gline = (long long)"long long ";
    gline = string_concat(gline, get_list(cstmt, 1LL));
    gline = string_concat(gline, (long long)" = 0;\n");
    ok = emit(state, gline);
    ci = (ci + 1LL);
    }
    ok = emit(state, (long long)"static void __ep_mark_globals_major(void) {\n");
    ci = 0LL;
    while (ci < const_n) {
    gn = get_list(get_list(constants, ci), 1LL);
    ml = (long long)"    if (";
    ml = string_concat(ml, gn);
    ml = string_concat(ml, (long long)" != 0) ep_gc_mark_object((void*)");
    ml = string_concat(ml, gn);
    ml = string_concat(ml, (long long)");\n");
    ok = emit(state, ml);
    ci = (ci + 1LL);
    }
    ok = emit(state, (long long)"}\n");
    ok = emit(state, (long long)"static void __ep_mark_globals_minor(void) {\n");
    ci = 0LL;
    while (ci < const_n) {
    gn = get_list(get_list(constants, ci), 1LL);
    ml = (long long)"    if (";
    ml = string_concat(ml, gn);
    ml = string_concat(ml, (long long)" != 0) ep_gc_mark_object_minor((void*)");
    ml = string_concat(ml, gn);
    ml = string_concat(ml, (long long)");\n");
    ok = emit(state, ml);
    ci = (ci + 1LL);
    }
    ok = emit(state, (long long)"}\n");
    ok = emit(state, (long long)"void __ep_init_constants(void) {\n");
    ok = emit(state, (long long)"    ep_gc_mark_globals_major = __ep_mark_globals_major;\n");
    ok = emit(state, (long long)"    ep_gc_mark_globals_minor = __ep_mark_globals_minor;\n");
    gk = create_list();
    gv = create_list();
    ci = 0LL;
    while (ci < const_n) {
    cstmt = get_list(constants, ci);
    ival = gen_expr(state, get_list(cstmt, 2LL), gk, gv);
    il = (long long)"    ";
    il = string_concat(il, get_list(cstmt, 1LL));
    il = string_concat(il, (long long)" = ");
    il = string_concat(il, ival);
    il = string_concat(il, (long long)";\n");
    ok = emit(state, il);
    ci = (ci + 1LL);
    }
    ok = emit(state, (long long)"}\n");
    externals = get_list(program, 2LL);
    ext_len = length_list(externals);
    ok = emit(state, (long long)"\n/* External Function Prototypes (FFI) */\n");
    idx = 0LL;
    while (idx < ext_len) {
    ext = get_list(externals, idx);
    name = get_list(ext, 1LL);
    if (is_builtin_c_func(state, name) == 0LL) {
    params = get_list(ext, 2LL);
    p_len = length_list(params);
    proto = (long long)"long long ";
    proto = string_concat(proto, name);
    proto = string_concat(proto, (long long)"(");
    p_i = 0LL;
    while (p_i < p_len) {
    proto = string_concat(proto, (long long)"long long");
    if (p_i < (p_len - 1LL)) {
    proto = string_concat(proto, (long long)", ");
    }
    p_i = (p_i + 1LL);
    }
    proto = string_concat(proto, (long long)");\n");
    ok = emit(state, proto);
    }
    idx = (idx + 1LL);
    }
    ok = emit(state, (long long)"\n");
    funcs = get_list(program, 3LL);
    len = length_list(funcs);
    ok = emit(state, (long long)"\n/* User Function Prototypes */\n");
    idx = 0LL;
    while (idx < len) {
    func = get_list(funcs, idx);
    name = get_list(func, 1LL);
    if (is_builtin_c_func(state, name) == 1LL) {
    idx = (idx + 1LL);
    continue;
    }
    params = get_list(func, 2LL);
    p_len = length_list(params);
    is_async = 0LL;
    if (length_list(func) > 4LL) {
    is_async = get_list(func, 4LL);
    }
    proto = (long long)"long long ";
    proto = string_concat(proto, get_fn_c_name(func));
    proto = string_concat(proto, (long long)"(");
    p_i = 0LL;
    while (p_i < p_len) {
    proto = string_concat(proto, (long long)"long long");
    if (p_i < (p_len - 1LL)) {
    proto = string_concat(proto, (long long)", ");
    }
    p_i = (p_i + 1LL);
    }
    proto = string_concat(proto, (long long)");\n");
    ok = emit(state, proto);
    if (is_async == 1LL) {
    proto2 = (long long)"long long ";
    proto2 = string_concat(proto2, get_fn_c_name(func));
    proto2 = string_concat(proto2, (long long)"_step(void* r);\n");
    ok = emit(state, proto2);
    }
    idx = (idx + 1LL);
    }
    ok = emit(state, (long long)"\n");
    spawn_list = collect_all_spawns(program);
    dummy = set_codegen_spawn_list(state, spawn_list);
    spawn_len = length_list(spawn_list);
    ok = emit(state, (long long)"\n/* Thread Spawn Wrappers */\n");
    idx = 0LL;
    while (idx < spawn_len) {
    spawn_node = get_list(spawn_list, idx);
    func_name = get_list(spawn_node, 1LL);
    args = get_list(spawn_node, 2LL);
    args_len = length_list(args);
    struct_decl = (long long)"typedef struct {\n";
    j = 0LL;
    while (j < args_len) {
    struct_decl = string_concat(struct_decl, (long long)"    long long arg");
    struct_decl = string_concat(struct_decl, cg_int_to_str(j));
    struct_decl = string_concat(struct_decl, (long long)";\n");
    j = (j + 1LL);
    }
    if (args_len == 0LL) {
    struct_decl = string_concat(struct_decl, (long long)"    int dummy;\n");
    }
    struct_decl = string_concat(struct_decl, (long long)"} spawn_args_");
    struct_decl = string_concat(struct_decl, cg_int_to_str(idx));
    struct_decl = string_concat(struct_decl, (long long)";\n\n");
    ok = emit(state, struct_decl);
    wrap_fn = (long long)"void* spawn_wrapper_";
    wrap_fn = string_concat(wrap_fn, cg_int_to_str(idx));
    wrap_fn = string_concat(wrap_fn, (long long)"(void* r) {\n");
    wrap_fn = string_concat(wrap_fn, (long long)"    int stack_dummy;\n");
    wrap_fn = string_concat(wrap_fn, (long long)"    ep_gc_register_thread(&stack_dummy);\n");
    wrap_fn = string_concat(wrap_fn, (long long)"    spawn_args_");
    wrap_fn = string_concat(wrap_fn, cg_int_to_str(idx));
    wrap_fn = string_concat(wrap_fn, (long long)"* args = (spawn_args_");
    wrap_fn = string_concat(wrap_fn, cg_int_to_str(idx));
    wrap_fn = string_concat(wrap_fn, (long long)"*)r;\n");
    wrap_fn = string_concat(wrap_fn, (long long)"    ");
    c_name = func_name;
    if ((strcmp((char*)(long long)"main", (char*)func_name) == 0)) {
    c_name = (long long)"_main";
    }
    wrap_fn = string_concat(wrap_fn, c_name);
    wrap_fn = string_concat(wrap_fn, (long long)"(");
    j = 0LL;
    while (j < args_len) {
    wrap_fn = string_concat(wrap_fn, (long long)"args->arg");
    wrap_fn = string_concat(wrap_fn, cg_int_to_str(j));
    if (j < (args_len - 1LL)) {
    wrap_fn = string_concat(wrap_fn, (long long)", ");
    }
    j = (j + 1LL);
    }
    wrap_fn = string_concat(wrap_fn, (long long)");\n");
    wrap_fn = string_concat(wrap_fn, (long long)"    free(args);\n");
    wrap_fn = string_concat(wrap_fn, (long long)"    ep_gc_unregister_thread();\n");
    wrap_fn = string_concat(wrap_fn, (long long)"    return NULL;\n");
    wrap_fn = string_concat(wrap_fn, (long long)"}\n\n");
    ok = emit(state, wrap_fn);
    idx = (idx + 1LL);
    }
    ok = emit(state, (long long)"\n");
    out_lines = get_list(state, 0LL);
    closure_slot = length_list(out_lines);
    ok = emit(state, (long long)"\n/* EP_CLOSURE_BODIES */\n");
    dummy = set_codegen_spawn_index(state, 0LL);
    if (prog_len > 6LL) {
    method_defs = get_list(program, 6LL);
    md_len = length_list(method_defs);
    md_idx = 0LL;
    while (md_idx < md_len) {
    mdef = get_list(method_defs, md_idx);
    mname = get_list(mdef, 1LL);
    msname = get_list(mdef, 2LL);
    mparams = get_list(mdef, 3LL);
    mbody = get_list(mdef, 4LL);
    full_params = (create_list() + 0LL);
    self_param = (create_list() + 0LL);
    ok = append_list(self_param, (long long)"self");
    ok = append_list(self_param, 0LL);
    ok = append_list(full_params, self_param);
    mp_len = length_list(mparams);
    mp_idx = 0LL;
    while (mp_idx < mp_len) {
    mp = get_list(mparams, mp_idx);
    ok = append_list(full_params, mp);
    mp_idx = (mp_idx + 1LL);
    }
    method_func = (make_node_func(mname, full_params, mbody, 0LL) + 0LL);
    ok = gen_function(state, method_func);
    md_idx = (md_idx + 1LL);
    }
    }
    emitted_methods = create_list();
    if (prog_len > 8LL) {
    trait_impls = get_list(program, 8LL);
    ti_len = length_list(trait_impls);
    ti_idx = 0LL;
    while (ti_idx < ti_len) {
    timpl = get_list(trait_impls, ti_idx);
    imethods = get_list(timpl, 3LL);
    im_len = length_list(imethods);
    im_idx = 0LL;
    while (im_idx < im_len) {
    imeth = get_list(imethods, im_idx);
    imeth_name = get_list(imeth, 1LL);
    if (contains_string_val(emitted_methods, imeth_name) == 0LL) {
    ok = append_list(emitted_methods, imeth_name);
    tparams = get_list(imeth, 2LL);
    tbody = get_list(imeth, 3LL);
    tfull = (create_list() + 0LL);
    tself = (create_list() + 0LL);
    ok = append_list(tself, (long long)"self");
    ok = append_list(tself, 0LL);
    ok = append_list(tfull, tself);
    tp_len = length_list(tparams);
    tp_idx = 0LL;
    while (tp_idx < tp_len) {
    ok = append_list(tfull, get_list(tparams, tp_idx));
    tp_idx = (tp_idx + 1LL);
    }
    tfunc = (make_node_func(imeth_name, tfull, tbody, 0LL) + 0LL);
    ok = gen_function(state, tfunc);
    }
    im_idx = (im_idx + 1LL);
    }
    ti_idx = (ti_idx + 1LL);
    }
    }
    idx = 0LL;
    while (idx < len) {
    func = get_list(funcs, idx);
    if (is_builtin_c_func(state, get_list(func, 1LL)) == 0LL) {
    ok = gen_function(state, func);
    }
    idx = (idx + 1LL);
    }
    if (is_test_mode == 1LL) {
    ok = emit(state, get_c_test_main_source(program));
    } else {
    ok = emit(state, get_c_main_source());
    }
    lines = get_list(state, 0LL);
    cbodies = get_list(state, 11LL);
    if (length_list(cbodies) > 0LL) {
    marker_line = get_list(lines, closure_slot);
    spliced = string_concat(marker_line, join_strings(cbodies));
    oksp = set_list(lines, closure_slot, spliced);
    }
    c_code = join_strings(lines);
    ret_val = c_code;
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(104);
    return ret_val;
}

long long ep_rt_core_0() {
    long long lines = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&lines);
    ep_gc_maybe_collect();

    lines = create_list();
    ok = append_list(lines, (long long)"#include <stdio.h>\n");
    ok = append_list(lines, (long long)"#include <stdlib.h>\n");
    ok = append_list(lines, (long long)"#include <stdint.h>\n");
    ok = append_list(lines, (long long)"#include <string.h>\n");
    ok = append_list(lines, (long long)"#ifdef __wasm__\n");
    ok = append_list(lines, (long long)"#define _SETJMP_H\n");
    ok = append_list(lines, (long long)"typedef int jmp_buf[1];\n");
    ok = append_list(lines, (long long)"#define setjmp(buf) (0)\n");
    ok = append_list(lines, (long long)"#define longjmp(buf, val) abort()\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"// Mock pthreads for single-threaded WASM\n");
    ok = append_list(lines, (long long)"typedef struct { int lock_state; } pthread_mutex_t;\n");
    ok = append_list(lines, (long long)"typedef struct { int cond_state; } pthread_cond_t;\n");
    ok = append_list(lines, (long long)"typedef struct { int rw_state; } pthread_rwlock_t;\n");
    ok = append_list(lines, (long long)"typedef int pthread_t;\n");
    ok = append_list(lines, (long long)"typedef int pthread_attr_t;\n");
    ok = append_list(lines, (long long)"#define PTHREAD_MUTEX_INITIALIZER {0}\n");
    ok = append_list(lines, (long long)"#define PTHREAD_COND_INITIALIZER {0}\n");
    ok = append_list(lines, (long long)"#define PTHREAD_RWLOCK_INITIALIZER {0}\n");
    ok = append_list(lines, (long long)"#define pthread_mutex_init(m, a) ((void)(a), (m)->lock_state = 0, 0)\n");
    ok = append_list(lines, (long long)"#define pthread_mutex_lock(m) ((m)->lock_state = 1, 0)\n");
    ok = append_list(lines, (long long)"#define pthread_mutex_unlock(m) ((m)->lock_state = 0, 0)\n");
    ok = append_list(lines, (long long)"#define pthread_mutex_trylock(m) ((m)->lock_state == 0 ? ((m)->lock_state = 1, 0) : 1)\n");
    ok = append_list(lines, (long long)"#define pthread_mutex_destroy(m) ((void)(m), 0)\n");
    ok = append_list(lines, (long long)"#define pthread_cond_init(c, a) ((void)(a), (c)->cond_state = 0, 0)\n");
    ok = append_list(lines, (long long)"#define pthread_cond_wait(c, m) ((void)(c), (void)(m), 0)\n");
    ok = append_list(lines, (long long)"#define pthread_cond_signal(c) ((void)(c), 0)\n");
    ok = append_list(lines, (long long)"#define pthread_cond_broadcast(c) ((void)(c), 0)\n");
    ok = append_list(lines, (long long)"#define pthread_cond_destroy(c) ((void)(c), 0)\n");
    ok = append_list(lines, (long long)"#define pthread_rwlock_init(r, a) ((void)(a), (r)->rw_state = 0, 0)\n");
    ok = append_list(lines, (long long)"#define pthread_rwlock_rdlock(r) ((r)->rw_state = 1, 0)\n");
    ok = append_list(lines, (long long)"#define pthread_rwlock_wrlock(r) ((r)->rw_state = 2, 0)\n");
    ok = append_list(lines, (long long)"#define pthread_rwlock_unlock(r) ((r)->rw_state = 0, 0)\n");
    ok = append_list(lines, (long long)"#define pthread_rwlock_destroy(r) ((void)(r), 0)\n");
    ok = append_list(lines, (long long)"#define pthread_create(t, a, f, arg) ((void)(t), (void)(a), (void)(f), (void)(arg), 0)\n");
    ok = append_list(lines, (long long)"#define pthread_join(t, r) ((void)(t), (void)(r), 0)\n");
    ok = append_list(lines, (long long)"#define pthread_detach(t) ((void)(t), 0)\n");
    ok = append_list(lines, (long long)"#else\n");
    ok = append_list(lines, (long long)"#include <setjmp.h>\n");
    ok = append_list(lines, (long long)"#include <limits.h>\n");
    ok = append_list(lines, (long long)"#endif\n");
    ok = append_list(lines, (long long)"#include <signal.h>\n");
    ok = append_list(lines, (long long)"#include <time.h>\n");
    ok = append_list(lines, (long long)"#ifndef _WIN32\n");
    ok = append_list(lines, (long long)"#include <unistd.h>\n");
    ok = append_list(lines, (long long)"#endif\n");
    ok = append_list(lines, (long long)"/* Terminal control for read_key/terminal_columns/terminal_rows: raw keyboard\n");
    ok = append_list(lines, (long long)"   input and window size, each with a native path per platform. */\n");
    ok = append_list(lines, (long long)"#if defined(_WIN32)\n");
    ok = append_list(lines, (long long)"#include <conio.h>\n");
    ok = append_list(lines, (long long)"#include <io.h>\n");
    ok = append_list(lines, (long long)"#elif !defined(__wasm__)\n");
    ok = append_list(lines, (long long)"#include <termios.h>\n");
    ok = append_list(lines, (long long)"#include <sys/ioctl.h>\n");
    ok = append_list(lines, (long long)"#endif\n");
    ok = append_list(lines, (long long)"#if defined(__APPLE__)\n");
    ok = append_list(lines, (long long)"#include <mach/mach.h>\n");
    ok = append_list(lines, (long long)"#endif\n");
    ok = append_list(lines, (long long)"#if defined(__linux__)\n");
    ok = append_list(lines, (long long)"#include <sys/random.h>\n");
    ok = append_list(lines, (long long)"#endif\n");
    ok = append_list(lines, (long long)"#include <fcntl.h>\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* Cryptographically secure random bytes. Uses the OS CSPRNG: arc4random on\n");
    ok = append_list(lines, (long long)"   Apple/BSD, getrandom(2) on Linux (falling back to /dev/urandom), and a\n");
    ok = append_list(lines, (long long)"   /dev/urandom read elsewhere. Only if all of those are unavailable does it\n");
    ok = append_list(lines, (long long)"   fall back to rand() — never on a supported platform. */\n");
    ok = append_list(lines, (long long)"static void ep_secure_random_bytes(unsigned char* buf, size_t n) {\n");
    ok = append_list(lines, (long long)"#if defined(__APPLE__) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__)\n");
    ok = append_list(lines, (long long)"    arc4random_buf(buf, n);\n");
    ok = append_list(lines, (long long)"#else\n");
    ok = append_list(lines, (long long)"    size_t got = 0;\n");
    ok = append_list(lines, (long long)"  #if defined(__linux__)\n");
    ok = append_list(lines, (long long)"    while (got < n) {\n");
    ok = append_list(lines, (long long)"        ssize_t r = getrandom(buf + got, n - got, 0);\n");
    ok = append_list(lines, (long long)"        if (r <= 0) break;\n");
    ok = append_list(lines, (long long)"        got += (size_t)r;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"  #endif\n");
    ok = append_list(lines, (long long)"    if (got < n) {\n");
    ok = append_list(lines, (long long)"        FILE* f = fopen(\"/dev/urandom\", \"rb\");\n");
    ok = append_list(lines, (long long)"        if (f) {\n");
    ok = append_list(lines, (long long)"            got += fread(buf + got, 1, n - got, f);\n");
    ok = append_list(lines, (long long)"            fclose(f);\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    while (got < n) {\n");
    ok = append_list(lines, (long long)"        buf[got++] = (unsigned char)(rand() & 0xFF);\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"#endif\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* Try/catch infrastructure */\n");
    ok = append_list(lines, (long long)"static jmp_buf ep_try_buf;\n");
    ok = append_list(lines, (long long)"static volatile int ep_try_active = 0;\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"static void ep_signal_handler(int sig) {\n");
    ok = append_list(lines, (long long)"    if (ep_try_active) {\n");
    ok = append_list(lines, (long long)"        ep_try_active = 0;\n");
    ok = append_list(lines, (long long)"        longjmp(ep_try_buf, sig);\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    /* Outside try: print error and exit */\n");
    ok = append_list(lines, (long long)"    const char* name = sig == SIGSEGV ? \"segmentation fault (null pointer or invalid memory access)\"\n");
    ok = append_list(lines, (long long)"                     : sig == SIGFPE  ? \"arithmetic error (division by zero)\"\n");
    ok = append_list(lines, (long long)"                     : sig == SIGABRT ? \"aborted\"\n");
    ok = append_list(lines, (long long)"                     : \"unknown signal\";\n");
    ok = append_list(lines, (long long)"    fprintf(stderr, \"\\nRuntime Error: %s (signal %d)\\n\", name, sig);\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"    /* Write to daemon/general log file if environment variable is set */\n");
    ok = append_list(lines, (long long)"    const char* daemon_log = getenv(\"ERNOS_DAEMON_LOG\");\n");
    ok = append_list(lines, (long long)"    if (!daemon_log || daemon_log[0] == '\\0') {\n");
    ok = append_list(lines, (long long)"        daemon_log = getenv(\"ERNOS_LOG_FILE\");\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    if (daemon_log && daemon_log[0] != '\\0') {\n");
    ok = append_list(lines, (long long)"        FILE* f = fopen(daemon_log, \"ab\");\n");
    ok = append_list(lines, (long long)"        if (f) {\n");
    ok = append_list(lines, (long long)"            time_t rawtime;\n");
    ok = append_list(lines, (long long)"            time(&rawtime);\n");
    ok = append_list(lines, (long long)"            struct tm * timeinfo = localtime(&rawtime);\n");
    ok = append_list(lines, (long long)"            char time_buf[80];\n");
    ok = append_list(lines, (long long)"            if (timeinfo) {\n");
    ok = append_list(lines, (long long)"                strftime(time_buf, sizeof(time_buf), \"%Y-%m-%d %H:%M:%S\", timeinfo);\n");
    ok = append_list(lines, (long long)"            } else {\n");
    ok = append_list(lines, (long long)"                snprintf(time_buf, sizeof(time_buf), \"%lld\", (long long)rawtime);\n");
    ok = append_list(lines, (long long)"            }\n");
    ok = append_list(lines, (long long)"            fprintf(f, \"[%s] FATAL: Runtime Error: %s (signal %d)\\n\", time_buf, name, sig);\n");
    ok = append_list(lines, (long long)"            fclose(f);\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"    _exit(128 + sig);\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"#ifdef _MSC_VER\n");
    ok = append_list(lines, (long long)"static void ep_install_signal_handlers(void);\n");
    ok = append_list(lines, (long long)"#pragma section(\".CRT$XCU\", read)\n");
    ok = append_list(lines, (long long)"__declspec(allocate(\".CRT$XCU\")) static void (*_ep_init_signals)(void) = ep_install_signal_handlers;\n");
    ok = append_list(lines, (long long)"static void ep_install_signal_handlers(void) {\n");
    ok = append_list(lines, (long long)"#else\n");
    ok = append_list(lines, (long long)"__attribute__((constructor))\n");
    ok = append_list(lines, (long long)"static void ep_install_signal_handlers(void) {\n");
    ok = append_list(lines, (long long)"#endif\n");
    ok = append_list(lines, (long long)"    signal(SIGFPE, ep_signal_handler);\n");
    ok = append_list(lines, (long long)"    signal(SIGSEGV, ep_signal_handler);\n");
    ok = append_list(lines, (long long)"    signal(SIGABRT, ep_signal_handler);\n");
    ok = append_list(lines, (long long)"#ifdef _WIN32\n");
    ok = append_list(lines, (long long)"    { WSADATA wsa; WSAStartup(MAKEWORD(2,2), &wsa); }\n");
    ok = append_list(lines, (long long)"#endif\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ret_val = join_strings(lines);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(1);
    return ret_val;
}

long long ep_rt_core_1() {
    long long lines = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&lines);
    ep_gc_maybe_collect();

    lines = create_list();
    ok = append_list(lines, (long long)"#if defined(__wasm__)\n");
    ok = append_list(lines, (long long)"  typedef int ep_thread_t;\n");
    ok = append_list(lines, (long long)"  typedef int ep_mutex_t;\n");
    ok = append_list(lines, (long long)"  typedef int ep_cond_t;\n");
    ok = append_list(lines, (long long)"  #define ep_mutex_init(m) (void)(0)\n");
    ok = append_list(lines, (long long)"  #define ep_mutex_lock(m) (void)(0)\n");
    ok = append_list(lines, (long long)"  #define ep_mutex_unlock(m) (void)(0)\n");
    ok = append_list(lines, (long long)"  #define ep_cond_init(c) (void)(0)\n");
    ok = append_list(lines, (long long)"  #define ep_cond_wait(c, m) (void)(0)\n");
    ok = append_list(lines, (long long)"  #define ep_cond_signal(c) (void)(0)\n");
    ok = append_list(lines, (long long)"#elif defined(_WIN32)\n");
    ok = append_list(lines, (long long)"  #include <winsock2.h>\n");
    ok = append_list(lines, (long long)"  #include <ws2tcpip.h>\n");
    ok = append_list(lines, (long long)"  #include <windows.h>\n");
    ok = append_list(lines, (long long)"  #pragma comment(lib, \"ws2_32.lib\")\n");
    ok = append_list(lines, (long long)"  typedef HANDLE ep_thread_t;\n");
    ok = append_list(lines, (long long)"  typedef CRITICAL_SECTION ep_mutex_t;\n");
    ok = append_list(lines, (long long)"  typedef CONDITION_VARIABLE ep_cond_t;\n");
    ok = append_list(lines, (long long)"  #define ep_mutex_init(m) InitializeCriticalSection(m)\n");
    ok = append_list(lines, (long long)"  #define ep_mutex_lock(m) EnterCriticalSection(m)\n");
    ok = append_list(lines, (long long)"  #define ep_mutex_unlock(m) LeaveCriticalSection(m)\n");
    ok = append_list(lines, (long long)"  #define ep_cond_init(c) InitializeConditionVariable(c)\n");
    ok = append_list(lines, (long long)"  #define ep_cond_wait(c, m) SleepConditionVariableCS(c, m, INFINITE)\n");
    ok = append_list(lines, (long long)"  #define ep_cond_signal(c) WakeConditionVariable(c)\n");
    ok = append_list(lines, (long long)"#else\n");
    ok = append_list(lines, (long long)"  #include <sys/socket.h>\n");
    ok = append_list(lines, (long long)"  #include <netinet/in.h>\n");
    ok = append_list(lines, (long long)"  #include <arpa/inet.h>\n");
    ok = append_list(lines, (long long)"  #include <unistd.h>\n");
    ok = append_list(lines, (long long)"  #include <netdb.h>\n");
    ok = append_list(lines, (long long)"  #include <fcntl.h>\n");
    ok = append_list(lines, (long long)"  #include <errno.h>\n");
    ok = append_list(lines, (long long)"  #include <sys/select.h>\n");
    ok = append_list(lines, (long long)"  #include <pthread.h>\n");
    ok = append_list(lines, (long long)"  typedef pthread_t ep_thread_t;\n");
    ok = append_list(lines, (long long)"  typedef pthread_mutex_t ep_mutex_t;\n");
    ok = append_list(lines, (long long)"  typedef pthread_cond_t ep_cond_t;\n");
    ok = append_list(lines, (long long)"  #define ep_mutex_init(m) pthread_mutex_init(m, NULL)\n");
    ok = append_list(lines, (long long)"  #define ep_mutex_lock(m) pthread_mutex_lock(m)\n");
    ok = append_list(lines, (long long)"  #define ep_mutex_unlock(m) pthread_mutex_unlock(m)\n");
    ok = append_list(lines, (long long)"  #define ep_cond_init(c) pthread_cond_init(c, NULL)\n");
    ok = append_list(lines, (long long)"  #define ep_cond_wait(c, m) pthread_cond_wait(c, m)\n");
    ok = append_list(lines, (long long)"  #define ep_cond_signal(c) pthread_cond_signal(c)\n");
    ok = append_list(lines, (long long)"#endif\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* ========== Ernos Mark-and-Sweep Garbage Collector ========== */\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"#include <setjmp.h>\n");
    ok = append_list(lines, (long long)"#if !defined(__wasm__) && !defined(_WIN32)\n");
    ok = append_list(lines, (long long)"#include <pthread.h>\n");
    ok = append_list(lines, (long long)"#endif\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"typedef enum {\n");
    ok = append_list(lines, (long long)"    EP_OBJ_LIST,\n");
    ok = append_list(lines, (long long)"    EP_OBJ_STRING,\n");
    ok = append_list(lines, (long long)"    EP_OBJ_STRUCT,\n");
    ok = append_list(lines, (long long)"    EP_OBJ_CLOSURE,\n");
    ok = append_list(lines, (long long)"    EP_OBJ_MAP\n");
    ok = append_list(lines, (long long)"} EpObjKind;\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"typedef struct EpGCObject {\n");
    ok = append_list(lines, (long long)"    EpObjKind kind;\n");
    ok = append_list(lines, (long long)"    int marked;\n");
    ok = append_list(lines, (long long)"    void* ptr;                /* actual allocation pointer */\n");
    ok = append_list(lines, (long long)"    long long size;           /* payload size for structs */\n");
    ok = append_list(lines, (long long)"    long long num_fields;     /* number of fields for structs (each is long long) */\n");
    ok = append_list(lines, (long long)"    int generation;           /* 0 = Nursery/young, 1 = Old */\n");
    ok = append_list(lines, (long long)"    struct EpGCObject* next;  /* intrusive linked list */\n");
    ok = append_list(lines, (long long)"} EpGCObject;\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_time_now_ms(void);\n");
    ok = append_list(lines, (long long)"long long ep_sleep_ms(long long ms);\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"typedef struct EpTask EpTask;\n");
    ok = append_list(lines, (long long)"typedef struct {\n");
    ok = append_list(lines, (long long)"    long long chan;\n");
    ok = append_list(lines, (long long)"    int completed;\n");
    ok = append_list(lines, (long long)"    long long value;\n");
    ok = append_list(lines, (long long)"    EpTask* waiting_task;\n");
    ok = append_list(lines, (long long)"} EpFuture;\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"static long long ep_await_future(EpFuture* fut);\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"struct EpTask {\n");
    ok = append_list(lines, (long long)"    long long (*step)(void*); /* pointer to step function */\n");
    ok = append_list(lines, (long long)"    void* args;               /* pointer to step state arguments */\n");
    ok = append_list(lines, (long long)"    long long args_size_bytes; /* size of args struct for GC tracing */\n");
    ok = append_list(lines, (long long)"    EpTask* next;             /* run-queue link pointer */\n");
    ok = append_list(lines, (long long)"    EpFuture* fut;            /* future associated with this task */\n");
    ok = append_list(lines, (long long)"    int state;                /* coroutine execution state */\n");
    ok = append_list(lines, (long long)"    int is_cancelled;         /* cancellation flag for structured concurrency */\n");
    ok = append_list(lines, (long long)"    struct EpTask* parent;    /* parent task for structured concurrency cancellation */\n");
    ok = append_list(lines, (long long)"};\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* Event Loop Scheduler Globals & Functions */\n");
    ok = append_list(lines, (long long)"static EpTask* ep_run_queue_head = NULL;\n");
    ok = append_list(lines, (long long)"static EpTask* ep_run_queue_tail = NULL;\n");
    ok = append_list(lines, (long long)"static EpTask* ep_current_task = NULL;\n");
    ok = append_list(lines, (long long)"static int ep_event_loop_fd = -1; /* epoll or kqueue fd */\n");
    ok = append_list(lines, (long long)"static int ep_active_io_sources = 0;\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"static void ep_task_enqueue(EpTask* task) {\n");
    ok = append_list(lines, (long long)"    if (!task) return;\n");
    ok = append_list(lines, (long long)"    task->next = NULL;\n");
    ok = append_list(lines, (long long)"    if (ep_run_queue_tail) {\n");
    ok = append_list(lines, (long long)"        ep_run_queue_tail->next = task;\n");
    ok = append_list(lines, (long long)"        ep_run_queue_tail = task;\n");
    ok = append_list(lines, (long long)"    } else {\n");
    ok = append_list(lines, (long long)"        ep_run_queue_head = ep_run_queue_tail = task;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"static EpTask* ep_task_dequeue(void) {\n");
    ok = append_list(lines, (long long)"    if (!ep_run_queue_head) return NULL;\n");
    ok = append_list(lines, (long long)"    EpTask* task = ep_run_queue_head;\n");
    ok = append_list(lines, (long long)"    ep_run_queue_head = ep_run_queue_head->next;\n");
    ok = append_list(lines, (long long)"    if (!ep_run_queue_head) ep_run_queue_tail = NULL;\n");
    ok = append_list(lines, (long long)"    return task;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"#ifndef __wasm__\n");
    ok = append_list(lines, (long long)"#ifdef __APPLE__\n");
    ok = append_list(lines, (long long)"#include <sys/event.h>\n");
    ok = append_list(lines, (long long)"#else\n");
    ok = append_list(lines, (long long)"#include <sys/epoll.h>\n");
    ok = append_list(lines, (long long)"#endif\n");
    ok = append_list(lines, (long long)"#endif\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"static void ep_async_loop_init(void) {\n");
    ok = append_list(lines, (long long)"    if (ep_event_loop_fd != -1) return;\n");
    ok = append_list(lines, (long long)"#ifdef __wasm__\n");
    ok = append_list(lines, (long long)"    ep_event_loop_fd = 999;\n");
    ok = append_list(lines, (long long)"#elif defined(__APPLE__)\n");
    ok = append_list(lines, (long long)"    ep_event_loop_fd = kqueue();\n");
    ok = append_list(lines, (long long)"#else\n");
    ok = append_list(lines, (long long)"    ep_event_loop_fd = epoll_create1(0);\n");
    ok = append_list(lines, (long long)"#endif\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"typedef struct EpTimer {\n");
    ok = append_list(lines, (long long)"    long long expiry_ms;\n");
    ok = append_list(lines, (long long)"    EpTask* task;\n");
    ok = append_list(lines, (long long)"    struct EpTimer* next;\n");
    ok = append_list(lines, (long long)"} EpTimer;\n");
    ok = append_list(lines, (long long)"static EpTimer* ep_timers_head = NULL;\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"static void ep_async_register_timer(long long timeout_ms, EpTask* task) {\n");
    ok = append_list(lines, (long long)"    long long expiry = ep_time_now_ms() + timeout_ms;\n");
    ok = append_list(lines, (long long)"    EpTimer* timer = (EpTimer*)malloc(sizeof(EpTimer));\n");
    ok = append_list(lines, (long long)"    timer->expiry_ms = expiry;\n");
    ret_val = join_strings(lines);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(1);
    return ret_val;
}

long long ep_rt_core_2() {
    long long lines = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&lines);
    ep_gc_maybe_collect();

    lines = create_list();
    ok = append_list(lines, (long long)"    timer->task = task;\n");
    ok = append_list(lines, (long long)"    timer->next = NULL;\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"    /* Insert sorted */\n");
    ok = append_list(lines, (long long)"    if (!ep_timers_head || expiry < ep_timers_head->expiry_ms) {\n");
    ok = append_list(lines, (long long)"        timer->next = ep_timers_head;\n");
    ok = append_list(lines, (long long)"        ep_timers_head = timer;\n");
    ok = append_list(lines, (long long)"    } else {\n");
    ok = append_list(lines, (long long)"        EpTimer* cur = ep_timers_head;\n");
    ok = append_list(lines, (long long)"        while (cur->next && cur->next->expiry_ms <= expiry) {\n");
    ok = append_list(lines, (long long)"            cur = cur->next;\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"        timer->next = cur->next;\n");
    ok = append_list(lines, (long long)"        cur->next = timer;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"static long long ep_get_next_timer_timeout(void) {\n");
    ok = append_list(lines, (long long)"    if (!ep_timers_head) return -1; /* block indefinitely */\n");
    ok = append_list(lines, (long long)"    long long now = ep_time_now_ms();\n");
    ok = append_list(lines, (long long)"    long long diff = ep_timers_head->expiry_ms - now;\n");
    ok = append_list(lines, (long long)"    return diff < 0 ? 0 : diff;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"static void ep_process_expired_timers(void) {\n");
    ok = append_list(lines, (long long)"    long long now = ep_time_now_ms();\n");
    ok = append_list(lines, (long long)"    while (ep_timers_head && ep_timers_head->expiry_ms <= now) {\n");
    ok = append_list(lines, (long long)"        EpTimer* expired = ep_timers_head;\n");
    ok = append_list(lines, (long long)"        ep_timers_head = ep_timers_head->next;\n");
    ok = append_list(lines, (long long)"        ep_task_enqueue(expired->task);\n");
    ok = append_list(lines, (long long)"        free(expired);\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"static void ep_async_register_read(int fd, EpTask* task) {\n");
    ok = append_list(lines, (long long)"#ifdef __wasm__\n");
    ok = append_list(lines, (long long)"    (void)fd;\n");
    ok = append_list(lines, (long long)"    (void)task;\n");
    ok = append_list(lines, (long long)"#else\n");
    ok = append_list(lines, (long long)"    ep_async_loop_init();\n");
    ok = append_list(lines, (long long)"    ep_active_io_sources++;\n");
    ok = append_list(lines, (long long)"#ifdef __APPLE__\n");
    ok = append_list(lines, (long long)"    struct kevent ev;\n");
    ok = append_list(lines, (long long)"    EV_SET(&ev, fd, EVFILT_READ, EV_ADD | EV_ONESHOT, 0, 0, task);\n");
    ok = append_list(lines, (long long)"    kevent(ep_event_loop_fd, &ev, 1, NULL, 0, NULL);\n");
    ok = append_list(lines, (long long)"#else\n");
    ok = append_list(lines, (long long)"    struct epoll_event ev;\n");
    ok = append_list(lines, (long long)"    ev.events = EPOLLIN | EPOLLONESHOT;\n");
    ok = append_list(lines, (long long)"    ev.data.ptr = task;\n");
    ok = append_list(lines, (long long)"    if (epoll_ctl(ep_event_loop_fd, EPOLL_CTL_ADD, fd, &ev) < 0) {\n");
    ok = append_list(lines, (long long)"        epoll_ctl(ep_event_loop_fd, EPOLL_CTL_MOD, fd, &ev);\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"#endif\n");
    ok = append_list(lines, (long long)"#endif\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"static void ep_async_wait_step(long long timeout) {\n");
    ok = append_list(lines, (long long)"#ifdef __wasm__\n");
    ok = append_list(lines, (long long)"    if (timeout > 0) {\n");
    ok = append_list(lines, (long long)"        ep_sleep_ms(timeout);\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"#else\n");
    ok = append_list(lines, (long long)"#ifdef __APPLE__\n");
    ok = append_list(lines, (long long)"    struct kevent events[16];\n");
    ok = append_list(lines, (long long)"    struct timespec ts;\n");
    ok = append_list(lines, (long long)"    struct timespec* p_ts = NULL;\n");
    ok = append_list(lines, (long long)"    if (timeout >= 0) {\n");
    ok = append_list(lines, (long long)"        ts.tv_sec = timeout / 1000;\n");
    ok = append_list(lines, (long long)"        ts.tv_nsec = (timeout % 1000) * 1000000;\n");
    ok = append_list(lines, (long long)"        p_ts = &ts;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    int n = kevent(ep_event_loop_fd, NULL, 0, events, 16, p_ts);\n");
    ok = append_list(lines, (long long)"    for (int i = 0; i < n; i++) {\n");
    ok = append_list(lines, (long long)"        EpTask* t = (EpTask*)events[i].udata;\n");
    ok = append_list(lines, (long long)"        ep_task_enqueue(t);\n");
    ok = append_list(lines, (long long)"        ep_active_io_sources--;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"#else\n");
    ok = append_list(lines, (long long)"    struct epoll_event events[16];\n");
    ok = append_list(lines, (long long)"    int n = epoll_wait(ep_event_loop_fd, events, 16, (int)timeout);\n");
    ok = append_list(lines, (long long)"    for (int i = 0; i < n; i++) {\n");
    ok = append_list(lines, (long long)"        EpTask* t = (EpTask*)events[i].data.ptr;\n");
    ok = append_list(lines, (long long)"        ep_task_enqueue(t);\n");
    ok = append_list(lines, (long long)"        ep_active_io_sources--;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"#endif\n");
    ok = append_list(lines, (long long)"#endif\n");
    ok = append_list(lines, (long long)"    ep_process_expired_timers();\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"static void ep_async_loop_run(void) {\n");
    ok = append_list(lines, (long long)"    ep_async_loop_init();\n");
    ok = append_list(lines, (long long)"    while (ep_run_queue_head || ep_timers_head || ep_active_io_sources > 0) {\n");
    ok = append_list(lines, (long long)"        /* 1. Run all runnable tasks */\n");
    ok = append_list(lines, (long long)"        while (ep_run_queue_head) {\n");
    ok = append_list(lines, (long long)"            EpTask* task = ep_task_dequeue();\n");
    ok = append_list(lines, (long long)"            if (task->is_cancelled) {\n");
    ok = append_list(lines, (long long)"                if (task->fut) {\n");
    ok = append_list(lines, (long long)"                    task->fut->completed = 1;\n");
    ok = append_list(lines, (long long)"                    task->fut->value = -1;\n");
    ok = append_list(lines, (long long)"                }\n");
    ok = append_list(lines, (long long)"                free(task->args);\n");
    ok = append_list(lines, (long long)"                free(task);\n");
    ok = append_list(lines, (long long)"                continue;\n");
    ok = append_list(lines, (long long)"            }\n");
    ok = append_list(lines, (long long)"            ep_current_task = task;\n");
    ok = append_list(lines, (long long)"            long long res = task->step(task->args);\n");
    ok = append_list(lines, (long long)"            ep_current_task = NULL;\n");
    ok = append_list(lines, (long long)"            if (res != -999999) {\n");
    ok = append_list(lines, (long long)"                if (task->fut) {\n");
    ok = append_list(lines, (long long)"                    task->fut->value = res;\n");
    ok = append_list(lines, (long long)"                    task->fut->completed = 1;\n");
    ok = append_list(lines, (long long)"                    if (task->fut->waiting_task) {\n");
    ok = append_list(lines, (long long)"                        ep_task_enqueue(task->fut->waiting_task);\n");
    ok = append_list(lines, (long long)"                        task->fut->waiting_task = NULL;\n");
    ok = append_list(lines, (long long)"                    }\n");
    ok = append_list(lines, (long long)"                }\n");
    ok = append_list(lines, (long long)"                free(task->args);\n");
    ok = append_list(lines, (long long)"                free(task);\n");
    ok = append_list(lines, (long long)"            }\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"        /* 2. If no tasks runnable, wait for I/O / timers */\n");
    ok = append_list(lines, (long long)"        if (!ep_run_queue_head) {\n");
    ok = append_list(lines, (long long)"            long long timeout = ep_get_next_timer_timeout();\n");
    ok = append_list(lines, (long long)"            if (timeout == -1 && !ep_timers_head && ep_active_io_sources == 0) {\n");
    ok = append_list(lines, (long long)"                break;\n");
    ok = append_list(lines, (long long)"            }\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"            if (ep_event_loop_fd == -1) {\n");
    ok = append_list(lines, (long long)"                if (timeout > 0) {\n");
    ok = append_list(lines, (long long)"                    ep_sleep_ms(timeout);\n");
    ok = append_list(lines, (long long)"                }\n");
    ok = append_list(lines, (long long)"                ep_process_expired_timers();\n");
    ok = append_list(lines, (long long)"                continue;\n");
    ok = append_list(lines, (long long)"            }\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"            ep_async_wait_step(timeout);\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"static long long ep_await_future(EpFuture* fut) {\n");
    ok = append_list(lines, (long long)"    if (!fut) return 0;\n");
    ok = append_list(lines, (long long)"    while (!fut->completed) {\n");
    ok = append_list(lines, (long long)"        if (ep_run_queue_head) {\n");
    ok = append_list(lines, (long long)"            EpTask* task = ep_task_dequeue();\n");
    ok = append_list(lines, (long long)"            if (task) {\n");
    ok = append_list(lines, (long long)"                if (task->is_cancelled) {\n");
    ok = append_list(lines, (long long)"                    if (task->fut) {\n");
    ret_val = join_strings(lines);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(1);
    return ret_val;
}

long long ep_rt_core_3() {
    long long lines = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&lines);
    ep_gc_maybe_collect();

    lines = create_list();
    ok = append_list(lines, (long long)"                        task->fut->completed = 1;\n");
    ok = append_list(lines, (long long)"                        task->fut->value = -1;\n");
    ok = append_list(lines, (long long)"                    }\n");
    ok = append_list(lines, (long long)"                    free(task->args);\n");
    ok = append_list(lines, (long long)"                    free(task);\n");
    ok = append_list(lines, (long long)"                } else {\n");
    ok = append_list(lines, (long long)"                    EpTask* saved_current = ep_current_task;\n");
    ok = append_list(lines, (long long)"                    ep_current_task = task;\n");
    ok = append_list(lines, (long long)"                    long long res = task->step(task->args);\n");
    ok = append_list(lines, (long long)"                    ep_current_task = saved_current;\n");
    ok = append_list(lines, (long long)"                    if (res != -999999) {\n");
    ok = append_list(lines, (long long)"                        if (task->fut) {\n");
    ok = append_list(lines, (long long)"                            task->fut->value = res;\n");
    ok = append_list(lines, (long long)"                            task->fut->completed = 1;\n");
    ok = append_list(lines, (long long)"                            if (task->fut->waiting_task) {\n");
    ok = append_list(lines, (long long)"                                ep_task_enqueue(task->fut->waiting_task);\n");
    ok = append_list(lines, (long long)"                                task->fut->waiting_task = NULL;\n");
    ok = append_list(lines, (long long)"                            }\n");
    ok = append_list(lines, (long long)"                        }\n");
    ok = append_list(lines, (long long)"                        free(task->args);\n");
    ok = append_list(lines, (long long)"                        free(task);\n");
    ok = append_list(lines, (long long)"                    }\n");
    ok = append_list(lines, (long long)"                }\n");
    ok = append_list(lines, (long long)"            }\n");
    ok = append_list(lines, (long long)"        } else {\n");
    ok = append_list(lines, (long long)"            long long timeout = ep_get_next_timer_timeout();\n");
    ok = append_list(lines, (long long)"            if (timeout == -1 && !ep_timers_head && ep_active_io_sources == 0) {\n");
    ok = append_list(lines, (long long)"                fprintf(stderr, \"Deadlock detected: awaiting incomplete future with no active tasks or timers.\\n\");\n");
    ok = append_list(lines, (long long)"                exit(1);\n");
    ok = append_list(lines, (long long)"            }\n");
    ok = append_list(lines, (long long)"            if (ep_event_loop_fd == -1) {\n");
    ok = append_list(lines, (long long)"                if (timeout > 0) {\n");
    ok = append_list(lines, (long long)"                    ep_sleep_ms(timeout);\n");
    ok = append_list(lines, (long long)"                }\n");
    ok = append_list(lines, (long long)"                ep_process_expired_timers();\n");
    ok = append_list(lines, (long long)"            } else {\n");
    ok = append_list(lines, (long long)"                ep_async_wait_step(timeout);\n");
    ok = append_list(lines, (long long)"            }\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    return fut->value;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"static EpGCObject* ep_gc_register(void* ptr, EpObjKind kind);\n");
    ok = append_list(lines, (long long)"long long create_list(void);\n");
    ok = append_list(lines, (long long)"long long append_list(long long list_ptr, long long value);\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"typedef struct {\n");
    ok = append_list(lines, (long long)"    EpFuture* futures[128];\n");
    ok = append_list(lines, (long long)"    int count;\n");
    ok = append_list(lines, (long long)"    int has_error;\n");
    ok = append_list(lines, (long long)"} EpTaskGroup;\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"typedef struct {\n");
    ok = append_list(lines, (long long)"    EpFuture* fut;\n");
    ok = append_list(lines, (long long)"    int timer_fired;\n");
    ok = append_list(lines, (long long)"} EpTimeoutArgs;\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"static EpTask* ep_find_task_by_future(EpFuture* fut) {\n");
    ok = append_list(lines, (long long)"    if (!fut) return NULL;\n");
    ok = append_list(lines, (long long)"    EpTask* cur = ep_run_queue_head;\n");
    ok = append_list(lines, (long long)"    while (cur) {\n");
    ok = append_list(lines, (long long)"        if (cur->fut == fut) return cur;\n");
    ok = append_list(lines, (long long)"        cur = cur->next;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    EpTimer* timer = ep_timers_head;\n");
    ok = append_list(lines, (long long)"    while (timer) {\n");
    ok = append_list(lines, (long long)"        if (timer->task && timer->task->fut == fut) return timer->task;\n");
    ok = append_list(lines, (long long)"        timer = timer->next;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    return NULL;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"static void ep_cancel_task(EpTask* task) {\n");
    ok = append_list(lines, (long long)"    if (!task) return;\n");
    ok = append_list(lines, (long long)"    task->is_cancelled = 1;\n");
    ok = append_list(lines, (long long)"    if (task->fut) {\n");
    ok = append_list(lines, (long long)"        task->fut->completed = 1;\n");
    ok = append_list(lines, (long long)"        task->fut->value = -1;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    // Cancel children in run queue\n");
    ok = append_list(lines, (long long)"    EpTask* cur = ep_run_queue_head;\n");
    ok = append_list(lines, (long long)"    while (cur) {\n");
    ok = append_list(lines, (long long)"        if (cur->parent == task) {\n");
    ok = append_list(lines, (long long)"            ep_cancel_task(cur);\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"        cur = cur->next;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    // Cancel children in timers queue\n");
    ok = append_list(lines, (long long)"    EpTimer* timer = ep_timers_head;\n");
    ok = append_list(lines, (long long)"    while (timer) {\n");
    ok = append_list(lines, (long long)"        if (timer->task && timer->task->parent == task) {\n");
    ok = append_list(lines, (long long)"            ep_cancel_task(timer->task);\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"        timer = timer->next;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"static long long create_task_group(void) {\n");
    ok = append_list(lines, (long long)"    EpTaskGroup* tg = (EpTaskGroup*)calloc(1, sizeof(EpTaskGroup));\n");
    ok = append_list(lines, (long long)"    tg->count = 0;\n");
    ok = append_list(lines, (long long)"    tg->has_error = 0;\n");
    ok = append_list(lines, (long long)"    { EpGCObject* _go = ep_gc_register(tg, EP_OBJ_STRUCT); if(_go) _go->num_fields = 0; }\n");
    ok = append_list(lines, (long long)"    return (long long)tg;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"static long long add_task_group(long long group_ptr, long long fut_ptr) {\n");
    ok = append_list(lines, (long long)"    EpTaskGroup* tg = (EpTaskGroup*)group_ptr;\n");
    ok = append_list(lines, (long long)"    EpFuture* fut = (EpFuture*)fut_ptr;\n");
    ok = append_list(lines, (long long)"    if (!tg || !fut) return 0;\n");
    ok = append_list(lines, (long long)"    if (tg->count < 128) {\n");
    ok = append_list(lines, (long long)"        tg->futures[tg->count++] = fut;\n");
    ok = append_list(lines, (long long)"        // Associate the task's parent with the current task so it's cancellation-linked\n");
    ok = append_list(lines, (long long)"        EpTask* task = ep_find_task_by_future(fut);\n");
    ok = append_list(lines, (long long)"        if (task) {\n");
    ok = append_list(lines, (long long)"            task->parent = ep_current_task;\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    return 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"static long long wait_task_group(long long group_ptr) {\n");
    ok = append_list(lines, (long long)"    EpTaskGroup* tg = (EpTaskGroup*)group_ptr;\n");
    ok = append_list(lines, (long long)"    if (!tg) return 0;\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"    long long ep_wait_group_spin = 0;\n");
    ok = append_list(lines, (long long)"    int all_done = 0;\n");
    ok = append_list(lines, (long long)"    while (!all_done) {\n");
    ok = append_list(lines, (long long)"        all_done = 1;\n");
    ok = append_list(lines, (long long)"        for (int i = 0; i < tg->count; i++) {\n");
    ok = append_list(lines, (long long)"            EpFuture* fut = tg->futures[i];\n");
    ok = append_list(lines, (long long)"            if (!fut->completed) {\n");
    ok = append_list(lines, (long long)"                all_done = 0;\n");
    ok = append_list(lines, (long long)"                break;\n");
    ok = append_list(lines, (long long)"            }\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"        \n");
    ok = append_list(lines, (long long)"        if (all_done) break;\n");
    ok = append_list(lines, (long long)"        \n");
    ok = append_list(lines, (long long)"        if (ep_run_queue_head) {\n");
    ok = append_list(lines, (long long)"            EpTask* task = ep_task_dequeue();\n");
    ok = append_list(lines, (long long)"            if (task) {\n");
    ok = append_list(lines, (long long)"                if (task->is_cancelled) {\n");
    ok = append_list(lines, (long long)"                    if (task->fut) {\n");
    ok = append_list(lines, (long long)"                        task->fut->completed = 1;\n");
    ok = append_list(lines, (long long)"                        task->fut->value = -1;\n");
    ok = append_list(lines, (long long)"                    }\n");
    ok = append_list(lines, (long long)"                    free(task->args);\n");
    ok = append_list(lines, (long long)"                    free(task);\n");
    ok = append_list(lines, (long long)"                } else {\n");
    ret_val = join_strings(lines);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(1);
    return ret_val;
}

long long ep_rt_core_4() {
    long long lines = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&lines);
    ep_gc_maybe_collect();

    lines = create_list();
    ok = append_list(lines, (long long)"                    EpTask* saved_current = ep_current_task;\n");
    ok = append_list(lines, (long long)"                    ep_current_task = task;\n");
    ok = append_list(lines, (long long)"                    long long res = task->step(task->args);\n");
    ok = append_list(lines, (long long)"                    ep_current_task = saved_current;\n");
    ok = append_list(lines, (long long)"                    if (res != -999999) {\n");
    ok = append_list(lines, (long long)"                        if (task->fut) {\n");
    ok = append_list(lines, (long long)"                            task->fut->value = res;\n");
    ok = append_list(lines, (long long)"                            task->fut->completed = 1;\n");
    ok = append_list(lines, (long long)"                            if (task->fut->waiting_task) {\n");
    ok = append_list(lines, (long long)"                                ep_task_enqueue(task->fut->waiting_task);\n");
    ok = append_list(lines, (long long)"                                task->fut->waiting_task = NULL;\n");
    ok = append_list(lines, (long long)"                            }\n");
    ok = append_list(lines, (long long)"                        }\n");
    ok = append_list(lines, (long long)"                        free(task->args);\n");
    ok = append_list(lines, (long long)"                        free(task);\n");
    ok = append_list(lines, (long long)"                    }\n");
    ok = append_list(lines, (long long)"                }\n");
    ok = append_list(lines, (long long)"            }\n");
    ok = append_list(lines, (long long)"        } else {\n");
    ok = append_list(lines, (long long)"            long long timeout = ep_get_next_timer_timeout();\n");
    ok = append_list(lines, (long long)"            if (timeout == -1 && !ep_timers_head && ep_active_io_sources == 0) {\n");
    ok = append_list(lines, (long long)"                /* No coroutine tasks/timers/IO to drive. The futures may still be\n");
    ok = append_list(lines, (long long)"                   completed by detached worker THREADS (the self-hosted compiler\n");
    ok = append_list(lines, (long long)"                   emits thread-based async), so poll for their completion rather\n");
    ok = append_list(lines, (long long)"                   than declaring deadlock. Bounded so a genuinely stuck group\n");
    ok = append_list(lines, (long long)"                   still fails instead of hanging forever. */\n");
    ok = append_list(lines, (long long)"                ep_sleep_ms(1);\n");
    ok = append_list(lines, (long long)"                if (++ep_wait_group_spin > 60000) {\n");
    ok = append_list(lines, (long long)"                    fprintf(stderr, \"Deadlock detected: waiting on task group with no active tasks or timers.\\n\");\n");
    ok = append_list(lines, (long long)"                    exit(1);\n");
    ok = append_list(lines, (long long)"                }\n");
    ok = append_list(lines, (long long)"                continue;\n");
    ok = append_list(lines, (long long)"            }\n");
    ok = append_list(lines, (long long)"            ep_wait_group_spin = 0;\n");
    ok = append_list(lines, (long long)"            if (ep_event_loop_fd == -1) {\n");
    ok = append_list(lines, (long long)"                if (timeout > 0) {\n");
    ok = append_list(lines, (long long)"                    ep_sleep_ms(timeout);\n");
    ok = append_list(lines, (long long)"                }\n");
    ok = append_list(lines, (long long)"                ep_process_expired_timers();\n");
    ok = append_list(lines, (long long)"            } else {\n");
    ok = append_list(lines, (long long)"                ep_async_wait_step(timeout);\n");
    ok = append_list(lines, (long long)"            }\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"        \n");
    ok = append_list(lines, (long long)"        // Propagate cancellation/failure inside task group\n");
    ok = append_list(lines, (long long)"        for (int i = 0; i < tg->count; i++) {\n");
    ok = append_list(lines, (long long)"            EpFuture* fut = tg->futures[i];\n");
    ok = append_list(lines, (long long)"            if (fut->completed && fut->value == -1) {\n");
    ok = append_list(lines, (long long)"                tg->has_error = 1;\n");
    ok = append_list(lines, (long long)"                for (int j = 0; j < tg->count; j++) {\n");
    ok = append_list(lines, (long long)"                    EpFuture* other_fut = tg->futures[j];\n");
    ok = append_list(lines, (long long)"                    if (!other_fut->completed) {\n");
    ok = append_list(lines, (long long)"                        EpTask* other_task = ep_find_task_by_future(other_fut);\n");
    ok = append_list(lines, (long long)"                        if (other_task) {\n");
    ok = append_list(lines, (long long)"                            ep_cancel_task(other_task);\n");
    ok = append_list(lines, (long long)"                        } else {\n");
    ok = append_list(lines, (long long)"                            other_fut->completed = 1;\n");
    ok = append_list(lines, (long long)"                            other_fut->value = -1;\n");
    ok = append_list(lines, (long long)"                        }\n");
    ok = append_list(lines, (long long)"                    }\n");
    ok = append_list(lines, (long long)"                }\n");
    ok = append_list(lines, (long long)"            }\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    \n");
    ok = append_list(lines, (long long)"    long long list = create_list();\n");
    ok = append_list(lines, (long long)"    for (int i = 0; i < tg->count; i++) {\n");
    ok = append_list(lines, (long long)"        append_list(list, tg->futures[i]->value);\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    return list;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"static long long ep_timeout_timer_step(void* r) {\n");
    ok = append_list(lines, (long long)"    EpTimeoutArgs* args = (EpTimeoutArgs*)r;\n");
    ok = append_list(lines, (long long)"    if (args && args->fut && !args->fut->completed) {\n");
    ok = append_list(lines, (long long)"        args->timer_fired = 1;\n");
    ok = append_list(lines, (long long)"        EpTask* task = ep_find_task_by_future(args->fut);\n");
    ok = append_list(lines, (long long)"        if (task) {\n");
    ok = append_list(lines, (long long)"            ep_cancel_task(task);\n");
    ok = append_list(lines, (long long)"        } else {\n");
    ok = append_list(lines, (long long)"            args->fut->completed = 1;\n");
    ok = append_list(lines, (long long)"            args->fut->value = -1;\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    return 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"static long long async_timeout(long long timeout_ms, long long fut_ptr) {\n");
    ok = append_list(lines, (long long)"    EpFuture* fut = (EpFuture*)fut_ptr;\n");
    ok = append_list(lines, (long long)"    if (!fut) return -1;\n");
    ok = append_list(lines, (long long)"    if (fut->completed) return fut->value;\n");
    ok = append_list(lines, (long long)"    \n");
    ok = append_list(lines, (long long)"    EpTimeoutArgs* args = (EpTimeoutArgs*)malloc(sizeof(EpTimeoutArgs));\n");
    ok = append_list(lines, (long long)"    args->fut = fut;\n");
    ok = append_list(lines, (long long)"    args->timer_fired = 0;\n");
    ok = append_list(lines, (long long)"    \n");
    ok = append_list(lines, (long long)"    EpTask* timer_task = (EpTask*)malloc(sizeof(EpTask));\n");
    ok = append_list(lines, (long long)"    timer_task->step = ep_timeout_timer_step;\n");
    ok = append_list(lines, (long long)"    timer_task->args = args;\n");
    ok = append_list(lines, (long long)"    timer_task->args_size_bytes = sizeof(EpTimeoutArgs);\n");
    ok = append_list(lines, (long long)"    timer_task->fut = NULL;\n");
    ok = append_list(lines, (long long)"    timer_task->state = 0;\n");
    ok = append_list(lines, (long long)"    timer_task->is_cancelled = 0;\n");
    ok = append_list(lines, (long long)"    timer_task->parent = NULL;\n");
    ok = append_list(lines, (long long)"    \n");
    ok = append_list(lines, (long long)"    ep_async_register_timer(timeout_ms, timer_task);\n");
    ok = append_list(lines, (long long)"    \n");
    ok = append_list(lines, (long long)"    while (!fut->completed && !(args->timer_fired)) {\n");
    ok = append_list(lines, (long long)"        if (ep_run_queue_head) {\n");
    ok = append_list(lines, (long long)"            EpTask* task = ep_task_dequeue();\n");
    ok = append_list(lines, (long long)"            if (task) {\n");
    ok = append_list(lines, (long long)"                if (task->is_cancelled) {\n");
    ok = append_list(lines, (long long)"                    if (task->fut) {\n");
    ok = append_list(lines, (long long)"                        task->fut->completed = 1;\n");
    ok = append_list(lines, (long long)"                        task->fut->value = -1;\n");
    ok = append_list(lines, (long long)"                    }\n");
    ok = append_list(lines, (long long)"                    free(task->args);\n");
    ok = append_list(lines, (long long)"                    free(task);\n");
    ok = append_list(lines, (long long)"                } else {\n");
    ok = append_list(lines, (long long)"                    EpTask* saved_current = ep_current_task;\n");
    ok = append_list(lines, (long long)"                    ep_current_task = task;\n");
    ok = append_list(lines, (long long)"                    long long res = task->step(task->args);\n");
    ok = append_list(lines, (long long)"                    ep_current_task = saved_current;\n");
    ok = append_list(lines, (long long)"                    if (res != -999999) {\n");
    ok = append_list(lines, (long long)"                        if (task->fut) {\n");
    ok = append_list(lines, (long long)"                            task->fut->value = res;\n");
    ok = append_list(lines, (long long)"                            task->fut->completed = 1;\n");
    ok = append_list(lines, (long long)"                            if (task->fut->waiting_task) {\n");
    ok = append_list(lines, (long long)"                                ep_task_enqueue(task->fut->waiting_task);\n");
    ok = append_list(lines, (long long)"                                task->fut->waiting_task = NULL;\n");
    ok = append_list(lines, (long long)"                            }\n");
    ok = append_list(lines, (long long)"                        }\n");
    ok = append_list(lines, (long long)"                        free(task->args);\n");
    ok = append_list(lines, (long long)"                        free(task);\n");
    ok = append_list(lines, (long long)"                    }\n");
    ok = append_list(lines, (long long)"                }\n");
    ok = append_list(lines, (long long)"            }\n");
    ok = append_list(lines, (long long)"        } else {\n");
    ok = append_list(lines, (long long)"            long long timeout = ep_get_next_timer_timeout();\n");
    ok = append_list(lines, (long long)"            if (timeout == -1 && !ep_timers_head && ep_active_io_sources == 0) {\n");
    ok = append_list(lines, (long long)"                break;\n");
    ok = append_list(lines, (long long)"            }\n");
    ok = append_list(lines, (long long)"            if (ep_event_loop_fd == -1) {\n");
    ok = append_list(lines, (long long)"                if (timeout > 0) {\n");
    ok = append_list(lines, (long long)"                    ep_sleep_ms(timeout);\n");
    ok = append_list(lines, (long long)"                }\n");
    ok = append_list(lines, (long long)"                ep_process_expired_timers();\n");
    ok = append_list(lines, (long long)"            } else {\n");
    ok = append_list(lines, (long long)"                ep_async_wait_step(timeout);\n");
    ok = append_list(lines, (long long)"            }\n");
    ret_val = join_strings(lines);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(1);
    return ret_val;
}

long long ep_rt_core_5() {
    long long lines = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&lines);
    ep_gc_maybe_collect();

    lines = create_list();
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    \n");
    ok = append_list(lines, (long long)"    return fut->value;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* ── Awaitable async socket-readability ─────────────────────────────────────\n");
    ok = append_list(lines, (long long)"   `await async_wait_readable(fd)` suspends the calling async task until `fd` is\n");
    ok = append_list(lines, (long long)"   readable, letting the event loop run other tasks (e.g. another agent waiting on\n");
    ok = append_list(lines, (long long)"   its own LLM socket) meanwhile. Mirrors sleep_ms: build a future, register a\n");
    ok = append_list(lines, (long long)"   oneshot read-readiness task with the loop, return the future. When fd becomes\n");
    ok = append_list(lines, (long long)"   readable, ep_async_wait_step re-enqueues the task; its step completes the future\n");
    ok = append_list(lines, (long long)"   and wakes whoever awaited it. This is what lets I/O-bound agents run concurrently\n");
    ok = append_list(lines, (long long)"   on ONE thread — no OS threads, no shared-heap GC race. */\n");
    ok = append_list(lines, (long long)"typedef struct { EpFuture* fut; } EpReadReadyArgs;\n");
    ok = append_list(lines, (long long)"static long long ep_read_ready_step(void* r) {\n");
    ok = append_list(lines, (long long)"    EpReadReadyArgs* args = (EpReadReadyArgs*)r;\n");
    ok = append_list(lines, (long long)"    if (args && args->fut) {\n");
    ok = append_list(lines, (long long)"        args->fut->completed = 1;\n");
    ok = append_list(lines, (long long)"        args->fut->value = 1;\n");
    ok = append_list(lines, (long long)"        if (args->fut->waiting_task) {\n");
    ok = append_list(lines, (long long)"            ep_task_enqueue(args->fut->waiting_task);\n");
    ok = append_list(lines, (long long)"            args->fut->waiting_task = NULL;\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    return 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"long long async_wait_readable(long long fd) {\n");
    ok = append_list(lines, (long long)"    EpFuture* fut = (EpFuture*)malloc(sizeof(EpFuture));\n");
    ok = append_list(lines, (long long)"    fut->completed = 0;\n");
    ok = append_list(lines, (long long)"    fut->value = 0;\n");
    ok = append_list(lines, (long long)"    fut->waiting_task = NULL;\n");
    ok = append_list(lines, (long long)"    fut->chan = 0;\n");
    ok = append_list(lines, (long long)"    { EpGCObject* _go = ep_gc_register(fut, EP_OBJ_STRUCT); if(_go) _go->num_fields = 3; }\n");
    ok = append_list(lines, (long long)"    EpReadReadyArgs* args = (EpReadReadyArgs*)malloc(sizeof(EpReadReadyArgs));\n");
    ok = append_list(lines, (long long)"    args->fut = fut;\n");
    ok = append_list(lines, (long long)"    EpTask* task = (EpTask*)malloc(sizeof(EpTask));\n");
    ok = append_list(lines, (long long)"    task->step = ep_read_ready_step;\n");
    ok = append_list(lines, (long long)"    task->args = args;\n");
    ok = append_list(lines, (long long)"    task->args_size_bytes = sizeof(EpReadReadyArgs);\n");
    ok = append_list(lines, (long long)"    task->fut = NULL;\n");
    ok = append_list(lines, (long long)"    task->state = 0;\n");
    ok = append_list(lines, (long long)"    task->is_cancelled = 0;\n");
    ok = append_list(lines, (long long)"    task->parent = ep_current_task;\n");
    ok = append_list(lines, (long long)"    ep_async_register_read((int)fd, task);\n");
    ok = append_list(lines, (long long)"    return (long long)fut;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"typedef struct {\n");
    ok = append_list(lines, (long long)"    EpFuture* fut;\n");
    ok = append_list(lines, (long long)"} EpSleepTimerArgs;\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"static long long ep_sleep_timer_step(void* r) {\n");
    ok = append_list(lines, (long long)"    EpSleepTimerArgs* args = (EpSleepTimerArgs*)r;\n");
    ok = append_list(lines, (long long)"    if (args && args->fut) {\n");
    ok = append_list(lines, (long long)"        args->fut->completed = 1;\n");
    ok = append_list(lines, (long long)"        args->fut->value = 0;\n");
    ok = append_list(lines, (long long)"        if (args->fut->waiting_task) {\n");
    ok = append_list(lines, (long long)"            ep_task_enqueue(args->fut->waiting_task);\n");
    ok = append_list(lines, (long long)"            args->fut->waiting_task = NULL;\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    return 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"static long long sleep_ms(long long ms) {\n");
    ok = append_list(lines, (long long)"    /* Outside the event loop (no task is being stepped) a registered timer\n");
    ok = append_list(lines, (long long)"       would never fire on its own, so the cooperative path used to sleep for\n");
    ok = append_list(lines, (long long)"       0 ms. Block for real instead, and hand back an already-completed\n");
    ok = append_list(lines, (long long)"       future so `await sleep_ms(...)` from synchronous code still works. */\n");
    ok = append_list(lines, (long long)"    if (!ep_current_task) {\n");
    ok = append_list(lines, (long long)"        if (ms > 0) ep_sleep_ms(ms);\n");
    ok = append_list(lines, (long long)"        EpFuture* done = (EpFuture*)malloc(sizeof(EpFuture));\n");
    ok = append_list(lines, (long long)"        done->completed = 1;\n");
    ok = append_list(lines, (long long)"        done->value = 0;\n");
    ok = append_list(lines, (long long)"        done->waiting_task = NULL;\n");
    ok = append_list(lines, (long long)"        done->chan = 0;\n");
    ok = append_list(lines, (long long)"        { EpGCObject* _go = ep_gc_register(done, EP_OBJ_STRUCT); if(_go) _go->num_fields = 3; }\n");
    ok = append_list(lines, (long long)"        return (long long)done;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    EpFuture* fut = (EpFuture*)malloc(sizeof(EpFuture));\n");
    ok = append_list(lines, (long long)"    fut->completed = 0;\n");
    ok = append_list(lines, (long long)"    fut->value = 0;\n");
    ok = append_list(lines, (long long)"    fut->waiting_task = NULL;\n");
    ok = append_list(lines, (long long)"    fut->chan = 0;\n");
    ok = append_list(lines, (long long)"    { EpGCObject* _go = ep_gc_register(fut, EP_OBJ_STRUCT); if(_go) _go->num_fields = 3; }\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"    EpSleepTimerArgs* args = (EpSleepTimerArgs*)malloc(sizeof(EpSleepTimerArgs));\n");
    ok = append_list(lines, (long long)"    args->fut = fut;\n");
    ok = append_list(lines, (long long)"    \n");
    ok = append_list(lines, (long long)"    EpTask* task = (EpTask*)malloc(sizeof(EpTask));\n");
    ok = append_list(lines, (long long)"    task->step = ep_sleep_timer_step;\n");
    ok = append_list(lines, (long long)"    task->args = args;\n");
    ok = append_list(lines, (long long)"    task->args_size_bytes = sizeof(EpSleepTimerArgs);\n");
    ok = append_list(lines, (long long)"    task->fut = NULL;\n");
    ok = append_list(lines, (long long)"    task->state = 0;\n");
    ok = append_list(lines, (long long)"    task->is_cancelled = 0;\n");
    ok = append_list(lines, (long long)"    task->parent = ep_current_task;\n");
    ok = append_list(lines, (long long)"    \n");
    ok = append_list(lines, (long long)"    ep_async_register_timer(ms, task);\n");
    ok = append_list(lines, (long long)"    return (long long)fut;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"static long long cancel_task(long long fut_ptr) {\n");
    ok = append_list(lines, (long long)"    EpFuture* fut = (EpFuture*)fut_ptr;\n");
    ok = append_list(lines, (long long)"    if (fut) {\n");
    ok = append_list(lines, (long long)"        EpTask* task = ep_find_task_by_future(fut);\n");
    ok = append_list(lines, (long long)"        if (task) {\n");
    ok = append_list(lines, (long long)"            ep_cancel_task(task);\n");
    ok = append_list(lines, (long long)"        } else {\n");
    ok = append_list(lines, (long long)"            fut->completed = 1;\n");
    ok = append_list(lines, (long long)"            fut->value = -1;\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    return 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* Closure environment — captures travel with the function pointer */\n");
    ok = append_list(lines, (long long)"#define EP_CLOSURE_MAGIC 0x4550434C4FL\n");
    ok = append_list(lines, (long long)"typedef struct {\n");
    ok = append_list(lines, (long long)"    long long magic;\n");
    ok = append_list(lines, (long long)"    long long fn_ptr;\n");
    ok = append_list(lines, (long long)"    long long env[];  /* flexible array of captured values */\n");
    ok = append_list(lines, (long long)"} EpClosure;\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* GC globals */\n");
    ok = append_list(lines, (long long)"static EpGCObject* ep_gc_head = NULL;\n");
    ok = append_list(lines, (long long)"static long long ep_gc_count = 0;\n");
    ok = append_list(lines, (long long)"static long long ep_gc_threshold = 4096;\n");
    ok = append_list(lines, (long long)"static int ep_gc_enabled = 1;\n");
    ok = append_list(lines, (long long)"static long long ep_gc_nursery_count = 0;\n");
    ok = append_list(lines, (long long)"static long long ep_gc_nursery_threshold = 512;\n");
    ok = append_list(lines, (long long)"static int ep_gc_minor_count = 0;\n");
    ok = append_list(lines, (long long)"static int ep_gc_major_count = 0;\n");
    ok = append_list(lines, (long long)"static void** ep_gc_remembered_set = NULL;\n");
    ok = append_list(lines, (long long)"static long long ep_gc_remembered_cap = 0;\n");
    ok = append_list(lines, (long long)"static long long ep_gc_remembered_size = 0;\n");
    ok = append_list(lines, (long long)"/* Single mutex for ALL GC and thread registry operations.\n");
    ok = append_list(lines, (long long)"   Previous design had two mutexes (ep_gc_mutex + ep_thread_registry_mutex)\n");
    ok = append_list(lines, (long long)"   which caused deadlock under concurrent channel load: thread A held gc_mutex\n");
    ok = append_list(lines, (long long)"   and waited for registry_mutex, thread B held registry_mutex and waited for\n");
    ok = append_list(lines, (long long)"   gc_mutex. Single lock eliminates the ordering problem. */\n");
    ok = append_list(lines, (long long)"#ifdef __wasm__\n");
    ok = append_list(lines, (long long)"#define __thread\n");
    ok = append_list(lines, (long long)"#endif\n");
    ok = append_list(lines, (long long)"static pthread_mutex_t ep_gc_mutex = PTHREAD_MUTEX_INITIALIZER;\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* Stop-the-world coordination. The collector sets ep_gc_stop_requested and, in\n");
    ok = append_list(lines, (long long)"   ep_gc_stop_the_world(), waits until every *other* registered thread has parked\n");
    ok = append_list(lines, (long long)"   at a safepoint (ep_gc_park_if_stopped). This guarantees mark/sweep never runs\n");
    ret_val = join_strings(lines);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(1);
    return ret_val;
}

long long ep_rt_core_6() {
    long long lines = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&lines);
    ep_gc_maybe_collect();

    lines = create_list();
    ok = append_list(lines, (long long)"   concurrently with a mutator changing its roots or an object's fields — the\n");
    ok = append_list(lines, (long long)"   \"marking races with running mutators\" hazard. All three fields are touched\n");
    ok = append_list(lines, (long long)"   only while holding ep_gc_mutex (the lock-free reads of ep_gc_stop_requested at\n");
    ok = append_list(lines, (long long)"   safepoints are a benign optimization: a missed set just defers parking to the\n");
    ok = append_list(lines, (long long)"   next safepoint, and the collector's bounded wait covers it). */\n");
    ok = append_list(lines, (long long)"static volatile int ep_gc_stop_requested = 0;\n");
    ok = append_list(lines, (long long)"static int ep_gc_parked_count = 0;\n");
    ok = append_list(lines, (long long)"static pthread_cond_t ep_gc_resume_cond = PTHREAD_COND_INITIALIZER;\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* Function pointer for channel scanning — set after EpChannel is defined.\n");
    ok = append_list(lines, (long long)"   GC mark calls this to scan values in-transit in channel buffers. */\n");
    ok = append_list(lines, (long long)"static void (*ep_gc_scan_channels_major)(void) = NULL;\n");
    ok = append_list(lines, (long long)"static void (*ep_gc_scan_channels_minor)(void) = NULL;\n");
    ok = append_list(lines, (long long)"/* Function pointers for marking top-level constant/global variables, which are\n");
    ok = append_list(lines, (long long)"   GC roots that live outside any function frame. Set by __ep_init_constants. */\n");
    ok = append_list(lines, (long long)"static void (*ep_gc_mark_globals_major)(void) = NULL;\n");
    ok = append_list(lines, (long long)"static void (*ep_gc_mark_globals_minor)(void) = NULL;\n");
    ok = append_list(lines, (long long)"/* Function pointers for map value traversal — set after EpMap is defined.\n");
    ok = append_list(lines, (long long)"   GC mark calls these to recursively mark values stored in maps. */\n");
    ok = append_list(lines, (long long)"static void (*ep_gc_mark_map_values)(void* ptr) = NULL;\n");
    ok = append_list(lines, (long long)"static void (*ep_gc_mark_map_values_minor)(void* ptr) = NULL;\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* Thread registry for GC root scanning in multi-threaded environment */\n");
    ok = append_list(lines, (long long)"#define EP_MAX_THREADS 256\n");
    ok = append_list(lines, (long long)"static __thread void* volatile ep_thread_local_top = NULL;\n");
    ok = append_list(lines, (long long)"static __thread void* ep_thread_local_bottom = NULL;\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"static void* volatile* ep_thread_tops[EP_MAX_THREADS];\n");
    ok = append_list(lines, (long long)"static void* ep_thread_bottoms[EP_MAX_THREADS];\n");
    ok = append_list(lines, (long long)"static volatile int ep_thread_active[EP_MAX_THREADS];\n");
    ok = append_list(lines, (long long)"static int ep_num_threads = 0;\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* Per-thread GC root state — heap-allocated, stable across thread lifetime.\n");
    ok = append_list(lines, (long long)"   Previous design stored raw pointers to __thread arrays (ep_gc_root_stack,\n");
    ok = append_list(lines, (long long)"   ep_gc_root_sp) in the global registry. When a thread exited, the __thread\n");
    ok = append_list(lines, (long long)"   storage was freed, leaving dangling pointers that ep_gc_mark would\n");
    ok = append_list(lines, (long long)"   dereference → segfault. Now each thread gets a heap-allocated state struct\n");
    ok = append_list(lines, (long long)"   that survives thread exit and is only recycled when the slot is reused. */\n");
    ok = append_list(lines, (long long)"typedef struct {\n");
    ok = append_list(lines, (long long)"    long long* roots[4096];  /* copy of root pointers, updated under lock */\n");
    ok = append_list(lines, (long long)"    volatile int sp;         /* current root stack pointer */\n");
    ok = append_list(lines, (long long)"} EpThreadGCState;\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"static EpThreadGCState* ep_thread_gc_states[EP_MAX_THREADS];\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* Shadow stack for explicit GC roots — thread-local to prevent cross-thread corruption */\n");
    ok = append_list(lines, (long long)"#define EP_GC_MAX_ROOTS 4096\n");
    ok = append_list(lines, (long long)"static __thread long long* ep_gc_root_stack[EP_GC_MAX_ROOTS];\n");
    ok = append_list(lines, (long long)"static __thread int ep_gc_root_sp = 0;\n");
    ok = append_list(lines, (long long)"static __thread int ep_thread_slot = -1;\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* ep_gc_root_sp is the *logical* shadow-stack depth. It always advances on\n");
    ok = append_list(lines, (long long)"   push and retreats on pop so that per-frame push/pop counts stay balanced.\n");
    ok = append_list(lines, (long long)"   Array storage is capped at EP_GC_MAX_ROOTS: once the stack is full, further\n");
    ok = append_list(lines, (long long)"   roots are counted but not stored (those deep-overflow locals are simply not\n");
    ok = append_list(lines, (long long)"   traced) — crucially, we never overwrite or drop an outer frame's stored\n");
    ok = append_list(lines, (long long)"   roots, which the old \"silently skip the push but still pop\" path did. */\n");
    ok = append_list(lines, (long long)"static void ep_gc_push_root(long long* root) {\n");
    ok = append_list(lines, (long long)"    int idx = ep_gc_root_sp;\n");
    ok = append_list(lines, (long long)"    ep_gc_root_sp++;\n");
    ok = append_list(lines, (long long)"    if (idx < EP_GC_MAX_ROOTS) {\n");
    ok = append_list(lines, (long long)"        ep_gc_root_stack[idx] = root;\n");
    ok = append_list(lines, (long long)"        /* Update the heap-allocated state so GC mark can see it safely */\n");
    ok = append_list(lines, (long long)"        if (ep_thread_slot >= 0 && ep_thread_gc_states[ep_thread_slot]) {\n");
    ok = append_list(lines, (long long)"            ep_thread_gc_states[ep_thread_slot]->roots[idx] = root;\n");
    ok = append_list(lines, (long long)"            ep_thread_gc_states[ep_thread_slot]->sp =\n");
    ok = append_list(lines, (long long)"                (ep_gc_root_sp < EP_GC_MAX_ROOTS) ? ep_gc_root_sp : EP_GC_MAX_ROOTS;\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"static void ep_gc_pop_roots(long long count) {\n");
    ok = append_list(lines, (long long)"    ep_gc_root_sp -= (int)count;\n");
    ok = append_list(lines, (long long)"    if (ep_gc_root_sp < 0) ep_gc_root_sp = 0;\n");
    ok = append_list(lines, (long long)"    /* Update the heap-allocated state (clamped to the array bound) */\n");
    ok = append_list(lines, (long long)"    if (ep_thread_slot >= 0 && ep_thread_gc_states[ep_thread_slot]) {\n");
    ok = append_list(lines, (long long)"        ep_thread_gc_states[ep_thread_slot]->sp =\n");
    ok = append_list(lines, (long long)"            (ep_gc_root_sp < EP_GC_MAX_ROOTS) ? ep_gc_root_sp : EP_GC_MAX_ROOTS;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* Park the calling thread if the collector has stopped the world.\n");
    ok = append_list(lines, (long long)"   MUST be called with ep_gc_mutex held. The thread's shadow stack (its precise\n");
    ok = append_list(lines, (long long)"   root set) is stable while parked, so the collector can scan it race-free. */\n");
    ok = append_list(lines, (long long)"static void ep_gc_park_if_stopped(void) {\n");
    ok = append_list(lines, (long long)"    if (!ep_gc_stop_requested) return;\n");
    ok = append_list(lines, (long long)"    /* Spill registers onto the stack and publish this thread's current stack top\n");
    ok = append_list(lines, (long long)"       so the collector can conservatively scan its frozen C stack while parked —\n");
    ok = append_list(lines, (long long)"       this catches roots held only in registers/temporaries that the precise\n");
    ok = append_list(lines, (long long)"       shadow stack does not yet record. _dummy is declared below _pregs, so its\n");
    ok = append_list(lines, (long long)"       (lower) address bounds a scan range that covers the spilled registers. */\n");
    ok = append_list(lines, (long long)"    jmp_buf _pregs;\n");
    ok = append_list(lines, (long long)"    volatile char _top_marker;  /* function-scope: stays valid while parked */\n");
    ok = append_list(lines, (long long)"    memset(&_pregs, 0, sizeof(_pregs));\n");
    ok = append_list(lines, (long long)"    setjmp(_pregs);\n");
    ok = append_list(lines, (long long)"    /* _top_marker is declared after _pregs, so its (lower) address bounds a scan\n");
    ok = append_list(lines, (long long)"       range [&_top_marker, stack_bottom] that covers the spilled registers. */\n");
    ok = append_list(lines, (long long)"    ep_thread_local_top = (void*)&_top_marker;\n");
    ok = append_list(lines, (long long)"    __sync_synchronize();  /* publish shadow-stack + top writes before parking */\n");
    ok = append_list(lines, (long long)"    ep_gc_parked_count++;\n");
    ok = append_list(lines, (long long)"    while (ep_gc_stop_requested) {\n");
    ok = append_list(lines, (long long)"        pthread_cond_wait(&ep_gc_resume_cond, &ep_gc_mutex);\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    ep_gc_parked_count--;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* Begin a stop-the-world pause. MUST be called with ep_gc_mutex held.\n");
    ok = append_list(lines, (long long)"   Waits (briefly releasing the lock so blocked mutators can reach a safepoint)\n");
    ok = append_list(lines, (long long)"   until all other registered threads have parked. After a bounded fallback\n");
    ok = append_list(lines, (long long)"   (~50ms) it proceeds anyway: any thread that hasn't parked by then is blocked\n");
    ok = append_list(lines, (long long)"   or idle with a stable shadow stack, so scanning it is still safe in practice. */\n");
    ok = append_list(lines, (long long)"static void ep_gc_stop_the_world(void) {\n");
    ok = append_list(lines, (long long)"    ep_gc_stop_requested = 1;\n");
    ok = append_list(lines, (long long)"    /* Actively-running threads reach a safepoint (every allocation and every\n");
    ok = append_list(lines, (long long)"       function entry) within microseconds, so they park on the first spin or\n");
    ok = append_list(lines, (long long)"       two. The bound only caps the rare case where a thread is blocked/idle\n");
    ok = append_list(lines, (long long)"       (e.g. just entered a channel op) and won't park — those have a stable\n");
    ok = append_list(lines, (long long)"       shadow stack, so proceeding to scan them is safe. ~40 * 250us ≈ 10ms. */\n");
    ok = append_list(lines, (long long)"    for (int spins = 0; spins < 40; spins++) {\n");
    ok = append_list(lines, (long long)"        int others = 0;\n");
    ok = append_list(lines, (long long)"        for (int t = 0; t < ep_num_threads; t++) {\n");
    ok = append_list(lines, (long long)"            if (ep_thread_active[t] && t != ep_thread_slot) others++;\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"        if (others <= 0 || ep_gc_parked_count >= others) return;\n");
    ok = append_list(lines, (long long)"        pthread_mutex_unlock(&ep_gc_mutex);\n");
    ok = append_list(lines, (long long)"#ifdef _WIN32\n");
    ok = append_list(lines, (long long)"        Sleep(1);\n");
    ok = append_list(lines, (long long)"#elif !defined(__wasm__)\n");
    ok = append_list(lines, (long long)"        usleep(250);\n");
    ok = append_list(lines, (long long)"#endif\n");
    ok = append_list(lines, (long long)"        pthread_mutex_lock(&ep_gc_mutex);\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* End a stop-the-world pause and wake all parked threads. MUST hold ep_gc_mutex. */\n");
    ok = append_list(lines, (long long)"static void ep_gc_start_the_world(void) {\n");
    ok = append_list(lines, (long long)"    ep_gc_stop_requested = 0;\n");
    ok = append_list(lines, (long long)"    pthread_cond_broadcast(&ep_gc_resume_cond);\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"static void ep_gc_register_thread(void* stack_bottom) {\n");
    ok = append_list(lines, (long long)"    ep_thread_local_bottom = stack_bottom;\n");
    ok = append_list(lines, (long long)"    ep_thread_local_top = stack_bottom;\n");
    ok = append_list(lines, (long long)"    \n");
    ok = append_list(lines, (long long)"    pthread_mutex_lock(&ep_gc_mutex);\n");
    ok = append_list(lines, (long long)"    int slot = -1;\n");
    ok = append_list(lines, (long long)"    for (int i = 0; i < ep_num_threads; i++) {\n");
    ok = append_list(lines, (long long)"        if (!ep_thread_active[i]) {\n");
    ok = append_list(lines, (long long)"            slot = i;\n");
    ok = append_list(lines, (long long)"            break;\n");
    ok = append_list(lines, (long long)"        }\n");
    ret_val = join_strings(lines);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(1);
    return ret_val;
}

long long ep_rt_core_7() {
    long long lines = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&lines);
    ep_gc_maybe_collect();

    lines = create_list();
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    if (slot == -1 && ep_num_threads < EP_MAX_THREADS) {\n");
    ok = append_list(lines, (long long)"        slot = ep_num_threads++;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    if (slot != -1) {\n");
    ok = append_list(lines, (long long)"        ep_thread_tops[slot] = &ep_thread_local_top;\n");
    ok = append_list(lines, (long long)"        ep_thread_bottoms[slot] = stack_bottom;\n");
    ok = append_list(lines, (long long)"        /* Allocate or reuse heap state for this slot */\n");
    ok = append_list(lines, (long long)"        if (!ep_thread_gc_states[slot]) {\n");
    ok = append_list(lines, (long long)"            ep_thread_gc_states[slot] = (EpThreadGCState*)calloc(1, sizeof(EpThreadGCState));\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"        ep_thread_gc_states[slot]->sp = 0;\n");
    ok = append_list(lines, (long long)"        ep_thread_slot = slot;\n");
    ok = append_list(lines, (long long)"        __sync_synchronize();  /* Memory barrier: state must be visible before active */\n");
    ok = append_list(lines, (long long)"        ep_thread_active[slot] = 1;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    pthread_mutex_unlock(&ep_gc_mutex);\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"static void ep_gc_unregister_thread(void) {\n");
    ok = append_list(lines, (long long)"    pthread_mutex_lock(&ep_gc_mutex);\n");
    ok = append_list(lines, (long long)"    for (int i = 0; i < ep_num_threads; i++) {\n");
    ok = append_list(lines, (long long)"        if (ep_thread_active[i] && ep_thread_tops[i] == &ep_thread_local_top) {\n");
    ok = append_list(lines, (long long)"            /* Zero root count FIRST — even if ep_gc_mark races past the\n");
    ok = append_list(lines, (long long)"               active check, it will see sp=0 and walk no roots instead\n");
    ok = append_list(lines, (long long)"               of dereferencing stale __thread pointers */\n");
    ok = append_list(lines, (long long)"            if (ep_thread_gc_states[i]) {\n");
    ok = append_list(lines, (long long)"                ep_thread_gc_states[i]->sp = 0;\n");
    ok = append_list(lines, (long long)"            }\n");
    ok = append_list(lines, (long long)"            __sync_synchronize();  /* Memory barrier: sp=0 visible before deactivation */\n");
    ok = append_list(lines, (long long)"            ep_thread_active[i] = 0;\n");
    ok = append_list(lines, (long long)"            ep_thread_slot = -1;\n");
    ok = append_list(lines, (long long)"            break;\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    pthread_mutex_unlock(&ep_gc_mutex);\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"#define EP_GC_UPDATE_TOP() { volatile int _dummy; ep_thread_local_top = (void*)&_dummy; }\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* Simple open-addressed hash map with linear probing for O(1) GC object lookup */\n");
    ok = append_list(lines, (long long)"typedef struct {\n");
    ok = append_list(lines, (long long)"    void* key;\n");
    ok = append_list(lines, (long long)"    EpGCObject* value;\n");
    ok = append_list(lines, (long long)"} EpGCEntry;\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"static EpGCEntry* ep_gc_table = NULL;\n");
    ok = append_list(lines, (long long)"static long long ep_gc_table_cap = 0;\n");
    ok = append_list(lines, (long long)"static long long ep_gc_table_size = 0;\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* Bucket index for a pointer key. The previous hash was ((uintptr_t)key % cap)\n");
    ok = append_list(lines, (long long)"   with cap a power of two; malloc returns 16-byte-aligned pointers, so the low 4\n");
    ok = append_list(lines, (long long)"   bits are always 0 and only every 16th bucket was ever a home slot. That caused\n");
    ok = append_list(lines, (long long)"   catastrophic primary clustering -> O(n) probe runs -> ep_gc_table_remove's\n");
    ok = append_list(lines, (long long)"   rehash became O(n^2), which (under the single global GC mutex) wedged the whole\n");
    ok = append_list(lines, (long long)"   node when a large object list was freed. A splitmix64 finalizer avalanches all\n");
    ok = append_list(lines, (long long)"   bits, so even the low bits taken by the (cap-1) mask are well distributed. */\n");
    ok = append_list(lines, (long long)"static inline long long ep_gc_index(void* key, long long cap) {\n");
    ok = append_list(lines, (long long)"    uint64_t z = (uint64_t)(uintptr_t)key;\n");
    ok = append_list(lines, (long long)"    z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ULL;\n");
    ok = append_list(lines, (long long)"    z = (z ^ (z >> 27)) * 0x94d049bb133111ebULL;\n");
    ok = append_list(lines, (long long)"    z = z ^ (z >> 31);\n");
    ok = append_list(lines, (long long)"    return (long long)(z & (uint64_t)(cap - 1));   /* cap is always a power of two */\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* Insert without growing — assumes a free slot exists. Used by the resize and by\n");
    ok = append_list(lines, (long long)"   ep_gc_table_remove's rehash, neither of which may trigger a (re)allocation of\n");
    ok = append_list(lines, (long long)"   the table mid-iteration. */\n");
    ok = append_list(lines, (long long)"static void ep_gc_table_place(void* key, EpGCObject* value) {\n");
    ok = append_list(lines, (long long)"    long long idx = ep_gc_index(key, ep_gc_table_cap);\n");
    ok = append_list(lines, (long long)"    while (ep_gc_table[idx].key != NULL) {\n");
    ok = append_list(lines, (long long)"        if (ep_gc_table[idx].key == key) {\n");
    ok = append_list(lines, (long long)"            ep_gc_table[idx].value = value;\n");
    ok = append_list(lines, (long long)"            return;\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"        idx = (idx + 1) & (ep_gc_table_cap - 1);\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    ep_gc_table[idx].key = key;\n");
    ok = append_list(lines, (long long)"    ep_gc_table[idx].value = value;\n");
    ok = append_list(lines, (long long)"    ep_gc_table_size++;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"static void ep_gc_table_insert(void* key, EpGCObject* value) {\n");
    ok = append_list(lines, (long long)"    if (ep_gc_table_size * 2 >= ep_gc_table_cap) {\n");
    ok = append_list(lines, (long long)"        long long old_cap = ep_gc_table_cap;\n");
    ok = append_list(lines, (long long)"        long long new_cap = old_cap == 0 ? 512 : old_cap * 2;\n");
    ok = append_list(lines, (long long)"        EpGCEntry* new_table = (EpGCEntry*)calloc(new_cap, sizeof(EpGCEntry));\n");
    ok = append_list(lines, (long long)"        EpGCEntry* old_table = ep_gc_table;\n");
    ok = append_list(lines, (long long)"        ep_gc_table = new_table;\n");
    ok = append_list(lines, (long long)"        ep_gc_table_cap = new_cap;\n");
    ok = append_list(lines, (long long)"        ep_gc_table_size = 0;\n");
    ok = append_list(lines, (long long)"        for (long long i = 0; i < old_cap; i++) {\n");
    ok = append_list(lines, (long long)"            if (old_table[i].key != NULL) {\n");
    ok = append_list(lines, (long long)"                ep_gc_table_place(old_table[i].key, old_table[i].value);\n");
    ok = append_list(lines, (long long)"            }\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"        free(old_table);\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    ep_gc_table_place(key, value);\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"static EpGCObject* ep_gc_table_get(void* key) {\n");
    ok = append_list(lines, (long long)"    if (ep_gc_table_cap == 0) return NULL;\n");
    ok = append_list(lines, (long long)"    long long idx = ep_gc_index(key, ep_gc_table_cap);\n");
    ok = append_list(lines, (long long)"    while (ep_gc_table[idx].key != NULL) {\n");
    ok = append_list(lines, (long long)"        if (ep_gc_table[idx].key == key) return ep_gc_table[idx].value;\n");
    ok = append_list(lines, (long long)"        idx = (idx + 1) & (ep_gc_table_cap - 1);\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    return NULL;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"static void ep_gc_table_remove(void* key) {\n");
    ok = append_list(lines, (long long)"    if (ep_gc_table_cap == 0) return;\n");
    ok = append_list(lines, (long long)"    long long idx = ep_gc_index(key, ep_gc_table_cap);\n");
    ok = append_list(lines, (long long)"    while (ep_gc_table[idx].key != NULL) {\n");
    ok = append_list(lines, (long long)"        if (ep_gc_table[idx].key == key) {\n");
    ok = append_list(lines, (long long)"            ep_gc_table[idx].key = NULL;\n");
    ok = append_list(lines, (long long)"            ep_gc_table[idx].value = NULL;\n");
    ok = append_list(lines, (long long)"            ep_gc_table_size--;\n");
    ok = append_list(lines, (long long)"            /* Backward-shift rehash of the rest of this cluster. Re-place (no\n");
    ok = append_list(lines, (long long)"               resize: size is not growing) so a mid-iteration realloc can never\n");
    ok = append_list(lines, (long long)"               free the table out from under this loop. */\n");
    ok = append_list(lines, (long long)"            long long next_idx = (idx + 1) & (ep_gc_table_cap - 1);\n");
    ok = append_list(lines, (long long)"            while (ep_gc_table[next_idx].key != NULL) {\n");
    ok = append_list(lines, (long long)"                void* rehash_key = ep_gc_table[next_idx].key;\n");
    ok = append_list(lines, (long long)"                EpGCObject* rehash_val = ep_gc_table[next_idx].value;\n");
    ok = append_list(lines, (long long)"                ep_gc_table[next_idx].key = NULL;\n");
    ok = append_list(lines, (long long)"                ep_gc_table[next_idx].value = NULL;\n");
    ok = append_list(lines, (long long)"                ep_gc_table_size--;\n");
    ok = append_list(lines, (long long)"                ep_gc_table_place(rehash_key, rehash_val);\n");
    ok = append_list(lines, (long long)"                next_idx = (next_idx + 1) & (ep_gc_table_cap - 1);\n");
    ok = append_list(lines, (long long)"            }\n");
    ok = append_list(lines, (long long)"            return;\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"        idx = (idx + 1) & (ep_gc_table_cap - 1);\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* Register a new GC object */\n");
    ok = append_list(lines, (long long)"static EpGCObject* ep_gc_register(void* ptr, EpObjKind kind) {\n");
    ok = append_list(lines, (long long)"    if (!ptr) return NULL;\n");
    ok = append_list(lines, (long long)"    pthread_mutex_lock(&ep_gc_mutex);\n");
    ok = append_list(lines, (long long)"    ep_gc_park_if_stopped();  /* safepoint: don't allocate/touch the table mid-collection */\n");
    ok = append_list(lines, (long long)"    EpGCObject* obj = (EpGCObject*)malloc(sizeof(EpGCObject));\n");
    ok = append_list(lines, (long long)"    if (!obj) {\n");
    ok = append_list(lines, (long long)"        pthread_mutex_unlock(&ep_gc_mutex);\n");
    ok = append_list(lines, (long long)"        return NULL;\n");
    ok = append_list(lines, (long long)"    }\n");
    ret_val = join_strings(lines);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(1);
    return ret_val;
}

long long ep_rt_core_8() {
    long long lines = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&lines);
    ep_gc_maybe_collect();

    lines = create_list();
    ok = append_list(lines, (long long)"    obj->kind = kind;\n");
    ok = append_list(lines, (long long)"    obj->marked = 0;\n");
    ok = append_list(lines, (long long)"    obj->ptr = ptr;\n");
    ok = append_list(lines, (long long)"    obj->size = 0;\n");
    ok = append_list(lines, (long long)"    obj->num_fields = 0;\n");
    ok = append_list(lines, (long long)"    obj->generation = 0;\n");
    ok = append_list(lines, (long long)"    obj->next = ep_gc_head;\n");
    ok = append_list(lines, (long long)"    ep_gc_head = obj;\n");
    ok = append_list(lines, (long long)"    ep_gc_count++;\n");
    ok = append_list(lines, (long long)"    ep_gc_nursery_count++;\n");
    ok = append_list(lines, (long long)"    ep_gc_table_insert(ptr, obj);\n");
    ok = append_list(lines, (long long)"    pthread_mutex_unlock(&ep_gc_mutex);\n");
    ok = append_list(lines, (long long)"    return obj;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* Find GC object by pointer.\n");
    ok = append_list(lines, (long long)"   Takes ep_gc_mutex because ep_gc_table_insert may realloc+free the table\n");
    ok = append_list(lines, (long long)"   concurrently (from another thread's allocation). Mutator-side callers\n");
    ok = append_list(lines, (long long)"   (write barrier, free_struct/free_map/free_list, to-string) must use this\n");
    ok = append_list(lines, (long long)"   locking variant; code already holding the mutex (mark/sweep) calls\n");
    ok = append_list(lines, (long long)"   ep_gc_table_get directly to avoid a non-recursive double-lock deadlock. */\n");
    ok = append_list(lines, (long long)"static EpGCObject* ep_gc_find(void* ptr) {\n");
    ok = append_list(lines, (long long)"    pthread_mutex_lock(&ep_gc_mutex);\n");
    ok = append_list(lines, (long long)"    ep_gc_park_if_stopped();  /* safepoint */\n");
    ok = append_list(lines, (long long)"    EpGCObject* obj = ep_gc_table_get(ptr);\n");
    ok = append_list(lines, (long long)"    pthread_mutex_unlock(&ep_gc_mutex);\n");
    ok = append_list(lines, (long long)"    return obj;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* Write barrier for generational GC: tracks references from old objects (gen 1) to young objects (gen 0).\n");
    ok = append_list(lines, (long long)"   The whole operation runs under ep_gc_mutex so the table lookups and the\n");
    ok = append_list(lines, (long long)"   remembered-set update see a consistent table (no race with a concurrent\n");
    ok = append_list(lines, (long long)"   resize) and use the no-lock ep_gc_table_get to avoid re-entering the lock. */\n");
    ok = append_list(lines, (long long)"static void ep_gc_write_barrier(void* host_ptr, long long val) {\n");
    ok = append_list(lines, (long long)"    if (val == 0) return;\n");
    ok = append_list(lines, (long long)"    pthread_mutex_lock(&ep_gc_mutex);\n");
    ok = append_list(lines, (long long)"    ep_gc_park_if_stopped();  /* safepoint: don't update the remembered set mid-collection */\n");
    ok = append_list(lines, (long long)"    EpGCObject* host_obj = ep_gc_table_get(host_ptr);\n");
    ok = append_list(lines, (long long)"    EpGCObject* val_obj = ep_gc_table_get((void*)val);\n");
    ok = append_list(lines, (long long)"    if (host_obj && val_obj && host_obj->generation == 1 && val_obj->generation == 0) {\n");
    ok = append_list(lines, (long long)"        /* Check if already in remembered set */\n");
    ok = append_list(lines, (long long)"        int found = 0;\n");
    ok = append_list(lines, (long long)"        for (long long i = 0; i < ep_gc_remembered_size; i++) {\n");
    ok = append_list(lines, (long long)"            if (ep_gc_remembered_set[i] == (void*)val) {\n");
    ok = append_list(lines, (long long)"                found = 1;\n");
    ok = append_list(lines, (long long)"                break;\n");
    ok = append_list(lines, (long long)"            }\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"        if (!found) {\n");
    ok = append_list(lines, (long long)"            if (ep_gc_remembered_size >= ep_gc_remembered_cap) {\n");
    ok = append_list(lines, (long long)"                long long new_cap = ep_gc_remembered_cap == 0 ? 128 : ep_gc_remembered_cap * 2;\n");
    ok = append_list(lines, (long long)"                void** new_set = (void**)realloc(ep_gc_remembered_set, new_cap * sizeof(void*));\n");
    ok = append_list(lines, (long long)"                if (new_set) {\n");
    ok = append_list(lines, (long long)"                    ep_gc_remembered_set = new_set;\n");
    ok = append_list(lines, (long long)"                    ep_gc_remembered_cap = new_cap;\n");
    ok = append_list(lines, (long long)"                }\n");
    ok = append_list(lines, (long long)"            }\n");
    ok = append_list(lines, (long long)"            if (ep_gc_remembered_size < ep_gc_remembered_cap) {\n");
    ok = append_list(lines, (long long)"                ep_gc_remembered_set[ep_gc_remembered_size++] = (void*)val;\n");
    ok = append_list(lines, (long long)"            }\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    pthread_mutex_unlock(&ep_gc_mutex);\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* Forward declarations for list type (needed by GC mark) */\n");
    ok = append_list(lines, (long long)"typedef struct {\n");
    ok = append_list(lines, (long long)"    long long* data;\n");
    ok = append_list(lines, (long long)"    long long length;\n");
    ok = append_list(lines, (long long)"    long long capacity;\n");
    ok = append_list(lines, (long long)"} EpList;\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* A real heap object (list/map/string) is malloc'd, so its address is far above\n");
    ok = append_list(lines, (long long)"   the never-mapped first page. EP values that are NOT pointers — small ints,\n");
    ok = append_list(lines, (long long)"   booleans, and JSON type-tags (2=string, 3=list, 4=object) — land in [0,4096).\n");
    ok = append_list(lines, (long long)"   Guarding the object accessors with this turns \"deref a non-pointer\" (the cause\n");
    ok = append_list(lines, (long long)"   of the read_transcripts segfault, and that whole class) into a safe null return\n");
    ok = append_list(lines, (long long)"   instead of a daemon-killing SIGSEGV. One comparison; negligible on hot paths. */\n");
    ok = append_list(lines, (long long)"#define EP_BADPTR(p) (((unsigned long long)(p)) < 4096ULL)\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* Mark a single object and recursively mark its children */\n");
    ok = append_list(lines, (long long)"static void ep_gc_mark_object(void* ptr) {\n");
    ok = append_list(lines, (long long)"    if (!ptr) return;\n");
    ok = append_list(lines, (long long)"    /* Runs under ep_gc_mutex (held by the collector) — use the no-lock lookup. */\n");
    ok = append_list(lines, (long long)"    EpGCObject* obj = ep_gc_table_get(ptr);\n");
    ok = append_list(lines, (long long)"    if (!obj || obj->marked) return;\n");
    ok = append_list(lines, (long long)"    obj->marked = 1;\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"    if (obj->kind == EP_OBJ_LIST) {\n");
    ok = append_list(lines, (long long)"        EpList* list = (EpList*)ptr;\n");
    ok = append_list(lines, (long long)"        for (long long i = 0; i < list->length; i++) {\n");
    ok = append_list(lines, (long long)"            long long val = list->data[i];\n");
    ok = append_list(lines, (long long)"            if (val != 0) {\n");
    ok = append_list(lines, (long long)"                ep_gc_mark_object((void*)val);\n");
    ok = append_list(lines, (long long)"            }\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"    } else if (obj->kind == EP_OBJ_STRUCT) {\n");
    ok = append_list(lines, (long long)"        long long* fields = (long long*)ptr;\n");
    ok = append_list(lines, (long long)"        for (long long i = 0; i < obj->num_fields; i++) {\n");
    ok = append_list(lines, (long long)"            if (fields[i] != 0) {\n");
    ok = append_list(lines, (long long)"                ep_gc_mark_object((void*)fields[i]);\n");
    ok = append_list(lines, (long long)"            }\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"    } else if (obj->kind == EP_OBJ_MAP) {\n");
    ok = append_list(lines, (long long)"        if (ep_gc_mark_map_values) ep_gc_mark_map_values(ptr);\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* Mark a single object and recursively mark its children (only if it is Gen 0) */\n");
    ok = append_list(lines, (long long)"static void ep_gc_mark_object_minor(void* ptr) {\n");
    ok = append_list(lines, (long long)"    if (!ptr) return;\n");
    ok = append_list(lines, (long long)"    /* Runs under ep_gc_mutex (held by the collector) — use the no-lock lookup. */\n");
    ok = append_list(lines, (long long)"    EpGCObject* obj = ep_gc_table_get(ptr);\n");
    ok = append_list(lines, (long long)"    if (!obj || obj->generation != 0 || obj->marked) return;\n");
    ok = append_list(lines, (long long)"    obj->marked = 1;\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"    if (obj->kind == EP_OBJ_LIST) {\n");
    ok = append_list(lines, (long long)"        EpList* list = (EpList*)ptr;\n");
    ok = append_list(lines, (long long)"        for (long long i = 0; i < list->length; i++) {\n");
    ok = append_list(lines, (long long)"            long long val = list->data[i];\n");
    ok = append_list(lines, (long long)"            if (val != 0) {\n");
    ok = append_list(lines, (long long)"                ep_gc_mark_object_minor((void*)val);\n");
    ok = append_list(lines, (long long)"            }\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"    } else if (obj->kind == EP_OBJ_STRUCT) {\n");
    ok = append_list(lines, (long long)"        long long* fields = (long long*)ptr;\n");
    ok = append_list(lines, (long long)"        for (long long i = 0; i < obj->num_fields; i++) {\n");
    ok = append_list(lines, (long long)"            if (fields[i] != 0) {\n");
    ok = append_list(lines, (long long)"                ep_gc_mark_object_minor((void*)fields[i]);\n");
    ok = append_list(lines, (long long)"            }\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"    } else if (obj->kind == EP_OBJ_MAP) {\n");
    ok = append_list(lines, (long long)"        if (ep_gc_mark_map_values_minor) ep_gc_mark_map_values_minor(ptr);\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* Conservatively scan every registered thread's C stack and mark any word that\n");
    ok = append_list(lines, (long long)"   looks like a tracked pointer. The collector spills its own registers and\n");
    ok = append_list(lines, (long long)"   publishes its top here; all other threads are parked at a safepoint with their\n");
    ok = append_list(lines, (long long)"   registers spilled and top published (ep_gc_park_if_stopped), so their stacks\n");
    ok = append_list(lines, (long long)"   are frozen. This complements the precise shadow stacks: it catches roots held\n");
    ok = append_list(lines, (long long)"   only in registers/temporaries (e.g. a freshly allocated object not yet stored\n");
    ok = append_list(lines, (long long)"   into a rooted slot). Non-pointer words are harmlessly ignored by ep_gc_find.\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"   Only run on MAJOR collections: minor collections rely on the precise shadow\n");
    ok = append_list(lines, (long long)"   stacks plus the write barrier's remembered set (the standard generational\n");
    ok = append_list(lines, (long long)"   approach), so they do no stack scan at all — which means there is no racy\n");
    ok = append_list(lines, (long long)"   cross-thread stack read on the frequent minor path either. The expensive\n");
    ok = append_list(lines, (long long)"   full-stack scan is paid only on the rarer major collection, where it pins\n");
    ok = append_list(lines, (long long)"   any long-lived object reachable only via a register across many GCs.\n");
    ret_val = join_strings(lines);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(1);
    return ret_val;
}

long long ep_rt_core_9() {
    long long lines = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&lines);
    ep_gc_maybe_collect();

    lines = create_list();
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"   Marked no_sanitize_address: a conservative scan deliberately reads whole stack\n");
    ok = append_list(lines, (long long)"   ranges (including ASAN redzones and out-of-frame slots), which is not a bug. */\n");
    ok = append_list(lines, (long long)"#if defined(__SANITIZE_ADDRESS__)\n");
    ok = append_list(lines, (long long)"# define EP_NO_ASAN __attribute__((no_sanitize_address))\n");
    ok = append_list(lines, (long long)"#elif defined(__has_feature)\n");
    ok = append_list(lines, (long long)"# if __has_feature(address_sanitizer)\n");
    ok = append_list(lines, (long long)"#  define EP_NO_ASAN __attribute__((no_sanitize_address))\n");
    ok = append_list(lines, (long long)"# endif\n");
    ok = append_list(lines, (long long)"#endif\n");
    ok = append_list(lines, (long long)"#ifndef EP_NO_ASAN\n");
    ok = append_list(lines, (long long)"# define EP_NO_ASAN\n");
    ok = append_list(lines, (long long)"#endif\n");
    ok = append_list(lines, (long long)"EP_NO_ASAN\n");
    ok = append_list(lines, (long long)"static void ep_gc_scan_thread_stacks(void) {\n");
    ok = append_list(lines, (long long)"    jmp_buf _regs;\n");
    ok = append_list(lines, (long long)"    volatile char _top_marker;\n");
    ok = append_list(lines, (long long)"    memset(&_regs, 0, sizeof(_regs));\n");
    ok = append_list(lines, (long long)"    setjmp(_regs);   /* spill the collector's own registers onto its stack */\n");
    ok = append_list(lines, (long long)"    /* Publish the LOWEST of our own local addresses as this thread's live top, so the\n");
    ok = append_list(lines, (long long)"       scanned range covers both the stack marker and the register-spill buffer whatever\n");
    ok = append_list(lines, (long long)"       order the compiler laid them out (a missed _regs would drop a register-only root). */\n");
    ok = append_list(lines, (long long)"    { char* _a = (char*)(void*)&_top_marker; char* _b = (char*)(void*)&_regs;\n");
    ok = append_list(lines, (long long)"      ep_thread_local_top = (void*)((_a < _b) ? _a : _b); }\n");
    ok = append_list(lines, (long long)"    for (int t = 0; t < ep_num_threads; t++) {\n");
    ok = append_list(lines, (long long)"        if (!ep_thread_active[t]) continue;\n");
    ok = append_list(lines, (long long)"        if (!ep_thread_tops[t]) continue;\n");
    ok = append_list(lines, (long long)"        /* The published top comes from a char local, so it may not be pointer-aligned;\n");
    ok = append_list(lines, (long long)"           mask DOWN to 8 bytes. Aligning down only widens the conservative window by a\n");
    ok = append_list(lines, (long long)"           few harmless bytes — aligning up could skip the slot holding a live root.\n");
    ok = append_list(lines, (long long)"           Unaligned void** dereferences are UB and produce a skewed scan window on\n");
    ok = append_list(lines, (long long)"           strict platforms (caught by valgrind on Linux). */\n");
    ok = append_list(lines, (long long)"        void** start = (void**)((uintptr_t)*ep_thread_tops[t] & ~(uintptr_t)7);\n");
    ok = append_list(lines, (long long)"        void** end = (void**)ep_thread_bottoms[t];\n");
    ok = append_list(lines, (long long)"        if (!start || !end) continue;\n");
    ok = append_list(lines, (long long)"        if (start > end) { void** tmp = start; start = end; end = tmp; }\n");
    ok = append_list(lines, (long long)"        for (void** cur = start; cur < end; cur++) {\n");
    ok = append_list(lines, (long long)"            void* p = *cur;\n");
    ok = append_list(lines, (long long)"            if (p) ep_gc_mark_object(p);\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* Mark phase: traverse from ALL threads' explicit GC roots.\n");
    ok = append_list(lines, (long long)"   Uses the heap-allocated EpThreadGCState instead of raw __thread pointers. */\n");
    ok = append_list(lines, (long long)"static void ep_gc_mark(void) {\n");
    ok = append_list(lines, (long long)"    ep_gc_scan_thread_stacks();  /* conservative C-stack scan of all (parked) threads — major only */\n");
    ok = append_list(lines, (long long)"    for (int t = 0; t < ep_num_threads; t++) {\n");
    ok = append_list(lines, (long long)"        if (!ep_thread_active[t]) continue;\n");
    ok = append_list(lines, (long long)"        EpThreadGCState* state = ep_thread_gc_states[t];\n");
    ok = append_list(lines, (long long)"        if (!state) continue;\n");
    ok = append_list(lines, (long long)"        int sp = state->sp;\n");
    ok = append_list(lines, (long long)"        if (sp <= 0 || sp > EP_GC_MAX_ROOTS) continue;\n");
    ok = append_list(lines, (long long)"        for (int i = 0; i < sp; i++) {\n");
    ok = append_list(lines, (long long)"            long long* root_ptr = state->roots[i];\n");
    ok = append_list(lines, (long long)"            if (!root_ptr) continue;\n");
    ok = append_list(lines, (long long)"            long long val = *root_ptr;\n");
    ok = append_list(lines, (long long)"            if (val != 0) {\n");
    ok = append_list(lines, (long long)"                ep_gc_mark_object((void*)val);\n");
    ok = append_list(lines, (long long)"            }\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    /* Also mark from main thread's local root stack (thread 0 / unregistered) */\n");
    ok = append_list(lines, (long long)"    int local_sp = ep_gc_root_sp;\n");
    ok = append_list(lines, (long long)"    if (local_sp > EP_GC_MAX_ROOTS) local_sp = EP_GC_MAX_ROOTS;\n");
    ok = append_list(lines, (long long)"    for (int i = 0; i < local_sp; i++) {\n");
    ok = append_list(lines, (long long)"        long long val = *ep_gc_root_stack[i];\n");
    ok = append_list(lines, (long long)"        if (val != 0) {\n");
    ok = append_list(lines, (long long)"            ep_gc_mark_object((void*)val);\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    /* Mark active tasks in the scheduler run queue */\n");
    ok = append_list(lines, (long long)"    EpTask* task = ep_run_queue_head;\n");
    ok = append_list(lines, (long long)"    while (task) {\n");
    ok = append_list(lines, (long long)"        if (task->fut) {\n");
    ok = append_list(lines, (long long)"            ep_gc_mark_object((void*)task->fut);\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"        if (task->args && task->args_size_bytes > 0) {\n");
    ok = append_list(lines, (long long)"            long long* ptr = (long long*)task->args;\n");
    ok = append_list(lines, (long long)"            for (int i = 0; i < task->args_size_bytes / 8; i++) {\n");
    ok = append_list(lines, (long long)"                long long val = ptr[i];\n");
    ok = append_list(lines, (long long)"                if (val != 0) ep_gc_mark_object((void*)val);\n");
    ok = append_list(lines, (long long)"            }\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"        task = task->next;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    /* Mark active tasks in the timers queue */\n");
    ok = append_list(lines, (long long)"    EpTimer* timer = ep_timers_head;\n");
    ok = append_list(lines, (long long)"    while (timer) {\n");
    ok = append_list(lines, (long long)"        if (timer->task) {\n");
    ok = append_list(lines, (long long)"            EpTask* t = timer->task;\n");
    ok = append_list(lines, (long long)"            if (t->fut) {\n");
    ok = append_list(lines, (long long)"                ep_gc_mark_object((void*)t->fut);\n");
    ok = append_list(lines, (long long)"            }\n");
    ok = append_list(lines, (long long)"            if (t->args && t->args_size_bytes > 0) {\n");
    ok = append_list(lines, (long long)"                long long* ptr = (long long*)t->args;\n");
    ok = append_list(lines, (long long)"                for (int i = 0; i < t->args_size_bytes / 8; i++) {\n");
    ok = append_list(lines, (long long)"                    long long val = ptr[i];\n");
    ok = append_list(lines, (long long)"                    if (val != 0) ep_gc_mark_object((void*)val);\n");
    ok = append_list(lines, (long long)"                }\n");
    ok = append_list(lines, (long long)"            }\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"        timer = timer->next;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    /* Mark top-level constant/global variables (roots outside any frame) */\n");
    ok = append_list(lines, (long long)"    if (ep_gc_mark_globals_major) ep_gc_mark_globals_major();\n");
    ok = append_list(lines, (long long)"    /* Scan all registered channel buffers — values in-transit have no root */\n");
    ok = append_list(lines, (long long)"    if (ep_gc_scan_channels_major) ep_gc_scan_channels_major();\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* Conservatively scan the CURRENT thread's own live C stack and mark any YOUNG object it\n");
    ok = append_list(lines, (long long)"   finds. This closes a use-after-free on the frequent minor path: a freshly-allocated\n");
    ok = append_list(lines, (long long)"   argument temporary — e.g. the result of g() while f(g() and h()) is still evaluating\n");
    ok = append_list(lines, (long long)"   h() — lives only on the C stack / in registers and is not yet on the precise shadow\n");
    ok = append_list(lines, (long long)"   stack, so a minor collection triggered mid-expression would otherwise free it. Scanning\n");
    ok = append_list(lines, (long long)"   ONLY the collecting thread's own stack is race-free (no cross-thread read) and cheap\n");
    ok = append_list(lines, (long long)"   (one bounded stack, current thread only). Non-pointer words are harmlessly ignored by\n");
    ok = append_list(lines, (long long)"   ep_gc_table_get; only generation-0 objects are marked. The setjmp spills register-held\n");
    ok = append_list(lines, (long long)"   roots onto the stack so the scan can see them. */\n");
    ok = append_list(lines, (long long)"EP_NO_ASAN\n");
    ok = append_list(lines, (long long)"static void ep_gc_scan_own_stack_minor(void) {\n");
    ok = append_list(lines, (long long)"    jmp_buf _regs;\n");
    ok = append_list(lines, (long long)"    volatile char _marker;\n");
    ok = append_list(lines, (long long)"    memset(&_regs, 0, sizeof(_regs));\n");
    ok = append_list(lines, (long long)"    setjmp(_regs);   /* spill callee-saved registers into _regs, on the stack */\n");
    ok = append_list(lines, (long long)"    void* bottom = ep_thread_local_bottom;\n");
    ok = append_list(lines, (long long)"    if (!bottom) return;\n");
    ok = append_list(lines, (long long)"    /* Start at the LOWEST of our own local addresses so the scanned range covers both\n");
    ok = append_list(lines, (long long)"       the current stack top (_marker) and the register-spill buffer (_regs), regardless\n");
    ok = append_list(lines, (long long)"       of how the compiler ordered these locals on the stack. Missing _regs would drop a\n");
    ok = append_list(lines, (long long)"       root held only in a callee-saved register -> a rare use-after-free. */\n");
    ok = append_list(lines, (long long)"    char* a = (char*)(void*)&_marker;\n");
    ok = append_list(lines, (long long)"    char* b = (char*)(void*)&_regs;\n");
    ok = append_list(lines, (long long)"    char* lo = (a < b) ? a : b;\n");
    ok = append_list(lines, (long long)"    /* lo comes from a char local, so it may not be pointer-aligned; mask DOWN to 8\n");
    ok = append_list(lines, (long long)"       bytes. Aligning down only widens the conservative window by a few harmless\n");
    ok = append_list(lines, (long long)"       bytes — aligning up could skip the slot holding a live root. Unaligned void**\n");
    ok = append_list(lines, (long long)"       dereferences are UB and skew the scan window on strict platforms (valgrind). */\n");
    ok = append_list(lines, (long long)"    void** start = (void**)((uintptr_t)lo & ~(uintptr_t)7);\n");
    ok = append_list(lines, (long long)"    void** end = (void**)bottom;\n");
    ok = append_list(lines, (long long)"    if (start > end) { void** tmp = start; start = end; end = tmp; }\n");
    ok = append_list(lines, (long long)"    for (void** cur = start; cur < end; cur++) {\n");
    ok = append_list(lines, (long long)"        void* p = *cur;\n");
    ok = append_list(lines, (long long)"        if (p) ep_gc_mark_object_minor(p);\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"static void ep_gc_mark_minor(void) {\n");
    ok = append_list(lines, (long long)"    /* Conservatively scan our OWN live C stack first, to catch freshly-allocated argument\n");
    ok = append_list(lines, (long long)"       temporaries (only on the stack / in registers, not yet on the shadow stack) that a\n");
    ret_val = join_strings(lines);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(1);
    return ret_val;
}

long long ep_rt_core_10() {
    long long lines = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&lines);
    ep_gc_maybe_collect();

    lines = create_list();
    ok = append_list(lines, (long long)"       minor collection mid-expression would otherwise free. Own-thread only, so race-free. */\n");
    ok = append_list(lines, (long long)"    ep_gc_scan_own_stack_minor();\n");
    ok = append_list(lines, (long long)"    for (int t = 0; t < ep_num_threads; t++) {\n");
    ok = append_list(lines, (long long)"        if (!ep_thread_active[t]) continue;\n");
    ok = append_list(lines, (long long)"        EpThreadGCState* state = ep_thread_gc_states[t];\n");
    ok = append_list(lines, (long long)"        if (!state) continue;\n");
    ok = append_list(lines, (long long)"        int sp = state->sp;\n");
    ok = append_list(lines, (long long)"        if (sp <= 0 || sp > EP_GC_MAX_ROOTS) continue;\n");
    ok = append_list(lines, (long long)"        for (int i = 0; i < sp; i++) {\n");
    ok = append_list(lines, (long long)"            long long* root_ptr = state->roots[i];\n");
    ok = append_list(lines, (long long)"            if (!root_ptr) continue;\n");
    ok = append_list(lines, (long long)"            long long val = *root_ptr;\n");
    ok = append_list(lines, (long long)"            if (val != 0) {\n");
    ok = append_list(lines, (long long)"                ep_gc_mark_object_minor((void*)val);\n");
    ok = append_list(lines, (long long)"            }\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    int local_sp = ep_gc_root_sp;\n");
    ok = append_list(lines, (long long)"    if (local_sp > EP_GC_MAX_ROOTS) local_sp = EP_GC_MAX_ROOTS;\n");
    ok = append_list(lines, (long long)"    for (int i = 0; i < local_sp; i++) {\n");
    ok = append_list(lines, (long long)"        long long val = *ep_gc_root_stack[i];\n");
    ok = append_list(lines, (long long)"        if (val != 0) {\n");
    ok = append_list(lines, (long long)"            ep_gc_mark_object_minor((void*)val);\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    /* Mark active tasks in the scheduler run queue for minor collection */\n");
    ok = append_list(lines, (long long)"    EpTask* task = ep_run_queue_head;\n");
    ok = append_list(lines, (long long)"    while (task) {\n");
    ok = append_list(lines, (long long)"        if (task->fut) {\n");
    ok = append_list(lines, (long long)"            ep_gc_mark_object_minor((void*)task->fut);\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"        if (task->args && task->args_size_bytes > 0) {\n");
    ok = append_list(lines, (long long)"            long long* ptr = (long long*)task->args;\n");
    ok = append_list(lines, (long long)"            for (int i = 0; i < task->args_size_bytes / 8; i++) {\n");
    ok = append_list(lines, (long long)"                long long val = ptr[i];\n");
    ok = append_list(lines, (long long)"                if (val != 0) ep_gc_mark_object_minor((void*)val);\n");
    ok = append_list(lines, (long long)"            }\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"        task = task->next;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    /* Mark active tasks in the timers queue for minor collection */\n");
    ok = append_list(lines, (long long)"    EpTimer* timer = ep_timers_head;\n");
    ok = append_list(lines, (long long)"    while (timer) {\n");
    ok = append_list(lines, (long long)"        if (timer->task) {\n");
    ok = append_list(lines, (long long)"            EpTask* t = timer->task;\n");
    ok = append_list(lines, (long long)"            if (t->fut) {\n");
    ok = append_list(lines, (long long)"                ep_gc_mark_object_minor((void*)t->fut);\n");
    ok = append_list(lines, (long long)"            }\n");
    ok = append_list(lines, (long long)"            if (t->args && t->args_size_bytes > 0) {\n");
    ok = append_list(lines, (long long)"                long long* ptr = (long long*)t->args;\n");
    ok = append_list(lines, (long long)"                for (int i = 0; i < t->args_size_bytes / 8; i++) {\n");
    ok = append_list(lines, (long long)"                    long long val = ptr[i];\n");
    ok = append_list(lines, (long long)"                    if (val != 0) ep_gc_mark_object_minor((void*)val);\n");
    ok = append_list(lines, (long long)"                }\n");
    ok = append_list(lines, (long long)"            }\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"        timer = timer->next;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    /* Also mark from the remembered set */\n");
    ok = append_list(lines, (long long)"    for (long long i = 0; i < ep_gc_remembered_size; i++) {\n");
    ok = append_list(lines, (long long)"        ep_gc_mark_object_minor(ep_gc_remembered_set[i]);\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    /* Mark top-level constant/global variables (roots outside any frame) */\n");
    ok = append_list(lines, (long long)"    if (ep_gc_mark_globals_minor) ep_gc_mark_globals_minor();\n");
    ok = append_list(lines, (long long)"    /* Scan all registered channel buffers — values in-transit have no root */\n");
    ok = append_list(lines, (long long)"    if (ep_gc_scan_channels_minor) ep_gc_scan_channels_minor();\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"static void ep_gc_sweep_minor(void) {\n");
    ok = append_list(lines, (long long)"    EpGCObject** cur = &ep_gc_head;\n");
    ok = append_list(lines, (long long)"    while (*cur) {\n");
    ok = append_list(lines, (long long)"        if ((*cur)->generation == 0) {\n");
    ok = append_list(lines, (long long)"            if (!(*cur)->marked) {\n");
    ok = append_list(lines, (long long)"                EpGCObject* garbage = *cur;\n");
    ok = append_list(lines, (long long)"                *cur = garbage->next;\n");
    ok = append_list(lines, (long long)"                ep_gc_table_remove(garbage->ptr);\n");
    ok = append_list(lines, (long long)"                if (garbage->kind == EP_OBJ_LIST) {\n");
    ok = append_list(lines, (long long)"                    EpList* list = (EpList*)garbage->ptr;\n");
    ok = append_list(lines, (long long)"                    if (list) {\n");
    ok = append_list(lines, (long long)"                        free(list->data);\n");
    ok = append_list(lines, (long long)"                        free(list);\n");
    ok = append_list(lines, (long long)"                    }\n");
    ok = append_list(lines, (long long)"                } else if (garbage->kind == EP_OBJ_STRING) {\n");
    ok = append_list(lines, (long long)"                    free(garbage->ptr);\n");
    ok = append_list(lines, (long long)"                } else if (garbage->kind == EP_OBJ_STRUCT) {\n");
    ok = append_list(lines, (long long)"                    free(garbage->ptr);\n");
    ok = append_list(lines, (long long)"                } else if (garbage->kind == EP_OBJ_CLOSURE) {\n");
    ok = append_list(lines, (long long)"                    free(garbage->ptr);\n");
    ok = append_list(lines, (long long)"                } else if (garbage->kind == EP_OBJ_MAP) {\n");
    ok = append_list(lines, (long long)"                    /* EpMap layout: entries*, capacity, size. Free entries then map. */\n");
    ok = append_list(lines, (long long)"                    void** map_fields = (void**)garbage->ptr;\n");
    ok = append_list(lines, (long long)"                    if (map_fields && map_fields[0]) free(map_fields[0]); /* entries */\n");
    ok = append_list(lines, (long long)"                    free(garbage->ptr);\n");
    ok = append_list(lines, (long long)"                }\n");
    ok = append_list(lines, (long long)"                free(garbage);\n");
    ok = append_list(lines, (long long)"                ep_gc_count--;\n");
    ok = append_list(lines, (long long)"                ep_gc_nursery_count--;\n");
    ok = append_list(lines, (long long)"            } else {\n");
    ok = append_list(lines, (long long)"                (*cur)->marked = 0;\n");
    ok = append_list(lines, (long long)"                (*cur)->generation = 1;\n");
    ok = append_list(lines, (long long)"                ep_gc_nursery_count--;\n");
    ok = append_list(lines, (long long)"                cur = &(*cur)->next;\n");
    ok = append_list(lines, (long long)"            }\n");
    ok = append_list(lines, (long long)"        } else {\n");
    ok = append_list(lines, (long long)"            cur = &(*cur)->next;\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    ep_gc_remembered_size = 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"static void ep_gc_sweep_major(void) {\n");
    ok = append_list(lines, (long long)"    EpGCObject** cur = &ep_gc_head;\n");
    ok = append_list(lines, (long long)"    while (*cur) {\n");
    ok = append_list(lines, (long long)"        if (!(*cur)->marked) {\n");
    ok = append_list(lines, (long long)"            EpGCObject* garbage = *cur;\n");
    ok = append_list(lines, (long long)"            *cur = garbage->next;\n");
    ok = append_list(lines, (long long)"            ep_gc_table_remove(garbage->ptr);\n");
    ok = append_list(lines, (long long)"            if (garbage->generation == 0) {\n");
    ok = append_list(lines, (long long)"                ep_gc_nursery_count--;\n");
    ok = append_list(lines, (long long)"            }\n");
    ok = append_list(lines, (long long)"            if (garbage->kind == EP_OBJ_LIST) {\n");
    ok = append_list(lines, (long long)"                EpList* list = (EpList*)garbage->ptr;\n");
    ok = append_list(lines, (long long)"                if (list) {\n");
    ok = append_list(lines, (long long)"                    free(list->data);\n");
    ok = append_list(lines, (long long)"                    free(list);\n");
    ok = append_list(lines, (long long)"                }\n");
    ok = append_list(lines, (long long)"            } else if (garbage->kind == EP_OBJ_STRING) {\n");
    ok = append_list(lines, (long long)"                free(garbage->ptr);\n");
    ok = append_list(lines, (long long)"            } else if (garbage->kind == EP_OBJ_STRUCT) {\n");
    ok = append_list(lines, (long long)"                free(garbage->ptr);\n");
    ok = append_list(lines, (long long)"            } else if (garbage->kind == EP_OBJ_CLOSURE) {\n");
    ok = append_list(lines, (long long)"                free(garbage->ptr);\n");
    ok = append_list(lines, (long long)"            } else if (garbage->kind == EP_OBJ_MAP) {\n");
    ok = append_list(lines, (long long)"                void** map_fields = (void**)garbage->ptr;\n");
    ok = append_list(lines, (long long)"                if (map_fields && map_fields[0]) free(map_fields[0]);\n");
    ok = append_list(lines, (long long)"                free(garbage->ptr);\n");
    ok = append_list(lines, (long long)"            }\n");
    ok = append_list(lines, (long long)"            free(garbage);\n");
    ok = append_list(lines, (long long)"            ep_gc_count--;\n");
    ok = append_list(lines, (long long)"        } else {\n");
    ok = append_list(lines, (long long)"            (*cur)->marked = 0;\n");
    ok = append_list(lines, (long long)"            if ((*cur)->generation == 0) {\n");
    ok = append_list(lines, (long long)"                (*cur)->generation = 1;\n");
    ok = append_list(lines, (long long)"                ep_gc_nursery_count--;\n");
    ok = append_list(lines, (long long)"            }\n");
    ok = append_list(lines, (long long)"            cur = &(*cur)->next;\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    ep_gc_remembered_size = 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ret_val = join_strings(lines);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(1);
    return ret_val;
}

long long ep_rt_core_11() {
    long long lines = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&lines);
    ep_gc_maybe_collect();

    lines = create_list();
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"static void ep_gc_collect_minor(void) {\n");
    ok = append_list(lines, (long long)"    if (!ep_gc_enabled) return;\n");
    ok = append_list(lines, (long long)"    ep_gc_minor_count++;\n");
    ok = append_list(lines, (long long)"    ep_gc_mark_minor();\n");
    ok = append_list(lines, (long long)"    ep_gc_sweep_minor();\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"static void ep_gc_collect_major(void) {\n");
    ok = append_list(lines, (long long)"    if (!ep_gc_enabled) return;\n");
    ok = append_list(lines, (long long)"    ep_gc_major_count++;\n");
    ok = append_list(lines, (long long)"    ep_gc_mark();\n");
    ok = append_list(lines, (long long)"    ep_gc_sweep_major();\n");
    ok = append_list(lines, (long long)"    ep_gc_threshold = ep_gc_count * 2;\n");
    ok = append_list(lines, (long long)"    if (ep_gc_threshold < 4096) ep_gc_threshold = 4096;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* Run a full GC collection — caller MUST hold ep_gc_mutex */\n");
    ok = append_list(lines, (long long)"static void ep_gc_collect(void) {\n");
    ok = append_list(lines, (long long)"    ep_gc_collect_major();\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* Maybe trigger GC if we've exceeded threshold. Also serves as the per-function\n");
    ok = append_list(lines, (long long)"   GC safepoint: if another thread has stopped the world, park here until it's done. */\n");
    ok = append_list(lines, (long long)"static void ep_gc_maybe_collect(void) {\n");
    ok = append_list(lines, (long long)"    if (!ep_gc_enabled) return;  /* Early exit if GC suppressed (e.g. during channel ops) */\n");
    ok = append_list(lines, (long long)"    /* Safepoint: lock-free fast check, then park under the lock if a collection\n");
    ok = append_list(lines, (long long)"       is in progress on another thread. Keeps the no-GC path lock-free. */\n");
    ok = append_list(lines, (long long)"    if (ep_gc_stop_requested) {\n");
    ok = append_list(lines, (long long)"        pthread_mutex_lock(&ep_gc_mutex);\n");
    ok = append_list(lines, (long long)"        ep_gc_park_if_stopped();\n");
    ok = append_list(lines, (long long)"        pthread_mutex_unlock(&ep_gc_mutex);\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    /* Fast path: check thresholds before acquiring mutex.\n");
    ok = append_list(lines, (long long)"       Counters are only incremented under the mutex, so worst case\n");
    ok = append_list(lines, (long long)"       we miss one collection cycle — safe trade-off for avoiding\n");
    ok = append_list(lines, (long long)"       a mutex lock/unlock (~20-50ns) on every function call. */\n");
    ok = append_list(lines, (long long)"    if (ep_gc_nursery_count < ep_gc_nursery_threshold && ep_gc_count < ep_gc_threshold) return;\n");
    ok = append_list(lines, (long long)"    EP_GC_UPDATE_TOP();\n");
    ok = append_list(lines, (long long)"    pthread_mutex_lock(&ep_gc_mutex);\n");
    ok = append_list(lines, (long long)"    /* Another thread may have started collecting between the check and the lock —\n");
    ok = append_list(lines, (long long)"       park instead of racing it, then re-check thresholds under the lock. */\n");
    ok = append_list(lines, (long long)"    ep_gc_park_if_stopped();\n");
    ok = append_list(lines, (long long)"    if (ep_gc_nursery_count >= ep_gc_nursery_threshold || ep_gc_count >= ep_gc_threshold) {\n");
    ok = append_list(lines, (long long)"        ep_gc_stop_the_world();\n");
    ok = append_list(lines, (long long)"        if (ep_gc_nursery_count >= ep_gc_nursery_threshold) {\n");
    ok = append_list(lines, (long long)"            ep_gc_collect_minor();\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"        if (ep_gc_count >= ep_gc_threshold) {\n");
    ok = append_list(lines, (long long)"            ep_gc_collect_major();\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"        ep_gc_start_the_world();\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    pthread_mutex_unlock(&ep_gc_mutex);\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* Unregister an object (for explicit free — removes from GC tracking) */\n");
    ok = append_list(lines, (long long)"static void ep_gc_unregister(void* ptr) {\n");
    ok = append_list(lines, (long long)"    if (!ptr) return;\n");
    ok = append_list(lines, (long long)"    pthread_mutex_lock(&ep_gc_mutex);\n");
    ok = append_list(lines, (long long)"    ep_gc_park_if_stopped();  /* safepoint: don't mutate the table mid-collection */\n");
    ok = append_list(lines, (long long)"    /* Clean up references from the remembered set to prevent dangling pointers */\n");
    ok = append_list(lines, (long long)"    for (long long i = 0; i < ep_gc_remembered_size; ) {\n");
    ok = append_list(lines, (long long)"        if (ep_gc_remembered_set[i] == ptr) {\n");
    ok = append_list(lines, (long long)"            for (long long j = i; j < ep_gc_remembered_size - 1; j++) {\n");
    ok = append_list(lines, (long long)"                ep_gc_remembered_set[j] = ep_gc_remembered_set[j + 1];\n");
    ok = append_list(lines, (long long)"            }\n");
    ok = append_list(lines, (long long)"            ep_gc_remembered_size--;\n");
    ok = append_list(lines, (long long)"        } else {\n");
    ok = append_list(lines, (long long)"            i++;\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    ep_gc_table_remove(ptr);\n");
    ok = append_list(lines, (long long)"    EpGCObject** cur = &ep_gc_head;\n");
    ok = append_list(lines, (long long)"    while (*cur) {\n");
    ok = append_list(lines, (long long)"        if ((*cur)->ptr == ptr) {\n");
    ok = append_list(lines, (long long)"            EpGCObject* found = *cur;\n");
    ok = append_list(lines, (long long)"            *cur = found->next;\n");
    ok = append_list(lines, (long long)"            if (found->generation == 0) {\n");
    ok = append_list(lines, (long long)"                ep_gc_nursery_count--;\n");
    ok = append_list(lines, (long long)"            }\n");
    ok = append_list(lines, (long long)"            free(found);\n");
    ok = append_list(lines, (long long)"            ep_gc_count--;\n");
    ok = append_list(lines, (long long)"            pthread_mutex_unlock(&ep_gc_mutex);\n");
    ok = append_list(lines, (long long)"            return;\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"        cur = &(*cur)->next;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    pthread_mutex_unlock(&ep_gc_mutex);\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* Cleanup all remaining GC objects (called at program exit) */\n");
    ok = append_list(lines, (long long)"static void ep_gc_shutdown(void) {\n");
    ok = append_list(lines, (long long)"    ep_gc_enabled = 0;\n");
    ok = append_list(lines, (long long)"    /* Only free GC bookkeeping structures, not the tracked objects themselves.\n");
    ok = append_list(lines, (long long)"       The RAII auto-cleanup has already freed owned objects, and the OS will\n");
    ok = append_list(lines, (long long)"       reclaim everything else on process exit. Attempting to free individual\n");
    ok = append_list(lines, (long long)"       objects here causes double-free aborts when RAII and GC both track\n");
    ok = append_list(lines, (long long)"       the same allocation. */\n");
    ok = append_list(lines, (long long)"    EpGCObject* cur = ep_gc_head;\n");
    ok = append_list(lines, (long long)"    while (cur) {\n");
    ok = append_list(lines, (long long)"        EpGCObject* next = cur->next;\n");
    ok = append_list(lines, (long long)"        free(cur);  /* free the GCObject wrapper only */\n");
    ok = append_list(lines, (long long)"        cur = next;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    ep_gc_head = NULL;\n");
    ok = append_list(lines, (long long)"    ep_gc_count = 0;\n");
    ok = append_list(lines, (long long)"    if (ep_gc_table) {\n");
    ok = append_list(lines, (long long)"        free(ep_gc_table);\n");
    ok = append_list(lines, (long long)"        ep_gc_table = NULL;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    ep_gc_table_cap = 0;\n");
    ok = append_list(lines, (long long)"    ep_gc_table_size = 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* ========== End Garbage Collector ========== */\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long create_list(void);\n");
    ok = append_list(lines, (long long)"long long append_list(long long list_ptr, long long value);\n");
    ok = append_list(lines, (long long)"long long get_list(long long list_ptr, long long index);\n");
    ok = append_list(lines, (long long)"long long set_list(long long list_ptr, long long index, long long value);\n");
    ok = append_list(lines, (long long)"long long length_list(long long list_ptr);\n");
    ok = append_list(lines, (long long)"long long free_list(long long list_ptr);\n");
    ok = append_list(lines, (long long)"long long pop_list(long long list_ptr);\n");
    ok = append_list(lines, (long long)"long long remove_list(long long list_ptr, long long index);\n");
    ok = append_list(lines, (long long)"char* string_from_list(long long list_ptr);\n");
    ok = append_list(lines, (long long)"long long string_to_list(const char* s);\n");
    ok = append_list(lines, (long long)"long long string_length(const char* s);\n");
    ok = append_list(lines, (long long)"long long display_string(const char* s);\n");
    ok = append_list(lines, (long long)"long long screen_write(const char* s);\n");
    ok = append_list(lines, (long long)"long long file_read(long long path_val);\n");
    ok = append_list(lines, (long long)"long long file_write(long long path_val, long long content_val);\n");
    ok = append_list(lines, (long long)"long long file_append(long long path_val, long long content_val);\n");
    ok = append_list(lines, (long long)"long long file_exists(long long path_val);\n");
    ok = append_list(lines, (long long)"long long string_contains(long long s_val, long long sub_val);\n");
    ok = append_list(lines, (long long)"long long string_index_of(long long s_val, long long sub_val);\n");
    ok = append_list(lines, (long long)"long long string_replace(long long s_val, long long old_val, long long new_val);\n");
    ok = append_list(lines, (long long)"long long string_upper(long long s_val);\n");
    ok = append_list(lines, (long long)"long long string_lower(long long s_val);\n");
    ok = append_list(lines, (long long)"long long string_trim(long long s_val);\n");
    ok = append_list(lines, (long long)"long long string_split(long long s_val, long long delim_val);\n");
    ok = append_list(lines, (long long)"long long char_at(long long s_val, long long index);\n");
    ok = append_list(lines, (long long)"long long char_from_code(long long code);\n");
    ok = append_list(lines, (long long)"long long ep_abs(long long n);\n");
    ok = append_list(lines, (long long)"long long json_get_string(long long json_val, long long key_val);\n");
    ok = append_list(lines, (long long)"long long json_get_int(long long json_val, long long key_val);\n");
    ok = append_list(lines, (long long)"long long json_get_bool(long long json_val, long long key_val);\n");
    ok = append_list(lines, (long long)"long long ep_sha1(long long data_val);\n");
    ok = append_list(lines, (long long)"long long ep_net_recv_bytes(long long fd, long long count);\n");
    ok = append_list(lines, (long long)"long long ep_net_send_raw(long long fd, long long data_ptr, long long count);\n");
    ret_val = join_strings(lines);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(1);
    return ret_val;
}

long long ep_rt_core_12() {
    long long lines = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&lines);
    ep_gc_maybe_collect();

    lines = create_list();
    ok = append_list(lines, (long long)"long long channel_try_recv(long long chan_ptr, long long out_ptr);\n");
    ok = append_list(lines, (long long)"long long channel_has_data(long long chan_ptr);\n");
    ok = append_list(lines, (long long)"long long channel_select(long long channels_list, long long timeout_ms);\n");
    ok = append_list(lines, (long long)"long long ep_auto_to_string(long long val);\n");
    ok = append_list(lines, (long long)"long long ep_float_to_string(long long bits);\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"typedef struct EpChannel_ {\n");
    ok = append_list(lines, (long long)"    long long* data;\n");
    ok = append_list(lines, (long long)"    long long capacity;\n");
    ok = append_list(lines, (long long)"    long long head;\n");
    ok = append_list(lines, (long long)"    long long tail;\n");
    ok = append_list(lines, (long long)"    long long size;\n");
    ok = append_list(lines, (long long)"    ep_mutex_t mutex;\n");
    ok = append_list(lines, (long long)"    ep_cond_t cond_recv;\n");
    ok = append_list(lines, (long long)"    ep_cond_t cond_send;\n");
    ok = append_list(lines, (long long)"} EpChannel;\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* Global channel registry — allows GC to scan values in-transit in channel buffers.\n");
    ok = append_list(lines, (long long)"   Without this, an object sent to a channel but not yet received has NO GC root:\n");
    ok = append_list(lines, (long long)"   the sender has popped it, the receiver hasn't pushed it, and the channel buffer\n");
    ok = append_list(lines, (long long)"   is not scanned. The GC sweeps it → receiver gets a dangling pointer. */\n");
    ok = append_list(lines, (long long)"#define EP_MAX_CHANNELS 1024\n");
    ok = append_list(lines, (long long)"static EpChannel* ep_channel_registry[EP_MAX_CHANNELS];\n");
    ok = append_list(lines, (long long)"static int ep_channel_count = 0;\n");
    ok = append_list(lines, (long long)"static pthread_mutex_t ep_channel_registry_mutex = PTHREAD_MUTEX_INITIALIZER;\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"static void ep_register_channel(EpChannel* chan) {\n");
    ok = append_list(lines, (long long)"    pthread_mutex_lock(&ep_channel_registry_mutex);\n");
    ok = append_list(lines, (long long)"    if (ep_channel_count < EP_MAX_CHANNELS) {\n");
    ok = append_list(lines, (long long)"        ep_channel_registry[ep_channel_count++] = chan;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    pthread_mutex_unlock(&ep_channel_registry_mutex);\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* Channel scanning implementations — called by GC mark via function pointers.\n");
    ok = append_list(lines, (long long)"   These are defined here (after EpChannel) so they can access struct fields. */\n");
    ok = append_list(lines, (long long)"static void ep_gc_mark_object(void* ptr);     /* forward decl */\n");
    ok = append_list(lines, (long long)"static void ep_gc_mark_object_minor(void* ptr); /* forward decl */\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"static void ep_gc_scan_channels_major_impl(void) {\n");
    ok = append_list(lines, (long long)"    pthread_mutex_lock(&ep_channel_registry_mutex);\n");
    ok = append_list(lines, (long long)"    for (int c = 0; c < ep_channel_count; c++) {\n");
    ok = append_list(lines, (long long)"        EpChannel* chan = ep_channel_registry[c];\n");
    ok = append_list(lines, (long long)"        if (!chan || chan->size <= 0) continue;\n");
    ok = append_list(lines, (long long)"        ep_mutex_lock(&chan->mutex);\n");
    ok = append_list(lines, (long long)"        for (long long j = 0; j < chan->size; j++) {\n");
    ok = append_list(lines, (long long)"            long long idx = (chan->head + j) % chan->capacity;\n");
    ok = append_list(lines, (long long)"            long long val = chan->data[idx];\n");
    ok = append_list(lines, (long long)"            if (val != 0) ep_gc_mark_object((void*)val);\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"        ep_mutex_unlock(&chan->mutex);\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    pthread_mutex_unlock(&ep_channel_registry_mutex);\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"static void ep_gc_scan_channels_minor_impl(void) {\n");
    ok = append_list(lines, (long long)"    pthread_mutex_lock(&ep_channel_registry_mutex);\n");
    ok = append_list(lines, (long long)"    for (int c = 0; c < ep_channel_count; c++) {\n");
    ok = append_list(lines, (long long)"        EpChannel* chan = ep_channel_registry[c];\n");
    ok = append_list(lines, (long long)"        if (!chan || chan->size <= 0) continue;\n");
    ok = append_list(lines, (long long)"        ep_mutex_lock(&chan->mutex);\n");
    ok = append_list(lines, (long long)"        for (long long j = 0; j < chan->size; j++) {\n");
    ok = append_list(lines, (long long)"            long long idx = (chan->head + j) % chan->capacity;\n");
    ok = append_list(lines, (long long)"            long long val = chan->data[idx];\n");
    ok = append_list(lines, (long long)"            if (val != 0) ep_gc_mark_object_minor((void*)val);\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"        ep_mutex_unlock(&chan->mutex);\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    pthread_mutex_unlock(&ep_channel_registry_mutex);\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long create_channel(void) {\n");
    ok = append_list(lines, (long long)"    EpChannel* chan = malloc(sizeof(EpChannel));\n");
    ok = append_list(lines, (long long)"    if (!chan) return 0;\n");
    ok = append_list(lines, (long long)"    chan->capacity = 1024;\n");
    ok = append_list(lines, (long long)"    chan->data = malloc(chan->capacity * sizeof(long long));\n");
    ok = append_list(lines, (long long)"    chan->head = 0;\n");
    ok = append_list(lines, (long long)"    chan->tail = 0;\n");
    ok = append_list(lines, (long long)"    chan->size = 0;\n");
    ok = append_list(lines, (long long)"    ep_mutex_init(&chan->mutex);\n");
    ok = append_list(lines, (long long)"    ep_cond_init(&chan->cond_recv);\n");
    ok = append_list(lines, (long long)"    ep_cond_init(&chan->cond_send);\n");
    ok = append_list(lines, (long long)"    ep_register_channel(chan);\n");
    ok = append_list(lines, (long long)"    return (long long)chan;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long send_channel(long long chan_ptr, long long value) {\n");
    ok = append_list(lines, (long long)"    EpChannel* chan = (EpChannel*)chan_ptr;\n");
    ok = append_list(lines, (long long)"    if (!chan) return 0;\n");
    ok = append_list(lines, (long long)"    /* Suppress GC during channel operations. The blocking condvar wait\n");
    ok = append_list(lines, (long long)"       can interleave with GC mark/sweep on another thread, causing\n");
    ok = append_list(lines, (long long)"       use-after-free when the GC sweeps objects that are live on a\n");
    ok = append_list(lines, (long long)"       thread currently blocked in send/receive. Channel buffers contain\n");
    ok = append_list(lines, (long long)"       raw long long values (not GC-tracked pointers), so suppressing\n");
    ok = append_list(lines, (long long)"       GC here is safe. */\n");
    ok = append_list(lines, (long long)"    int gc_was_enabled = ep_gc_enabled;\n");
    ok = append_list(lines, (long long)"    ep_gc_enabled = 0;\n");
    ok = append_list(lines, (long long)"    ep_mutex_lock(&chan->mutex);\n");
    ok = append_list(lines, (long long)"    while (chan->size >= chan->capacity) {\n");
    ok = append_list(lines, (long long)"        ep_cond_wait(&chan->cond_send, &chan->mutex);\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    chan->data[chan->tail] = value;\n");
    ok = append_list(lines, (long long)"    chan->tail = (chan->tail + 1) % chan->capacity;\n");
    ok = append_list(lines, (long long)"    chan->size += 1;\n");
    ok = append_list(lines, (long long)"    ep_cond_signal(&chan->cond_recv);\n");
    ok = append_list(lines, (long long)"    ep_mutex_unlock(&chan->mutex);\n");
    ok = append_list(lines, (long long)"    ep_gc_enabled = gc_was_enabled;\n");
    ok = append_list(lines, (long long)"    return value;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long receive_channel(long long chan_ptr) {\n");
    ok = append_list(lines, (long long)"    EpChannel* chan = (EpChannel*)chan_ptr;\n");
    ok = append_list(lines, (long long)"    if (!chan) return 0;\n");
    ok = append_list(lines, (long long)"    /* Suppress GC during channel receive — same rationale as send_channel */\n");
    ok = append_list(lines, (long long)"    int gc_was_enabled = ep_gc_enabled;\n");
    ok = append_list(lines, (long long)"    ep_gc_enabled = 0;\n");
    ok = append_list(lines, (long long)"    ep_mutex_lock(&chan->mutex);\n");
    ok = append_list(lines, (long long)"    while (chan->size <= 0) {\n");
    ok = append_list(lines, (long long)"        ep_cond_wait(&chan->cond_recv, &chan->mutex);\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    long long value = chan->data[chan->head];\n");
    ok = append_list(lines, (long long)"    chan->head = (chan->head + 1) % chan->capacity;\n");
    ok = append_list(lines, (long long)"    chan->size -= 1;\n");
    ok = append_list(lines, (long long)"    ep_cond_signal(&chan->cond_send);\n");
    ok = append_list(lines, (long long)"    ep_mutex_unlock(&chan->mutex);\n");
    ok = append_list(lines, (long long)"    ep_gc_enabled = gc_was_enabled;\n");
    ok = append_list(lines, (long long)"    return value;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"// Non-blocking receive — returns 1 if data was available, 0 if channel empty\n");
    ok = append_list(lines, (long long)"long long channel_try_recv(long long chan_ptr, long long out_ptr) {\n");
    ok = append_list(lines, (long long)"    EpChannel* chan = (EpChannel*)chan_ptr;\n");
    ok = append_list(lines, (long long)"    if (!chan) return 0;\n");
    ok = append_list(lines, (long long)"    ep_mutex_lock(&chan->mutex);\n");
    ok = append_list(lines, (long long)"    if (chan->size <= 0) {\n");
    ok = append_list(lines, (long long)"        ep_mutex_unlock(&chan->mutex);\n");
    ok = append_list(lines, (long long)"        return 0;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    long long value = chan->data[chan->head];\n");
    ok = append_list(lines, (long long)"    chan->head = (chan->head + 1) % chan->capacity;\n");
    ok = append_list(lines, (long long)"    chan->size -= 1;\n");
    ok = append_list(lines, (long long)"    ep_cond_signal(&chan->cond_send);\n");
    ok = append_list(lines, (long long)"    ep_mutex_unlock(&chan->mutex);\n");
    ok = append_list(lines, (long long)"    if (out_ptr) {\n");
    ok = append_list(lines, (long long)"        *((long long*)out_ptr) = value;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    return 1;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"// Check if channel has data without consuming it\n");
    ret_val = join_strings(lines);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(1);
    return ret_val;
}

long long ep_rt_core_13() {
    long long lines = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&lines);
    ep_gc_maybe_collect();

    lines = create_list();
    ok = append_list(lines, (long long)"long long channel_has_data(long long chan_ptr) {\n");
    ok = append_list(lines, (long long)"    EpChannel* chan = (EpChannel*)chan_ptr;\n");
    ok = append_list(lines, (long long)"    if (!chan) return 0;\n");
    ok = append_list(lines, (long long)"    ep_mutex_lock(&chan->mutex);\n");
    ok = append_list(lines, (long long)"    int has = (chan->size > 0) ? 1 : 0;\n");
    ok = append_list(lines, (long long)"    ep_mutex_unlock(&chan->mutex);\n");
    ok = append_list(lines, (long long)"    return has;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* Leave a GC-safe blocking region (see channel_select). If a collection is\n");
    ok = append_list(lines, (long long)"   in progress, wait for it to finish before running mutator code again. */\n");
    ok = append_list(lines, (long long)"static void ep_gc_leave_blocking_region(void) {\n");
    ok = append_list(lines, (long long)"    if (ep_thread_slot < 0) return;\n");
    ok = append_list(lines, (long long)"    pthread_mutex_lock(&ep_gc_mutex);\n");
    ok = append_list(lines, (long long)"    while (ep_gc_stop_requested) {\n");
    ok = append_list(lines, (long long)"        pthread_cond_wait(&ep_gc_resume_cond, &ep_gc_mutex);\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    ep_gc_parked_count--;\n");
    ok = append_list(lines, (long long)"    pthread_mutex_unlock(&ep_gc_mutex);\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"// Select: wait for any of N channels to have data, with timeout in ms\n");
    ok = append_list(lines, (long long)"// channels_list is a list of channel pointers\n");
    ok = append_list(lines, (long long)"// Returns index (0-based) of first ready channel, or -1 on timeout\n");
    ok = append_list(lines, (long long)"long long channel_select(long long channels_list, long long timeout_ms) {\n");
    ok = append_list(lines, (long long)"    EpList* list = (EpList*)channels_list;\n");
    ok = append_list(lines, (long long)"    if (!list || list->length == 0) return -1;\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"#ifdef _WIN32\n");
    ok = append_list(lines, (long long)"    ULONGLONG start_tick = GetTickCount64();\n");
    ok = append_list(lines, (long long)"#else\n");
    ok = append_list(lines, (long long)"    struct timespec start, now;\n");
    ok = append_list(lines, (long long)"    clock_gettime(CLOCK_MONOTONIC, &start);\n");
    ok = append_list(lines, (long long)"#endif\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"    /* A GC-safe blocking region. A thread sitting in a long select must not\n");
    ok = append_list(lines, (long long)"       stall every stop-the-world (waking it to park costs ~0.5ms per major\n");
    ok = append_list(lines, (long long)"       collection — allocation-heavy code on another thread pays that tens of\n");
    ok = append_list(lines, (long long)"       thousands of times). Instead, pin this frame's roots once — spill the\n");
    ok = append_list(lines, (long long)"       callee-saved registers into this frame and publish the frame as the\n");
    ok = append_list(lines, (long long)"       thread's stack top — and count the thread as already parked. The\n");
    ok = append_list(lines, (long long)"       collector then proceeds immediately and scans [this frame, stack\n");
    ok = append_list(lines, (long long)"       bottom], a range that is frozen for the whole select: the poll loop\n");
    ok = append_list(lines, (long long)"       below only grows the stack DOWNWARD from here (excluded from the scan)\n");
    ok = append_list(lines, (long long)"       and allocates nothing. On every way out, ep_gc_leave_blocking_region\n");
    ok = append_list(lines, (long long)"       waits for any in-flight collection before mutator code resumes.\n");
    ok = append_list(lines, (long long)"       (Without any of this, the collector scans a live, mutating stack\n");
    ok = append_list(lines, (long long)"       against a stale top, misses the channels list held in a callee-saved\n");
    ok = append_list(lines, (long long)"       register, sweeps it, and the next poll dereferences freed memory.) */\n");
    ok = append_list(lines, (long long)"    jmp_buf _pin_regs;\n");
    ok = append_list(lines, (long long)"    volatile char _pin_marker;\n");
    ok = append_list(lines, (long long)"    if (ep_thread_slot >= 0) {\n");
    ok = append_list(lines, (long long)"        memset(&_pin_regs, 0, sizeof(_pin_regs));\n");
    ok = append_list(lines, (long long)"        setjmp(_pin_regs);\n");
    ok = append_list(lines, (long long)"        pthread_mutex_lock(&ep_gc_mutex);\n");
    ok = append_list(lines, (long long)"        { char* _a = (char*)(void*)&_pin_marker; char* _b = (char*)(void*)&_pin_regs;\n");
    ok = append_list(lines, (long long)"          ep_thread_local_top = (void*)((uintptr_t)((_a < _b) ? _a : _b) & ~(uintptr_t)7); }\n");
    ok = append_list(lines, (long long)"        __sync_synchronize();\n");
    ok = append_list(lines, (long long)"        ep_gc_parked_count++;\n");
    ok = append_list(lines, (long long)"        pthread_mutex_unlock(&ep_gc_mutex);\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"    while (1) {\n");
    ok = append_list(lines, (long long)"        // Poll all channels\n");
    ok = append_list(lines, (long long)"        for (long long i = 0; i < list->length; i++) {\n");
    ok = append_list(lines, (long long)"            EpChannel* chan = (EpChannel*)list->data[i];\n");
    ok = append_list(lines, (long long)"            if (chan) {\n");
    ok = append_list(lines, (long long)"                ep_mutex_lock(&chan->mutex);\n");
    ok = append_list(lines, (long long)"                if (chan->size > 0) {\n");
    ok = append_list(lines, (long long)"                    ep_mutex_unlock(&chan->mutex);\n");
    ok = append_list(lines, (long long)"                    ep_gc_leave_blocking_region();\n");
    ok = append_list(lines, (long long)"                    return i;\n");
    ok = append_list(lines, (long long)"                }\n");
    ok = append_list(lines, (long long)"                ep_mutex_unlock(&chan->mutex);\n");
    ok = append_list(lines, (long long)"            }\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"        // Check timeout\n");
    ok = append_list(lines, (long long)"        if (timeout_ms >= 0) {\n");
    ok = append_list(lines, (long long)"#ifdef _WIN32\n");
    ok = append_list(lines, (long long)"            ULONGLONG now_tick = GetTickCount64();\n");
    ok = append_list(lines, (long long)"            long long elapsed = (long long)(now_tick - start_tick);\n");
    ok = append_list(lines, (long long)"#else\n");
    ok = append_list(lines, (long long)"            clock_gettime(CLOCK_MONOTONIC, &now);\n");
    ok = append_list(lines, (long long)"            long long elapsed = (now.tv_sec - start.tv_sec) * 1000 + (now.tv_nsec - start.tv_nsec) / 1000000;\n");
    ok = append_list(lines, (long long)"#endif\n");
    ok = append_list(lines, (long long)"            if (elapsed >= timeout_ms) {\n");
    ok = append_list(lines, (long long)"                ep_gc_leave_blocking_region();\n");
    ok = append_list(lines, (long long)"                return -1;\n");
    ok = append_list(lines, (long long)"            }\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"        \n");
    ok = append_list(lines, (long long)"        // Brief sleep to avoid busy-wait\n");
    ok = append_list(lines, (long long)"#ifdef _WIN32\n");
    ok = append_list(lines, (long long)"        Sleep(1);\n");
    ok = append_list(lines, (long long)"#else\n");
    ok = append_list(lines, (long long)"        usleep(1000); // 1ms\n");
    ok = append_list(lines, (long long)"#endif\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"#ifdef __wasm__\n");
    ok = append_list(lines, (long long)"long long ep_net_connect(const char* host, long long port) {\n");
    ok = append_list(lines, (long long)"    (void)host; (void)port;\n");
    ok = append_list(lines, (long long)"    return -1;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_net_listen(long long port) {\n");
    ok = append_list(lines, (long long)"    (void)port;\n");
    ok = append_list(lines, (long long)"    return -1;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_net_accept(long long server_fd) {\n");
    ok = append_list(lines, (long long)"    (void)server_fd;\n");
    ok = append_list(lines, (long long)"    return -1;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_net_send(long long fd, const char* data) {\n");
    ok = append_list(lines, (long long)"    (void)fd; (void)data;\n");
    ok = append_list(lines, (long long)"    return 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_net_send_raw(long long fd, long long data_ptr, long long count) {\n");
    ok = append_list(lines, (long long)"    (void)fd; (void)data_ptr; (void)count;\n");
    ok = append_list(lines, (long long)"    return 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"char* ep_net_recv(long long fd, long long max_len) {\n");
    ok = append_list(lines, (long long)"    (void)fd; (void)max_len;\n");
    ok = append_list(lines, (long long)"    char* empty = malloc(1);\n");
    ok = append_list(lines, (long long)"    if (empty) empty[0] = '\\0';\n");
    ok = append_list(lines, (long long)"    return empty;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_net_close(long long fd) {\n");
    ok = append_list(lines, (long long)"    (void)fd;\n");
    ok = append_list(lines, (long long)"    return -1;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_sleep_ms(long long ms) {\n");
    ok = append_list(lines, (long long)"    struct timespec ts;\n");
    ok = append_list(lines, (long long)"    ts.tv_sec = ms / 1000;\n");
    ok = append_list(lines, (long long)"    ts.tv_nsec = (ms % 1000) * 1000000;\n");
    ok = append_list(lines, (long long)"    nanosleep(&ts, NULL);\n");
    ok = append_list(lines, (long long)"    return 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_system(long long cmd) {\n");
    ok = append_list(lines, (long long)"    (void)cmd;\n");
    ok = append_list(lines, (long long)"    return -1;\n");
    ret_val = join_strings(lines);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(1);
    return ret_val;
}

long long ep_rt_core_14() {
    long long lines = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&lines);
    ep_gc_maybe_collect();

    lines = create_list();
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_play_sound(long long path) {\n");
    ok = append_list(lines, (long long)"    (void)path;\n");
    ok = append_list(lines, (long long)"    return -1;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_dlopen(long long path) {\n");
    ok = append_list(lines, (long long)"    (void)path;\n");
    ok = append_list(lines, (long long)"    return 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_dlsym(long long handle, long long name) {\n");
    ok = append_list(lines, (long long)"    (void)handle; (void)name;\n");
    ok = append_list(lines, (long long)"    return 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_dlclose(long long handle) {\n");
    ok = append_list(lines, (long long)"    (void)handle;\n");
    ok = append_list(lines, (long long)"    return 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"#else\n");
    ok = append_list(lines, (long long)"long long ep_net_connect(const char* host, long long port) {\n");
    ok = append_list(lines, (long long)"    int sockfd = socket(AF_INET, SOCK_STREAM, 0);\n");
    ok = append_list(lines, (long long)"    if (sockfd < 0) return -1;\n");
    ok = append_list(lines, (long long)"    struct hostent* server = gethostbyname(host);\n");
    ok = append_list(lines, (long long)"    if (!server) {\n");
    ok = append_list(lines, (long long)"#ifdef _WIN32\n");
    ok = append_list(lines, (long long)"        closesocket(sockfd);\n");
    ok = append_list(lines, (long long)"#else\n");
    ok = append_list(lines, (long long)"        close(sockfd);\n");
    ok = append_list(lines, (long long)"#endif\n");
    ok = append_list(lines, (long long)"        return -1;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    struct sockaddr_in serv_addr;\n");
    ok = append_list(lines, (long long)"    memset(&serv_addr, 0, sizeof(serv_addr));\n");
    ok = append_list(lines, (long long)"    serv_addr.sin_family = AF_INET;\n");
    ok = append_list(lines, (long long)"    memcpy(&serv_addr.sin_addr.s_addr, server->h_addr_list[0], server->h_length);\n");
    ok = append_list(lines, (long long)"    serv_addr.sin_port = htons(port);\n");
    ok = append_list(lines, (long long)"#ifdef _WIN32\n");
    ok = append_list(lines, (long long)"    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {\n");
    ok = append_list(lines, (long long)"        closesocket(sockfd);\n");
    ok = append_list(lines, (long long)"        return -1;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"#else\n");
    ok = append_list(lines, (long long)"    // Bounded connect: an unreachable peer must not block ~75s on the OS SYN\n");
    ok = append_list(lines, (long long)"    // timeout (this stalled node startup). Non-blocking connect + 5s select, then\n");
    ok = append_list(lines, (long long)"    // restore blocking mode for the rest of the session.\n");
    ok = append_list(lines, (long long)"    int _ep_flags = fcntl(sockfd, F_GETFL, 0);\n");
    ok = append_list(lines, (long long)"    fcntl(sockfd, F_SETFL, _ep_flags | O_NONBLOCK);\n");
    ok = append_list(lines, (long long)"    int _ep_cr = connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));\n");
    ok = append_list(lines, (long long)"    if (_ep_cr < 0) {\n");
    ok = append_list(lines, (long long)"        if (errno != EINPROGRESS) { close(sockfd); return -1; }\n");
    ok = append_list(lines, (long long)"        fd_set _ep_wset; FD_ZERO(&_ep_wset); FD_SET(sockfd, &_ep_wset);\n");
    ok = append_list(lines, (long long)"        struct timeval _ep_tv; _ep_tv.tv_sec = 5; _ep_tv.tv_usec = 0;\n");
    ok = append_list(lines, (long long)"        int _ep_sel = select(sockfd + 1, NULL, &_ep_wset, NULL, &_ep_tv);\n");
    ok = append_list(lines, (long long)"        if (_ep_sel <= 0) { close(sockfd); return -1; } // timeout or error\n");
    ok = append_list(lines, (long long)"        int _ep_so_err = 0; socklen_t _ep_slen = sizeof(_ep_so_err);\n");
    ok = append_list(lines, (long long)"        if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &_ep_so_err, &_ep_slen) < 0 || _ep_so_err != 0) {\n");
    ok = append_list(lines, (long long)"            close(sockfd);\n");
    ok = append_list(lines, (long long)"            return -1;\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    fcntl(sockfd, F_SETFL, _ep_flags);\n");
    ok = append_list(lines, (long long)"#endif\n");
    ok = append_list(lines, (long long)"    return sockfd;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_net_listen(long long port) {\n");
    ok = append_list(lines, (long long)"    int sockfd = socket(AF_INET, SOCK_STREAM, 0);\n");
    ok = append_list(lines, (long long)"    if (sockfd < 0) return -1;\n");
    ok = append_list(lines, (long long)"    int opt = 1;\n");
    ok = append_list(lines, (long long)"    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));\n");
    ok = append_list(lines, (long long)"    struct sockaddr_in serv_addr;\n");
    ok = append_list(lines, (long long)"    memset(&serv_addr, 0, sizeof(serv_addr));\n");
    ok = append_list(lines, (long long)"    serv_addr.sin_family = AF_INET;\n");
    ok = append_list(lines, (long long)"    serv_addr.sin_addr.s_addr = INADDR_ANY;\n");
    ok = append_list(lines, (long long)"    serv_addr.sin_port = htons(port);\n");
    ok = append_list(lines, (long long)"    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {\n");
    ok = append_list(lines, (long long)"#ifdef _WIN32\n");
    ok = append_list(lines, (long long)"        closesocket(sockfd);\n");
    ok = append_list(lines, (long long)"#else\n");
    ok = append_list(lines, (long long)"        close(sockfd);\n");
    ok = append_list(lines, (long long)"#endif\n");
    ok = append_list(lines, (long long)"        return -1;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    if (listen(sockfd, 10) < 0) {\n");
    ok = append_list(lines, (long long)"#ifdef _WIN32\n");
    ok = append_list(lines, (long long)"        closesocket(sockfd);\n");
    ok = append_list(lines, (long long)"#else\n");
    ok = append_list(lines, (long long)"        close(sockfd);\n");
    ok = append_list(lines, (long long)"#endif\n");
    ok = append_list(lines, (long long)"        return -1;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    return sockfd;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_net_accept(long long server_fd) {\n");
    ok = append_list(lines, (long long)"    struct sockaddr_in cli_addr;\n");
    ok = append_list(lines, (long long)"    socklen_t clilen = sizeof(cli_addr);\n");
    ok = append_list(lines, (long long)"    int newsockfd = accept((int)server_fd, (struct sockaddr*)&cli_addr, &clilen);\n");
    ok = append_list(lines, (long long)"    if (newsockfd >= 0) {\n");
    ok = append_list(lines, (long long)"        /* Bound how long a single recv/send may block so a slow or silent\n");
    ok = append_list(lines, (long long)"           client cannot pin a handler thread forever (slowloris). */\n");
    ok = append_list(lines, (long long)"        struct timeval tv;\n");
    ok = append_list(lines, (long long)"        tv.tv_sec = 30;\n");
    ok = append_list(lines, (long long)"        tv.tv_usec = 0;\n");
    ok = append_list(lines, (long long)"        setsockopt(newsockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv));\n");
    ok = append_list(lines, (long long)"        setsockopt(newsockfd, SOL_SOCKET, SO_SNDTIMEO, (const char*)&tv, sizeof(tv));\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    return newsockfd;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_net_send(long long fd, const char* data) {\n");
    ok = append_list(lines, (long long)"    if (!data) return 0;\n");
    ok = append_list(lines, (long long)"    return ep_net_send_raw(fd, (long long)data, (long long)strlen(data));\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_net_send_raw(long long fd, long long data_ptr, long long count) {\n");
    ok = append_list(lines, (long long)"    if (data_ptr == 0 || count <= 0) return 0;\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"    /* send() may write fewer bytes than requested. Keep sending until the\n");
    ok = append_list(lines, (long long)"       explicit byte count is exhausted, including bytes after embedded NULs. */\n");
    ok = append_list(lines, (long long)"    const char* data = (const char*)data_ptr;\n");
    ok = append_list(lines, (long long)"    long long off = 0;\n");
    ok = append_list(lines, (long long)"    while (off < count) {\n");
    ok = append_list(lines, (long long)"#ifdef _WIN32\n");
    ok = append_list(lines, (long long)"        int chunk = count - off > INT_MAX ? INT_MAX : (int)(count - off);\n");
    ok = append_list(lines, (long long)"        int n = send((int)fd, data + off, chunk, 0);\n");
    ok = append_list(lines, (long long)"        if (n < 0 && WSAGetLastError() == WSAEINTR) continue;\n");
    ok = append_list(lines, (long long)"#else\n");
    ok = append_list(lines, (long long)"        ssize_t n = send((int)fd, data + off, (size_t)(count - off), 0);\n");
    ok = append_list(lines, (long long)"        if (n < 0 && errno == EINTR) continue;\n");
    ok = append_list(lines, (long long)"#endif\n");
    ok = append_list(lines, (long long)"        if (n <= 0) break;\n");
    ok = append_list(lines, (long long)"        off += (long long)n;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    return off;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"char* ep_net_recv(long long fd, long long max_len) {\n");
    ok = append_list(lines, (long long)"    char* buf = malloc(max_len + 1);\n");
    ok = append_list(lines, (long long)"    if (!buf) {\n");
    ok = append_list(lines, (long long)"        char* empty = malloc(1);\n");
    ok = append_list(lines, (long long)"        if (empty) empty[0] = '\\0';\n");
    ok = append_list(lines, (long long)"        return empty;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"#ifdef _WIN32\n");
    ok = append_list(lines, (long long)"    int n = recv((int)fd, buf, (int)max_len, 0);\n");
    ok = append_list(lines, (long long)"#else\n");
    ret_val = join_strings(lines);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(1);
    return ret_val;
}

long long ep_rt_core_15() {
    long long lines = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&lines);
    ep_gc_maybe_collect();

    lines = create_list();
    ok = append_list(lines, (long long)"    ssize_t n = recv((int)fd, buf, max_len, 0);\n");
    ok = append_list(lines, (long long)"#endif\n");
    ok = append_list(lines, (long long)"    if (n < 0) n = 0;\n");
    ok = append_list(lines, (long long)"    buf[n] = '\\0';\n");
    ok = append_list(lines, (long long)"    return buf;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_net_close(long long fd) {\n");
    ok = append_list(lines, (long long)"#ifdef _WIN32\n");
    ok = append_list(lines, (long long)"    return closesocket((int)fd);\n");
    ok = append_list(lines, (long long)"#else\n");
    ok = append_list(lines, (long long)"    return close((int)fd);\n");
    ok = append_list(lines, (long long)"#endif\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_sleep_ms(long long ms) {\n");
    ok = append_list(lines, (long long)"#ifdef _WIN32\n");
    ok = append_list(lines, (long long)"    Sleep((DWORD)ms);\n");
    ok = append_list(lines, (long long)"#else\n");
    ok = append_list(lines, (long long)"    usleep((useconds_t)(ms * 1000));\n");
    ok = append_list(lines, (long long)"#endif\n");
    ok = append_list(lines, (long long)"    return 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_system(long long cmd) {\n");
    ok = append_list(lines, (long long)"    return (long long)system((const char*)cmd);\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_play_sound(long long path) {\n");
    ok = append_list(lines, (long long)"    char cmd[512];\n");
    ok = append_list(lines, (long long)"    snprintf(cmd, sizeof(cmd), \"afplay '%s' &\", (const char*)path);\n");
    ok = append_list(lines, (long long)"    return (long long)system(cmd);\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* ========== Dynamic Library Loading (FFI) ========== */\n");
    ok = append_list(lines, (long long)"#ifndef _WIN32\n");
    ok = append_list(lines, (long long)"#include <dlfcn.h>\n");
    ok = append_list(lines, (long long)"#endif\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_dlopen(long long path) {\n");
    ok = append_list(lines, (long long)"#ifdef _WIN32\n");
    ok = append_list(lines, (long long)"    HMODULE h = LoadLibraryA((const char*)path);\n");
    ok = append_list(lines, (long long)"    return (long long)h;\n");
    ok = append_list(lines, (long long)"#else\n");
    ok = append_list(lines, (long long)"    const char* p = (const char*)path;\n");
    ok = append_list(lines, (long long)"    void* handle = dlopen(p, RTLD_LAZY);\n");
    ok = append_list(lines, (long long)"    return (long long)handle;\n");
    ok = append_list(lines, (long long)"#endif\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_dlsym(long long handle, long long name) {\n");
    ok = append_list(lines, (long long)"#ifdef _WIN32\n");
    ok = append_list(lines, (long long)"    FARPROC sym = GetProcAddress((HMODULE)handle, (const char*)name);\n");
    ok = append_list(lines, (long long)"    return (long long)sym;\n");
    ok = append_list(lines, (long long)"#else\n");
    ok = append_list(lines, (long long)"    void* sym = dlsym((void*)handle, (const char*)name);\n");
    ok = append_list(lines, (long long)"    return (long long)sym;\n");
    ok = append_list(lines, (long long)"#endif\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_dlclose(long long handle) {\n");
    ok = append_list(lines, (long long)"#ifdef _WIN32\n");
    ok = append_list(lines, (long long)"    return (long long)FreeLibrary((HMODULE)handle);\n");
    ok = append_list(lines, (long long)"#else\n");
    ok = append_list(lines, (long long)"    return (long long)dlclose((void*)handle);\n");
    ok = append_list(lines, (long long)"#endif\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"#endif\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* Call a function pointer with 0..6 arguments.\n");
    ok = append_list(lines, (long long)"   These are type-punned through long long — the C calling convention\n");
    ok = append_list(lines, (long long)"   makes this work for integer and pointer arguments. */\n");
    ok = append_list(lines, (long long)"typedef long long (*ep_fn0)(void);\n");
    ok = append_list(lines, (long long)"typedef long long (*ep_fn1)(long long);\n");
    ok = append_list(lines, (long long)"typedef long long (*ep_fn2)(long long, long long);\n");
    ok = append_list(lines, (long long)"typedef long long (*ep_fn3)(long long, long long, long long);\n");
    ok = append_list(lines, (long long)"typedef long long (*ep_fn4)(long long, long long, long long, long long);\n");
    ok = append_list(lines, (long long)"typedef long long (*ep_fn5)(long long, long long, long long, long long, long long);\n");
    ok = append_list(lines, (long long)"typedef long long (*ep_fn6)(long long, long long, long long, long long, long long, long long);\n");
    ok = append_list(lines, (long long)"typedef long long (*ep_fn7)(long long, long long, long long, long long, long long, long long, long long);\n");
    ok = append_list(lines, (long long)"typedef long long (*ep_fn8)(long long, long long, long long, long long, long long, long long, long long, long long);\n");
    ok = append_list(lines, (long long)"typedef long long (*ep_fn9)(long long, long long, long long, long long, long long, long long, long long, long long, long long);\n");
    ok = append_list(lines, (long long)"typedef long long (*ep_fn10)(long long, long long, long long, long long, long long, long long, long long, long long, long long, long long);\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_dlcall0(long long fptr) {\n");
    ok = append_list(lines, (long long)"    return ((ep_fn0)fptr)();\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"long long ep_dlcall1(long long fptr, long long a0) {\n");
    ok = append_list(lines, (long long)"    return ((ep_fn1)fptr)(a0);\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"long long ep_dlcall2(long long fptr, long long a0, long long a1) {\n");
    ok = append_list(lines, (long long)"    return ((ep_fn2)fptr)(a0, a1);\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"long long ep_dlcall3(long long fptr, long long a0, long long a1, long long a2) {\n");
    ok = append_list(lines, (long long)"    return ((ep_fn3)fptr)(a0, a1, a2);\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"long long ep_dlcall4(long long fptr, long long a0, long long a1, long long a2, long long a3) {\n");
    ok = append_list(lines, (long long)"    return ((ep_fn4)fptr)(a0, a1, a2, a3);\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"long long ep_dlcall5(long long fptr, long long a0, long long a1, long long a2, long long a3, long long a4) {\n");
    ok = append_list(lines, (long long)"    return ((ep_fn5)fptr)(a0, a1, a2, a3, a4);\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"long long ep_dlcall6(long long fptr, long long a0, long long a1, long long a2, long long a3, long long a4, long long a5) {\n");
    ok = append_list(lines, (long long)"    return ((ep_fn6)fptr)(a0, a1, a2, a3, a4, a5);\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"long long ep_dlcall7(long long fptr, long long a0, long long a1, long long a2, long long a3, long long a4, long long a5, long long a6) {\n");
    ok = append_list(lines, (long long)"    return ((ep_fn7)fptr)(a0, a1, a2, a3, a4, a5, a6);\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"long long ep_dlcall8(long long fptr, long long a0, long long a1, long long a2, long long a3, long long a4, long long a5, long long a6, long long a7) {\n");
    ok = append_list(lines, (long long)"    return ((ep_fn8)fptr)(a0, a1, a2, a3, a4, a5, a6, a7);\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"long long ep_dlcall9(long long fptr, long long a0, long long a1, long long a2, long long a3, long long a4, long long a5, long long a6, long long a7, long long a8) {\n");
    ok = append_list(lines, (long long)"    return ((ep_fn9)fptr)(a0, a1, a2, a3, a4, a5, a6, a7, a8);\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"long long ep_dlcall10(long long fptr, long long a0, long long a1, long long a2, long long a3, long long a4, long long a5, long long a6, long long a7, long long a8, long long a9) {\n");
    ok = append_list(lines, (long long)"    return ((ep_fn10)fptr)(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9);\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* ========== Float FFI: ep_dlcall_f* ========== */\n");
    ok = append_list(lines, (long long)"/* For calling C functions that accept/return double values.\n");
    ok = append_list(lines, (long long)"   Arguments are passed as long long (bit-punned doubles).\n");
    ok = append_list(lines, (long long)"   Return value is a double bit-punned back to long long.\n");
    ok = append_list(lines, (long long)"   Use ep_double_to_bits() / ep_bits_to_double() to convert. */\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"typedef union { long long i; double f; } ep_float_bits;\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"static inline double ep_ll_to_double(long long v) {\n");
    ok = append_list(lines, (long long)"    ep_float_bits u; u.i = v; return u.f;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"static inline long long ep_double_to_ll(double v) {\n");
    ok = append_list(lines, (long long)"    ep_float_bits u; u.f = v; return u.i;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* Convert between ErnosPlain float representation and raw bits */\n");
    ok = append_list(lines, (long long)"long long ep_double_to_bits(long long float_val) {\n");
    ok = append_list(lines, (long long)"    /* float_val is already an EP Float stored as long long bits */\n");
    ok = append_list(lines, (long long)"    return float_val;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"long long ep_bits_to_double(long long bits) {\n");
    ok = append_list(lines, (long long)"    return bits;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* Float function pointer typedefs */\n");
    ok = append_list(lines, (long long)"typedef double (*ep_ff0)(void);\n");
    ok = append_list(lines, (long long)"typedef double (*ep_ff1)(double);\n");
    ok = append_list(lines, (long long)"typedef double (*ep_ff2)(double, double);\n");
    ok = append_list(lines, (long long)"typedef double (*ep_ff3)(double, double, double);\n");
    ok = append_list(lines, (long long)"typedef double (*ep_ff4)(double, double, double, double);\n");
    ok = append_list(lines, (long long)"typedef double (*ep_ff5)(double, double, double, double, double);\n");
    ok = append_list(lines, (long long)"typedef double (*ep_ff6)(double, double, double, double, double, double);\n");
    ret_val = join_strings(lines);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(1);
    return ret_val;
}

long long ep_rt_core_16() {
    long long lines = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&lines);
    ep_gc_maybe_collect();

    lines = create_list();
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* Call functions that take doubles and return double */\n");
    ok = append_list(lines, (long long)"long long ep_dlcall_f0(long long fptr) {\n");
    ok = append_list(lines, (long long)"    return ep_double_to_ll(((ep_ff0)fptr)());\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"long long ep_dlcall_f1(long long fptr, long long a0) {\n");
    ok = append_list(lines, (long long)"    return ep_double_to_ll(((ep_ff1)fptr)(ep_ll_to_double(a0)));\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"long long ep_dlcall_f2(long long fptr, long long a0, long long a1) {\n");
    ok = append_list(lines, (long long)"    return ep_double_to_ll(((ep_ff2)fptr)(ep_ll_to_double(a0), ep_ll_to_double(a1)));\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"long long ep_dlcall_f3(long long fptr, long long a0, long long a1, long long a2) {\n");
    ok = append_list(lines, (long long)"    return ep_double_to_ll(((ep_ff3)fptr)(ep_ll_to_double(a0), ep_ll_to_double(a1), ep_ll_to_double(a2)));\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"long long ep_dlcall_f4(long long fptr, long long a0, long long a1, long long a2, long long a3) {\n");
    ok = append_list(lines, (long long)"    return ep_double_to_ll(((ep_ff4)fptr)(ep_ll_to_double(a0), ep_ll_to_double(a1), ep_ll_to_double(a2), ep_ll_to_double(a3)));\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"long long ep_dlcall_f5(long long fptr, long long a0, long long a1, long long a2, long long a3, long long a4) {\n");
    ok = append_list(lines, (long long)"    return ep_double_to_ll(((ep_ff5)fptr)(ep_ll_to_double(a0), ep_ll_to_double(a1), ep_ll_to_double(a2), ep_ll_to_double(a3), ep_ll_to_double(a4)));\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"long long ep_dlcall_f6(long long fptr, long long a0, long long a1, long long a2, long long a3, long long a4, long long a5) {\n");
    ok = append_list(lines, (long long)"    return ep_double_to_ll(((ep_ff6)fptr)(ep_ll_to_double(a0), ep_ll_to_double(a1), ep_ll_to_double(a2), ep_ll_to_double(a3), ep_ll_to_double(a4), ep_ll_to_double(a5)));\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* Variants that take doubles but return int (for comparison functions etc.) */\n");
    ok = append_list(lines, (long long)"typedef long long (*ep_fdi1)(double);\n");
    ok = append_list(lines, (long long)"typedef long long (*ep_fdi2)(double, double);\n");
    ok = append_list(lines, (long long)"typedef long long (*ep_fdi3)(double, double, double);\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_dlcall_fd1(long long fptr, long long a0) {\n");
    ok = append_list(lines, (long long)"    return ((ep_fdi1)fptr)(ep_ll_to_double(a0));\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"long long ep_dlcall_fd2(long long fptr, long long a0, long long a1) {\n");
    ok = append_list(lines, (long long)"    return ((ep_fdi2)fptr)(ep_ll_to_double(a0), ep_ll_to_double(a1));\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"long long ep_dlcall_fd3(long long fptr, long long a0, long long a1, long long a2) {\n");
    ok = append_list(lines, (long long)"    return ((ep_fdi3)fptr)(ep_ll_to_double(a0), ep_ll_to_double(a1), ep_ll_to_double(a2));\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"/* ========== End Float FFI ========== */\n");
    ok = append_list(lines, (long long)"/* ========== End Dynamic Library Loading ========== */\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"unsigned long hash_string(const char* str) {\n");
    ok = append_list(lines, (long long)"    unsigned long hash = 5381;\n");
    ok = append_list(lines, (long long)"    int c;\n");
    ok = append_list(lines, (long long)"    while ((c = *str++)) {\n");
    ok = append_list(lines, (long long)"        hash = ((hash << 5) + hash) + c;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    return hash;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"typedef struct {\n");
    ok = append_list(lines, (long long)"    char* key;\n");
    ok = append_list(lines, (long long)"    long long value;\n");
    ok = append_list(lines, (long long)"    int used;\n");
    ok = append_list(lines, (long long)"} EpMapEntry;\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"typedef struct {\n");
    ok = append_list(lines, (long long)"    EpMapEntry* entries;\n");
    ok = append_list(lines, (long long)"    long long capacity;\n");
    ok = append_list(lines, (long long)"    long long size;\n");
    ok = append_list(lines, (long long)"} EpMap;\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* Map value traversal for GC — walks all entries and marks values.\n");
    ok = append_list(lines, (long long)"   Called by ep_gc_mark_object() via function pointer. */\n");
    ok = append_list(lines, (long long)"static void ep_gc_mark_map_values_impl(void* ptr) {\n");
    ok = append_list(lines, (long long)"    EpMap* map = (EpMap*)ptr;\n");
    ok = append_list(lines, (long long)"    if (!map || !map->entries) return;\n");
    ok = append_list(lines, (long long)"    for (long long i = 0; i < map->capacity; i++) {\n");
    ok = append_list(lines, (long long)"        if (map->entries[i].used && map->entries[i].value != 0) {\n");
    ok = append_list(lines, (long long)"            ep_gc_mark_object((void*)map->entries[i].value);\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"        /* Also mark keys if they are heap strings */\n");
    ok = append_list(lines, (long long)"        if (map->entries[i].used && map->entries[i].key != NULL) {\n");
    ok = append_list(lines, (long long)"            ep_gc_mark_object((void*)map->entries[i].key);\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"static void ep_gc_mark_map_values_minor_impl(void* ptr) {\n");
    ok = append_list(lines, (long long)"    EpMap* map = (EpMap*)ptr;\n");
    ok = append_list(lines, (long long)"    if (!map || !map->entries) return;\n");
    ok = append_list(lines, (long long)"    for (long long i = 0; i < map->capacity; i++) {\n");
    ok = append_list(lines, (long long)"        if (map->entries[i].used && map->entries[i].value != 0) {\n");
    ok = append_list(lines, (long long)"            ep_gc_mark_object_minor((void*)map->entries[i].value);\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"        if (map->entries[i].used && map->entries[i].key != NULL) {\n");
    ok = append_list(lines, (long long)"            ep_gc_mark_object_minor((void*)map->entries[i].key);\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long create_map(void) {\n");
    ok = append_list(lines, (long long)"    EpMap* map = malloc(sizeof(EpMap));\n");
    ok = append_list(lines, (long long)"    if (!map) return 0;\n");
    ok = append_list(lines, (long long)"    map->capacity = 16;\n");
    ok = append_list(lines, (long long)"    map->size = 0;\n");
    ok = append_list(lines, (long long)"    map->entries = calloc(map->capacity, sizeof(EpMapEntry));\n");
    ok = append_list(lines, (long long)"    if (!map->entries) {\n");
    ok = append_list(lines, (long long)"        free(map);\n");
    ok = append_list(lines, (long long)"        return 0;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    ep_gc_register(map, EP_OBJ_MAP);\n");
    ok = append_list(lines, (long long)"    return (long long)map;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"static void map_resize(EpMap* map, long long new_capacity) {\n");
    ok = append_list(lines, (long long)"    EpMapEntry* old_entries = map->entries;\n");
    ok = append_list(lines, (long long)"    long long old_capacity = map->capacity;\n");
    ok = append_list(lines, (long long)"    map->capacity = new_capacity;\n");
    ok = append_list(lines, (long long)"    map->entries = calloc(new_capacity, sizeof(EpMapEntry));\n");
    ok = append_list(lines, (long long)"    map->size = 0;\n");
    ok = append_list(lines, (long long)"    for (long long i = 0; i < old_capacity; i++) {\n");
    ok = append_list(lines, (long long)"        if (old_entries[i].used && old_entries[i].key != NULL) {\n");
    ok = append_list(lines, (long long)"            char* key = old_entries[i].key;\n");
    ok = append_list(lines, (long long)"            long long value = old_entries[i].value;\n");
    ok = append_list(lines, (long long)"            unsigned long h = hash_string(key) % new_capacity;\n");
    ok = append_list(lines, (long long)"            while (map->entries[h].used) {\n");
    ok = append_list(lines, (long long)"                h = (h + 1) % new_capacity;\n");
    ok = append_list(lines, (long long)"            }\n");
    ok = append_list(lines, (long long)"            map->entries[h].key = key;\n");
    ok = append_list(lines, (long long)"            map->entries[h].value = value;\n");
    ok = append_list(lines, (long long)"            map->entries[h].used = 1;\n");
    ok = append_list(lines, (long long)"            map->size++;\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    free(old_entries);\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* Convert a key value to a string — handles both string pointers and integers */\n");
    ok = append_list(lines, (long long)"static const char* ep_map_key_str(long long key_val, char* buf, int bufsize) {\n");
    ok = append_list(lines, (long long)"    if (key_val == 0) { buf[0] = '0'; buf[1] = '\\0'; return buf; }\n");
    ok = append_list(lines, (long long)"    /* Check if value is in plausible pointer range for a string */\n");
    ok = append_list(lines, (long long)"    if (key_val > 0x100000) {\n");
    ok = append_list(lines, (long long)"        const char* p = (const char*)(void*)key_val;\n");
    ok = append_list(lines, (long long)"        unsigned char first = (unsigned char)*p;\n");
    ok = append_list(lines, (long long)"        if ((first >= 0x20 && first < 0x7F) || first >= 0xC0 || first == 0) {\n");
    ok = append_list(lines, (long long)"            return p; /* valid string pointer */\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    snprintf(buf, bufsize, \"%lld\", key_val);\n");
    ok = append_list(lines, (long long)"    return buf;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long map_insert(long long map_ptr, long long key_val, long long value) {\n");
    ok = append_list(lines, (long long)"    if (EP_BADPTR(map_ptr)) return 0;\n");
    ok = append_list(lines, (long long)"    EpMap* map = (EpMap*)map_ptr;\n");
    ok = append_list(lines, (long long)"    char keybuf[32];\n");
    ok = append_list(lines, (long long)"    const char* key = ep_map_key_str(key_val, keybuf, sizeof(keybuf));\n");
    ok = append_list(lines, (long long)"    if (!map) return 0;\n");
    ok = append_list(lines, (long long)"    if (map->size * 2 >= map->capacity) {\n");
    ret_val = join_strings(lines);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(1);
    return ret_val;
}

long long ep_rt_core_17() {
    long long lines = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&lines);
    ep_gc_maybe_collect();

    lines = create_list();
    ok = append_list(lines, (long long)"        map_resize(map, map->capacity * 2);\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    unsigned long h = hash_string(key) % map->capacity;\n");
    ok = append_list(lines, (long long)"    while (map->entries[h].used) {\n");
    ok = append_list(lines, (long long)"        if (strcmp(map->entries[h].key, key) == 0) {\n");
    ok = append_list(lines, (long long)"            map->entries[h].value = value;\n");
    ok = append_list(lines, (long long)"            ep_gc_write_barrier((void*)map_ptr, value);\n");
    ok = append_list(lines, (long long)"            return value;\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"        h = (h + 1) % map->capacity;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    map->entries[h].key = strdup(key);\n");
    ok = append_list(lines, (long long)"    map->entries[h].value = value;\n");
    ok = append_list(lines, (long long)"    map->entries[h].used = 1;\n");
    ok = append_list(lines, (long long)"    map->size++;\n");
    ok = append_list(lines, (long long)"    ep_gc_write_barrier((void*)map_ptr, value);\n");
    ok = append_list(lines, (long long)"    return value;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long map_get_val(long long map_ptr, long long key_val) {\n");
    ok = append_list(lines, (long long)"    if (EP_BADPTR(map_ptr)) return 0;\n");
    ok = append_list(lines, (long long)"    EpMap* map = (EpMap*)map_ptr;\n");
    ok = append_list(lines, (long long)"    char keybuf[32];\n");
    ok = append_list(lines, (long long)"    const char* key = ep_map_key_str(key_val, keybuf, sizeof(keybuf));\n");
    ok = append_list(lines, (long long)"    if (!map) return 0;\n");
    ok = append_list(lines, (long long)"    unsigned long h = hash_string(key) % map->capacity;\n");
    ok = append_list(lines, (long long)"    long long start_h = h;\n");
    ok = append_list(lines, (long long)"    while (map->entries[h].used) {\n");
    ok = append_list(lines, (long long)"        if (map->entries[h].key && strcmp(map->entries[h].key, key) == 0) {\n");
    ok = append_list(lines, (long long)"            return map->entries[h].value;\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"        h = (h + 1) % map->capacity;\n");
    ok = append_list(lines, (long long)"        if (h == start_h) break;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    return 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* map_set_str: store a string value (strdup'd copy) under a string key */\n");
    ok = append_list(lines, (long long)"long long map_set_str(long long map_ptr, long long key_val, long long str_val) {\n");
    ok = append_list(lines, (long long)"    /* Store the string pointer as a long long value — same as map_insert */\n");
    ok = append_list(lines, (long long)"    return map_insert(map_ptr, key_val, str_val);\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* map_get_str: retrieve a string value from a map (returns char* as long long) */\n");
    ok = append_list(lines, (long long)"long long map_get_str(long long map_ptr, long long key_val) {\n");
    ok = append_list(lines, (long long)"    /* Same as map_get_val — the stored long long IS a char* pointer */\n");
    ok = append_list(lines, (long long)"    return map_get_val(map_ptr, key_val);\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long map_contains(long long map_ptr, long long key_val) {\n");
    ok = append_list(lines, (long long)"    if (EP_BADPTR(map_ptr)) return 0;\n");
    ok = append_list(lines, (long long)"    EpMap* map = (EpMap*)map_ptr;\n");
    ok = append_list(lines, (long long)"    char keybuf[32];\n");
    ok = append_list(lines, (long long)"    const char* key = ep_map_key_str(key_val, keybuf, sizeof(keybuf));\n");
    ok = append_list(lines, (long long)"    if (!map) return 0;\n");
    ok = append_list(lines, (long long)"    unsigned long h = hash_string(key) % map->capacity;\n");
    ok = append_list(lines, (long long)"    long long start_h = h;\n");
    ok = append_list(lines, (long long)"    while (map->entries[h].used) {\n");
    ok = append_list(lines, (long long)"        if (map->entries[h].key && strcmp(map->entries[h].key, key) == 0) {\n");
    ok = append_list(lines, (long long)"            return 1;\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"        h = (h + 1) % map->capacity;\n");
    ok = append_list(lines, (long long)"        if (h == start_h) break;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    return 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long map_delete(long long map_ptr, long long key_val) {\n");
    ok = append_list(lines, (long long)"    if (EP_BADPTR(map_ptr)) return 0;\n");
    ok = append_list(lines, (long long)"    EpMap* map = (EpMap*)map_ptr;\n");
    ok = append_list(lines, (long long)"    char keybuf[32];\n");
    ok = append_list(lines, (long long)"    const char* key = ep_map_key_str(key_val, keybuf, sizeof(keybuf));\n");
    ok = append_list(lines, (long long)"    if (!map) return 0;\n");
    ok = append_list(lines, (long long)"    unsigned long h = hash_string(key) % map->capacity;\n");
    ok = append_list(lines, (long long)"    long long start_h = h;\n");
    ok = append_list(lines, (long long)"    while (map->entries[h].used) {\n");
    ok = append_list(lines, (long long)"        if (map->entries[h].key && strcmp(map->entries[h].key, key) == 0) {\n");
    ok = append_list(lines, (long long)"            free(map->entries[h].key);\n");
    ok = append_list(lines, (long long)"            map->entries[h].key = NULL;\n");
    ok = append_list(lines, (long long)"            map->entries[h].value = 0;\n");
    ok = append_list(lines, (long long)"            map->entries[h].used = 0;\n");
    ok = append_list(lines, (long long)"            map->size--;\n");
    ok = append_list(lines, (long long)"            long long next_h = (h + 1) % map->capacity;\n");
    ok = append_list(lines, (long long)"            while (map->entries[next_h].used) {\n");
    ok = append_list(lines, (long long)"                char* k = map->entries[next_h].key;\n");
    ok = append_list(lines, (long long)"                long long v = map->entries[next_h].value;\n");
    ok = append_list(lines, (long long)"                map->entries[next_h].key = NULL;\n");
    ok = append_list(lines, (long long)"                map->entries[next_h].value = 0;\n");
    ok = append_list(lines, (long long)"                map->entries[next_h].used = 0;\n");
    ok = append_list(lines, (long long)"                map->size--;\n");
    ok = append_list(lines, (long long)"                map_insert(map_ptr, (long long)k, v);\n");
    ok = append_list(lines, (long long)"                free(k);\n");
    ok = append_list(lines, (long long)"                next_h = (next_h + 1) % map->capacity;\n");
    ok = append_list(lines, (long long)"            }\n");
    ok = append_list(lines, (long long)"            return 1;\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"        h = (h + 1) % map->capacity;\n");
    ok = append_list(lines, (long long)"        if (h == start_h) break;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    return 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long map_keys(long long map_ptr) {\n");
    ok = append_list(lines, (long long)"    EpMap* map = (EpMap*)map_ptr;\n");
    ok = append_list(lines, (long long)"    if (!map) return (long long)create_list();\n");
    ok = append_list(lines, (long long)"    long long list = create_list();\n");
    ok = append_list(lines, (long long)"    for (long long i = 0; i < map->capacity; i++) {\n");
    ok = append_list(lines, (long long)"        if (map->entries[i].used && map->entries[i].key) {\n");
    ok = append_list(lines, (long long)"            append_list(list, (long long)strdup(map->entries[i].key));\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    return list;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long map_values(long long map_ptr) {\n");
    ok = append_list(lines, (long long)"    EpMap* map = (EpMap*)map_ptr;\n");
    ok = append_list(lines, (long long)"    if (!map) return (long long)create_list();\n");
    ok = append_list(lines, (long long)"    long long list = create_list();\n");
    ok = append_list(lines, (long long)"    for (long long i = 0; i < map->capacity; i++) {\n");
    ok = append_list(lines, (long long)"        if (map->entries[i].used && map->entries[i].key) {\n");
    ok = append_list(lines, (long long)"            append_list(list, map->entries[i].value);\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    return list;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long map_size(long long map_ptr) {\n");
    ok = append_list(lines, (long long)"    EpMap* map = (EpMap*)map_ptr;\n");
    ok = append_list(lines, (long long)"    if (!map) return 0;\n");
    ok = append_list(lines, (long long)"    return map->size;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long free_map(long long map_ptr) {\n");
    ok = append_list(lines, (long long)"    EpMap* map = (EpMap*)map_ptr;\n");
    ok = append_list(lines, (long long)"    if (!map) return 0;\n");
    ok = append_list(lines, (long long)"    /* Skip if already freed (idempotent) */\n");
    ok = append_list(lines, (long long)"    if (!ep_gc_find(map)) return 0;\n");
    ok = append_list(lines, (long long)"    ep_gc_unregister(map);\n");
    ok = append_list(lines, (long long)"    for (long long i = 0; i < map->capacity; i++) {\n");
    ok = append_list(lines, (long long)"        if (map->entries[i].used && map->entries[i].key != NULL) {\n");
    ok = append_list(lines, (long long)"            free(map->entries[i].key);\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    free(map->entries);\n");
    ok = append_list(lines, (long long)"    free(map);\n");
    ok = append_list(lines, (long long)"    return 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"typedef struct {\n");
    ok = append_list(lines, (long long)"    long long* data;\n");
    ret_val = join_strings(lines);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(1);
    return ret_val;
}

long long ep_rt_core_18() {
    long long lines = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&lines);
    ep_gc_maybe_collect();

    lines = create_list();
    ok = append_list(lines, (long long)"    long long capacity;\n");
    ok = append_list(lines, (long long)"    long long head;\n");
    ok = append_list(lines, (long long)"    long long tail;\n");
    ok = append_list(lines, (long long)"    long long size;\n");
    ok = append_list(lines, (long long)"} EpDeque;\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long create_deque(void) {\n");
    ok = append_list(lines, (long long)"    EpDeque* dq = malloc(sizeof(EpDeque));\n");
    ok = append_list(lines, (long long)"    if (!dq) return 0;\n");
    ok = append_list(lines, (long long)"    dq->capacity = 16;\n");
    ok = append_list(lines, (long long)"    dq->size = 0;\n");
    ok = append_list(lines, (long long)"    dq->head = 0;\n");
    ok = append_list(lines, (long long)"    dq->tail = 0;\n");
    ok = append_list(lines, (long long)"    dq->data = malloc(dq->capacity * sizeof(long long));\n");
    ok = append_list(lines, (long long)"    if (!dq->data) {\n");
    ok = append_list(lines, (long long)"        free(dq);\n");
    ok = append_list(lines, (long long)"        return 0;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    return (long long)dq;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"static void deque_resize(EpDeque* dq, long long new_capacity) {\n");
    ok = append_list(lines, (long long)"    long long* new_data = malloc(new_capacity * sizeof(long long));\n");
    ok = append_list(lines, (long long)"    for (long long i = 0; i < dq->size; i++) {\n");
    ok = append_list(lines, (long long)"        new_data[i] = dq->data[(dq->head + i) % dq->capacity];\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    free(dq->data);\n");
    ok = append_list(lines, (long long)"    dq->data = new_data;\n");
    ok = append_list(lines, (long long)"    dq->capacity = new_capacity;\n");
    ok = append_list(lines, (long long)"    dq->head = 0;\n");
    ok = append_list(lines, (long long)"    dq->tail = dq->size;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long deque_push_back(long long dq_ptr, long long value) {\n");
    ok = append_list(lines, (long long)"    EpDeque* dq = (EpDeque*)dq_ptr;\n");
    ok = append_list(lines, (long long)"    if (!dq) return 0;\n");
    ok = append_list(lines, (long long)"    if (dq->size >= dq->capacity) {\n");
    ok = append_list(lines, (long long)"        deque_resize(dq, dq->capacity * 2);\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    dq->data[dq->tail] = value;\n");
    ok = append_list(lines, (long long)"    dq->tail = (dq->tail + 1) % dq->capacity;\n");
    ok = append_list(lines, (long long)"    dq->size++;\n");
    ok = append_list(lines, (long long)"    return value;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long deque_push_front(long long dq_ptr, long long value) {\n");
    ok = append_list(lines, (long long)"    EpDeque* dq = (EpDeque*)dq_ptr;\n");
    ok = append_list(lines, (long long)"    if (!dq) return 0;\n");
    ok = append_list(lines, (long long)"    if (dq->size >= dq->capacity) {\n");
    ok = append_list(lines, (long long)"        deque_resize(dq, dq->capacity * 2);\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    dq->head = (dq->head - 1 + dq->capacity) % dq->capacity;\n");
    ok = append_list(lines, (long long)"    dq->data[dq->head] = value;\n");
    ok = append_list(lines, (long long)"    dq->size++;\n");
    ok = append_list(lines, (long long)"    return value;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long deque_pop_back(long long dq_ptr) {\n");
    ok = append_list(lines, (long long)"    EpDeque* dq = (EpDeque*)dq_ptr;\n");
    ok = append_list(lines, (long long)"    if (!dq || dq->size == 0) return 0;\n");
    ok = append_list(lines, (long long)"    dq->tail = (dq->tail - 1 + dq->capacity) % dq->capacity;\n");
    ok = append_list(lines, (long long)"    long long value = dq->data[dq->tail];\n");
    ok = append_list(lines, (long long)"    dq->size--;\n");
    ok = append_list(lines, (long long)"    return value;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long deque_pop_front(long long dq_ptr) {\n");
    ok = append_list(lines, (long long)"    EpDeque* dq = (EpDeque*)dq_ptr;\n");
    ok = append_list(lines, (long long)"    if (!dq || dq->size == 0) return 0;\n");
    ok = append_list(lines, (long long)"    long long value = dq->data[dq->head];\n");
    ok = append_list(lines, (long long)"    dq->head = (dq->head + 1) % dq->capacity;\n");
    ok = append_list(lines, (long long)"    dq->size--;\n");
    ok = append_list(lines, (long long)"    return value;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long deque_length(long long dq_ptr) {\n");
    ok = append_list(lines, (long long)"    EpDeque* dq = (EpDeque*)dq_ptr;\n");
    ok = append_list(lines, (long long)"    if (!dq) return 0;\n");
    ok = append_list(lines, (long long)"    return dq->size;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long free_deque(long long dq_ptr) {\n");
    ok = append_list(lines, (long long)"    EpDeque* dq = (EpDeque*)dq_ptr;\n");
    ok = append_list(lines, (long long)"    if (!dq) return 0;\n");
    ok = append_list(lines, (long long)"    free(dq->data);\n");
    ok = append_list(lines, (long long)"    free(dq);\n");
    ok = append_list(lines, (long long)"    return 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* Filesystem Operations */\n");
    ok = append_list(lines, (long long)"#include <dirent.h>\n");
    ok = append_list(lines, (long long)"#include <sys/stat.h>\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long fs_scan_dir(long long path_val) {\n");
    ok = append_list(lines, (long long)"    const char* path = (const char*)path_val;\n");
    ok = append_list(lines, (long long)"    long long list_ptr = create_list();\n");
    ok = append_list(lines, (long long)"    if (!path) return list_ptr;\n");
    ok = append_list(lines, (long long)"    DIR* d = opendir(path);\n");
    ok = append_list(lines, (long long)"    if (!d) return list_ptr;\n");
    ok = append_list(lines, (long long)"    struct dirent* dir;\n");
    ok = append_list(lines, (long long)"    while ((dir = readdir(d)) != NULL) {\n");
    ok = append_list(lines, (long long)"        if (strcmp(dir->d_name, \".\") == 0 || strcmp(dir->d_name, \"..\") == 0) {\n");
    ok = append_list(lines, (long long)"            continue;\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"        char* name = strdup(dir->d_name);\n");
    ok = append_list(lines, (long long)"        append_list(list_ptr, (long long)name);\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    closedir(d);\n");
    ok = append_list(lines, (long long)"    return list_ptr;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long fs_copy_file(long long src_val, long long dest_val) {\n");
    ok = append_list(lines, (long long)"    const char* src = (const char*)src_val;\n");
    ok = append_list(lines, (long long)"    const char* dest = (const char*)dest_val;\n");
    ok = append_list(lines, (long long)"    if (!src || !dest) return 0;\n");
    ok = append_list(lines, (long long)"    FILE* f_src = fopen(src, \"rb\");\n");
    ok = append_list(lines, (long long)"    if (!f_src) return 0;\n");
    ok = append_list(lines, (long long)"    FILE* f_dest = fopen(dest, \"wb\");\n");
    ok = append_list(lines, (long long)"    if (!f_dest) {\n");
    ok = append_list(lines, (long long)"        fclose(f_src);\n");
    ok = append_list(lines, (long long)"        return 0;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    char buf[4096];\n");
    ok = append_list(lines, (long long)"    size_t n;\n");
    ok = append_list(lines, (long long)"    while ((n = fread(buf, 1, sizeof(buf), f_src)) > 0) {\n");
    ok = append_list(lines, (long long)"        fwrite(buf, 1, n, f_dest);\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    fclose(f_src);\n");
    ok = append_list(lines, (long long)"    fclose(f_dest);\n");
    ok = append_list(lines, (long long)"    return 1;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long fs_delete_file(long long path_val) {\n");
    ok = append_list(lines, (long long)"    const char* path = (const char*)path_val;\n");
    ok = append_list(lines, (long long)"    if (!path) return 0;\n");
    ok = append_list(lines, (long long)"    return remove(path) == 0 ? 1 : 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long fs_move_file(long long src_val, long long dest_val) {\n");
    ok = append_list(lines, (long long)"    const char* src = (const char*)src_val;\n");
    ok = append_list(lines, (long long)"    const char* dest = (const char*)dest_val;\n");
    ok = append_list(lines, (long long)"    if (!src || !dest) return 0;\n");
    ok = append_list(lines, (long long)"    return rename(src, dest) == 0 ? 1 : 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long fs_exists(long long path_val) {\n");
    ok = append_list(lines, (long long)"    const char* path = (const char*)path_val;\n");
    ok = append_list(lines, (long long)"    if (!path) return 0;\n");
    ok = append_list(lines, (long long)"    struct stat st;\n");
    ok = append_list(lines, (long long)"    return stat(path, &st) == 0 ? 1 : 0;\n");
    ret_val = join_strings(lines);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(1);
    return ret_val;
}

long long ep_rt_core_19() {
    long long lines = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&lines);
    ep_gc_maybe_collect();

    lines = create_list();
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long fs_is_dir(long long path_val) {\n");
    ok = append_list(lines, (long long)"    const char* path = (const char*)path_val;\n");
    ok = append_list(lines, (long long)"    if (!path) return 0;\n");
    ok = append_list(lines, (long long)"    struct stat st;\n");
    ok = append_list(lines, (long long)"    if (stat(path, &st) != 0) return 0;\n");
    ok = append_list(lines, (long long)"    return S_ISDIR(st.st_mode) ? 1 : 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long fs_is_file(long long path_val) {\n");
    ok = append_list(lines, (long long)"    const char* path = (const char*)path_val;\n");
    ok = append_list(lines, (long long)"    if (!path) return 0;\n");
    ok = append_list(lines, (long long)"    struct stat st;\n");
    ok = append_list(lines, (long long)"    if (stat(path, &st) != 0) return 0;\n");
    ok = append_list(lines, (long long)"    return S_ISREG(st.st_mode) ? 1 : 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long fs_get_size(long long path_val) {\n");
    ok = append_list(lines, (long long)"    const char* path = (const char*)path_val;\n");
    ok = append_list(lines, (long long)"    if (!path) return 0;\n");
    ok = append_list(lines, (long long)"    struct stat st;\n");
    ok = append_list(lines, (long long)"    if (stat(path, &st) != 0) return 0;\n");
    ok = append_list(lines, (long long)"    return (long long)st.st_size;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* HTTP Client */\n");
    ok = append_list(lines, (long long)"#ifdef __wasm__\n");
    ok = append_list(lines, (long long)"long long ep_http_request(long long method_val, long long url_val, long long headers_val, long long body_val) {\n");
    ok = append_list(lines, (long long)"    (void)method_val; (void)url_val; (void)headers_val; (void)body_val;\n");
    ok = append_list(lines, (long long)"    return (long long)strdup(\"Error: HTTP request is not supported on WebAssembly\");\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"#else\n");
    ok = append_list(lines, (long long)"long long ep_http_request(long long method_val, long long url_val, long long headers_val, long long body_val) {\n");
    ok = append_list(lines, (long long)"    const char* method = (const char*)method_val;\n");
    ok = append_list(lines, (long long)"    const char* url = (const char*)url_val;\n");
    ok = append_list(lines, (long long)"    const char* headers = (const char*)headers_val;\n");
    ok = append_list(lines, (long long)"    const char* body = (const char*)body_val;\n");
    ok = append_list(lines, (long long)"    if (!method || !url) return (long long)strdup(\"\");\n");
    ok = append_list(lines, (long long)"    if (strncmp(url, \"http://\", 7) != 0) {\n");
    ok = append_list(lines, (long long)"        return (long long)strdup(\"Error: only http:// protocol supported\");\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    const char* host_start = url + 7;\n");
    ok = append_list(lines, (long long)"    const char* path_start = strchr(host_start, '/');\n");
    ok = append_list(lines, (long long)"    char host[256];\n");
    ok = append_list(lines, (long long)"    char path[1024];\n");
    ok = append_list(lines, (long long)"    if (path_start) {\n");
    ok = append_list(lines, (long long)"        size_t host_len = path_start - host_start;\n");
    ok = append_list(lines, (long long)"        if (host_len >= sizeof(host)) host_len = sizeof(host) - 1;\n");
    ok = append_list(lines, (long long)"        strncpy(host, host_start, host_len);\n");
    ok = append_list(lines, (long long)"        host[host_len] = '\\0';\n");
    ok = append_list(lines, (long long)"        strncpy(path, path_start, sizeof(path) - 1);\n");
    ok = append_list(lines, (long long)"        path[sizeof(path) - 1] = '\\0';\n");
    ok = append_list(lines, (long long)"    } else {\n");
    ok = append_list(lines, (long long)"        strncpy(host, host_start, sizeof(host) - 1);\n");
    ok = append_list(lines, (long long)"        host[sizeof(host) - 1] = '\\0';\n");
    ok = append_list(lines, (long long)"        strcpy(path, \"/\");\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    int port = 80;\n");
    ok = append_list(lines, (long long)"    char* colon = strchr(host, ':');\n");
    ok = append_list(lines, (long long)"    if (colon) {\n");
    ok = append_list(lines, (long long)"        *colon = '\\0';\n");
    ok = append_list(lines, (long long)"        port = atoi(colon + 1);\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    int sockfd = socket(AF_INET, SOCK_STREAM, 0);\n");
    ok = append_list(lines, (long long)"    if (sockfd < 0) return (long long)strdup(\"Error: socket creation failed\");\n");
    ok = append_list(lines, (long long)"    struct hostent* server = gethostbyname(host);\n");
    ok = append_list(lines, (long long)"    if (!server) {\n");
    ok = append_list(lines, (long long)"        close(sockfd);\n");
    ok = append_list(lines, (long long)"        return (long long)strdup(\"Error: host resolution failed\");\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    struct sockaddr_in serv_addr;\n");
    ok = append_list(lines, (long long)"    memset(&serv_addr, 0, sizeof(serv_addr));\n");
    ok = append_list(lines, (long long)"    serv_addr.sin_family = AF_INET;\n");
    ok = append_list(lines, (long long)"    memcpy(&serv_addr.sin_addr.s_addr, server->h_addr_list[0], server->h_length);\n");
    ok = append_list(lines, (long long)"    serv_addr.sin_port = htons(port);\n");
    ok = append_list(lines, (long long)"    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {\n");
    ok = append_list(lines, (long long)"        close(sockfd);\n");
    ok = append_list(lines, (long long)"        return (long long)strdup(\"Error: connection failed\");\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    char req[4096];\n");
    ok = append_list(lines, (long long)"    size_t body_len = body ? strlen(body) : 0;\n");
    ok = append_list(lines, (long long)"    int req_len = snprintf(req, sizeof(req),\n");
    ok = append_list(lines, (long long)"        \"%s %s HTTP/1.1\\r\\n\"\n");
    ok = append_list(lines, (long long)"        \"Host: %s\\r\\n\"\n");
    ok = append_list(lines, (long long)"        \"Content-Length: %zu\\r\\n\"\n");
    ok = append_list(lines, (long long)"        \"Connection: close\\r\\n\"\n");
    ok = append_list(lines, (long long)"        \"%s%s\"\n");
    ok = append_list(lines, (long long)"        \"\\r\\n\",\n");
    ok = append_list(lines, (long long)"        method, path, host, body_len, headers ? headers : \"\", (headers && strlen(headers) > 0 && headers[strlen(headers)-1] != '\\n') ? \"\\r\\n\" : \"\");\n");
    ok = append_list(lines, (long long)"    if (send(sockfd, req, req_len, 0) < 0) {\n");
    ok = append_list(lines, (long long)"        close(sockfd);\n");
    ok = append_list(lines, (long long)"        return (long long)strdup(\"Error: send failed\");\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    if (body_len > 0) {\n");
    ok = append_list(lines, (long long)"        if (send(sockfd, body, body_len, 0) < 0) {\n");
    ok = append_list(lines, (long long)"            close(sockfd);\n");
    ok = append_list(lines, (long long)"            return (long long)strdup(\"Error: send body failed\");\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    size_t resp_cap = 4096;\n");
    ok = append_list(lines, (long long)"    size_t resp_len = 0;\n");
    ok = append_list(lines, (long long)"    char* resp = malloc(resp_cap);\n");
    ok = append_list(lines, (long long)"    if (!resp) {\n");
    ok = append_list(lines, (long long)"        close(sockfd);\n");
    ok = append_list(lines, (long long)"        return (long long)strdup(\"\");\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    char recv_buf[4096];\n");
    ok = append_list(lines, (long long)"    ssize_t n;\n");
    ok = append_list(lines, (long long)"    while ((n = recv(sockfd, recv_buf, sizeof(recv_buf), 0)) > 0) {\n");
    ok = append_list(lines, (long long)"        if (resp_len + n >= resp_cap) {\n");
    ok = append_list(lines, (long long)"            resp_cap *= 2;\n");
    ok = append_list(lines, (long long)"            char* new_resp = realloc(resp, resp_cap);\n");
    ok = append_list(lines, (long long)"            if (!new_resp) {\n");
    ok = append_list(lines, (long long)"                free(resp);\n");
    ok = append_list(lines, (long long)"                close(sockfd);\n");
    ok = append_list(lines, (long long)"                return (long long)strdup(\"Error: memory allocation failed\");\n");
    ok = append_list(lines, (long long)"            }\n");
    ok = append_list(lines, (long long)"            resp = new_resp;\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"        memcpy(resp + resp_len, recv_buf, n);\n");
    ok = append_list(lines, (long long)"        resp_len += n;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    resp[resp_len] = '\\0';\n");
    ok = append_list(lines, (long long)"    close(sockfd);\n");
    ok = append_list(lines, (long long)"    // Strip HTTP headers — return only the body after \\r\\n\\r\\n\n");
    ok = append_list(lines, (long long)"    char* http_body = strstr(resp, \"\\r\\n\\r\\n\");\n");
    ok = append_list(lines, (long long)"    if (http_body) {\n");
    ok = append_list(lines, (long long)"        http_body += 4;\n");
    ok = append_list(lines, (long long)"        char* result = strdup(http_body);\n");
    ok = append_list(lines, (long long)"        free(resp);\n");
    ok = append_list(lines, (long long)"        return (long long)result;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    return (long long)resp;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"#endif\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"#define ROTRIGHT(word,bits) (((word) >> (bits)) | ((word) << (32-(bits))))\n");
    ok = append_list(lines, (long long)"#define CH(x,y,z) (((x) & (y)) ^ (~(x) & (z)))\n");
    ok = append_list(lines, (long long)"#define MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))\n");
    ok = append_list(lines, (long long)"#define EP0(x) (ROTRIGHT(x,2) ^ ROTRIGHT(x,13) ^ ROTRIGHT(x,22))\n");
    ok = append_list(lines, (long long)"#define EP1(x) (ROTRIGHT(x,6) ^ ROTRIGHT(x,11) ^ ROTRIGHT(x,25))\n");
    ok = append_list(lines, (long long)"#define SIG0(x) (ROTRIGHT(x,7) ^ ROTRIGHT(x,18) ^ ((x) >> 3))\n");
    ok = append_list(lines, (long long)"#define SIG1(x) (ROTRIGHT(x,17) ^ ROTRIGHT(x,19) ^ ((x) >> 10))\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"typedef struct {\n");
    ok = append_list(lines, (long long)"    unsigned char data[64];\n");
    ok = append_list(lines, (long long)"    unsigned int datalen;\n");
    ok = append_list(lines, (long long)"    unsigned long long bitlen;\n");
    ok = append_list(lines, (long long)"    unsigned int state[8];\n");
    ret_val = join_strings(lines);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(1);
    return ret_val;
}

long long ep_rt_core_20() {
    long long lines = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&lines);
    ep_gc_maybe_collect();

    lines = create_list();
    ok = append_list(lines, (long long)"} EP_SHA256_CTX;\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"static const unsigned int sha256_k[64] = {\n");
    ok = append_list(lines, (long long)"    0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,\n");
    ok = append_list(lines, (long long)"    0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,\n");
    ok = append_list(lines, (long long)"    0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,\n");
    ok = append_list(lines, (long long)"    0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,\n");
    ok = append_list(lines, (long long)"    0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,\n");
    ok = append_list(lines, (long long)"    0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,\n");
    ok = append_list(lines, (long long)"    0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,\n");
    ok = append_list(lines, (long long)"    0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2\n");
    ok = append_list(lines, (long long)"};\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"void ep_sha256_transform(EP_SHA256_CTX *ctx, const unsigned char *data) {\n");
    ok = append_list(lines, (long long)"    unsigned int a, b, c, d, e, f, g, h, i, j, t1, t2, m[64];\n");
    ok = append_list(lines, (long long)"    for (i = 0, j = 0; i < 16; ++i, j += 4)\n");
    ok = append_list(lines, (long long)"        m[i] = (data[j] << 24) | (data[j + 1] << 16) | (data[j + 2] << 8) | (data[j + 3]);\n");
    ok = append_list(lines, (long long)"    for ( ; i < 64; ++i)\n");
    ok = append_list(lines, (long long)"        m[i] = SIG1(m[i - 2]) + m[i - 7] + SIG0(m[i - 15]) + m[i - 16];\n");
    ok = append_list(lines, (long long)"    a = ctx->state[0]; b = ctx->state[1]; c = ctx->state[2]; d = ctx->state[3];\n");
    ok = append_list(lines, (long long)"    e = ctx->state[4]; f = ctx->state[5]; g = ctx->state[6]; h = ctx->state[7];\n");
    ok = append_list(lines, (long long)"    for (i = 0; i < 64; ++i) {\n");
    ok = append_list(lines, (long long)"        t1 = h + EP1(e) + CH(e,f,g) + sha256_k[i] + m[i];\n");
    ok = append_list(lines, (long long)"        t2 = EP0(a) + MAJ(a,b,c);\n");
    ok = append_list(lines, (long long)"        h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    ctx->state[0] += a; ctx->state[1] += b; ctx->state[2] += c; ctx->state[3] += d;\n");
    ok = append_list(lines, (long long)"    ctx->state[4] += e; ctx->state[5] += f; ctx->state[6] += g; ctx->state[7] += h;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"void ep_sha256_init(EP_SHA256_CTX *ctx) {\n");
    ok = append_list(lines, (long long)"    ctx->datalen = 0; ctx->bitlen = 0;\n");
    ok = append_list(lines, (long long)"    ctx->state[0] = 0x6a09e667; ctx->state[1] = 0xbb67ae85; ctx->state[2] = 0x3c6ef372; ctx->state[3] = 0xa54ff53a;\n");
    ok = append_list(lines, (long long)"    ctx->state[4] = 0x510e527f; ctx->state[5] = 0x9b05688c; ctx->state[6] = 0x1f83d9ab; ctx->state[7] = 0x5be0cd19;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"void ep_sha256_update(EP_SHA256_CTX *ctx, const unsigned char *data, size_t len) {\n");
    ok = append_list(lines, (long long)"    for (size_t i = 0; i < len; ++i) {\n");
    ok = append_list(lines, (long long)"        ctx->data[ctx->datalen] = data[i];\n");
    ok = append_list(lines, (long long)"        ctx->datalen++;\n");
    ok = append_list(lines, (long long)"        if (ctx->datalen == 64) {\n");
    ok = append_list(lines, (long long)"            ep_sha256_transform(ctx, ctx->data);\n");
    ok = append_list(lines, (long long)"            ctx->bitlen += 512;\n");
    ok = append_list(lines, (long long)"            ctx->datalen = 0;\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"void ep_sha256_final(EP_SHA256_CTX *ctx, unsigned char *hash) {\n");
    ok = append_list(lines, (long long)"    unsigned int i = ctx->datalen;\n");
    ok = append_list(lines, (long long)"    if (ctx->datalen < 56) {\n");
    ok = append_list(lines, (long long)"        ctx->data[i++] = 0x80;\n");
    ok = append_list(lines, (long long)"        while (i < 56) ctx->data[i++] = 0x00;\n");
    ok = append_list(lines, (long long)"    } else {\n");
    ok = append_list(lines, (long long)"        ctx->data[i++] = 0x80;\n");
    ok = append_list(lines, (long long)"        while (i < 64) ctx->data[i++] = 0x00;\n");
    ok = append_list(lines, (long long)"        ep_sha256_transform(ctx, ctx->data);\n");
    ok = append_list(lines, (long long)"        memset(ctx->data, 0, 56);\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    ctx->bitlen += ctx->datalen * 8;\n");
    ok = append_list(lines, (long long)"    ctx->data[63] = ctx->bitlen; ctx->data[62] = ctx->bitlen >> 8;\n");
    ok = append_list(lines, (long long)"    ctx->data[61] = ctx->bitlen >> 16; ctx->data[60] = ctx->bitlen >> 24;\n");
    ok = append_list(lines, (long long)"    ctx->data[59] = ctx->bitlen >> 32; ctx->data[58] = ctx->bitlen >> 40;\n");
    ok = append_list(lines, (long long)"    ctx->data[57] = ctx->bitlen >> 48; ctx->data[56] = ctx->bitlen >> 56;\n");
    ok = append_list(lines, (long long)"    ep_sha256_transform(ctx, ctx->data);\n");
    ok = append_list(lines, (long long)"    for (i = 0; i < 4; ++i) {\n");
    ok = append_list(lines, (long long)"        hash[i]      = (ctx->state[0] >> (24 - i * 8)) & 0x000000ff;\n");
    ok = append_list(lines, (long long)"        hash[i + 4]  = (ctx->state[1] >> (24 - i * 8)) & 0x000000ff;\n");
    ok = append_list(lines, (long long)"        hash[i + 8]  = (ctx->state[2] >> (24 - i * 8)) & 0x000000ff;\n");
    ok = append_list(lines, (long long)"        hash[i + 12] = (ctx->state[3] >> (24 - i * 8)) & 0x000000ff;\n");
    ok = append_list(lines, (long long)"        hash[i + 16] = (ctx->state[4] >> (24 - i * 8)) & 0x000000ff;\n");
    ok = append_list(lines, (long long)"        hash[i + 20] = (ctx->state[5] >> (24 - i * 8)) & 0x000000ff;\n");
    ok = append_list(lines, (long long)"        hash[i + 24] = (ctx->state[6] >> (24 - i * 8)) & 0x000000ff;\n");
    ok = append_list(lines, (long long)"        hash[i + 28] = (ctx->state[7] >> (24 - i * 8)) & 0x000000ff;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"char* ep_sha256(const char* s) {\n");
    ok = append_list(lines, (long long)"    if (!s) s = \"\";\n");
    ok = append_list(lines, (long long)"    EP_SHA256_CTX ctx;\n");
    ok = append_list(lines, (long long)"    ep_sha256_init(&ctx);\n");
    ok = append_list(lines, (long long)"    ep_sha256_update(&ctx, (const unsigned char*)s, strlen(s));\n");
    ok = append_list(lines, (long long)"    unsigned char hash[32];\n");
    ok = append_list(lines, (long long)"    ep_sha256_final(&ctx, hash);\n");
    ok = append_list(lines, (long long)"    char* result = malloc(65);\n");
    ok = append_list(lines, (long long)"    if (result) {\n");
    ok = append_list(lines, (long long)"        for (int i = 0; i < 32; i++) {\n");
    ok = append_list(lines, (long long)"            snprintf(result + (i * 2), 3, \"%02x\", hash[i]);\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"        result[64] = '\\0';\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    return result;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* RFC 2104 HMAC-SHA256. Operates on raw bytes with explicit lengths (binary\n");
    ok = append_list(lines, (long long)"   safe), so keys/messages containing NUL bytes hash correctly. Returns a\n");
    ok = append_list(lines, (long long)"   malloc'd 64-char lowercase hex string. */\n");
    ok = append_list(lines, (long long)"long long ep_hmac_sha256(long long key_ptr, long long key_len, long long msg_ptr, long long msg_len) {\n");
    ok = append_list(lines, (long long)"    const unsigned char* key = (const unsigned char*)key_ptr;\n");
    ok = append_list(lines, (long long)"    const unsigned char* msg = (const unsigned char*)msg_ptr;\n");
    ok = append_list(lines, (long long)"    size_t klen = (size_t)key_len;\n");
    ok = append_list(lines, (long long)"    size_t mlen = (size_t)msg_len;\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"    unsigned char k0[64];\n");
    ok = append_list(lines, (long long)"    memset(k0, 0, sizeof(k0));\n");
    ok = append_list(lines, (long long)"    if (klen > 64) {\n");
    ok = append_list(lines, (long long)"        /* Keys longer than the block size are replaced by their hash. */\n");
    ok = append_list(lines, (long long)"        EP_SHA256_CTX kc;\n");
    ok = append_list(lines, (long long)"        ep_sha256_init(&kc);\n");
    ok = append_list(lines, (long long)"        ep_sha256_update(&kc, key ? key : (const unsigned char*)\"\", klen);\n");
    ok = append_list(lines, (long long)"        unsigned char kh[32];\n");
    ok = append_list(lines, (long long)"        ep_sha256_final(&kc, kh);\n");
    ok = append_list(lines, (long long)"        memcpy(k0, kh, 32);\n");
    ok = append_list(lines, (long long)"    } else if (key) {\n");
    ok = append_list(lines, (long long)"        memcpy(k0, key, klen);\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"    unsigned char ipad[64], opad[64];\n");
    ok = append_list(lines, (long long)"    for (int i = 0; i < 64; i++) {\n");
    ok = append_list(lines, (long long)"        ipad[i] = k0[i] ^ 0x36;\n");
    ok = append_list(lines, (long long)"        opad[i] = k0[i] ^ 0x5c;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"    /* inner = H((K0 ^ ipad) || message) */\n");
    ok = append_list(lines, (long long)"    EP_SHA256_CTX ic;\n");
    ok = append_list(lines, (long long)"    ep_sha256_init(&ic);\n");
    ok = append_list(lines, (long long)"    ep_sha256_update(&ic, ipad, 64);\n");
    ok = append_list(lines, (long long)"    if (msg && mlen) ep_sha256_update(&ic, msg, mlen);\n");
    ok = append_list(lines, (long long)"    unsigned char inner[32];\n");
    ok = append_list(lines, (long long)"    ep_sha256_final(&ic, inner);\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"    /* mac = H((K0 ^ opad) || inner) */\n");
    ok = append_list(lines, (long long)"    EP_SHA256_CTX oc;\n");
    ok = append_list(lines, (long long)"    ep_sha256_init(&oc);\n");
    ok = append_list(lines, (long long)"    ep_sha256_update(&oc, opad, 64);\n");
    ok = append_list(lines, (long long)"    ep_sha256_update(&oc, inner, 32);\n");
    ok = append_list(lines, (long long)"    unsigned char mac[32];\n");
    ok = append_list(lines, (long long)"    ep_sha256_final(&oc, mac);\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"    char* out = (char*)malloc(65);\n");
    ok = append_list(lines, (long long)"    if (out) {\n");
    ok = append_list(lines, (long long)"        for (int i = 0; i < 32; i++) {\n");
    ok = append_list(lines, (long long)"            snprintf(out + (i * 2), 3, \"%02x\", mac[i]);\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"        out[64] = '\\0';\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    return (long long)out;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"typedef struct {\n");
    ret_val = join_strings(lines);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(1);
    return ret_val;
}

long long ep_rt_core_21() {
    long long lines = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&lines);
    ep_gc_maybe_collect();

    lines = create_list();
    ok = append_list(lines, (long long)"    unsigned int count[2];\n");
    ok = append_list(lines, (long long)"    unsigned int state[4];\n");
    ok = append_list(lines, (long long)"    unsigned char buffer[64];\n");
    ok = append_list(lines, (long long)"} EP_MD5_CTX;\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"#define F(x,y,z) (((x) & (y)) | (~(x) & (z)))\n");
    ok = append_list(lines, (long long)"#define G(x,y,z) (((x) & (z)) | ((y) & ~(z)))\n");
    ok = append_list(lines, (long long)"#define H(x,y,z) ((x) ^ (y) ^ (z))\n");
    ok = append_list(lines, (long long)"#define I(x,y,z) ((y) ^ ((x) | ~(z)))\n");
    ok = append_list(lines, (long long)"#define ROTATE_LEFT(x,n) (((x) << (n)) | ((x) >> (32-(n))))\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"#define FF(a,b,c,d,x,s,ac) { \\\n");
    ok = append_list(lines, (long long)"    (a) += F((b),(c),(d)) + (x) + (ac); \\\n");
    ok = append_list(lines, (long long)"    (a) = ROTATE_LEFT((a),(s)); \\\n");
    ok = append_list(lines, (long long)"    (a) += (b); \\\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"#define GG(a,b,c,d,x,s,ac) { \\\n");
    ok = append_list(lines, (long long)"    (a) += G((b),(c),(d)) + (x) + (ac); \\\n");
    ok = append_list(lines, (long long)"    (a) = ROTATE_LEFT((a),(s)); \\\n");
    ok = append_list(lines, (long long)"    (a) += (b); \\\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"#define HH(a,b,c,d,x,s,ac) { \\\n");
    ok = append_list(lines, (long long)"    (a) += H((b),(c),(d)) + (x) + (ac); \\\n");
    ok = append_list(lines, (long long)"    (a) = ROTATE_LEFT((a),(s)); \\\n");
    ok = append_list(lines, (long long)"    (a) += (b); \\\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"#define II(a,b,c,d,x,s,ac) { \\\n");
    ok = append_list(lines, (long long)"    (a) += I((b),(c),(d)) + (x) + (ac); \\\n");
    ok = append_list(lines, (long long)"    (a) = ROTATE_LEFT((a),(s)); \\\n");
    ok = append_list(lines, (long long)"    (a) += (b); \\\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"void ep_md5_init(EP_MD5_CTX *ctx) {\n");
    ok = append_list(lines, (long long)"    ctx->count[0] = ctx->count[1] = 0;\n");
    ok = append_list(lines, (long long)"    ctx->state[0] = 0x67452301;\n");
    ok = append_list(lines, (long long)"    ctx->state[1] = 0xefcdab89;\n");
    ok = append_list(lines, (long long)"    ctx->state[2] = 0x98badcfe;\n");
    ok = append_list(lines, (long long)"    ctx->state[3] = 0x10325476;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"void ep_md5_transform(unsigned int state[4], const unsigned char block[64]) {\n");
    ok = append_list(lines, (long long)"    unsigned int a = state[0], b = state[1], c = state[2], d = state[3], x[16];\n");
    ok = append_list(lines, (long long)"    for (int i = 0, j = 0; i < 16; i++, j += 4)\n");
    ok = append_list(lines, (long long)"        x[i] = (block[j]) | (block[j+1] << 8) | (block[j+2] << 16) | (block[j+3] << 24);\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"    FF(a, b, c, d, x[0], 7, 0xd76aa478); FF(d, a, b, c, x[1], 12, 0xe8c7b756); FF(c, d, a, b, x[2], 17, 0x242070db); FF(b, c, d, a, x[3], 22, 0xc1bdceee);\n");
    ok = append_list(lines, (long long)"    FF(a, b, c, d, x[4], 7, 0xf57c0faf); FF(d, a, b, c, x[5], 12, 0x4787c62a); FF(c, d, a, b, x[6], 17, 0xa8304613); FF(b, c, d, a, x[7], 22, 0xfd469501);\n");
    ok = append_list(lines, (long long)"    FF(a, b, c, d, x[8], 7, 0x698098d8); FF(d, a, b, c, x[9], 12, 0x8b44f7af); FF(c, d, a, b, x[10], 17, 0xffff5bb1); FF(b, c, d, a, x[11], 22, 0x895cd7be);\n");
    ok = append_list(lines, (long long)"    FF(a, b, c, d, x[12], 7, 0x6b901122); FF(d, a, b, c, x[13], 12, 0xfd987193); FF(c, d, a, b, x[14], 17, 0xa679438e); FF(b, c, d, a, x[15], 22, 0x49b40821);\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"    GG(a, b, c, d, x[1], 5, 0xf61e2562); GG(d, a, b, c, x[6], 9, 0xc040b340); GG(c, d, a, b, x[11], 14, 0x265e5a51); GG(b, c, d, a, x[0], 20, 0xe9b6c7aa);\n");
    ok = append_list(lines, (long long)"    GG(a, b, c, d, x[5], 5, 0xd62f105d); GG(d, a, b, c, x[10], 9, 0x02441453); GG(c, d, a, b, x[15], 14, 0xd8a1e681); GG(b, c, d, a, x[4], 20, 0xe7d3fbc8);\n");
    ok = append_list(lines, (long long)"    GG(a, b, c, d, x[9], 5, 0x21e1cde6); GG(d, a, b, c, x[14], 9, 0xc33707d6); GG(c, d, a, b, x[3], 14, 0xf4d50d87); GG(b, c, d, a, x[8], 20, 0x455a14ed);\n");
    ok = append_list(lines, (long long)"    GG(a, b, c, d, x[13], 5, 0xa9e3e905); GG(d, a, b, c, x[2], 9, 0xfcefa3f8); GG(c, d, a, b, x[7], 14, 0x676f02d9); GG(b, c, d, a, x[12], 20, 0x8d2a4c8a);\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"    HH(a, b, c, d, x[5], 4, 0xfffa3942); HH(d, a, b, c, x[8], 11, 0x8771f681); HH(c, d, a, b, x[11], 16, 0x6d9d6122); HH(b, c, d, a, x[14], 23, 0xfde5380c);\n");
    ok = append_list(lines, (long long)"    HH(a, b, c, d, x[1], 4, 0xa4beea44); HH(d, a, b, c, x[4], 11, 0x4bdecfa9); HH(c, d, a, b, x[7], 16, 0xf6bb4b60); HH(b, c, d, a, x[10], 23, 0xbebfbc70);\n");
    ok = append_list(lines, (long long)"    HH(a, b, c, d, x[13], 4, 0x289b7ec6); HH(d, a, b, c, x[0], 11, 0xeaa127fa); HH(c, d, a, b, x[3], 16, 0xd4ef3085); HH(b, c, d, a, x[6], 23, 0x04881d05);\n");
    ok = append_list(lines, (long long)"    HH(a, b, c, d, x[9], 4, 0xd9d4d039); HH(d, a, b, c, x[12], 11, 0xe6db99e5); HH(c, d, a, b, x[15], 16, 0x1fa27cf8); HH(b, c, d, a, x[2], 23, 0xc4ac5665);\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"    II(a, b, c, d, x[0], 6, 0xf4292244); II(d, a, b, c, x[7], 10, 0x432aff97); II(c, d, a, b, x[14], 15, 0xab9423a7); II(b, c, d, a, x[5], 21, 0xfc93a039);\n");
    ok = append_list(lines, (long long)"    II(a, b, c, d, x[12], 6, 0x655b59c3); II(d, a, b, c, x[3], 10, 0x8f0ccc92); II(c, d, a, b, x[10], 15, 0xffeff47d); II(b, c, d, a, x[1], 21, 0x85845dd1);\n");
    ok = append_list(lines, (long long)"    II(a, b, c, d, x[8], 6, 0x6fa87e4f); II(d, a, b, c, x[15], 10, 0xfe2ce6e0); II(c, d, a, b, x[6], 15, 0xa3014314); II(b, c, d, a, x[13], 21, 0x4e0811a1);\n");
    ok = append_list(lines, (long long)"    II(a, b, c, d, x[4], 6, 0xf7537e82); II(d, a, b, c, x[11], 10, 0xbd3af235); II(c, d, a, b, x[2], 15, 0x2ad7d2bb); II(b, c, d, a, x[9], 21, 0xeb86d391);\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"    state[0] += a; state[1] += b; state[2] += c; state[3] += d;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"void ep_md5_update(EP_MD5_CTX *ctx, const unsigned char *input, size_t input_len) {\n");
    ok = append_list(lines, (long long)"    unsigned int i = 0, index = (ctx->count[0] >> 3) & 0x3F, part_len = 64 - index;\n");
    ok = append_list(lines, (long long)"    ctx->count[0] += input_len << 3;\n");
    ok = append_list(lines, (long long)"    if (ctx->count[0] < (input_len << 3)) ctx->count[1]++;\n");
    ok = append_list(lines, (long long)"    ctx->count[1] += input_len >> 29;\n");
    ok = append_list(lines, (long long)"    if (input_len >= part_len) {\n");
    ok = append_list(lines, (long long)"        memcpy(&ctx->buffer[index], input, part_len);\n");
    ok = append_list(lines, (long long)"        ep_md5_transform(ctx->state, ctx->buffer);\n");
    ok = append_list(lines, (long long)"        for (i = part_len; i + 63 < input_len; i += 64)\n");
    ok = append_list(lines, (long long)"            ep_md5_transform(ctx->state, &input[i]);\n");
    ok = append_list(lines, (long long)"        index = 0;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    memcpy(&ctx->buffer[index], &input[i], input_len - i);\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"void ep_md5_final(EP_MD5_CTX *ctx, unsigned char digest[16]) {\n");
    ok = append_list(lines, (long long)"    unsigned char bits[8];\n");
    ok = append_list(lines, (long long)"    bits[0] = ctx->count[0]; bits[1] = ctx->count[0] >> 8; bits[2] = ctx->count[0] >> 16; bits[3] = ctx->count[0] >> 24;\n");
    ok = append_list(lines, (long long)"    bits[4] = ctx->count[1]; bits[5] = ctx->count[1] >> 8; bits[6] = ctx->count[1] >> 16; bits[7] = ctx->count[1] >> 24;\n");
    ok = append_list(lines, (long long)"    unsigned int index = (ctx->count[0] >> 3) & 0x3F, pad_len = (index < 56) ? (56 - index) : (120 - index);\n");
    ok = append_list(lines, (long long)"    unsigned char padding[64];\n");
    ok = append_list(lines, (long long)"    memset(padding, 0, 64); padding[0] = 0x80;\n");
    ok = append_list(lines, (long long)"    ep_md5_update(ctx, padding, pad_len);\n");
    ok = append_list(lines, (long long)"    ep_md5_update(ctx, bits, 8);\n");
    ok = append_list(lines, (long long)"    for (int i = 0; i < 4; i++) {\n");
    ok = append_list(lines, (long long)"        digest[i*4]     = ctx->state[i];\n");
    ok = append_list(lines, (long long)"        digest[i*4 + 1] = ctx->state[i] >> 8;\n");
    ok = append_list(lines, (long long)"        digest[i*4 + 2] = ctx->state[i] >> 16;\n");
    ok = append_list(lines, (long long)"        digest[i*4 + 3] = ctx->state[i] >> 24;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"char* ep_md5(const char* s) {\n");
    ok = append_list(lines, (long long)"    if (!s) s = \"\";\n");
    ok = append_list(lines, (long long)"    EP_MD5_CTX ctx;\n");
    ok = append_list(lines, (long long)"    ep_md5_init(&ctx);\n");
    ok = append_list(lines, (long long)"    ep_md5_update(&ctx, (const unsigned char*)s, strlen(s));\n");
    ok = append_list(lines, (long long)"    unsigned char hash[16];\n");
    ok = append_list(lines, (long long)"    ep_md5_final(&ctx, hash);\n");
    ok = append_list(lines, (long long)"    char* result = malloc(33);\n");
    ok = append_list(lines, (long long)"    if (result) {\n");
    ok = append_list(lines, (long long)"        for (int i = 0; i < 16; i++) {\n");
    ok = append_list(lines, (long long)"            snprintf(result + (i * 2), 3, \"%02x\", hash[i]);\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"        result[32] = '\\0';\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    return result;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"char* read_file_content(const char* filepath) {\n");
    ok = append_list(lines, (long long)"    char mode[3];\n");
    ok = append_list(lines, (long long)"    mode[0] = 'r';\n");
    ok = append_list(lines, (long long)"    mode[1] = 'b';\n");
    ok = append_list(lines, (long long)"    mode[2] = '\\0';\n");
    ok = append_list(lines, (long long)"    FILE* f = fopen(filepath, mode);\n");
    ok = append_list(lines, (long long)"    if (!f) {\n");
    ok = append_list(lines, (long long)"        char* empty = malloc(1);\n");
    ok = append_list(lines, (long long)"        if (empty) empty[0] = '\\0';\n");
    ok = append_list(lines, (long long)"        ep_gc_register(empty, EP_OBJ_STRING);\n");
    ok = append_list(lines, (long long)"        return empty;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    fseek(f, 0, SEEK_END);\n");
    ok = append_list(lines, (long long)"    long size = ftell(f);\n");
    ok = append_list(lines, (long long)"    fseek(f, 0, SEEK_SET);\n");
    ok = append_list(lines, (long long)"    char* buf = malloc(size + 1);\n");
    ok = append_list(lines, (long long)"    if (!buf) {\n");
    ok = append_list(lines, (long long)"        fclose(f);\n");
    ok = append_list(lines, (long long)"        char* empty = malloc(1);\n");
    ok = append_list(lines, (long long)"        if (empty) empty[0] = '\\0';\n");
    ok = append_list(lines, (long long)"        ep_gc_register(empty, EP_OBJ_STRING);\n");
    ok = append_list(lines, (long long)"        return empty;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    size_t read_bytes = fread(buf, 1, size, f);\n");
    ok = append_list(lines, (long long)"    buf[read_bytes] = '\\0';\n");
    ok = append_list(lines, (long long)"    fclose(f);\n");
    ok = append_list(lines, (long long)"    ep_gc_register(buf, EP_OBJ_STRING);\n");
    ok = append_list(lines, (long long)"    return buf;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long string_length(const char* s) {\n");
    ok = append_list(lines, (long long)"    if (!s) return 0;\n");
    ok = append_list(lines, (long long)"    return strlen(s);\n");
    ret_val = join_strings(lines);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(1);
    return ret_val;
}

long long ep_rt_core_22() {
    long long lines = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&lines);
    ep_gc_maybe_collect();

    lines = create_list();
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long get_character(const char* s, long long index) {\n");
    ok = append_list(lines, (long long)"    if (!s) return 0;\n");
    ok = append_list(lines, (long long)"    long long len = strlen(s);\n");
    ok = append_list(lines, (long long)"    if (index < 0 || index >= len) return 0;\n");
    ok = append_list(lines, (long long)"    return (unsigned char)s[index];\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long create_list(void) {\n");
    ok = append_list(lines, (long long)"    EpList* list = malloc(sizeof(EpList));\n");
    ok = append_list(lines, (long long)"    if (!list) return 0;\n");
    ok = append_list(lines, (long long)"    list->capacity = 4;\n");
    ok = append_list(lines, (long long)"    list->length = 0;\n");
    ok = append_list(lines, (long long)"    list->data = malloc(list->capacity * sizeof(long long));\n");
    ok = append_list(lines, (long long)"    ep_gc_register(list, EP_OBJ_LIST);\n");
    ok = append_list(lines, (long long)"    return (long long)list;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long get_list_data_ptr(long long list_ptr) {\n");
    ok = append_list(lines, (long long)"    if (EP_BADPTR(list_ptr)) return 0;\n");
    ok = append_list(lines, (long long)"    EpList* list = (EpList*)list_ptr;\n");
    ok = append_list(lines, (long long)"    if (!list) return 0;\n");
    ok = append_list(lines, (long long)"    return (long long)list->data;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long append_list(long long list_ptr, long long value) {\n");
    ok = append_list(lines, (long long)"    if (EP_BADPTR(list_ptr)) return 0;\n");
    ok = append_list(lines, (long long)"    EpList* list = (EpList*)list_ptr;\n");
    ok = append_list(lines, (long long)"    if (!list) return 0;\n");
    ok = append_list(lines, (long long)"    if (list->length >= list->capacity) {\n");
    ok = append_list(lines, (long long)"        list->capacity *= 2;\n");
    ok = append_list(lines, (long long)"        list->data = realloc(list->data, list->capacity * sizeof(long long));\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    list->data[list->length] = value;\n");
    ok = append_list(lines, (long long)"    list->length += 1;\n");
    ok = append_list(lines, (long long)"    ep_gc_write_barrier((void*)list_ptr, value);\n");
    ok = append_list(lines, (long long)"    return value;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long get_list(long long list_ptr, long long index) {\n");
    ok = append_list(lines, (long long)"    if (EP_BADPTR(list_ptr)) return 0;\n");
    ok = append_list(lines, (long long)"    EpList* list = (EpList*)list_ptr;\n");
    ok = append_list(lines, (long long)"    if (index < 0 || index >= list->length) return 0;\n");
    ok = append_list(lines, (long long)"    return list->data[index];\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long set_list(long long list_ptr, long long index, long long value) {\n");
    ok = append_list(lines, (long long)"    if (EP_BADPTR(list_ptr)) return 0;\n");
    ok = append_list(lines, (long long)"    EpList* list = (EpList*)list_ptr;\n");
    ok = append_list(lines, (long long)"    if (index < 0 || index >= list->length) return 0;\n");
    ok = append_list(lines, (long long)"    list->data[index] = value;\n");
    ok = append_list(lines, (long long)"    ep_gc_write_barrier((void*)list_ptr, value);\n");
    ok = append_list(lines, (long long)"    return value;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long length_list(long long list_ptr) {\n");
    ok = append_list(lines, (long long)"    if (EP_BADPTR(list_ptr)) return 0;\n");
    ok = append_list(lines, (long long)"    EpList* list = (EpList*)list_ptr;\n");
    ok = append_list(lines, (long long)"    return list->length;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long free_list(long long list_ptr) {\n");
    ok = append_list(lines, (long long)"    EpList* list = (EpList*)list_ptr;\n");
    ok = append_list(lines, (long long)"    if (!list) return 0;\n");
    ok = append_list(lines, (long long)"    /* Skip if already freed (idempotent) */\n");
    ok = append_list(lines, (long long)"    if (!ep_gc_find(list)) return 0;\n");
    ok = append_list(lines, (long long)"    ep_gc_unregister(list);\n");
    ok = append_list(lines, (long long)"    free(list->data);\n");
    ok = append_list(lines, (long long)"    free(list);\n");
    ok = append_list(lines, (long long)"    return 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"static int sqlite_list_callback(void* arg, int argc, char** argv, char** col_names) {\n");
    ok = append_list(lines, (long long)"    EpList* rows = (EpList*)arg;\n");
    ok = append_list(lines, (long long)"    EpList* row = (EpList*)create_list();\n");
    ok = append_list(lines, (long long)"    for (int i = 0; i < argc; i++) {\n");
    ok = append_list(lines, (long long)"        char* val = argv[i] ? strdup(argv[i]) : strdup(\"\");\n");
    ok = append_list(lines, (long long)"        append_list((long long)row, (long long)val);\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    append_list((long long)rows, (long long)row);\n");
    ok = append_list(lines, (long long)"    return 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long sqlite_get_callback_ptr(long long dummy) {\n");
    ok = append_list(lines, (long long)"    return (long long)sqlite_list_callback;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* SQLite type-safe wrappers — marshal between int and long long */\n");
    ok = append_list(lines, (long long)"#ifdef EP_HAS_SQLITE\n");
    ok = append_list(lines, (long long)"typedef struct sqlite3 sqlite3;\n");
    ok = append_list(lines, (long long)"int sqlite3_open(const char*, sqlite3**);\n");
    ok = append_list(lines, (long long)"int sqlite3_close(sqlite3*);\n");
    ok = append_list(lines, (long long)"int sqlite3_exec(sqlite3*, const char*, int(*)(void*,int,char**,char**), void*, char**);\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_sqlite3_open(long long filename, long long db_ptr) {\n");
    ok = append_list(lines, (long long)"    sqlite3* db = NULL;\n");
    ok = append_list(lines, (long long)"    int rc = sqlite3_open((const char*)filename, &db);\n");
    ok = append_list(lines, (long long)"    if (rc == 0 && db_ptr != 0) {\n");
    ok = append_list(lines, (long long)"        *((long long*)db_ptr) = (long long)db;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    return (long long)rc;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_sqlite3_close(long long db) {\n");
    ok = append_list(lines, (long long)"    return (long long)sqlite3_close((sqlite3*)db);\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_sqlite3_exec(long long db, long long sql, long long callback, long long cb_arg, long long errmsg_ptr) {\n");
    ok = append_list(lines, (long long)"    return (long long)sqlite3_exec((sqlite3*)db, (const char*)sql,\n");
    ok = append_list(lines, (long long)"        (int(*)(void*,int,char**,char**))(callback),\n");
    ok = append_list(lines, (long long)"        (void*)cb_arg, (char**)errmsg_ptr);\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* Prepared-statement API for parameterized queries (defeats SQL injection). */\n");
    ok = append_list(lines, (long long)"typedef struct sqlite3_stmt sqlite3_stmt;\n");
    ok = append_list(lines, (long long)"int sqlite3_prepare_v2(sqlite3*, const char*, int, sqlite3_stmt**, const char**);\n");
    ok = append_list(lines, (long long)"int sqlite3_bind_text(sqlite3_stmt*, int, const char*, int, void(*)(void*));\n");
    ok = append_list(lines, (long long)"int sqlite3_bind_int64(sqlite3_stmt*, int, long long);\n");
    ok = append_list(lines, (long long)"int sqlite3_step(sqlite3_stmt*);\n");
    ok = append_list(lines, (long long)"int sqlite3_column_count(sqlite3_stmt*);\n");
    ok = append_list(lines, (long long)"const unsigned char* sqlite3_column_text(sqlite3_stmt*, int);\n");
    ok = append_list(lines, (long long)"long long sqlite3_column_int64(sqlite3_stmt*, int);\n");
    ok = append_list(lines, (long long)"int sqlite3_finalize(sqlite3_stmt*);\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_sqlite3_prepare_v2(long long db, long long sql) {\n");
    ok = append_list(lines, (long long)"    sqlite3_stmt* stmt = NULL;\n");
    ok = append_list(lines, (long long)"    int rc = sqlite3_prepare_v2((sqlite3*)db, (const char*)sql, -1, &stmt, NULL);\n");
    ok = append_list(lines, (long long)"    if (rc != 0) return 0;\n");
    ok = append_list(lines, (long long)"    return (long long)stmt;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_sqlite3_bind_text(long long stmt, long long idx, long long value) {\n");
    ok = append_list(lines, (long long)"    /* SQLITE_TRANSIENT ((void*)-1): sqlite copies the bound string. The value is\n");
    ok = append_list(lines, (long long)"       a bound parameter, never concatenated into SQL — this is the safe path. */\n");
    ok = append_list(lines, (long long)"    return (long long)sqlite3_bind_text((sqlite3_stmt*)stmt, (int)idx,\n");
    ok = append_list(lines, (long long)"        (const char*)value, -1, (void(*)(void*))(intptr_t)-1);\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_sqlite3_bind_int(long long stmt, long long idx, long long value) {\n");
    ok = append_list(lines, (long long)"    return (long long)sqlite3_bind_int64((sqlite3_stmt*)stmt, (int)idx, value);\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_sqlite3_step(long long stmt) {\n");
    ok = append_list(lines, (long long)"    return (long long)sqlite3_step((sqlite3_stmt*)stmt);\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_sqlite3_column_count(long long stmt) {\n");
    ok = append_list(lines, (long long)"    return (long long)sqlite3_column_count((sqlite3_stmt*)stmt);\n");
    ok = append_list(lines, (long long)"}\n");
    ret_val = join_strings(lines);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(1);
    return ret_val;
}

long long ep_rt_core_23() {
    long long lines = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&lines);
    ep_gc_maybe_collect();

    lines = create_list();
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_sqlite3_column_text(long long stmt, long long col) {\n");
    ok = append_list(lines, (long long)"    const unsigned char* t = sqlite3_column_text((sqlite3_stmt*)stmt, (int)col);\n");
    ok = append_list(lines, (long long)"    char* copy = (!t) ? strdup(\"\") : strdup((const char*)t);\n");
    ok = append_list(lines, (long long)"    /* Register the copy with the GC so it is reclaimed (not leaked) and so\n");
    ok = append_list(lines, (long long)"       ep_auto_to_string recognizes it as a string deterministically via\n");
    ok = append_list(lines, (long long)"       ep_gc_find, rather than relying on the memory-probe heuristic. */\n");
    ok = append_list(lines, (long long)"    if (copy) ep_gc_register(copy, EP_OBJ_STRING);\n");
    ok = append_list(lines, (long long)"    return (long long)copy;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_sqlite3_column_int(long long stmt, long long col) {\n");
    ok = append_list(lines, (long long)"    return sqlite3_column_int64((sqlite3_stmt*)stmt, (int)col);\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_sqlite3_finalize(long long stmt) {\n");
    ok = append_list(lines, (long long)"    return (long long)sqlite3_finalize((sqlite3_stmt*)stmt);\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"#endif /* EP_HAS_SQLITE */\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"int ep_argc = 0;\n");
    ok = append_list(lines, (long long)"char** ep_argv = NULL;\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"void init_ep_args(int argc, char** argv) {\n");
    ok = append_list(lines, (long long)"    ep_argc = argc;\n");
    ok = append_list(lines, (long long)"    ep_argv = argv;\n");
    ok = append_list(lines, (long long)"    ep_gc_register_thread((void*)&argc);\n");
    ok = append_list(lines, (long long)"    /* Wire up channel scanning for GC (defined after EpChannel struct) */\n");
    ok = append_list(lines, (long long)"    ep_gc_scan_channels_major = ep_gc_scan_channels_major_impl;\n");
    ok = append_list(lines, (long long)"    ep_gc_scan_channels_minor = ep_gc_scan_channels_minor_impl;\n");
    ok = append_list(lines, (long long)"    /* Wire up map value traversal for GC (defined after EpMap struct) */\n");
    ok = append_list(lines, (long long)"    ep_gc_mark_map_values = ep_gc_mark_map_values_impl;\n");
    ok = append_list(lines, (long long)"    ep_gc_mark_map_values_minor = ep_gc_mark_map_values_minor_impl;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long get_argument_count(void) {\n");
    ok = append_list(lines, (long long)"    return ep_argc;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"const char* get_argument(long long index) {\n");
    ok = append_list(lines, (long long)"    if (index < 0 || index >= ep_argc) {\n");
    ok = append_list(lines, (long long)"        char* empty = malloc(1);\n");
    ok = append_list(lines, (long long)"        if (empty) empty[0] = '\\0';\n");
    ok = append_list(lines, (long long)"        return empty;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    return ep_argv[index];\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long write_file_content(const char* filepath, const char* content) {\n");
    ok = append_list(lines, (long long)"    char mode[3];\n");
    ok = append_list(lines, (long long)"    mode[0] = 'w';\n");
    ok = append_list(lines, (long long)"    mode[1] = 'b';\n");
    ok = append_list(lines, (long long)"    mode[2] = '\\0';\n");
    ok = append_list(lines, (long long)"    FILE* f = fopen(filepath, mode);\n");
    ok = append_list(lines, (long long)"    if (!f) return 0;\n");
    ok = append_list(lines, (long long)"    size_t len = strlen(content);\n");
    ok = append_list(lines, (long long)"    size_t written = fwrite(content, 1, len, f);\n");
    ok = append_list(lines, (long long)"    fclose(f);\n");
    ok = append_list(lines, (long long)"    return written == len ? 1 : 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long run_command(const char* command) {\n");
    ok = append_list(lines, (long long)"    if (!command) return -1;\n");
    ok = append_list(lines, (long long)"    return system(command);\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"char* substring(const char* s, long long start, long long len) {\n");
    ok = append_list(lines, (long long)"    if (!s) {\n");
    ok = append_list(lines, (long long)"        char* empty = malloc(1);\n");
    ok = append_list(lines, (long long)"        if (empty) empty[0] = '\\0';\n");
    ok = append_list(lines, (long long)"        ep_gc_register(empty, EP_OBJ_STRING);\n");
    ok = append_list(lines, (long long)"        return empty;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    long long total_len = strlen(s);\n");
    ok = append_list(lines, (long long)"    if (start < 0 || start >= total_len || len <= 0) {\n");
    ok = append_list(lines, (long long)"        char* empty = malloc(1);\n");
    ok = append_list(lines, (long long)"        if (empty) empty[0] = '\\0';\n");
    ok = append_list(lines, (long long)"        ep_gc_register(empty, EP_OBJ_STRING);\n");
    ok = append_list(lines, (long long)"        return empty;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    if (start + len > total_len) {\n");
    ok = append_list(lines, (long long)"        len = total_len - start;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    char* sub = malloc(len + 1);\n");
    ok = append_list(lines, (long long)"    if (!sub) {\n");
    ok = append_list(lines, (long long)"        char* empty = malloc(1);\n");
    ok = append_list(lines, (long long)"        if (empty) empty[0] = '\\0';\n");
    ok = append_list(lines, (long long)"        ep_gc_register(empty, EP_OBJ_STRING);\n");
    ok = append_list(lines, (long long)"        return empty;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    strncpy(sub, s + start, len);\n");
    ok = append_list(lines, (long long)"    sub[len] = '\\0';\n");
    ok = append_list(lines, (long long)"    ep_gc_register(sub, EP_OBJ_STRING);\n");
    ok = append_list(lines, (long long)"    return sub;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"char* string_from_list(long long list_ptr) {\n");
    ok = append_list(lines, (long long)"    EpList* list = (EpList*)list_ptr;\n");
    ok = append_list(lines, (long long)"    if (!list) {\n");
    ok = append_list(lines, (long long)"        char* empty = malloc(1);\n");
    ok = append_list(lines, (long long)"        if (empty) empty[0] = '\\0';\n");
    ok = append_list(lines, (long long)"        ep_gc_register(empty, EP_OBJ_STRING);\n");
    ok = append_list(lines, (long long)"        return empty;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    char* s = malloc(list->length + 1);\n");
    ok = append_list(lines, (long long)"    if (!s) {\n");
    ok = append_list(lines, (long long)"        char* empty = malloc(1);\n");
    ok = append_list(lines, (long long)"        if (empty) empty[0] = '\\0';\n");
    ok = append_list(lines, (long long)"        ep_gc_register(empty, EP_OBJ_STRING);\n");
    ok = append_list(lines, (long long)"        return empty;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    for (long long i = 0; i < list->length; i++) {\n");
    ok = append_list(lines, (long long)"        s[i] = (char)list->data[i];\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    s[list->length] = '\\0';\n");
    ok = append_list(lines, (long long)"    ep_gc_register(s, EP_OBJ_STRING);\n");
    ok = append_list(lines, (long long)"    return s;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"// Inverse of string_from_list: convert a string to a list of its byte values in\n");
    ok = append_list(lines, (long long)"// a single O(n) pass (one strlen + one copy). This lets callers iterate a string\n");
    ok = append_list(lines, (long long)"// in O(n) total via O(1) get_list, instead of O(n) get_character per index\n");
    ok = append_list(lines, (long long)"// (which is O(n^2) over the whole string).\n");
    ok = append_list(lines, (long long)"long long string_to_list(const char* s) {\n");
    ok = append_list(lines, (long long)"    EpList* list = malloc(sizeof(EpList));\n");
    ok = append_list(lines, (long long)"    if (!list) return 0;\n");
    ok = append_list(lines, (long long)"    long long len = s ? (long long)strlen(s) : 0;\n");
    ok = append_list(lines, (long long)"    list->capacity = len > 0 ? len : 4;\n");
    ok = append_list(lines, (long long)"    list->length = len;\n");
    ok = append_list(lines, (long long)"    list->data = malloc(list->capacity * sizeof(long long));\n");
    ok = append_list(lines, (long long)"    if (!list->data) {\n");
    ok = append_list(lines, (long long)"        list->capacity = 0;\n");
    ok = append_list(lines, (long long)"        list->length = 0;\n");
    ok = append_list(lines, (long long)"        ep_gc_register(list, EP_OBJ_LIST);\n");
    ok = append_list(lines, (long long)"        return (long long)list;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    for (long long i = 0; i < len; i++) {\n");
    ok = append_list(lines, (long long)"        list->data[i] = (unsigned char)s[i];\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    ep_gc_register(list, EP_OBJ_LIST);\n");
    ok = append_list(lines, (long long)"    return (long long)list;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long pop_list(long long list_ptr) {\n");
    ok = append_list(lines, (long long)"    if (EP_BADPTR(list_ptr)) return 0;\n");
    ok = append_list(lines, (long long)"    EpList* list = (EpList*)list_ptr;\n");
    ok = append_list(lines, (long long)"    if (!list || list->length <= 0) return 0;\n");
    ok = append_list(lines, (long long)"    list->length -= 1;\n");
    ok = append_list(lines, (long long)"    return list->data[list->length];\n");
    ok = append_list(lines, (long long)"}\n");
    ret_val = join_strings(lines);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(1);
    return ret_val;
}

long long ep_rt_core_24() {
    long long lines = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&lines);
    ep_gc_maybe_collect();

    lines = create_list();
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long remove_list(long long list_ptr, long long index) {\n");
    ok = append_list(lines, (long long)"    if (EP_BADPTR(list_ptr)) return 0;\n");
    ok = append_list(lines, (long long)"    EpList* list = (EpList*)list_ptr;\n");
    ok = append_list(lines, (long long)"    if (!list || index < 0 || index >= list->length) return 0;\n");
    ok = append_list(lines, (long long)"    long long removed = list->data[index];\n");
    ok = append_list(lines, (long long)"    for (long long i = index; i < list->length - 1; i++) {\n");
    ok = append_list(lines, (long long)"        list->data[i] = list->data[i + 1];\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    list->length -= 1;\n");
    ok = append_list(lines, (long long)"    return removed;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long display_string(const char* s) {\n");
    ok = append_list(lines, (long long)"    if (s) puts(s);\n");
    ok = append_list(lines, (long long)"    return 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* Write text with NO trailing newline, and flush at once — for drawing to\n");
    ok = append_list(lines, (long long)"   the screen where every byte's position matters (cursor moves, escape\n");
    ok = append_list(lines, (long long)"   codes, a full-screen frame). puts()/display_string would append a\n");
    ok = append_list(lines, (long long)"   newline and scroll a full-height frame; this does not. */\n");
    ok = append_list(lines, (long long)"long long screen_write(const char* s) {\n");
    ok = append_list(lines, (long long)"    if (s) {\n");
    ok = append_list(lines, (long long)"        fputs(s, stdout);\n");
    ok = append_list(lines, (long long)"        fflush(stdout);\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    return 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* ========== File System Runtime ========== */\n");
    ok = append_list(lines, (long long)"#include <sys/stat.h>\n");
    ok = append_list(lines, (long long)"#ifdef _WIN32\n");
    ok = append_list(lines, (long long)"  #include <io.h>\n");
    ok = append_list(lines, (long long)"  #include <direct.h>\n");
    ok = append_list(lines, (long long)"  #define mkdir(p, m) _mkdir(p)\n");
    ok = append_list(lines, (long long)"  #define rmdir _rmdir\n");
    ok = append_list(lines, (long long)"  #define getcwd _getcwd\n");
    ok = append_list(lines, (long long)"  #define popen _popen\n");
    ok = append_list(lines, (long long)"  #define pclose _pclose\n");
    ok = append_list(lines, (long long)"  #define getpid _getpid\n");
    ok = append_list(lines, (long long)"  #define setenv(k, v, o) _putenv_s(k, v)\n");
    ok = append_list(lines, (long long)"  /* Minimal dirent polyfill for Windows */\n");
    ok = append_list(lines, (long long)"  #include <windows.h>\n");
    ok = append_list(lines, (long long)"  typedef struct { char d_name[260]; } ep_dirent;\n");
    ok = append_list(lines, (long long)"  typedef struct { HANDLE hFind; WIN32_FIND_DATAA data; int first; } EP_DIR;\n");
    ok = append_list(lines, (long long)"  static EP_DIR* ep_opendir(const char* p) {\n");
    ok = append_list(lines, (long long)"      EP_DIR* d = (EP_DIR*)malloc(sizeof(EP_DIR));\n");
    ok = append_list(lines, (long long)"      char buf[270]; snprintf(buf, sizeof(buf), \"%s\\\\*\", p);\n");
    ok = append_list(lines, (long long)"      d->hFind = FindFirstFileA(buf, &d->data);\n");
    ok = append_list(lines, (long long)"      d->first = 1;\n");
    ok = append_list(lines, (long long)"      return (d->hFind == INVALID_HANDLE_VALUE) ? (free(d), (EP_DIR*)NULL) : d;\n");
    ok = append_list(lines, (long long)"  }\n");
    ok = append_list(lines, (long long)"  static ep_dirent* ep_readdir(EP_DIR* d) {\n");
    ok = append_list(lines, (long long)"      static ep_dirent ent;\n");
    ok = append_list(lines, (long long)"      if (d->first) { d->first = 0; strcpy(ent.d_name, d->data.cFileName); return &ent; }\n");
    ok = append_list(lines, (long long)"      if (!FindNextFileA(d->hFind, &d->data)) return NULL;\n");
    ok = append_list(lines, (long long)"      strcpy(ent.d_name, d->data.cFileName); return &ent;\n");
    ok = append_list(lines, (long long)"  }\n");
    ok = append_list(lines, (long long)"  static void ep_closedir(EP_DIR* d) { FindClose(d->hFind); free(d); }\n");
    ok = append_list(lines, (long long)"  #define DIR EP_DIR\n");
    ok = append_list(lines, (long long)"  #define dirent ep_dirent\n");
    ok = append_list(lines, (long long)"  #define opendir ep_opendir\n");
    ok = append_list(lines, (long long)"  #define readdir ep_readdir\n");
    ok = append_list(lines, (long long)"  #define closedir ep_closedir\n");
    ok = append_list(lines, (long long)"#else\n");
    ok = append_list(lines, (long long)"  #include <dirent.h>\n");
    ok = append_list(lines, (long long)"  #include <unistd.h>\n");
    ok = append_list(lines, (long long)"#endif\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_read_file(long long path_ptr) {\n");
    ok = append_list(lines, (long long)"    const char* path = (const char*)path_ptr;\n");
    ok = append_list(lines, (long long)"    FILE* f = fopen(path, \"rb\");\n");
    ok = append_list(lines, (long long)"    if (!f) return (long long)\"\";\n");
    ok = append_list(lines, (long long)"    fseek(f, 0, SEEK_END);\n");
    ok = append_list(lines, (long long)"    long size = ftell(f);\n");
    ok = append_list(lines, (long long)"    fseek(f, 0, SEEK_SET);\n");
    ok = append_list(lines, (long long)"    char* buf = (char*)malloc(size + 1);\n");
    ok = append_list(lines, (long long)"    fread(buf, 1, size, f);\n");
    ok = append_list(lines, (long long)"    buf[size] = '\\0';\n");
    ok = append_list(lines, (long long)"    fclose(f);\n");
    ok = append_list(lines, (long long)"    return (long long)buf;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_write_file(long long path_ptr, long long content_ptr) {\n");
    ok = append_list(lines, (long long)"    const char* path = (const char*)path_ptr;\n");
    ok = append_list(lines, (long long)"    const char* content = (const char*)content_ptr;\n");
    ok = append_list(lines, (long long)"    FILE* f = fopen(path, \"wb\");\n");
    ok = append_list(lines, (long long)"    if (!f) return 0;\n");
    ok = append_list(lines, (long long)"    fputs(content, f);\n");
    ok = append_list(lines, (long long)"    fclose(f);\n");
    ok = append_list(lines, (long long)"    return 1;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_append_file(long long path_ptr, long long content_ptr) {\n");
    ok = append_list(lines, (long long)"    const char* path = (const char*)path_ptr;\n");
    ok = append_list(lines, (long long)"    const char* content = (const char*)content_ptr;\n");
    ok = append_list(lines, (long long)"    FILE* f = fopen(path, \"ab\");\n");
    ok = append_list(lines, (long long)"    if (!f) return 0;\n");
    ok = append_list(lines, (long long)"    fputs(content, f);\n");
    ok = append_list(lines, (long long)"    fclose(f);\n");
    ok = append_list(lines, (long long)"    return 1;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_file_exists(long long path_ptr) {\n");
    ok = append_list(lines, (long long)"    const char* path = (const char*)path_ptr;\n");
    ok = append_list(lines, (long long)"    struct stat st;\n");
    ok = append_list(lines, (long long)"    return stat(path, &st) == 0 ? 1 : 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_is_directory(long long path_ptr) {\n");
    ok = append_list(lines, (long long)"    const char* path = (const char*)path_ptr;\n");
    ok = append_list(lines, (long long)"    struct stat st;\n");
    ok = append_list(lines, (long long)"    if (stat(path, &st) != 0) return 0;\n");
    ok = append_list(lines, (long long)"    return S_ISDIR(st.st_mode) ? 1 : 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_file_size(long long path_ptr) {\n");
    ok = append_list(lines, (long long)"    const char* path = (const char*)path_ptr;\n");
    ok = append_list(lines, (long long)"    struct stat st;\n");
    ok = append_list(lines, (long long)"    if (stat(path, &st) != 0) return -1;\n");
    ok = append_list(lines, (long long)"    return (long long)st.st_size;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_list_directory(long long path_ptr) {\n");
    ok = append_list(lines, (long long)"    const char* path = (const char*)path_ptr;\n");
    ok = append_list(lines, (long long)"    DIR* dir = opendir(path);\n");
    ok = append_list(lines, (long long)"    if (!dir) return (long long)create_list();\n");
    ok = append_list(lines, (long long)"    long long list = create_list();\n");
    ok = append_list(lines, (long long)"    struct dirent* entry;\n");
    ok = append_list(lines, (long long)"    while ((entry = readdir(dir)) != NULL) {\n");
    ok = append_list(lines, (long long)"        if (entry->d_name[0] == '.' && (entry->d_name[1] == '\\0' || \n");
    ok = append_list(lines, (long long)"            (entry->d_name[1] == '.' && entry->d_name[2] == '\\0'))) continue;\n");
    ok = append_list(lines, (long long)"        char* name = strdup(entry->d_name);\n");
    ok = append_list(lines, (long long)"        append_list(list, (long long)name);\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    closedir(dir);\n");
    ok = append_list(lines, (long long)"    return list;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_create_directory(long long path_ptr) {\n");
    ok = append_list(lines, (long long)"    const char* path = (const char*)path_ptr;\n");
    ok = append_list(lines, (long long)"    return mkdir(path, 0755) == 0 ? 1 : 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_remove_file(long long path_ptr) {\n");
    ok = append_list(lines, (long long)"    const char* path = (const char*)path_ptr;\n");
    ok = append_list(lines, (long long)"    return remove(path) == 0 ? 1 : 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ret_val = join_strings(lines);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(1);
    return ret_val;
}

long long ep_rt_core_25() {
    long long lines = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&lines);
    ep_gc_maybe_collect();

    lines = create_list();
    ok = append_list(lines, (long long)"long long ep_remove_directory(long long path_ptr) {\n");
    ok = append_list(lines, (long long)"    const char* path = (const char*)path_ptr;\n");
    ok = append_list(lines, (long long)"    return rmdir(path) == 0 ? 1 : 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_rename_file(long long old_ptr, long long new_ptr) {\n");
    ok = append_list(lines, (long long)"    return rename((const char*)old_ptr, (const char*)new_ptr) == 0 ? 1 : 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_copy_file(long long src_ptr, long long dst_ptr) {\n");
    ok = append_list(lines, (long long)"    const char* src = (const char*)src_ptr;\n");
    ok = append_list(lines, (long long)"    const char* dst = (const char*)dst_ptr;\n");
    ok = append_list(lines, (long long)"    FILE* fin = fopen(src, \"rb\");\n");
    ok = append_list(lines, (long long)"    if (!fin) return 0;\n");
    ok = append_list(lines, (long long)"    FILE* fout = fopen(dst, \"wb\");\n");
    ok = append_list(lines, (long long)"    if (!fout) { fclose(fin); return 0; }\n");
    ok = append_list(lines, (long long)"    char buf[8192];\n");
    ok = append_list(lines, (long long)"    size_t n;\n");
    ok = append_list(lines, (long long)"    while ((n = fread(buf, 1, sizeof(buf), fin)) > 0) {\n");
    ok = append_list(lines, (long long)"        fwrite(buf, 1, n, fout);\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    fclose(fin);\n");
    ok = append_list(lines, (long long)"    fclose(fout);\n");
    ok = append_list(lines, (long long)"    return 1;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* ========== Date/Time Runtime ========== */\n");
    ok = append_list(lines, (long long)"#include <time.h>\n");
    ok = append_list(lines, (long long)"#include <sys/time.h>\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_time_now_ms(void) {\n");
    ok = append_list(lines, (long long)"    struct timeval tv;\n");
    ok = append_list(lines, (long long)"    gettimeofday(&tv, NULL);\n");
    ok = append_list(lines, (long long)"    return (long long)tv.tv_sec * 1000LL + (long long)tv.tv_usec / 1000LL;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_time_now_sec(void) {\n");
    ok = append_list(lines, (long long)"    return (long long)time(NULL);\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_time_year(long long ts) {\n");
    ok = append_list(lines, (long long)"    time_t t = (time_t)ts;\n");
    ok = append_list(lines, (long long)"    struct tm* tm = localtime(&t);\n");
    ok = append_list(lines, (long long)"    return tm ? tm->tm_year + 1900 : 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_time_month(long long ts) {\n");
    ok = append_list(lines, (long long)"    time_t t = (time_t)ts;\n");
    ok = append_list(lines, (long long)"    struct tm* tm = localtime(&t);\n");
    ok = append_list(lines, (long long)"    return tm ? tm->tm_mon + 1 : 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_time_day(long long ts) {\n");
    ok = append_list(lines, (long long)"    time_t t = (time_t)ts;\n");
    ok = append_list(lines, (long long)"    struct tm* tm = localtime(&t);\n");
    ok = append_list(lines, (long long)"    return tm ? tm->tm_mday : 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_time_hour(long long ts) {\n");
    ok = append_list(lines, (long long)"    time_t t = (time_t)ts;\n");
    ok = append_list(lines, (long long)"    struct tm* tm = localtime(&t);\n");
    ok = append_list(lines, (long long)"    return tm ? tm->tm_hour : 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_time_minute(long long ts) {\n");
    ok = append_list(lines, (long long)"    time_t t = (time_t)ts;\n");
    ok = append_list(lines, (long long)"    struct tm* tm = localtime(&t);\n");
    ok = append_list(lines, (long long)"    return tm ? tm->tm_min : 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_time_second(long long ts) {\n");
    ok = append_list(lines, (long long)"    time_t t = (time_t)ts;\n");
    ok = append_list(lines, (long long)"    struct tm* tm = localtime(&t);\n");
    ok = append_list(lines, (long long)"    return tm ? tm->tm_sec : 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_time_weekday(long long ts) {\n");
    ok = append_list(lines, (long long)"    time_t t = (time_t)ts;\n");
    ok = append_list(lines, (long long)"    struct tm* tm = localtime(&t);\n");
    ok = append_list(lines, (long long)"    return tm ? tm->tm_wday : 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_format_time(long long ts, long long fmt_ptr) {\n");
    ok = append_list(lines, (long long)"    time_t t = (time_t)ts;\n");
    ok = append_list(lines, (long long)"    struct tm* tm = localtime(&t);\n");
    ok = append_list(lines, (long long)"    if (!tm) return (long long)\"\";\n");
    ok = append_list(lines, (long long)"    char* buf = (char*)malloc(256);\n");
    ok = append_list(lines, (long long)"    strftime(buf, 256, (const char*)fmt_ptr, tm);\n");
    ok = append_list(lines, (long long)"    return (long long)buf;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* ========== OS Runtime ========== */\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_getenv(long long name_ptr) {\n");
    ok = append_list(lines, (long long)"    const char* val = getenv((const char*)name_ptr);\n");
    ok = append_list(lines, (long long)"    return val ? (long long)val : (long long)\"\";\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_setenv(long long name_ptr, long long val_ptr) {\n");
    ok = append_list(lines, (long long)"    return setenv((const char*)name_ptr, (const char*)val_ptr, 1) == 0 ? 1 : 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_get_cwd(void) {\n");
    ok = append_list(lines, (long long)"    char* buf = (char*)malloc(4096);\n");
    ok = append_list(lines, (long long)"    if (getcwd(buf, 4096)) return (long long)buf;\n");
    ok = append_list(lines, (long long)"    free(buf);\n");
    ok = append_list(lines, (long long)"    return (long long)\"\";\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_os_name(void) {\n");
    ok = append_list(lines, (long long)"    #if defined(__APPLE__)\n");
    ok = append_list(lines, (long long)"    return (long long)\"macos\";\n");
    ok = append_list(lines, (long long)"    #elif defined(__linux__)\n");
    ok = append_list(lines, (long long)"    return (long long)\"linux\";\n");
    ok = append_list(lines, (long long)"    #elif defined(_WIN32)\n");
    ok = append_list(lines, (long long)"    return (long long)\"windows\";\n");
    ok = append_list(lines, (long long)"    #else\n");
    ok = append_list(lines, (long long)"    return (long long)\"unknown\";\n");
    ok = append_list(lines, (long long)"    #endif\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_arch_name(void) {\n");
    ok = append_list(lines, (long long)"    #if defined(__aarch64__) || defined(__arm64__)\n");
    ok = append_list(lines, (long long)"    return (long long)\"arm64\";\n");
    ok = append_list(lines, (long long)"    #elif defined(__x86_64__)\n");
    ok = append_list(lines, (long long)"    return (long long)\"x86_64\";\n");
    ok = append_list(lines, (long long)"    #elif defined(__i386__)\n");
    ok = append_list(lines, (long long)"    return (long long)\"x86\";\n");
    ok = append_list(lines, (long long)"    #else\n");
    ok = append_list(lines, (long long)"    return (long long)\"unknown\";\n");
    ok = append_list(lines, (long long)"    #endif\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_exit(long long code) {\n");
    ok = append_list(lines, (long long)"    exit((int)code);\n");
    ok = append_list(lines, (long long)"    return 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_get_pid(void) {\n");
    ok = append_list(lines, (long long)"    return (long long)getpid();\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_get_home_dir(void) {\n");
    ok = append_list(lines, (long long)"    const char* home = getenv(\"HOME\");\n");
    ok = append_list(lines, (long long)"    return home ? (long long)home : (long long)\"\";\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"#ifdef __wasm__\n");
    ok = append_list(lines, (long long)"long long ep_run_command(long long cmd_ptr) {\n");
    ret_val = join_strings(lines);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(1);
    return ret_val;
}

long long ep_rt_core_26() {
    long long lines = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&lines);
    ep_gc_maybe_collect();

    lines = create_list();
    ok = append_list(lines, (long long)"    (void)cmd_ptr;\n");
    ok = append_list(lines, (long long)"    return (long long)\"Error: running external commands is not supported on WebAssembly\";\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"#else\n");
    ok = append_list(lines, (long long)"long long ep_run_command(long long cmd_ptr) {\n");
    ok = append_list(lines, (long long)"    const char* cmd = (const char*)cmd_ptr;\n");
    ok = append_list(lines, (long long)"    FILE* fp = popen(cmd, \"r\");\n");
    ok = append_list(lines, (long long)"    if (!fp) return (long long)\"\";\n");
    ok = append_list(lines, (long long)"    char* result = (char*)malloc(65536);\n");
    ok = append_list(lines, (long long)"    size_t total = 0;\n");
    ok = append_list(lines, (long long)"    char buf[4096];\n");
    ok = append_list(lines, (long long)"    while (fgets(buf, sizeof(buf), fp)) {\n");
    ok = append_list(lines, (long long)"        size_t len = strlen(buf);\n");
    ok = append_list(lines, (long long)"        memcpy(result + total, buf, len);\n");
    ok = append_list(lines, (long long)"        total += len;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    result[total] = '\\0';\n");
    ok = append_list(lines, (long long)"    pclose(fp);\n");
    ok = append_list(lines, (long long)"    return (long long)result;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"#endif\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* ========== HashMap helpers ========== */\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_hash_string(long long s_ptr) {\n");
    ok = append_list(lines, (long long)"    const char* s = (const char*)s_ptr;\n");
    ok = append_list(lines, (long long)"    if (!s) return 0;\n");
    ok = append_list(lines, (long long)"    unsigned long long hash = 5381;\n");
    ok = append_list(lines, (long long)"    int c;\n");
    ok = append_list(lines, (long long)"    while ((c = *s++)) {\n");
    ok = append_list(lines, (long long)"        hash = ((hash << 5) + hash) + c;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    return (long long)hash;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_str_equals(long long a_ptr, long long b_ptr) {\n");
    ok = append_list(lines, (long long)"    if (a_ptr == b_ptr) return 1;\n");
    ok = append_list(lines, (long long)"    if (!a_ptr || !b_ptr) return 0;\n");
    ok = append_list(lines, (long long)"    /* If either value looks like a small integer (not a valid heap pointer),\n");
    ok = append_list(lines, (long long)"       fall back to integer comparison — strcmp would segfault. */\n");
    ok = append_list(lines, (long long)"    if ((unsigned long long)a_ptr < 4096ULL || (unsigned long long)b_ptr < 4096ULL) return 0;\n");
    ok = append_list(lines, (long long)"    return strcmp((const char*)a_ptr, (const char*)b_ptr) == 0 ? 1 : 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* ========== Sync Primitives ========== */\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"#ifdef _WIN32\n");
    ok = append_list(lines, (long long)"long long ep_mutex_create(void) {\n");
    ok = append_list(lines, (long long)"    CRITICAL_SECTION* m = (CRITICAL_SECTION*)malloc(sizeof(CRITICAL_SECTION));\n");
    ok = append_list(lines, (long long)"    InitializeCriticalSection(m);\n");
    ok = append_list(lines, (long long)"    return (long long)m;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"long long ep_mutex_lock_fn(long long m) {\n");
    ok = append_list(lines, (long long)"    EnterCriticalSection((CRITICAL_SECTION*)m);\n");
    ok = append_list(lines, (long long)"    return 1;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"long long ep_mutex_unlock_fn(long long m) {\n");
    ok = append_list(lines, (long long)"    LeaveCriticalSection((CRITICAL_SECTION*)m);\n");
    ok = append_list(lines, (long long)"    return 1;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"long long ep_mutex_trylock(long long m) {\n");
    ok = append_list(lines, (long long)"    return TryEnterCriticalSection((CRITICAL_SECTION*)m) ? 1 : 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"long long ep_mutex_destroy(long long m) {\n");
    ok = append_list(lines, (long long)"    DeleteCriticalSection((CRITICAL_SECTION*)m);\n");
    ok = append_list(lines, (long long)"    free((void*)m);\n");
    ok = append_list(lines, (long long)"    return 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"#else\n");
    ok = append_list(lines, (long long)"long long ep_mutex_create(void) {\n");
    ok = append_list(lines, (long long)"    pthread_mutex_t* m = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));\n");
    ok = append_list(lines, (long long)"    pthread_mutex_init(m, NULL);\n");
    ok = append_list(lines, (long long)"    return (long long)m;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_mutex_lock_fn(long long m) {\n");
    ok = append_list(lines, (long long)"    return pthread_mutex_lock((pthread_mutex_t*)m) == 0 ? 1 : 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_mutex_unlock_fn(long long m) {\n");
    ok = append_list(lines, (long long)"    return pthread_mutex_unlock((pthread_mutex_t*)m) == 0 ? 1 : 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_mutex_trylock(long long m) {\n");
    ok = append_list(lines, (long long)"    return pthread_mutex_trylock((pthread_mutex_t*)m) == 0 ? 1 : 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_mutex_destroy(long long m) {\n");
    ok = append_list(lines, (long long)"    pthread_mutex_destroy((pthread_mutex_t*)m);\n");
    ok = append_list(lines, (long long)"    free((void*)m);\n");
    ok = append_list(lines, (long long)"    return 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"#endif\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"#ifdef _WIN32\n");
    ok = append_list(lines, (long long)"long long ep_rwlock_create(void) {\n");
    ok = append_list(lines, (long long)"    SRWLOCK* rwl = (SRWLOCK*)malloc(sizeof(SRWLOCK));\n");
    ok = append_list(lines, (long long)"    InitializeSRWLock(rwl);\n");
    ok = append_list(lines, (long long)"    return (long long)rwl;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"long long ep_rwlock_read_lock(long long rwl) {\n");
    ok = append_list(lines, (long long)"    AcquireSRWLockShared((SRWLOCK*)rwl);\n");
    ok = append_list(lines, (long long)"    return 1;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"long long ep_rwlock_write_lock(long long rwl) {\n");
    ok = append_list(lines, (long long)"    AcquireSRWLockExclusive((SRWLOCK*)rwl);\n");
    ok = append_list(lines, (long long)"    return 1;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"long long ep_rwlock_unlock(long long rwl) {\n");
    ok = append_list(lines, (long long)"    /* SRWLOCK does not have a single \"unlock\" — we try exclusive first.\n");
    ok = append_list(lines, (long long)"       In practice the caller should know which lock was taken.\n");
    ok = append_list(lines, (long long)"       ReleaseSRWLockExclusive on a shared lock is undefined, but\n");
    ok = append_list(lines, (long long)"       the runtime guarantees matched lock/unlock pairs. We default\n");
    ok = append_list(lines, (long long)"       to releasing the exclusive lock; shared unlock is handled\n");
    ok = append_list(lines, (long long)"       by pairing read_lock -> read_unlock if needed later. */\n");
    ok = append_list(lines, (long long)"    ReleaseSRWLockExclusive((SRWLOCK*)rwl);\n");
    ok = append_list(lines, (long long)"    return 1;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"long long ep_rwlock_destroy(long long rwl) {\n");
    ok = append_list(lines, (long long)"    /* SRWLOCK has no destroy */\n");
    ok = append_list(lines, (long long)"    free((void*)rwl);\n");
    ok = append_list(lines, (long long)"    return 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"#else\n");
    ok = append_list(lines, (long long)"long long ep_rwlock_create(void) {\n");
    ok = append_list(lines, (long long)"    pthread_rwlock_t* rwl = (pthread_rwlock_t*)malloc(sizeof(pthread_rwlock_t));\n");
    ok = append_list(lines, (long long)"    pthread_rwlock_init(rwl, NULL);\n");
    ok = append_list(lines, (long long)"    return (long long)rwl;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_rwlock_read_lock(long long rwl) {\n");
    ok = append_list(lines, (long long)"    return pthread_rwlock_rdlock((pthread_rwlock_t*)rwl) == 0 ? 1 : 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_rwlock_write_lock(long long rwl) {\n");
    ok = append_list(lines, (long long)"    return pthread_rwlock_wrlock((pthread_rwlock_t*)rwl) == 0 ? 1 : 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_rwlock_unlock(long long rwl) {\n");
    ok = append_list(lines, (long long)"    return pthread_rwlock_unlock((pthread_rwlock_t*)rwl) == 0 ? 1 : 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_rwlock_destroy(long long rwl) {\n");
    ok = append_list(lines, (long long)"    pthread_rwlock_destroy((pthread_rwlock_t*)rwl);\n");
    ok = append_list(lines, (long long)"    free((void*)rwl);\n");
    ok = append_list(lines, (long long)"    return 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"#endif\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"#ifdef _MSC_VER\n");
    ret_val = join_strings(lines);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(1);
    return ret_val;
}

long long ep_rt_core_27() {
    long long lines = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&lines);
    ep_gc_maybe_collect();

    lines = create_list();
    ok = append_list(lines, (long long)"long long ep_atomic_create(long long initial) {\n");
    ok = append_list(lines, (long long)"    volatile long long* a = (volatile long long*)malloc(sizeof(long long));\n");
    ok = append_list(lines, (long long)"    InterlockedExchange64(a, initial);\n");
    ok = append_list(lines, (long long)"    return (long long)a;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"long long ep_atomic_load(long long a) {\n");
    ok = append_list(lines, (long long)"    return InterlockedCompareExchange64((volatile long long*)a, 0, 0);\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"long long ep_atomic_store(long long a, long long value) {\n");
    ok = append_list(lines, (long long)"    InterlockedExchange64((volatile long long*)a, value);\n");
    ok = append_list(lines, (long long)"    return value;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"long long ep_atomic_add(long long a, long long delta) {\n");
    ok = append_list(lines, (long long)"    return InterlockedExchangeAdd64((volatile long long*)a, delta);\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"long long ep_atomic_sub(long long a, long long delta) {\n");
    ok = append_list(lines, (long long)"    return InterlockedExchangeAdd64((volatile long long*)a, -delta);\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"long long ep_atomic_cas(long long a, long long expected, long long desired) {\n");
    ok = append_list(lines, (long long)"    long long old = InterlockedCompareExchange64((volatile long long*)a, desired, expected);\n");
    ok = append_list(lines, (long long)"    return (old == expected) ? 1 : 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"#else\n");
    ok = append_list(lines, (long long)"long long ep_atomic_create(long long initial) {\n");
    ok = append_list(lines, (long long)"    long long* a = (long long*)malloc(sizeof(long long));\n");
    ok = append_list(lines, (long long)"    __atomic_store_n(a, initial, __ATOMIC_SEQ_CST);\n");
    ok = append_list(lines, (long long)"    return (long long)a;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_atomic_load(long long a) {\n");
    ok = append_list(lines, (long long)"    return __atomic_load_n((long long*)a, __ATOMIC_SEQ_CST);\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_atomic_store(long long a, long long value) {\n");
    ok = append_list(lines, (long long)"    __atomic_store_n((long long*)a, value, __ATOMIC_SEQ_CST);\n");
    ok = append_list(lines, (long long)"    return value;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_atomic_add(long long a, long long delta) {\n");
    ok = append_list(lines, (long long)"    return __atomic_fetch_add((long long*)a, delta, __ATOMIC_SEQ_CST);\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_atomic_sub(long long a, long long delta) {\n");
    ok = append_list(lines, (long long)"    return __atomic_fetch_sub((long long*)a, delta, __ATOMIC_SEQ_CST);\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_atomic_cas(long long a, long long expected, long long desired) {\n");
    ok = append_list(lines, (long long)"    long long exp = expected;\n");
    ok = append_list(lines, (long long)"    return __atomic_compare_exchange_n((long long*)a, &exp, desired, 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST) ? 1 : 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"#endif\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* Barrier — portable polyfill (macOS lacks pthread_barrier_t) */\n");
    ok = append_list(lines, (long long)"typedef struct {\n");
    ok = append_list(lines, (long long)"    pthread_mutex_t mutex;\n");
    ok = append_list(lines, (long long)"    pthread_cond_t cond;\n");
    ok = append_list(lines, (long long)"    unsigned count;\n");
    ok = append_list(lines, (long long)"    unsigned target;\n");
    ok = append_list(lines, (long long)"    unsigned generation;\n");
    ok = append_list(lines, (long long)"} EpBarrier;\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_barrier_create(long long count) {\n");
    ok = append_list(lines, (long long)"    EpBarrier* b = (EpBarrier*)malloc(sizeof(EpBarrier));\n");
    ok = append_list(lines, (long long)"    pthread_mutex_init(&b->mutex, NULL);\n");
    ok = append_list(lines, (long long)"    pthread_cond_init(&b->cond, NULL);\n");
    ok = append_list(lines, (long long)"    b->count = 0;\n");
    ok = append_list(lines, (long long)"    b->target = (unsigned)count;\n");
    ok = append_list(lines, (long long)"    b->generation = 0;\n");
    ok = append_list(lines, (long long)"    return (long long)b;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_barrier_wait(long long bp) {\n");
    ok = append_list(lines, (long long)"    EpBarrier* b = (EpBarrier*)bp;\n");
    ok = append_list(lines, (long long)"    pthread_mutex_lock(&b->mutex);\n");
    ok = append_list(lines, (long long)"    unsigned gen = b->generation;\n");
    ok = append_list(lines, (long long)"    b->count++;\n");
    ok = append_list(lines, (long long)"    if (b->count >= b->target) {\n");
    ok = append_list(lines, (long long)"        b->count = 0;\n");
    ok = append_list(lines, (long long)"        b->generation++;\n");
    ok = append_list(lines, (long long)"        pthread_cond_broadcast(&b->cond);\n");
    ok = append_list(lines, (long long)"        pthread_mutex_unlock(&b->mutex);\n");
    ok = append_list(lines, (long long)"        return 1; /* serial thread */\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    while (gen == b->generation) {\n");
    ok = append_list(lines, (long long)"        pthread_cond_wait(&b->cond, &b->mutex);\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    pthread_mutex_unlock(&b->mutex);\n");
    ok = append_list(lines, (long long)"    return 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_barrier_destroy(long long bp) {\n");
    ok = append_list(lines, (long long)"    EpBarrier* b = (EpBarrier*)bp;\n");
    ok = append_list(lines, (long long)"    pthread_mutex_destroy(&b->mutex);\n");
    ok = append_list(lines, (long long)"    pthread_cond_destroy(&b->cond);\n");
    ok = append_list(lines, (long long)"    free(b);\n");
    ok = append_list(lines, (long long)"    return 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* Semaphore via mutex+condvar (portable) */\n");
    ok = append_list(lines, (long long)"typedef struct {\n");
    ok = append_list(lines, (long long)"    pthread_mutex_t mutex;\n");
    ok = append_list(lines, (long long)"    pthread_cond_t cond;\n");
    ok = append_list(lines, (long long)"    long long value;\n");
    ok = append_list(lines, (long long)"} EpSemaphore;\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_semaphore_create(long long initial) {\n");
    ok = append_list(lines, (long long)"    EpSemaphore* s = (EpSemaphore*)malloc(sizeof(EpSemaphore));\n");
    ok = append_list(lines, (long long)"    pthread_mutex_init(&s->mutex, NULL);\n");
    ok = append_list(lines, (long long)"    pthread_cond_init(&s->cond, NULL);\n");
    ok = append_list(lines, (long long)"    s->value = initial;\n");
    ok = append_list(lines, (long long)"    return (long long)s;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_semaphore_wait(long long sp) {\n");
    ok = append_list(lines, (long long)"    EpSemaphore* s = (EpSemaphore*)sp;\n");
    ok = append_list(lines, (long long)"    pthread_mutex_lock(&s->mutex);\n");
    ok = append_list(lines, (long long)"    while (s->value <= 0) {\n");
    ok = append_list(lines, (long long)"        pthread_cond_wait(&s->cond, &s->mutex);\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    s->value--;\n");
    ok = append_list(lines, (long long)"    pthread_mutex_unlock(&s->mutex);\n");
    ok = append_list(lines, (long long)"    return 1;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_semaphore_post(long long sp) {\n");
    ok = append_list(lines, (long long)"    EpSemaphore* s = (EpSemaphore*)sp;\n");
    ok = append_list(lines, (long long)"    pthread_mutex_lock(&s->mutex);\n");
    ok = append_list(lines, (long long)"    s->value++;\n");
    ok = append_list(lines, (long long)"    pthread_cond_signal(&s->cond);\n");
    ok = append_list(lines, (long long)"    pthread_mutex_unlock(&s->mutex);\n");
    ok = append_list(lines, (long long)"    return 1;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_semaphore_trywait(long long sp) {\n");
    ok = append_list(lines, (long long)"    EpSemaphore* s = (EpSemaphore*)sp;\n");
    ok = append_list(lines, (long long)"    pthread_mutex_lock(&s->mutex);\n");
    ok = append_list(lines, (long long)"    if (s->value > 0) {\n");
    ok = append_list(lines, (long long)"        s->value--;\n");
    ok = append_list(lines, (long long)"        pthread_mutex_unlock(&s->mutex);\n");
    ok = append_list(lines, (long long)"        return 1;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    pthread_mutex_unlock(&s->mutex);\n");
    ok = append_list(lines, (long long)"    return 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_semaphore_destroy(long long sp) {\n");
    ok = append_list(lines, (long long)"    EpSemaphore* s = (EpSemaphore*)sp;\n");
    ok = append_list(lines, (long long)"    pthread_mutex_destroy(&s->mutex);\n");
    ok = append_list(lines, (long long)"    pthread_cond_destroy(&s->cond);\n");
    ok = append_list(lines, (long long)"    free(s);\n");
    ret_val = join_strings(lines);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(1);
    return ret_val;
}

long long ep_rt_core_28() {
    long long lines = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&lines);
    ep_gc_maybe_collect();

    lines = create_list();
    ok = append_list(lines, (long long)"    return 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_condvar_create(void) {\n");
    ok = append_list(lines, (long long)"    pthread_cond_t* cv = (pthread_cond_t*)malloc(sizeof(pthread_cond_t));\n");
    ok = append_list(lines, (long long)"    pthread_cond_init(cv, NULL);\n");
    ok = append_list(lines, (long long)"    return (long long)cv;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_condvar_wait(long long cv, long long m) {\n");
    ok = append_list(lines, (long long)"    return pthread_cond_wait((pthread_cond_t*)cv, (pthread_mutex_t*)m) == 0 ? 1 : 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_condvar_signal(long long cv) {\n");
    ok = append_list(lines, (long long)"    return pthread_cond_signal((pthread_cond_t*)cv) == 0 ? 1 : 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_condvar_broadcast(long long cv) {\n");
    ok = append_list(lines, (long long)"    return pthread_cond_broadcast((pthread_cond_t*)cv) == 0 ? 1 : 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_condvar_destroy(long long cv) {\n");
    ok = append_list(lines, (long long)"    pthread_cond_destroy((pthread_cond_t*)cv);\n");
    ok = append_list(lines, (long long)"    free((void*)cv);\n");
    ok = append_list(lines, (long long)"    return 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* ========== Regex (simple stub — delegates to POSIX regex) ========== */\n");
    ok = append_list(lines, (long long)"#include <regex.h>\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_regex_match(long long pattern_ptr, long long text_ptr) {\n");
    ok = append_list(lines, (long long)"    regex_t regex;\n");
    ok = append_list(lines, (long long)"    const char* pattern = (const char*)pattern_ptr;\n");
    ok = append_list(lines, (long long)"    const char* text = (const char*)text_ptr;\n");
    ok = append_list(lines, (long long)"    int ret = regcomp(&regex, pattern, REG_EXTENDED | REG_NOSUB);\n");
    ok = append_list(lines, (long long)"    if (ret) return 0;\n");
    ok = append_list(lines, (long long)"    ret = regexec(&regex, text, 0, NULL, 0);\n");
    ok = append_list(lines, (long long)"    regfree(&regex);\n");
    ok = append_list(lines, (long long)"    return ret == 0 ? 1 : 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_regex_find(long long pattern_ptr, long long text_ptr) {\n");
    ok = append_list(lines, (long long)"    regex_t regex;\n");
    ok = append_list(lines, (long long)"    regmatch_t match;\n");
    ok = append_list(lines, (long long)"    const char* pattern = (const char*)pattern_ptr;\n");
    ok = append_list(lines, (long long)"    const char* text = (const char*)text_ptr;\n");
    ok = append_list(lines, (long long)"    int ret = regcomp(&regex, pattern, REG_EXTENDED);\n");
    ok = append_list(lines, (long long)"    if (ret) return (long long)\"\";\n");
    ok = append_list(lines, (long long)"    ret = regexec(&regex, text, 1, &match, 0);\n");
    ok = append_list(lines, (long long)"    if (ret != 0) { regfree(&regex); return (long long)\"\"; }\n");
    ok = append_list(lines, (long long)"    int len = match.rm_eo - match.rm_so;\n");
    ok = append_list(lines, (long long)"    char* result = (char*)malloc(len + 1);\n");
    ok = append_list(lines, (long long)"    memcpy(result, text + match.rm_so, len);\n");
    ok = append_list(lines, (long long)"    result[len] = '\\0';\n");
    ok = append_list(lines, (long long)"    regfree(&regex);\n");
    ok = append_list(lines, (long long)"    return (long long)result;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_regex_find_all(long long pattern_ptr, long long text_ptr) {\n");
    ok = append_list(lines, (long long)"    regex_t regex;\n");
    ok = append_list(lines, (long long)"    regmatch_t match;\n");
    ok = append_list(lines, (long long)"    const char* pattern = (const char*)pattern_ptr;\n");
    ok = append_list(lines, (long long)"    const char* text = (const char*)text_ptr;\n");
    ok = append_list(lines, (long long)"    long long list = create_list();\n");
    ok = append_list(lines, (long long)"    int ret = regcomp(&regex, pattern, REG_EXTENDED);\n");
    ok = append_list(lines, (long long)"    if (ret) return list;\n");
    ok = append_list(lines, (long long)"    const char* cursor = text;\n");
    ok = append_list(lines, (long long)"    while (regexec(&regex, cursor, 1, &match, 0) == 0) {\n");
    ok = append_list(lines, (long long)"        int len = match.rm_eo - match.rm_so;\n");
    ok = append_list(lines, (long long)"        char* result = (char*)malloc(len + 1);\n");
    ok = append_list(lines, (long long)"        memcpy(result, cursor + match.rm_so, len);\n");
    ok = append_list(lines, (long long)"        result[len] = '\\0';\n");
    ok = append_list(lines, (long long)"        append_list(list, (long long)result);\n");
    ok = append_list(lines, (long long)"        cursor += match.rm_eo;\n");
    ok = append_list(lines, (long long)"        if (match.rm_eo == 0) break;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    regfree(&regex);\n");
    ok = append_list(lines, (long long)"    return list;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_regex_replace(long long pattern_ptr, long long text_ptr, long long repl_ptr) {\n");
    ok = append_list(lines, (long long)"    /* Simple single-replacement via regex */\n");
    ok = append_list(lines, (long long)"    regex_t regex;\n");
    ok = append_list(lines, (long long)"    regmatch_t match;\n");
    ok = append_list(lines, (long long)"    const char* pattern = (const char*)pattern_ptr;\n");
    ok = append_list(lines, (long long)"    const char* text = (const char*)text_ptr;\n");
    ok = append_list(lines, (long long)"    const char* repl = (const char*)repl_ptr;\n");
    ok = append_list(lines, (long long)"    int ret = regcomp(&regex, pattern, REG_EXTENDED);\n");
    ok = append_list(lines, (long long)"    if (ret) return text_ptr;\n");
    ok = append_list(lines, (long long)"    ret = regexec(&regex, text, 1, &match, 0);\n");
    ok = append_list(lines, (long long)"    if (ret != 0) { regfree(&regex); return text_ptr; }\n");
    ok = append_list(lines, (long long)"    size_t tlen = strlen(text);\n");
    ok = append_list(lines, (long long)"    size_t rlen = strlen(repl);\n");
    ok = append_list(lines, (long long)"    size_t new_len = tlen - (match.rm_eo - match.rm_so) + rlen;\n");
    ok = append_list(lines, (long long)"    char* result = (char*)malloc(new_len + 1);\n");
    ok = append_list(lines, (long long)"    memcpy(result, text, match.rm_so);\n");
    ok = append_list(lines, (long long)"    memcpy(result + match.rm_so, repl, rlen);\n");
    ok = append_list(lines, (long long)"    memcpy(result + match.rm_so + rlen, text + match.rm_eo, tlen - match.rm_eo);\n");
    ok = append_list(lines, (long long)"    result[new_len] = '\\0';\n");
    ok = append_list(lines, (long long)"    regfree(&regex);\n");
    ok = append_list(lines, (long long)"    return (long long)result;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_regex_split(long long pattern_ptr, long long text_ptr) {\n");
    ok = append_list(lines, (long long)"    long long list = create_list();\n");
    ok = append_list(lines, (long long)"    /* Simple split: find matches and split around them */\n");
    ok = append_list(lines, (long long)"    regex_t regex;\n");
    ok = append_list(lines, (long long)"    regmatch_t match;\n");
    ok = append_list(lines, (long long)"    const char* pattern = (const char*)pattern_ptr;\n");
    ok = append_list(lines, (long long)"    const char* text = (const char*)text_ptr;\n");
    ok = append_list(lines, (long long)"    int ret = regcomp(&regex, pattern, REG_EXTENDED);\n");
    ok = append_list(lines, (long long)"    if (ret) {\n");
    ok = append_list(lines, (long long)"        append_list(list, text_ptr);\n");
    ok = append_list(lines, (long long)"        return list;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    const char* cursor = text;\n");
    ok = append_list(lines, (long long)"    while (regexec(&regex, cursor, 1, &match, 0) == 0) {\n");
    ok = append_list(lines, (long long)"        int len = match.rm_so;\n");
    ok = append_list(lines, (long long)"        char* part = (char*)malloc(len + 1);\n");
    ok = append_list(lines, (long long)"        memcpy(part, cursor, len);\n");
    ok = append_list(lines, (long long)"        part[len] = '\\0';\n");
    ok = append_list(lines, (long long)"        append_list(list, (long long)part);\n");
    ok = append_list(lines, (long long)"        cursor += match.rm_eo;\n");
    ok = append_list(lines, (long long)"        if (match.rm_eo == 0) break;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    char* rest = strdup(cursor);\n");
    ok = append_list(lines, (long long)"    append_list(list, (long long)rest);\n");
    ok = append_list(lines, (long long)"    regfree(&regex);\n");
    ok = append_list(lines, (long long)"    return list;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* ========== Base64 ========== */\n");
    ok = append_list(lines, (long long)"static const char b64_table[] = \"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/\";\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_base64_encode(long long data_ptr) {\n");
    ok = append_list(lines, (long long)"    const unsigned char* data = (const unsigned char*)data_ptr;\n");
    ok = append_list(lines, (long long)"    size_t len = strlen((const char*)data);\n");
    ok = append_list(lines, (long long)"    size_t out_len = 4 * ((len + 2) / 3);\n");
    ok = append_list(lines, (long long)"    char* out = (char*)malloc(out_len + 1);\n");
    ok = append_list(lines, (long long)"    size_t i, j = 0;\n");
    ok = append_list(lines, (long long)"    for (i = 0; i < len; i += 3) {\n");
    ok = append_list(lines, (long long)"        unsigned int n = data[i] << 16;\n");
    ok = append_list(lines, (long long)"        if (i + 1 < len) n |= data[i+1] << 8;\n");
    ok = append_list(lines, (long long)"        if (i + 2 < len) n |= data[i+2];\n");
    ok = append_list(lines, (long long)"        out[j++] = b64_table[(n >> 18) & 63];\n");
    ok = append_list(lines, (long long)"        out[j++] = b64_table[(n >> 12) & 63];\n");
    ok = append_list(lines, (long long)"        out[j++] = (i + 1 < len) ? b64_table[(n >> 6) & 63] : '=';\n");
    ok = append_list(lines, (long long)"        out[j++] = (i + 2 < len) ? b64_table[n & 63] : '=';\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    out[j] = '\\0';\n");
    ret_val = join_strings(lines);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(1);
    return ret_val;
}

long long ep_rt_core_29() {
    long long lines = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&lines);
    ep_gc_maybe_collect();

    lines = create_list();
    ok = append_list(lines, (long long)"    return (long long)out;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_uuid_v4(void) {\n");
    ok = append_list(lines, (long long)"    char* uuid = (char*)malloc(37);\n");
    ok = append_list(lines, (long long)"    unsigned char bytes[16];\n");
    ok = append_list(lines, (long long)"    ep_secure_random_bytes(bytes, 16);\n");
    ok = append_list(lines, (long long)"    bytes[6] = (bytes[6] & 0x0F) | 0x40;\n");
    ok = append_list(lines, (long long)"    bytes[8] = (bytes[8] & 0x3F) | 0x80;\n");
    ok = append_list(lines, (long long)"    snprintf(uuid, 37, \"%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x\",\n");
    ok = append_list(lines, (long long)"        bytes[0], bytes[1], bytes[2], bytes[3],\n");
    ok = append_list(lines, (long long)"        bytes[4], bytes[5], bytes[6], bytes[7],\n");
    ok = append_list(lines, (long long)"        bytes[8], bytes[9], bytes[10], bytes[11],\n");
    ok = append_list(lines, (long long)"        bytes[12], bytes[13], bytes[14], bytes[15]);\n");
    ok = append_list(lines, (long long)"    return (long long)uuid;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long file_read(long long path_val) {\n");
    ok = append_list(lines, (long long)"    const char* path = (const char*)path_val;\n");
    ok = append_list(lines, (long long)"    if (!path) return (long long)strdup(\"\");\n");
    ok = append_list(lines, (long long)"    FILE* f = fopen(path, \"rb\");\n");
    ok = append_list(lines, (long long)"    if (!f) return (long long)strdup(\"\");\n");
    ok = append_list(lines, (long long)"    fseek(f, 0, SEEK_END);\n");
    ok = append_list(lines, (long long)"    long size = ftell(f);\n");
    ok = append_list(lines, (long long)"    fseek(f, 0, SEEK_SET);\n");
    ok = append_list(lines, (long long)"    char* buf = malloc(size + 1);\n");
    ok = append_list(lines, (long long)"    if (!buf) { fclose(f); return (long long)strdup(\"\"); }\n");
    ok = append_list(lines, (long long)"    fread(buf, 1, size, f);\n");
    ok = append_list(lines, (long long)"    buf[size] = '\\0';\n");
    ok = append_list(lines, (long long)"    fclose(f);\n");
    ok = append_list(lines, (long long)"    ep_gc_register(buf, EP_OBJ_STRING);\n");
    ok = append_list(lines, (long long)"    return (long long)buf;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long file_write(long long path_val, long long content_val) {\n");
    ok = append_list(lines, (long long)"    const char* path = (const char*)path_val;\n");
    ok = append_list(lines, (long long)"    const char* content = (const char*)content_val;\n");
    ok = append_list(lines, (long long)"    if (!path || !content) return 0;\n");
    ok = append_list(lines, (long long)"    FILE* f = fopen(path, \"wb\");\n");
    ok = append_list(lines, (long long)"    if (!f) return 0;\n");
    ok = append_list(lines, (long long)"    size_t len = strlen(content);\n");
    ok = append_list(lines, (long long)"    fwrite(content, 1, len, f);\n");
    ok = append_list(lines, (long long)"    fclose(f);\n");
    ok = append_list(lines, (long long)"    return 1;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long file_append(long long path_val, long long content_val) {\n");
    ok = append_list(lines, (long long)"    const char* path = (const char*)path_val;\n");
    ok = append_list(lines, (long long)"    const char* content = (const char*)content_val;\n");
    ok = append_list(lines, (long long)"    if (!path || !content) return 0;\n");
    ok = append_list(lines, (long long)"    FILE* f = fopen(path, \"ab\");\n");
    ok = append_list(lines, (long long)"    if (!f) return 0;\n");
    ok = append_list(lines, (long long)"    size_t len = strlen(content);\n");
    ok = append_list(lines, (long long)"    fwrite(content, 1, len, f);\n");
    ok = append_list(lines, (long long)"    fclose(f);\n");
    ok = append_list(lines, (long long)"    return 1;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long file_exists(long long path_val) {\n");
    ok = append_list(lines, (long long)"    const char* path = (const char*)path_val;\n");
    ok = append_list(lines, (long long)"    if (!path) return 0;\n");
    ok = append_list(lines, (long long)"    FILE* f = fopen(path, \"r\");\n");
    ok = append_list(lines, (long long)"    if (f) { fclose(f); return 1; }\n");
    ok = append_list(lines, (long long)"    return 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long string_contains(long long s_val, long long sub_val) {\n");
    ok = append_list(lines, (long long)"    const char* s = (const char*)s_val;\n");
    ok = append_list(lines, (long long)"    const char* sub = (const char*)sub_val;\n");
    ok = append_list(lines, (long long)"    if (!s || !sub) return 0;\n");
    ok = append_list(lines, (long long)"    return strstr(s, sub) != NULL ? 1 : 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long string_index_of(long long s_val, long long sub_val) {\n");
    ok = append_list(lines, (long long)"    const char* s = (const char*)s_val;\n");
    ok = append_list(lines, (long long)"    const char* sub = (const char*)sub_val;\n");
    ok = append_list(lines, (long long)"    if (!s || !sub) return -1;\n");
    ok = append_list(lines, (long long)"    const char* found = strstr(s, sub);\n");
    ok = append_list(lines, (long long)"    if (!found) return -1;\n");
    ok = append_list(lines, (long long)"    return (long long)(found - s);\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long string_replace(long long s_val, long long old_val, long long new_val) {\n");
    ok = append_list(lines, (long long)"    const char* s = (const char*)s_val;\n");
    ok = append_list(lines, (long long)"    const char* old_str = (const char*)old_val;\n");
    ok = append_list(lines, (long long)"    const char* new_str = (const char*)new_val;\n");
    ok = append_list(lines, (long long)"    if (!s || !old_str || !new_str) return (long long)strdup(s ? s : \"\");\n");
    ok = append_list(lines, (long long)"    size_t old_len = strlen(old_str);\n");
    ok = append_list(lines, (long long)"    size_t new_len = strlen(new_str);\n");
    ok = append_list(lines, (long long)"    if (old_len == 0) return (long long)strdup(s);\n");
    ok = append_list(lines, (long long)"    int count = 0;\n");
    ok = append_list(lines, (long long)"    const char* p = s;\n");
    ok = append_list(lines, (long long)"    while ((p = strstr(p, old_str)) != NULL) { count++; p += old_len; }\n");
    ok = append_list(lines, (long long)"    size_t result_len = strlen(s) + count * (new_len - old_len);\n");
    ok = append_list(lines, (long long)"    char* result = malloc(result_len + 1);\n");
    ok = append_list(lines, (long long)"    if (!result) return (long long)strdup(s);\n");
    ok = append_list(lines, (long long)"    char* dst = result;\n");
    ok = append_list(lines, (long long)"    p = s;\n");
    ok = append_list(lines, (long long)"    while (*p) {\n");
    ok = append_list(lines, (long long)"        if (strncmp(p, old_str, old_len) == 0) {\n");
    ok = append_list(lines, (long long)"            memcpy(dst, new_str, new_len);\n");
    ok = append_list(lines, (long long)"            dst += new_len;\n");
    ok = append_list(lines, (long long)"            p += old_len;\n");
    ok = append_list(lines, (long long)"        } else {\n");
    ok = append_list(lines, (long long)"            *dst++ = *p++;\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    *dst = '\\0';\n");
    ok = append_list(lines, (long long)"    ep_gc_register(result, EP_OBJ_STRING);\n");
    ok = append_list(lines, (long long)"    return (long long)result;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* ========== Additional String Functions ========== */\n");
    ok = append_list(lines, (long long)"#include <ctype.h>\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long string_upper(long long s_val) {\n");
    ok = append_list(lines, (long long)"    const char* s = (const char*)s_val;\n");
    ok = append_list(lines, (long long)"    if (!s) return (long long)strdup(\"\");\n");
    ok = append_list(lines, (long long)"    long long len = strlen(s);\n");
    ok = append_list(lines, (long long)"    char* result = malloc(len + 1);\n");
    ok = append_list(lines, (long long)"    for (long long i = 0; i < len; i++) result[i] = toupper((unsigned char)s[i]);\n");
    ok = append_list(lines, (long long)"    result[len] = '\\0';\n");
    ok = append_list(lines, (long long)"    ep_gc_register(result, EP_OBJ_STRING);\n");
    ok = append_list(lines, (long long)"    return (long long)result;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long string_lower(long long s_val) {\n");
    ok = append_list(lines, (long long)"    const char* s = (const char*)s_val;\n");
    ok = append_list(lines, (long long)"    if (!s) return (long long)strdup(\"\");\n");
    ok = append_list(lines, (long long)"    long long len = strlen(s);\n");
    ok = append_list(lines, (long long)"    char* result = malloc(len + 1);\n");
    ok = append_list(lines, (long long)"    for (long long i = 0; i < len; i++) result[i] = tolower((unsigned char)s[i]);\n");
    ok = append_list(lines, (long long)"    result[len] = '\\0';\n");
    ok = append_list(lines, (long long)"    ep_gc_register(result, EP_OBJ_STRING);\n");
    ok = append_list(lines, (long long)"    return (long long)result;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long string_trim(long long s_val) {\n");
    ok = append_list(lines, (long long)"    const char* s = (const char*)s_val;\n");
    ok = append_list(lines, (long long)"    if (!s) return (long long)strdup(\"\");\n");
    ok = append_list(lines, (long long)"    while (*s && isspace((unsigned char)*s)) s++;\n");
    ok = append_list(lines, (long long)"    long long len = strlen(s);\n");
    ok = append_list(lines, (long long)"    while (len > 0 && isspace((unsigned char)s[len - 1])) len--;\n");
    ok = append_list(lines, (long long)"    char* result = malloc(len + 1);\n");
    ok = append_list(lines, (long long)"    memcpy(result, s, len);\n");
    ok = append_list(lines, (long long)"    result[len] = '\\0';\n");
    ok = append_list(lines, (long long)"    ep_gc_register(result, EP_OBJ_STRING);\n");
    ok = append_list(lines, (long long)"    return (long long)result;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ret_val = join_strings(lines);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(1);
    return ret_val;
}

long long ep_rt_core_30() {
    long long lines = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&lines);
    ep_gc_maybe_collect();

    lines = create_list();
    ok = append_list(lines, (long long)"long long string_split(long long s_val, long long delim_val) {\n");
    ok = append_list(lines, (long long)"    const char* s = (const char*)s_val;\n");
    ok = append_list(lines, (long long)"    const char* delim = (const char*)delim_val;\n");
    ok = append_list(lines, (long long)"    if (!s || !delim) return create_list();\n");
    ok = append_list(lines, (long long)"    long long list = create_list();\n");
    ok = append_list(lines, (long long)"    long long dlen = strlen(delim);\n");
    ok = append_list(lines, (long long)"    if (dlen == 0) { append_list(list, s_val); return list; }\n");
    ok = append_list(lines, (long long)"    const char* p = s;\n");
    ok = append_list(lines, (long long)"    while (1) {\n");
    ok = append_list(lines, (long long)"        const char* found = strstr(p, delim);\n");
    ok = append_list(lines, (long long)"        long long partlen = found ? (found - p) : (long long)strlen(p);\n");
    ok = append_list(lines, (long long)"        char* part = malloc(partlen + 1);\n");
    ok = append_list(lines, (long long)"        memcpy(part, p, partlen);\n");
    ok = append_list(lines, (long long)"        part[partlen] = '\\0';\n");
    ok = append_list(lines, (long long)"        ep_gc_register(part, EP_OBJ_STRING);\n");
    ok = append_list(lines, (long long)"        append_list(list, (long long)part);\n");
    ok = append_list(lines, (long long)"        if (!found) break;\n");
    ok = append_list(lines, (long long)"        p = found + dlen;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    return list;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long char_at(long long s_val, long long index) {\n");
    ok = append_list(lines, (long long)"    const char* s = (const char*)s_val;\n");
    ok = append_list(lines, (long long)"    if (!s || index < 0 || index >= (long long)strlen(s)) return 0;\n");
    ok = append_list(lines, (long long)"    return (unsigned char)s[index];\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long char_from_code(long long code) {\n");
    ok = append_list(lines, (long long)"    char* result = malloc(2);\n");
    ok = append_list(lines, (long long)"    result[0] = (char)code;\n");
    ok = append_list(lines, (long long)"    result[1] = '\\0';\n");
    ok = append_list(lines, (long long)"    ep_gc_register(result, EP_OBJ_STRING);\n");
    ok = append_list(lines, (long long)"    return (long long)result;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_abs(long long n) {\n");
    ok = append_list(lines, (long long)"    return n < 0 ? -n : n;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"// Auto-convert any value to string for string interpolation\n");
    ok = append_list(lines, (long long)"long long ep_auto_to_string(long long val) {\n");
    ok = append_list(lines, (long long)"    // If the value is 0, return \"0\"\n");
    ok = append_list(lines, (long long)"    if (val == 0) return (long long)strdup(\"0\");\n");
    ok = append_list(lines, (long long)"    // Check if val is a GC-tracked string (heap-allocated)\n");
    ok = append_list(lines, (long long)"    EpGCObject* obj = ep_gc_find((void*)val);\n");
    ok = append_list(lines, (long long)"    if (obj && obj->kind == EP_OBJ_STRING) {\n");
    ok = append_list(lines, (long long)"        return val; // It's a known string pointer\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    // Check if val is a static string literal (in .rodata/.data segment)\n");
    ok = append_list(lines, (long long)"    // These aren't GC-tracked but ARE valid pointers. Use a safe probe:\n");
    ok = append_list(lines, (long long)"    // only dereference if the address is in a readable memory page.\n");
    ok = append_list(lines, (long long)"    if (val > 0x100000) {\n");
    ok = append_list(lines, (long long)"#if defined(_WIN32)\n");
    ok = append_list(lines, (long long)"        // Windows: use VirtualQuery to safely probe pointer validity\n");
    ok = append_list(lines, (long long)"        MEMORY_BASIC_INFORMATION mbi;\n");
    ok = append_list(lines, (long long)"        if (VirtualQuery((void*)val, &mbi, sizeof(mbi)) && mbi.State == MEM_COMMIT && !(mbi.Protect & (PAGE_NOACCESS | PAGE_GUARD))) {\n");
    ok = append_list(lines, (long long)"            const char* p = (const char*)(void*)val;\n");
    ok = append_list(lines, (long long)"            unsigned char first = (unsigned char)*p;\n");
    ok = append_list(lines, (long long)"            if ((first >= 0x20 && first <= 0x7E) || (first >= 0xC0 && first <= 0xFD) || first == '\\n' || first == '\\t' || first == '\\r' || first == 0) {\n");
    ok = append_list(lines, (long long)"                return val; // Readable memory, looks like a string\n");
    ok = append_list(lines, (long long)"            }\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"#elif defined(__APPLE__)\n");
    ok = append_list(lines, (long long)"        // macOS: use vm_read_overwrite to safely probe\n");
    ok = append_list(lines, (long long)"        char probe;\n");
    ok = append_list(lines, (long long)"        vm_size_t sz = 1;\n");
    ok = append_list(lines, (long long)"        kern_return_t kr = vm_read_overwrite(mach_task_self(), (mach_vm_address_t)val, 1, (mach_vm_address_t)&probe, &sz);\n");
    ok = append_list(lines, (long long)"        if (kr == KERN_SUCCESS) {\n");
    ok = append_list(lines, (long long)"            unsigned char first = (unsigned char)probe;\n");
    ok = append_list(lines, (long long)"            if ((first >= 0x20 && first <= 0x7E) || (first >= 0xC0 && first <= 0xFD) || first == '\\n' || first == '\\t' || first == '\\r' || first == 0) {\n");
    ok = append_list(lines, (long long)"                return val; // Readable memory, looks like a string\n");
    ok = append_list(lines, (long long)"            }\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"#else\n");
    ok = append_list(lines, (long long)"        // Linux: use write() to /dev/null as a safe pointer probe\n");
    ok = append_list(lines, (long long)"        // write() returns -1 with EFAULT for invalid pointers, no signal\n");
    ok = append_list(lines, (long long)"        int devnull = open(\"/dev/null\", 1); // O_WRONLY\n");
    ok = append_list(lines, (long long)"        if (devnull >= 0) {\n");
    ok = append_list(lines, (long long)"            ssize_t r = write(devnull, (const void*)val, 1);\n");
    ok = append_list(lines, (long long)"            close(devnull);\n");
    ok = append_list(lines, (long long)"            if (r == 1) {\n");
    ok = append_list(lines, (long long)"                const char* p = (const char*)(void*)val;\n");
    ok = append_list(lines, (long long)"                unsigned char first = (unsigned char)*p;\n");
    ok = append_list(lines, (long long)"                if ((first >= 0x20 && first <= 0x7E) || (first >= 0xC0 && first <= 0xFD) || first == '\\n' || first == '\\t' || first == '\\r' || first == 0) {\n");
    ok = append_list(lines, (long long)"                    return val;\n");
    ok = append_list(lines, (long long)"                }\n");
    ok = append_list(lines, (long long)"            }\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"#endif\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    // Otherwise, convert integer to string\n");
    ok = append_list(lines, (long long)"    char* buf = (char*)malloc(32);\n");
    ok = append_list(lines, (long long)"    snprintf(buf, 32, \"%lld\", val);\n");
    ok = append_list(lines, (long long)"    ep_gc_register(buf, EP_OBJ_STRING);\n");
    ok = append_list(lines, (long long)"    return (long long)buf;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* Format a Float (double bits carried in a long long) as a string. F-string\n");
    ok = append_list(lines, (long long)"   interpolation routes Float-typed expressions here: ep_auto_to_string cannot\n");
    ok = append_list(lines, (long long)"   know the bits are a double and would print them as a huge integer. Uses the\n");
    ok = append_list(lines, (long long)"   same %.15g format as `display` so a float reads identically both ways. */\n");
    ok = append_list(lines, (long long)"long long ep_float_to_string(long long bits) {\n");
    ok = append_list(lines, (long long)"    double d;\n");
    ok = append_list(lines, (long long)"    memcpy(&d, &bits, sizeof(double));\n");
    ok = append_list(lines, (long long)"    char* buf = (char*)malloc(40);\n");
    ok = append_list(lines, (long long)"    snprintf(buf, 40, \"%.15g\", d);\n");
    ok = append_list(lines, (long long)"    ep_gc_register(buf, EP_OBJ_STRING);\n");
    ok = append_list(lines, (long long)"    return (long long)buf;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_random_int(long long min, long long max) {\n");
    ok = append_list(lines, (long long)"    if (max <= min) return min;\n");
    ok = append_list(lines, (long long)"    /* Draw from the OS CSPRNG with rejection sampling to avoid modulo bias. */\n");
    ok = append_list(lines, (long long)"    unsigned long long range = (unsigned long long)(max - min) + 1ULL;\n");
    ok = append_list(lines, (long long)"    unsigned long long limit = UINT64_MAX - (UINT64_MAX % range);\n");
    ok = append_list(lines, (long long)"    unsigned long long r;\n");
    ok = append_list(lines, (long long)"    do {\n");
    ok = append_list(lines, (long long)"        ep_secure_random_bytes((unsigned char*)&r, sizeof(r));\n");
    ok = append_list(lines, (long long)"    } while (r >= limit);\n");
    ok = append_list(lines, (long long)"    return min + (long long)(r % range);\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"// JSON built-in functions\n");
    ok = append_list(lines, (long long)"static const char* json_skip_ws(const char* p) {\n");
    ok = append_list(lines, (long long)"    while (*p == ' ' || *p == '\\t' || *p == '\\n' || *p == '\\r') p++;\n");
    ok = append_list(lines, (long long)"    return p;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"static const char* json_skip_value(const char* p) {\n");
    ok = append_list(lines, (long long)"    p = json_skip_ws(p);\n");
    ok = append_list(lines, (long long)"    if (*p == '\"') {\n");
    ok = append_list(lines, (long long)"        p++;\n");
    ok = append_list(lines, (long long)"        while (*p && *p != '\"') { if (*p == '\\\\') p++; p++; }\n");
    ok = append_list(lines, (long long)"        if (*p == '\"') p++;\n");
    ok = append_list(lines, (long long)"    } else if (*p == '{') {\n");
    ok = append_list(lines, (long long)"        int depth = 1; p++;\n");
    ok = append_list(lines, (long long)"        while (*p && depth > 0) {\n");
    ok = append_list(lines, (long long)"            if (*p == '\"') { p++; while (*p && *p != '\"') { if (*p == '\\\\') p++; p++; } if (*p) p++; }\n");
    ok = append_list(lines, (long long)"            else if (*p == '{') { depth++; p++; }\n");
    ok = append_list(lines, (long long)"            else if (*p == '}') { depth--; p++; }\n");
    ok = append_list(lines, (long long)"            else p++;\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"    } else if (*p == '[') {\n");
    ok = append_list(lines, (long long)"        int depth = 1; p++;\n");
    ok = append_list(lines, (long long)"        while (*p && depth > 0) {\n");
    ok = append_list(lines, (long long)"            if (*p == '\"') { p++; while (*p && *p != '\"') { if (*p == '\\\\') p++; p++; } if (*p) p++; }\n");
    ok = append_list(lines, (long long)"            else if (*p == '[') { depth++; p++; }\n");
    ok = append_list(lines, (long long)"            else if (*p == ']') { depth--; p++; }\n");
    ok = append_list(lines, (long long)"            else p++;\n");
    ret_val = join_strings(lines);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(1);
    return ret_val;
}

long long ep_rt_core_31() {
    long long lines = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&lines);
    ep_gc_maybe_collect();

    lines = create_list();
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"    } else {\n");
    ok = append_list(lines, (long long)"        while (*p && *p != ',' && *p != '}' && *p != ']' && *p != ' ' && *p != '\\n') p++;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    return p;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"static const char* json_find_key(const char* json, const char* key) {\n");
    ok = append_list(lines, (long long)"    const char* p = json_skip_ws(json);\n");
    ok = append_list(lines, (long long)"    if (*p != '{') return NULL;\n");
    ok = append_list(lines, (long long)"    p++;\n");
    ok = append_list(lines, (long long)"    while (*p) {\n");
    ok = append_list(lines, (long long)"        p = json_skip_ws(p);\n");
    ok = append_list(lines, (long long)"        if (*p == '}') return NULL;\n");
    ok = append_list(lines, (long long)"        if (*p != '\"') return NULL;\n");
    ok = append_list(lines, (long long)"        p++;\n");
    ok = append_list(lines, (long long)"        const char* ks = p;\n");
    ok = append_list(lines, (long long)"        while (*p && *p != '\"') { if (*p == '\\\\') p++; p++; }\n");
    ok = append_list(lines, (long long)"        size_t klen = p - ks;\n");
    ok = append_list(lines, (long long)"        if (*p == '\"') p++;\n");
    ok = append_list(lines, (long long)"        p = json_skip_ws(p);\n");
    ok = append_list(lines, (long long)"        if (*p == ':') p++;\n");
    ok = append_list(lines, (long long)"        p = json_skip_ws(p);\n");
    ok = append_list(lines, (long long)"        if (klen == strlen(key) && strncmp(ks, key, klen) == 0) {\n");
    ok = append_list(lines, (long long)"            return p;\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"        p = json_skip_value(p);\n");
    ok = append_list(lines, (long long)"        p = json_skip_ws(p);\n");
    ok = append_list(lines, (long long)"        if (*p == ',') p++;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    return NULL;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long json_get_string(long long json_val, long long key_val) {\n");
    ok = append_list(lines, (long long)"    const char* json = (const char*)json_val;\n");
    ok = append_list(lines, (long long)"    const char* key = (const char*)key_val;\n");
    ok = append_list(lines, (long long)"    if (!json || !key) return (long long)strdup(\"\");\n");
    ok = append_list(lines, (long long)"    const char* val = json_find_key(json, key);\n");
    ok = append_list(lines, (long long)"    if (!val || *val != '\"') return (long long)strdup(\"\");\n");
    ok = append_list(lines, (long long)"    val++;\n");
    ok = append_list(lines, (long long)"    const char* end = val;\n");
    ok = append_list(lines, (long long)"    while (*end && *end != '\"') { if (*end == '\\\\') end++; end++; }\n");
    ok = append_list(lines, (long long)"    size_t len = end - val;\n");
    ok = append_list(lines, (long long)"    char* result = (char*)malloc(len + 1);\n");
    ok = append_list(lines, (long long)"    // Handle escape sequences\n");
    ok = append_list(lines, (long long)"    size_t di = 0;\n");
    ok = append_list(lines, (long long)"    const char* si = val;\n");
    ok = append_list(lines, (long long)"    while (si < end) {\n");
    ok = append_list(lines, (long long)"        if (*si == '\\\\' && si + 1 < end) {\n");
    ok = append_list(lines, (long long)"            si++;\n");
    ok = append_list(lines, (long long)"            switch (*si) {\n");
    ok = append_list(lines, (long long)"                case 'n': result[di++] = '\\n'; break;\n");
    ok = append_list(lines, (long long)"                case 't': result[di++] = '\\t'; break;\n");
    ok = append_list(lines, (long long)"                case 'r': result[di++] = '\\r'; break;\n");
    ok = append_list(lines, (long long)"                case '\"': result[di++] = '\"'; break;\n");
    ok = append_list(lines, (long long)"                case '\\\\': result[di++] = '\\\\'; break;\n");
    ok = append_list(lines, (long long)"                default: result[di++] = *si; break;\n");
    ok = append_list(lines, (long long)"            }\n");
    ok = append_list(lines, (long long)"        } else {\n");
    ok = append_list(lines, (long long)"            result[di++] = *si;\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"        si++;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    result[di] = '\\0';\n");
    ok = append_list(lines, (long long)"    ep_gc_register(result, EP_OBJ_STRING);\n");
    ok = append_list(lines, (long long)"    return (long long)result;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long json_get_int(long long json_val, long long key_val) {\n");
    ok = append_list(lines, (long long)"    const char* json = (const char*)json_val;\n");
    ok = append_list(lines, (long long)"    const char* key = (const char*)key_val;\n");
    ok = append_list(lines, (long long)"    if (!json || !key) return 0;\n");
    ok = append_list(lines, (long long)"    const char* val = json_find_key(json, key);\n");
    ok = append_list(lines, (long long)"    if (!val) return 0;\n");
    ok = append_list(lines, (long long)"    return atoll(val);\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long json_get_bool(long long json_val, long long key_val) {\n");
    ok = append_list(lines, (long long)"    const char* json = (const char*)json_val;\n");
    ok = append_list(lines, (long long)"    const char* key = (const char*)key_val;\n");
    ok = append_list(lines, (long long)"    if (!json || !key) return 0;\n");
    ok = append_list(lines, (long long)"    const char* val = json_find_key(json, key);\n");
    ok = append_list(lines, (long long)"    if (!val) return 0;\n");
    ok = append_list(lines, (long long)"    if (strncmp(val, \"true\", 4) == 0) return 1;\n");
    ok = append_list(lines, (long long)"    return 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"// SHA-1 implementation (RFC 3174) for WebSocket handshake\n");
    ok = append_list(lines, (long long)"static unsigned int sha1_left_rotate(unsigned int x, int n) {\n");
    ok = append_list(lines, (long long)"    return (x << n) | (x >> (32 - n));\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_sha1(long long data_val) {\n");
    ok = append_list(lines, (long long)"    const unsigned char* data = (const unsigned char*)data_val;\n");
    ok = append_list(lines, (long long)"    if (!data) return (long long)strdup(\"\");\n");
    ok = append_list(lines, (long long)"    size_t len = strlen((const char*)data);\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"    unsigned int h0 = 0x67452301, h1 = 0xEFCDAB89, h2 = 0x98BADCFE, h3 = 0x10325476, h4 = 0xC3D2E1F0;\n");
    ok = append_list(lines, (long long)"    size_t new_len = len + 1;\n");
    ok = append_list(lines, (long long)"    while (new_len % 64 != 56) new_len++;\n");
    ok = append_list(lines, (long long)"    unsigned char* msg = (unsigned char*)calloc(new_len + 8, 1);\n");
    ok = append_list(lines, (long long)"    memcpy(msg, data, len);\n");
    ok = append_list(lines, (long long)"    msg[len] = 0x80;\n");
    ok = append_list(lines, (long long)"    unsigned long long bits_len = (unsigned long long)len * 8;\n");
    ok = append_list(lines, (long long)"    for (int i = 0; i < 8; i++) msg[new_len + 7 - i] = (unsigned char)(bits_len >> (i * 8));\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"    for (size_t offset = 0; offset < new_len + 8; offset += 64) {\n");
    ok = append_list(lines, (long long)"        unsigned int w[80];\n");
    ok = append_list(lines, (long long)"        for (int i = 0; i < 16; i++) {\n");
    ok = append_list(lines, (long long)"            w[i] = ((unsigned int)msg[offset + i*4] << 24) | ((unsigned int)msg[offset + i*4+1] << 16) |\n");
    ok = append_list(lines, (long long)"                    ((unsigned int)msg[offset + i*4+2] << 8) | (unsigned int)msg[offset + i*4+3];\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"        for (int i = 16; i < 80; i++) w[i] = sha1_left_rotate(w[i-3] ^ w[i-8] ^ w[i-14] ^ w[i-16], 1);\n");
    ok = append_list(lines, (long long)"        unsigned int a = h0, b = h1, c = h2, d = h3, e = h4;\n");
    ok = append_list(lines, (long long)"        for (int i = 0; i < 80; i++) {\n");
    ok = append_list(lines, (long long)"            unsigned int f, k;\n");
    ok = append_list(lines, (long long)"            if (i < 20) { f = (b & c) | ((~b) & d); k = 0x5A827999; }\n");
    ok = append_list(lines, (long long)"            else if (i < 40) { f = b ^ c ^ d; k = 0x6ED9EBA1; }\n");
    ok = append_list(lines, (long long)"            else if (i < 60) { f = (b & c) | (b & d) | (c & d); k = 0x8F1BBCDC; }\n");
    ok = append_list(lines, (long long)"            else { f = b ^ c ^ d; k = 0xCA62C1D6; }\n");
    ok = append_list(lines, (long long)"            unsigned int temp = sha1_left_rotate(a, 5) + f + e + k + w[i];\n");
    ok = append_list(lines, (long long)"            e = d; d = c; c = sha1_left_rotate(b, 30); b = a; a = temp;\n");
    ok = append_list(lines, (long long)"        }\n");
    ok = append_list(lines, (long long)"        h0 += a; h1 += b; h2 += c; h3 += d; h4 += e;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    free(msg);\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"    // Return Base64-encoded hash directly (for WebSocket handshake)\n");
    ok = append_list(lines, (long long)"    unsigned char hash[20];\n");
    ok = append_list(lines, (long long)"    hash[0] = (h0>>24)&0xFF; hash[1] = (h0>>16)&0xFF; hash[2] = (h0>>8)&0xFF; hash[3] = h0&0xFF;\n");
    ok = append_list(lines, (long long)"    hash[4] = (h1>>24)&0xFF; hash[5] = (h1>>16)&0xFF; hash[6] = (h1>>8)&0xFF; hash[7] = h1&0xFF;\n");
    ok = append_list(lines, (long long)"    hash[8] = (h2>>24)&0xFF; hash[9] = (h2>>16)&0xFF; hash[10] = (h2>>8)&0xFF; hash[11] = h2&0xFF;\n");
    ok = append_list(lines, (long long)"    hash[12] = (h3>>24)&0xFF; hash[13] = (h3>>16)&0xFF; hash[14] = (h3>>8)&0xFF; hash[15] = h3&0xFF;\n");
    ok = append_list(lines, (long long)"    hash[16] = (h4>>24)&0xFF; hash[17] = (h4>>16)&0xFF; hash[18] = (h4>>8)&0xFF; hash[19] = h4&0xFF;\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"    // Base64 encode the 20-byte hash\n");
    ok = append_list(lines, (long long)"    size_t b64_len = 4 * ((20 + 2) / 3);\n");
    ok = append_list(lines, (long long)"    char* result = (char*)malloc(b64_len + 1);\n");
    ok = append_list(lines, (long long)"    size_t j = 0;\n");
    ok = append_list(lines, (long long)"    for (size_t bi = 0; bi < 20; bi += 3) {\n");
    ok = append_list(lines, (long long)"        unsigned int n2 = ((unsigned int)hash[bi]) << 16;\n");
    ok = append_list(lines, (long long)"        if (bi + 1 < 20) n2 |= ((unsigned int)hash[bi+1]) << 8;\n");
    ok = append_list(lines, (long long)"        if (bi + 2 < 20) n2 |= (unsigned int)hash[bi+2];\n");
    ok = append_list(lines, (long long)"        result[j++] = b64_table[(n2 >> 18) & 0x3F];\n");
    ok = append_list(lines, (long long)"        result[j++] = b64_table[(n2 >> 12) & 0x3F];\n");
    ok = append_list(lines, (long long)"        result[j++] = (bi + 1 < 20) ? b64_table[(n2 >> 6) & 0x3F] : '=';\n");
    ok = append_list(lines, (long long)"        result[j++] = (bi + 2 < 20) ? b64_table[n2 & 0x3F] : '=';\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    result[j] = '\\0';\n");
    ok = append_list(lines, (long long)"    ep_gc_register(result, EP_OBJ_STRING);\n");
    ret_val = join_strings(lines);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(1);
    return ret_val;
}

long long ep_rt_core_32() {
    long long lines = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&lines);
    ep_gc_maybe_collect();

    lines = create_list();
    ok = append_list(lines, (long long)"    return (long long)result;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"// Read exact N bytes from a socket\n");
    ok = append_list(lines, (long long)"#ifdef __wasm__\n");
    ok = append_list(lines, (long long)"long long ep_net_recv_bytes(long long fd, long long count) {\n");
    ok = append_list(lines, (long long)"    (void)fd; (void)count;\n");
    ok = append_list(lines, (long long)"    return (long long)strdup(\"\");\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"#else\n");
    ok = append_list(lines, (long long)"long long ep_net_recv_bytes(long long fd, long long count) {\n");
    ok = append_list(lines, (long long)"    if (count <= 0) return (long long)strdup(\"\");\n");
    ok = append_list(lines, (long long)"    char* buf = (char*)malloc(count + 1);\n");
    ok = append_list(lines, (long long)"#ifdef _WIN32\n");
    ok = append_list(lines, (long long)"    int total = 0;\n");
    ok = append_list(lines, (long long)"    while (total < (int)count) {\n");
    ok = append_list(lines, (long long)"        int n = recv((int)fd, buf + total, (int)(count - total), 0);\n");
    ok = append_list(lines, (long long)"        if (n <= 0) break;\n");
    ok = append_list(lines, (long long)"        total += n;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"#else\n");
    ok = append_list(lines, (long long)"    ssize_t total = 0;\n");
    ok = append_list(lines, (long long)"    while (total < count) {\n");
    ok = append_list(lines, (long long)"        ssize_t n = recv((int)fd, buf + total, count - total, 0);\n");
    ok = append_list(lines, (long long)"        if (n <= 0) break;\n");
    ok = append_list(lines, (long long)"        total += n;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"#endif\n");
    ok = append_list(lines, (long long)"    buf[total] = '\\0';\n");
    ok = append_list(lines, (long long)"    ep_gc_register(buf, EP_OBJ_STRING);\n");
    ok = append_list(lines, (long long)"    return (long long)buf;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"#endif\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_get_args(void) {\n");
    ok = append_list(lines, (long long)"    long long list_ptr = create_list();\n");
    ok = append_list(lines, (long long)"    for (int i = 0; i < ep_argc; i++) {\n");
    ok = append_list(lines, (long long)"        char* arg_copy = strdup(ep_argv[i]);\n");
    ok = append_list(lines, (long long)"        ep_gc_register(arg_copy, EP_OBJ_STRING);\n");
    ok = append_list(lines, (long long)"        append_list(list_ptr, (long long)arg_copy);\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    return list_ptr;\n");
    ok = append_list(lines, (long long)"}\n");
    ret_val = join_strings(lines);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(1);
    return ret_val;
}

long long ep_rt_builtins_0() {
    long long lines = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&lines);
    ep_gc_maybe_collect();

    lines = create_list();
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* Built-in: string concatenation */\n");
    ok = append_list(lines, (long long)"long long concat(long long a, long long b) {\n");
    ok = append_list(lines, (long long)"    const char* sa = (const char*)a;\n");
    ok = append_list(lines, (long long)"    const char* sb = (const char*)b;\n");
    ok = append_list(lines, (long long)"    long long la = strlen(sa);\n");
    ok = append_list(lines, (long long)"    long long lb = strlen(sb);\n");
    ok = append_list(lines, (long long)"    char* result = malloc(la + lb + 1);\n");
    ok = append_list(lines, (long long)"    memcpy(result, sa, la);\n");
    ok = append_list(lines, (long long)"    memcpy(result + la, sb, lb);\n");
    ok = append_list(lines, (long long)"    result[la + lb] = '\\0';\n");
    ok = append_list(lines, (long long)"    ep_gc_register(result, EP_OBJ_STRING);\n");
    ok = append_list(lines, (long long)"    return (long long)result;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long int_to_string(long long val) {\n");
    ok = append_list(lines, (long long)"    char* buf = malloc(32);\n");
    ok = append_list(lines, (long long)"    snprintf(buf, 32, \"%lld\", val);\n");
    ok = append_list(lines, (long long)"    ep_gc_register(buf, EP_OBJ_STRING);\n");
    ok = append_list(lines, (long long)"    return (long long)buf;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_int_to_str(long long val) { return int_to_string(val); }\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"typedef struct { char* data; long long len; long long cap; } EpStringBuilder;\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_sb_create(long long dummy) {\n");
    ok = append_list(lines, (long long)"    (void)dummy;\n");
    ok = append_list(lines, (long long)"    EpStringBuilder* sb = (EpStringBuilder*)malloc(sizeof(EpStringBuilder));\n");
    ok = append_list(lines, (long long)"    sb->cap = 256;\n");
    ok = append_list(lines, (long long)"    sb->len = 0;\n");
    ok = append_list(lines, (long long)"    sb->data = (char*)malloc(sb->cap);\n");
    ok = append_list(lines, (long long)"    sb->data[0] = '\\0';\n");
    ok = append_list(lines, (long long)"    return (long long)sb;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_sb_append(long long sb_ptr, long long str_ptr) {\n");
    ok = append_list(lines, (long long)"    EpStringBuilder* sb = (EpStringBuilder*)sb_ptr;\n");
    ok = append_list(lines, (long long)"    const char* s = (const char*)str_ptr;\n");
    ok = append_list(lines, (long long)"    if (!s) return sb_ptr;\n");
    ok = append_list(lines, (long long)"    long long slen = strlen(s);\n");
    ok = append_list(lines, (long long)"    while (sb->len + slen + 1 > sb->cap) {\n");
    ok = append_list(lines, (long long)"        sb->cap *= 2;\n");
    ok = append_list(lines, (long long)"        sb->data = (char*)realloc(sb->data, sb->cap);\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    memcpy(sb->data + sb->len, s, slen);\n");
    ok = append_list(lines, (long long)"    sb->len += slen;\n");
    ok = append_list(lines, (long long)"    sb->data[sb->len] = '\\0';\n");
    ok = append_list(lines, (long long)"    return sb_ptr;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_sb_append_int(long long sb_ptr, long long val) {\n");
    ok = append_list(lines, (long long)"    char buf[32];\n");
    ok = append_list(lines, (long long)"    snprintf(buf, sizeof(buf), \"%lld\", val);\n");
    ok = append_list(lines, (long long)"    return ep_sb_append(sb_ptr, (long long)buf);\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_sb_to_string(long long sb_ptr) {\n");
    ok = append_list(lines, (long long)"    EpStringBuilder* sb = (EpStringBuilder*)sb_ptr;\n");
    ok = append_list(lines, (long long)"    char* result = (char*)malloc(sb->len + 1);\n");
    ok = append_list(lines, (long long)"    memcpy(result, sb->data, sb->len + 1);\n");
    ok = append_list(lines, (long long)"    ep_gc_register(result, EP_OBJ_STRING);\n");
    ok = append_list(lines, (long long)"    free(sb->data);\n");
    ok = append_list(lines, (long long)"    free(sb);\n");
    ok = append_list(lines, (long long)"    return (long long)result;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_sb_length(long long sb_ptr) {\n");
    ok = append_list(lines, (long long)"    return ((EpStringBuilder*)sb_ptr)->len;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long str_to_ptr(long long s) { return s; }\n");
    ok = append_list(lines, (long long)"long long ptr_to_str(long long p) {\n");
    ok = append_list(lines, (long long)"    if (p == 0) return (long long)strdup(\"\");\n");
    ok = append_list(lines, (long long)"    char* copy = strdup((const char*)p);\n");
    ok = append_list(lines, (long long)"    ep_gc_register(copy, EP_OBJ_STRING);\n");
    ok = append_list(lines, (long long)"    return (long long)copy;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long peek_byte(long long ptr, long long offset) {\n");
    ok = append_list(lines, (long long)"    return (long long)((unsigned char*)ptr)[offset];\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"long long poke_byte(long long ptr, long long offset, long long value) {\n");
    ok = append_list(lines, (long long)"    ((unsigned char*)ptr)[offset] = (unsigned char)value;\n");
    ok = append_list(lines, (long long)"    return 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"long long alloc_bytes(long long size) {\n");
    ok = append_list(lines, (long long)"    return (long long)calloc((size_t)size, 1);\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"long long free_bytes(long long ptr) {\n");
    ok = append_list(lines, (long long)"    free((void*)ptr);\n");
    ok = append_list(lines, (long long)"    return 0;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"long long list_to_bytes(long long list_ptr) {\n");
    ok = append_list(lines, (long long)"    long long len = length_list(list_ptr);\n");
    ok = append_list(lines, (long long)"    unsigned char* buf = (unsigned char*)malloc(len);\n");
    ok = append_list(lines, (long long)"    for (long long i = 0; i < len; i++) {\n");
    ok = append_list(lines, (long long)"        buf[i] = (unsigned char)get_list(list_ptr, i);\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    return (long long)buf;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"long long bytes_to_list(long long ptr, long long len) {\n");
    ok = append_list(lines, (long long)"    long long list = create_list();\n");
    ok = append_list(lines, (long long)"    unsigned char* buf = (unsigned char*)ptr;\n");
    ok = append_list(lines, (long long)"    for (long long i = 0; i < len; i++) {\n");
    ok = append_list(lines, (long long)"        append_list(list, (long long)buf[i]);\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    return list;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long ep_gc_get_minor_count() {\n");
    ok = append_list(lines, (long long)"    return ep_gc_minor_count;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"long long ep_gc_get_major_count() {\n");
    ok = append_list(lines, (long long)"    return ep_gc_major_count;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"long long ep_gc_get_nursery_count() {\n");
    ok = append_list(lines, (long long)"    return ep_gc_nursery_count;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long string_to_int(long long s) {\n");
    ok = append_list(lines, (long long)"    if (s == 0) return 0;\n");
    ok = append_list(lines, (long long)"    return atoll((const char*)s);\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long read_line() {\n");
    ok = append_list(lines, (long long)"    char buf[4096];\n");
    ok = append_list(lines, (long long)"    if (fgets(buf, sizeof(buf), stdin) == NULL) { buf[0] = '\\0'; }\n");
    ok = append_list(lines, (long long)"    size_t len = strlen(buf);\n");
    ok = append_list(lines, (long long)"    if (len > 0 && buf[len-1] == '\\n') buf[len-1] = '\\0';\n");
    ok = append_list(lines, (long long)"    char* result = strdup(buf);\n");
    ok = append_list(lines, (long long)"    ep_gc_register(result, EP_OBJ_STRING);\n");
    ok = append_list(lines, (long long)"    return (long long)result;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long read_int() {\n");
    ok = append_list(lines, (long long)"    long long val = 0;\n");
    ok = append_list(lines, (long long)"    scanf(\"%lld\", &val);\n");
    ok = append_list(lines, (long long)"    while(getchar() != '\\n');\n");
    ok = append_list(lines, (long long)"    return val;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* Read ONE key immediately: no echo, no waiting for Enter. Returns the key's\n");
    ok = append_list(lines, (long long)"   number code (one byte at a time — escape sequences such as arrow keys\n");
    ok = append_list(lines, (long long)"   arrive as successive codes), or -1 at end of input. When stdin is a pipe\n");
    ok = append_list(lines, (long long)"   or a file (scripted tests), it simply reads the next byte. */\n");
    ok = append_list(lines, (long long)"long long read_key() {\n");
    ok = append_list(lines, (long long)"#if defined(__wasm__)\n");
    ok = append_list(lines, (long long)"    return -1;\n");
    ok = append_list(lines, (long long)"#elif defined(_WIN32)\n");
    ret_val = join_strings(lines);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(1);
    return ret_val;
}

long long ep_rt_builtins_1() {
    long long lines = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&lines);
    ep_gc_maybe_collect();

    lines = create_list();
    ok = append_list(lines, (long long)"    if (!_isatty(_fileno(stdin))) {\n");
    ok = append_list(lines, (long long)"        return (long long)fgetc(stdin);\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    return (long long)_getch();\n");
    ok = append_list(lines, (long long)"#else\n");
    ok = append_list(lines, (long long)"    if (!isatty(STDIN_FILENO)) {\n");
    ok = append_list(lines, (long long)"        return (long long)fgetc(stdin);\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    struct termios old_state, raw_state;\n");
    ok = append_list(lines, (long long)"    if (tcgetattr(STDIN_FILENO, &old_state) != 0) {\n");
    ok = append_list(lines, (long long)"        return (long long)fgetc(stdin);\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    raw_state = old_state;\n");
    ok = append_list(lines, (long long)"    raw_state.c_lflag &= ~(ICANON | ECHO);\n");
    ok = append_list(lines, (long long)"    raw_state.c_cc[VMIN] = 1;\n");
    ok = append_list(lines, (long long)"    raw_state.c_cc[VTIME] = 0;\n");
    ok = append_list(lines, (long long)"    tcsetattr(STDIN_FILENO, TCSANOW, &raw_state);\n");
    ok = append_list(lines, (long long)"    unsigned char ch = 0;\n");
    ok = append_list(lines, (long long)"    long long got = (long long)read(STDIN_FILENO, &ch, 1);\n");
    ok = append_list(lines, (long long)"    tcsetattr(STDIN_FILENO, TCSANOW, &old_state);\n");
    ok = append_list(lines, (long long)"    if (got <= 0) return -1;\n");
    ok = append_list(lines, (long long)"    return (long long)ch;\n");
    ok = append_list(lines, (long long)"#endif\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* How wide the terminal window is, in characters. 80 when unknown. */\n");
    ok = append_list(lines, (long long)"long long terminal_columns() {\n");
    ok = append_list(lines, (long long)"#if defined(__wasm__)\n");
    ok = append_list(lines, (long long)"    return 80;\n");
    ok = append_list(lines, (long long)"#elif defined(_WIN32)\n");
    ok = append_list(lines, (long long)"    CONSOLE_SCREEN_BUFFER_INFO info;\n");
    ok = append_list(lines, (long long)"    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info)) {\n");
    ok = append_list(lines, (long long)"        long long cols = (long long)(info.srWindow.Right - info.srWindow.Left + 1);\n");
    ok = append_list(lines, (long long)"        if (cols > 0) return cols;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    return 80;\n");
    ok = append_list(lines, (long long)"#else\n");
    ok = append_list(lines, (long long)"    struct winsize ws;\n");
    ok = append_list(lines, (long long)"    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0 && ws.ws_col > 0) {\n");
    ok = append_list(lines, (long long)"        return (long long)ws.ws_col;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    return 80;\n");
    ok = append_list(lines, (long long)"#endif\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"/* How tall the terminal window is, in lines. 24 when unknown. */\n");
    ok = append_list(lines, (long long)"long long terminal_rows() {\n");
    ok = append_list(lines, (long long)"#if defined(__wasm__)\n");
    ok = append_list(lines, (long long)"    return 24;\n");
    ok = append_list(lines, (long long)"#elif defined(_WIN32)\n");
    ok = append_list(lines, (long long)"    CONSOLE_SCREEN_BUFFER_INFO info;\n");
    ok = append_list(lines, (long long)"    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info)) {\n");
    ok = append_list(lines, (long long)"        long long rows = (long long)(info.srWindow.Bottom - info.srWindow.Top + 1);\n");
    ok = append_list(lines, (long long)"        if (rows > 0) return rows;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    return 24;\n");
    ok = append_list(lines, (long long)"#else\n");
    ok = append_list(lines, (long long)"    struct winsize ws;\n");
    ok = append_list(lines, (long long)"    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0 && ws.ws_row > 0) {\n");
    ok = append_list(lines, (long long)"        return (long long)ws.ws_row;\n");
    ok = append_list(lines, (long long)"    }\n");
    ok = append_list(lines, (long long)"    return 24;\n");
    ok = append_list(lines, (long long)"#endif\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long read_float() {\n");
    ok = append_list(lines, (long long)"    double val = 0.0;\n");
    ok = append_list(lines, (long long)"    scanf(\"%lf\", &val);\n");
    ok = append_list(lines, (long long)"    while(getchar() != '\\n');\n");
    ok = append_list(lines, (long long)"    long long result; memcpy(&result, &val, sizeof(double));\n");
    ok = append_list(lines, (long long)"    return result;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long int_to_float(long long val) {\n");
    ok = append_list(lines, (long long)"    double d = (double)val;\n");
    ok = append_list(lines, (long long)"    long long result; memcpy(&result, &d, sizeof(double));\n");
    ok = append_list(lines, (long long)"    return result;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ok = append_list(lines, (long long)"long long float_to_int(long long val) {\n");
    ok = append_list(lines, (long long)"    double d; memcpy(&d, &val, sizeof(double));\n");
    ok = append_list(lines, (long long)"    return (long long)d;\n");
    ok = append_list(lines, (long long)"}\n");
    ok = append_list(lines, (long long)"\n");
    ret_val = join_strings(lines);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(1);
    return ret_val;
}

long long get_shared_runtime_source() {
    long long parts = 0;
    long long ok = 0;
    long long ret_val = 0;

    ep_gc_push_root(&parts);
    ep_gc_maybe_collect();

    parts = create_list();
    ok = append_list(parts, ep_rt_core_0());
    ok = append_list(parts, ep_rt_core_1());
    ok = append_list(parts, ep_rt_core_2());
    ok = append_list(parts, ep_rt_core_3());
    ok = append_list(parts, ep_rt_core_4());
    ok = append_list(parts, ep_rt_core_5());
    ok = append_list(parts, ep_rt_core_6());
    ok = append_list(parts, ep_rt_core_7());
    ok = append_list(parts, ep_rt_core_8());
    ok = append_list(parts, ep_rt_core_9());
    ok = append_list(parts, ep_rt_core_10());
    ok = append_list(parts, ep_rt_core_11());
    ok = append_list(parts, ep_rt_core_12());
    ok = append_list(parts, ep_rt_core_13());
    ok = append_list(parts, ep_rt_core_14());
    ok = append_list(parts, ep_rt_core_15());
    ok = append_list(parts, ep_rt_core_16());
    ok = append_list(parts, ep_rt_core_17());
    ok = append_list(parts, ep_rt_core_18());
    ok = append_list(parts, ep_rt_core_19());
    ok = append_list(parts, ep_rt_core_20());
    ok = append_list(parts, ep_rt_core_21());
    ok = append_list(parts, ep_rt_core_22());
    ok = append_list(parts, ep_rt_core_23());
    ok = append_list(parts, ep_rt_core_24());
    ok = append_list(parts, ep_rt_core_25());
    ok = append_list(parts, ep_rt_core_26());
    ok = append_list(parts, ep_rt_core_27());
    ok = append_list(parts, ep_rt_core_28());
    ok = append_list(parts, ep_rt_core_29());
    ok = append_list(parts, ep_rt_core_30());
    ok = append_list(parts, ep_rt_core_31());
    ok = append_list(parts, ep_rt_core_32());
    ok = append_list(parts, ep_rt_builtins_0());
    ok = append_list(parts, ep_rt_builtins_1());
    ret_val = join_strings(parts);
    goto L_cleanup;
L_cleanup:
    ep_gc_pop_roots(1);
    return ret_val;
}


/* Bootstrapper C main */
void __ep_init_constants(void);
int main(int argc, char** argv) {
    init_ep_args(argc, argv);
    __ep_init_constants();
    int result = (int)_main();
    ep_gc_shutdown();
    return result;
}
