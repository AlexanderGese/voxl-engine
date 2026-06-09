#ifndef UTIL_JOBSYS_ATOMIC_H
#define UTIL_JOBSYS_ATOMIC_H

// thin shim over c11 atomics so the rest of jobsys doesnt have to type
// memory_order_* everywhere. i went back and forth on rolling our own with
// __atomic builtins but stdatomic is fine on every compiler we target and the
// builtins were just noise. if msvc ever shows up this is the one file to port.

#include <stdatomic.h>
#include <stdint.h>

typedef _Atomic(int64_t)  jat_i64;
typedef _Atomic(uint64_t) jat_u64;
typedef _Atomic(int32_t)  jat_i32;
typedef _Atomic(uint32_t) jat_u32;
typedef _Atomic(void*)    jat_ptr;

// relaxed: no ordering, just atomicity. use for counters nobody waits on.
#define jat_load_rlx(p)        atomic_load_explicit((p), memory_order_relaxed)
#define jat_store_rlx(p, v)    atomic_store_explicit((p), (v), memory_order_relaxed)
#define jat_add_rlx(p, v)      atomic_fetch_add_explicit((p), (v), memory_order_relaxed)
#define jat_sub_rlx(p, v)      atomic_fetch_sub_explicit((p), (v), memory_order_relaxed)

// acquire/release: the ones that actually matter for the deque and fences.
#define jat_load_acq(p)        atomic_load_explicit((p), memory_order_acquire)
#define jat_store_rel(p, v)    atomic_store_explicit((p), (v), memory_order_release)
#define jat_sub_rel(p, v)      atomic_fetch_sub_explicit((p), (v), memory_order_release)
#define jat_add_rel(p, v)      atomic_fetch_add_explicit((p), (v), memory_order_release)

// seq_cst cas, the heavy hammer. chase-lev needs the bottom/top race resolved
// with a full fence so we dont get cute here.
static inline int jat_cas_strong_i64(jat_i64 *p, int64_t expect, int64_t want) {
    return atomic_compare_exchange_strong_explicit(
        p, &expect, want, memory_order_seq_cst, memory_order_relaxed);
}

static inline int jat_cas_weak_u32(jat_u32 *p, uint32_t expect, uint32_t want) {
    return atomic_compare_exchange_weak_explicit(
        p, &expect, want, memory_order_acq_rel, memory_order_relaxed);
}

// full barrier. mostly to keep the steal path honest on weak memory archs.
#define jat_fence_seq()        atomic_thread_fence(memory_order_seq_cst)

#endif
