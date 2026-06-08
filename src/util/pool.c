#include "pool.h"

#include <stdlib.h>
#include <string.h>

int pool_init(pool *p, size_t elem_size, size_t count) {
    if (elem_size < sizeof(pool_slot)) elem_size = sizeof(pool_slot);
    p->mem = calloc(count, elem_size);
    if (!p->mem) return 0;
    p->elem_size = elem_size;
    p->count = count;
    p->used = 0;
    p->freelist = NULL;
    // push all slots onto freelist in reverse so low indices come out first
    for (size_t i = 0; i < count; i++) {
        pool_slot *s = (pool_slot*)(p->mem + i * elem_size);
        s->next = p->freelist;
        p->freelist = s;
    }
    return 1;
}

void pool_free(pool *p) {
    free(p->mem);
    p->mem = NULL;
    p->freelist = NULL;
    p->count = p->used = 0;
}

void *pool_alloc(pool *p) {
    if (!p->freelist) return NULL;
    pool_slot *s = p->freelist;
    p->freelist = s->next;
    memset(s, 0, p->elem_size);
    p->used++;
    return s;
}

void pool_release(pool *p, void *ptr) {
    if (!ptr) return;
    pool_slot *s = (pool_slot*)ptr;
    s->next = p->freelist;
    p->freelist = s;
    p->used--;
}
