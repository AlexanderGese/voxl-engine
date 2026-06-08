#ifndef UTIL_JOBSYS_H
#define UTIL_JOBSYS_H
#include <pthread.h>
#include "jobsys_types.h"
#include "jobsys_deque.h"
#include "jobsys_overflow.h"
#include "jobsys_fence.h"
#include "jobsys_chain.h"
#include "jobsys_trace.h"
typedef struct {
    jat_u64 submitted;     // jobs handed to submit()
    jat_u64 executed;      // jobs actually run to completion
    jat_u64 stolen;        // jobs a worker took from someone else's deque
    jat_u64 overflowed;    // jobs that spilled to the shared queue
    jat_u64 main_ran;      // main-only jobs drained on the main thread
} jobsys_stats;
typedef struct {
    jobsys_pool   *pool;
    jobsys_deque   deque;
    pthread_t      thread;
    int            id;          // 0..nworkers-1
    uint64_t       rng;         // per-worker prng for randomised victim picking
#ifdef JOBSYS_TRACE
    jobsys_trace   trace;       // recent job timings, debug builds only
#endif
} jobsys_worker;
struct jobsys_pool {
    jobsys_worker     workers[JOBSYS_MAX_WORKERS];
    int               nworkers;

    jobsys_overflow   overflow;
    jobsys_fence_pool fences;
    jobsys_chain_pool chains;
    jobsys_stats      stats;

    // sleeping workers park on this when theres no work, instead of spinning a
    // core flat. a submit broadcasts to wake them. classic condvar handshake.
    pthread_mutex_t   sleep_mtx;
    pthread_cond_t    sleep_cv;
    jat_i32           waiters;     // how many workers are parked right now

    jat_i32           running;     // 1 while workers should keep looping
    jat_u32           rr;          // round-robin cursor for external submits
}
#endif
