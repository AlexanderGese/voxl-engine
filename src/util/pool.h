#ifndef UTIL_POOL_H
#define UTIL_POOL_H

// fixed-size-element pool with free list. O(1) alloc and free.
// used for particles, maybe entities later.

#include <stddef.h>

typedef struct pool_slot {
    struct pool_slot *next;
} pool_slot;

typedef struct {
    char      *mem;
    pool_slot *freelist;
    size_t     elem_size;
    size_t     count;
    size_t     used;
} pool;

int   pool_init(pool *p, size_t elem_size, size_t count);
void  pool_free(pool *p);
void *pool_alloc(pool *p);
void  pool_release(pool *p, void *ptr);

#endif
