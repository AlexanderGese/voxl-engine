#ifndef UTIL_JOBSYS_PARALLEL_H
#define UTIL_JOBSYS_PARALLEL_H

// parallel-for. split a [0,count) index range into chunks, submit one job per
// chunk, and hand back a fence you can wait on. this is the ergonomic layer on
// top of raw submit -- the mesher uses it to fan out per-column work across a
// chunk (CHUNK_SIZE_X * CHUNK_SIZE_Z columns), the lighting pass uses it to seed
// sunlight columns in parallel, etc.
//
// the body runs over a sub-range, not a single index, so the per-job overhead
// (fence decrement, deque push) is amortised. you pick the grain; if you pass 0
// we guess one based on worker count so every worker gets a few chunks.

#include "jobsys.h"

// your loop body. runs [begin,end) of the index space. `arg` is your shared
// context (read-only or carefully partitioned -- jobsys gives you no locking).
// `worker` is the executing worker id, or -1 if it ran on the main thread.
typedef void (*jobsys_range_fn)(int begin, int end, void *arg, int worker);

// dispatch [0,count) across the pool in chunks of ~grain. returns a fence handle
// preloaded with the number of chunks; wait on it (or poll) for completion. if
// count==0 it returns a pre-signalled (count 0) fence. grain<=0 = auto.
jobsys_handle jobsys_parallel_for(jobsys_pool *p, int count, int grain,
                                  jobsys_range_fn body, void *arg,
                                  jobsys_prio prio);

// convenience: dispatch AND wait, help-draining the whole time. for the cases
// where you genuinely cant proceed without the result this frame (e.g. a
// synchronous remesh on block edit). returns when every chunk has run.
void jobsys_parallel_for_sync(jobsys_pool *p, int count, int grain,
                              jobsys_range_fn body, void *arg, jobsys_prio prio);

// pick a sane grain for `count` items so each worker gets roughly `per_worker`
// chunks of work to steal-balance against. exposed because the mesher wants to
// reuse the same heuristic for its own hand-rolled splits.
int jobsys_parallel_grain(const jobsys_pool *p, int count, int per_worker);

#endif
