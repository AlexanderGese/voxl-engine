#ifndef UTIL_ARENA_H
#define UTIL_ARENA_H

// bump allocator. pages allocated via malloc, freed all at once.
// great for per-frame scratch buffers.

#include <stddef.h>

typedef struct arena_page {
    struct arena_page *next;
    size_t used;
    size_t cap;
    char   data[];
} arena_page;

typedef struct {
    arena_page *head;
    size_t      page_size;
} arena;

void  arena_init(arena *a, size_t page_size);
void *arena_alloc(arena *a, size_t n);
void *arena_alloc_aligned(arena *a, size_t n, size_t align);
void  arena_reset(arena *a);   // keeps pages, zero used
void  arena_free(arena *a);    // frees all pages

#endif
