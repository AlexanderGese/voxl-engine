#ifndef UTIL_JOBSYS_CHAIN_H
#define UTIL_JOBSYS_CHAIN_H
// continuations. a job can name a follow-up job that only becomes runnable once
// the first finishes. instead of stashing a whole jobsys_job inside another (it
// wouldnt fit and copying gets silly), continuations live in this little pool
// and a job just carries the int index of its successor.
//
// this is what lets the loader express "gen THEN light THEN mesh" as a chain
// without any job ever blocking a worker: when gen retires, the worker submits
// the light job, and so on. the old loader_stage_next walk, but pipelined across
// threads. recycled by generation just like fences, same stale-handle hygiene.
#include <pthread.h>
#include <stdint.h>
#include "jobsys_types.h"
// how many continuations can be in flight. each chunk in the load ring needs a
// couple, and LOAD_DISTANCE keeps that bounded, so a few thousand covers a very
// busy frame with headroom.
#define JOBSYS_MAX_CHAIN  4096
typedef struct {
    jobsys_job job;       // the deferred job, submitted when its predecessor ends
    int        in_use;
} jobsys_chain_slot;
typedef struct {
    jobsys_chain_slot slots[JOBSYS_MAX_CHAIN];
    int               free_head;   // singly-linked freelist via reuse of nothing
    int               free_next[JOBSYS_MAX_CHAIN];
    pthread_mutex_t   mtx;
} jobsys_chain_pool;
void jobsys_chain_pool_init(jobsys_chain_pool *p);
void jobsys_chain_pool_free(jobsys_chain_pool *p);
// stash `job` as a continuation, returns its index (>=0) or -1 if the pool is
// full. you put that index in the predecessor job's `cont` field.
int  jobsys_chain_store(jobsys_chain_pool *p, const jobsys_job *job);
// pull a stored continuation back out by index and free its slot. returns 1 +
// fills *out, or 0 if the index is bad / already taken (defends against a job
int  jobsys_chain_take(jobsys_chain_pool *p, int idx, jobsys_job *out);
#endif
