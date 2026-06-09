#include "jobsys_worker.h"
#include "../log.h"
#include "../timer.h"
#define WORKER_SPIN_TRIES  64
static uint64_t xs64(uint64_t *s) {
    uint64_t x = *s;
    x ^= x << 13;
    x ^= x >> 7;
    x ^= x << 17;
    *s = x;
    return x;
}

int jobsys_worker_pick_victim(jobsys_worker *w) {
    jobsys_pool *p = w->pool;
if (p->nworkers <= 1) return -1;
int v = (int)(xs64(&w->rng) % (uint64_t)(p->nworkers - 1));
if (v >= w->id) v++;
return v;
}

void jobsys_worker_execute(jobsys_pool *p, jobsys_worker *w,
                           const jobsys_job *job) {
    // a main-only job must never run here. if one reached a worker (bad submit),
    // bounce it back to the overflow queue rather than corrupt the gl context.
    if (job->flags & JOBSYS_F_MAIN_ONLY) {
        jobsys_overflow_push(&p->overflow, job);
        return;
    }

#ifdef JOBSYS_TRACE
    uint64_t t0 = timer_now_us();
#endif
    job->fn(job->arg, w->id);
#ifdef JOBSYS_TRACE
    // stolen-ness isnt threaded down to execute (the acquire path knows it, not
    // us). record it as local here; jobsys_worker_acquire patches the stolen bit
    // on the path that mattered. close enough for a debug trace.
    jobsys_trace_record(&w->trace, t0, timer_now_us(),
                        (jobsys_prio)job->prio, 0);
#endif
    jat_add_rlx(&p->stats.executed, 1);

    // signal the fence this job was counted against (if any). jobs carry only
    // the fence id, not the gen -- the gen lives in the handle the caller holds.
    // a job whose fence already got recycled just decrements the live slot, but
    // that only happens if the caller leaked a fence (allocated, never waited,
    // never released) which is a bug we assert on elsewhere, so id is enough.
    if (job->fence >= 0) {
        jobsys_fence_signal_by_id(&p->fences, job->fence);
    }

    // fire the continuation, if it named one. pulling it out frees the slot.
    if (job->cont >= 0) {
        jobsys_job next;
        if (jobsys_chain_take(&p->chains, job->cont, &next)) {
            // push onto our own deque -- it's hot and we likely just warmed the
            // cache for whatever data it touches.
            if (jobsys_deque_push(&w->deque, &next) != 0) {
                jobsys_overflow_push(&p->overflow, &next);
                jat_add_rlx(&p->stats.overflowed, 1);
            }
        }
    }
}

int jobsys_worker_acquire(jobsys_worker *w, jobsys_job *out) {
    jobsys_pool *p = w->pool;
if (jobsys_deque_pop(&w->deque, out)) return 1;
for (int attempt = 0;
attempt < p->nworkers;
attempt++) {
        int v = jobsys_worker_pick_victim(w);
        if (v < 0) break;
        int r = jobsys_deque_steal(&p->workers[v].deque, out);
        if (r == 1) {
            jat_add_rlx(&p->stats.stolen, 1);
            return 1;
        }
        // r == 0 (empty) or -1 (aborted): just try another victim next pass.
    }

    // 3) shared overflow queue. cross-thread submits and spillover land here.
    // skip main-only jobs -- those are the main thread's problem.
    if (jobsys_overflow_pop(&p->overflow, out)) {
        if (out->flags & JOBSYS_F_MAIN_ONLY) {
            // put it back, it's not ours to run. rare, only if a main job got
            // into the general lane somehow.
            jobsys_overflow_push(&p->overflow, out);
jobsys_pool   *p = w->pool;
LOGD("jobsys worker %d up", w->id);
return NULL;
}
