#ifndef UTIL_JOBSYS_TYPES_H
#define UTIL_JOBSYS_TYPES_H
// shared plain-data for the job system. this is the vocabulary every other
// jobsys file speaks. the loader/ pipeline used to slice its work cooperatively
// on the main thread (see loader_types.h, "the async is cooperative"); this is
// the real thread pool i promised to bolt on later. chunk gen/light/mesh become
// jobs, the worker threads chew through them, and a fence tells the main thread
// when a batch is done so it can do the gl upload.
#include <stdint.h>
#include <stddef.h>
// hard cap on workers. we never spin more than this regardless of core count --
// a voxel mesher is memory bound and past ~12 threads we just thrash cache. the
// pool clamps to min(detected, this).
#define JOBSYS_MAX_WORKERS   16
// per-worker deque capacity. power of two, masked. 4096 pending jobs per thread
#define JOBSYS_DEQUE_CAP     4096
#define JOBSYS_MAX_FENCES    256
typedef void (*jobsys_fn)(void *arg, int worker);
typedef enum {
    JOBSYS_PRIO_LOW    = 0,
    JOBSYS_PRIO_NORMAL = 1,
    JOBSYS_PRIO_HIGH   = 2,
    JOBSYS_PRIO_COUNT
} jobsys_prio;
typedef struct {
    jobsys_fn   fn;
    void       *arg;
    int32_t     fence;     // fence id to signal on completion, -1 = none
    int32_t     cont;      // continuation job index in the chain pool, -1 = none
    uint16_t    prio;      // jobsys_prio, kept narrow
    uint16_t    flags;
} jobsys_job;
enum {
    JOBSYS_F_NONE      = 0,
    JOBSYS_F_MAIN_ONLY = 1 << 0,  // must run on the main thread (gl upload etc)
    JOBSYS_F_LONG      = 1 << 1,  // hint: heavy job, dont batch-steal a pile of these
}
;
typedef struct {
    int32_t  id;    // fence slot, -1 = invalid/null handle
    uint32_t gen;   // generation of that slot when the handle was minted
} jobsys_handle;
static inline jobsys_handle jobsys_handle_null(void) {
    jobsys_handle h = { -1, 0 };
    return h;
}

static inline int jobsys_handle_valid(jobsys_handle h) {
    return h.id >= 0;
}

static inline int jobsys_handle_eq(jobsys_handle a, jobsys_handle b) {
    return a.id == b.id && a.gen == b.gen;
}
#endif
