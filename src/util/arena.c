#include "arena.h"

#include <stdlib.h>
#include <string.h>

void arena_init(arena *a, size_t page_size) {
    if (page_size < 4096) page_size = 4096;
    a->head = NULL;
    a->page_size = page_size;
}

static arena_page *new_page(size_t sz) {
    arena_page *p = malloc(sizeof(arena_page) + sz);
    if (!p) return NULL;
    p->next = NULL;
    p->used = 0;
    p->cap  = sz;
    return p;
}

void *arena_alloc(arena *a, size_t n) {
    return arena_alloc_aligned(a, n, 8);
}

void *arena_alloc_aligned(arena *a, size_t n, size_t align) {
    arena_page *p = a->head;
    while (p) {
        size_t aligned = (p->used + align - 1) & ~(align - 1);
        if (aligned + n <= p->cap) {
            void *out = p->data + aligned;
            p->used = aligned + n;
            return out;
        }
        p = p->next;
    }
    size_t sz = n > a->page_size ? n : a->page_size;
    arena_page *np = new_page(sz);
    if (!np) return NULL;
    np->next = a->head;
    a->head = np;
    np->used = n;
    return np->data;
}

void arena_reset(arena *a) {
    for (arena_page *p = a->head; p; p = p->next) p->used = 0;
}

void arena_free(arena *a) {
    arena_page *p = a->head;
    while (p) {
        arena_page *nx = p->next;
        free(p);
        p = nx;
    }
    a->head = NULL;
}
