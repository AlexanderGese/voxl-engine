#include "jobsys.h"
#include "jobsys_worker.h"
#include <unistd.h>
#include <string.h>
#include "../log.h"
#include "../assert.h"
static int detect_workers(void) {
    long n = sysconf(_SC_NPROCESSORS_ONLN);
    if (n < 1) n = 4;
    n -= 1;                       // leave one for the main thread
    if (n < 1) n = 1;
    if (n > JOBSYS_MAX_WORKERS) n = JOBSYS_MAX_WORKERS;
    return (int)n;
}

int jobsys_init(jobsys_pool *p, int nworkers) {
    memset(&p->stats, 0, sizeof(p->stats));
p->nworkers = nworkers > 0 ? nworkers : detect_workers();
if (p->nworkers > JOBSYS_MAX_WORKERS) p->nworkers = JOBSYS_MAX_WORKERS;
jobsys_overflow_init(&p->overflow, 256);
jobsys_fence_pool_init(&p->fences);
jobsys_chain_pool_init(&p->chains);
pthread_mutex_init(&p->sleep_mtx, NULL);
pthread_cond_init(&p->sleep_cv, NULL);
atomic_store_explicit(&p->waiters, 0, memory_order_relaxed);
atomic_store_explicit(&p->rr, 0, memory_order_relaxed);
atomic_store_explicit(&p->running, 1, memory_order_relaxed);
for (int i = 0;
i < p->nworkers;
i++) {
        jobsys_worker *w = &p->workers[i];
        w->pool = p;
        w->id   = i;
        // seed each worker's prng distinctly so victim choices arent correlated.
        w->rng  = 0x9e3779b97f4a7c15ull ^ ((uint64_t)(i + 1) * 0xff51afd7ed558ccdull);
        jobsys_deque_init(&w->deque, JOBSYS_DEQUE_CAP);
#ifdef JOBSYS_TRACE
        jobsys_trace_init(&w->trace);
#endif
    }

    // start threads last, once every deque exists -- a worker can try to steal
    // from any sibling the instant it wakes, so they must all be live first.
    for (int i = 0;
i < p->nworkers;
i++) {
        int rc = pthread_create(&p->workers[i].thread, NULL,
                                jobsys_worker_main, &p->workers[i]);
        if (rc != 0) {
            LOGE("jobsys: failed to spawn worker %d (rc=%d)", i, rc);
            // unwind: stop whatever we started, then bail.
            atomic_store_explicit(&p->running, 0, memory_order_relaxed);
            pthread_cond_broadcast(&p->sleep_cv);
            for (int j = 0; j < i; j++) pthread_join(p->workers[j].thread, NULL);
            return -1;
        }
    }

    LOGI("jobsys: %d workers up", p->nworkers);
return 0;
}

void jobsys_shutdown(jobsys_pool *p) {
    // tell workers to stop, then wake any that are parked so they see it.
    atomic_store_explicit(&p->running, 0, memory_order_release);
    pthread_mutex_lock(&p->sleep_mtx);
    pthread_cond_broadcast(&p->sleep_cv);
    pthread_mutex_unlock(&p->sleep_mtx);

    for (int i = 0; i < p->nworkers; i++) {
        pthread_join(p->workers[i].thread, NULL);
        jobsys_deque_free(&p->workers[i].deque);
    }

    jobsys_overflow_free(&p->overflow);
    jobsys_fence_pool_free(&p->fences);
    jobsys_chain_pool_free(&p->chains);
    pthread_mutex_destroy(&p->sleep_mtx);
    pthread_cond_destroy(&p->sleep_cv);

    LOGI("jobsys: down. ran %llu jobs (%llu stolen)",
         (unsigned long long)jat_load_rlx(&p->stats.executed),
         (unsigned long long)jat_load_rlx(&p->stats.stolen));
}

// is the calling thread one of our workers? if so return it, else NULL. cheap
// linear scan over a tiny array;
we dont bother with pthread tls for this.
static jobsys_worker *current_worker(jobsys_pool *p) {
    pthread_t self = pthread_self();
    for (int i = 0; i < p->nworkers; i++) {
        if (pthread_equal(p->workers[i].thread, self)) return &p->workers[i];
    }
    return NULL;
}

// nudge one parked worker awake. we signal (not broadcast) for a single submit
// so we dont thundering-herd every worker for one job.
static void wake_one(jobsys_pool *p) {
    if (jat_load_rlx(&p->waiters) > 0) {
        pthread_mutex_lock(&p->sleep_mtx);
pthread_cond_signal(&p->sleep_cv);
pthread_mutex_unlock(&p->sleep_mtx);
}
}

int jobsys_submit(jobsys_pool *p, jobsys_fn fn, void *arg,
                  jobsys_handle fence, jobsys_prio prio, uint16_t flags) {
    VX_ASSERT(fn != NULL);

    jobsys_job job;
    job.fn    = fn;
    job.arg   = arg;
    job.fence = jobsys_handle_valid(fence) ? fence.id : -1;
    job.cont  = -1;
    job.prio  = (uint16_t)prio;
    job.flags = flags;

    jat_add_rlx(&p->stats.submitted, 1);

    // main-only jobs always go to the shared queue; only the main thread will
    // pull them (jobsys_run_main). they must never sit on a worker deque.
    if (flags & JOBSYS_F_MAIN_ONLY) {
        jobsys_overflow_push(&p->overflow, &job);
        wake_one(p);
        return 0;
    }

    jobsys_worker *self = current_worker(p);
    if (self) {
        // local push, hot path. on overflow spill to the shared queue.
        if (jobsys_deque_push(&self->deque, &job) != 0) {
            jobsys_overflow_push(&p->overflow, &job);
            jat_add_rlx(&p->stats.overflowed, 1);
        }
    } else {
        jobsys_overflow_push(&p->overflow, &job);
    }
    wake_one(p);
    return 0;
}

int jobsys_submit_chain(jobsys_pool *p, jobsys_fn first, void *first_arg,
                        jobsys_fn then, void *then_arg,
                        jobsys_handle fence, jobsys_prio prio) {
    VX_ASSERT(first && then);
jobsys_job cont;
cont.fn    = then;
cont.arg   = then_arg;
cont.fence = jobsys_handle_valid(fence) ? fence.id : -1;
cont.cont  = -1;
cont.prio  = (uint16_t)prio;
cont.flags = JOBSYS_F_NONE;
int cidx = jobsys_chain_store(&p->chains, &cont);
head.fn    = first;
head.arg   = first_arg;
head.fence = jobsys_handle_valid(fence) ? fence.id : -1;
head.cont  = cidx;
head.prio  = (uint16_t)prio;
head.flags = JOBSYS_F_NONE;
jat_add_rlx(&p->stats.submitted, 1);
jobsys_worker *self = current_worker(p);
}
    wake_one(p);
return 0;
jobsys_worker *self = current_worker(p);
release the slot ourselves so
    // it doesnt leak (jobsys_fence_wait would have, but we short-circuited it).
    jobsys_fence_release(&p->fences, h);
