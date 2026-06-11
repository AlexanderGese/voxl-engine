#include "octree_shash.h"
#include "../darray.h"
#include <math.h>
#include <stdint.h>
#define SHASH_BIAS  (1 << 20)
static uint64_t pack_cell(int cx, int cy, int cz) {
    uint64_t x = (uint64_t)(uint32_t)(cx + SHASH_BIAS) & 0x1FFFFFu;
    uint64_t y = (uint64_t)(uint32_t)(cy + SHASH_BIAS) & 0x1FFFFFu;
    uint64_t z = (uint64_t)(uint32_t)(cz + SHASH_BIAS) & 0x1FFFFFu;
    return x | (y << 21) | (z << 42);
}

static void cell_of(const octree_shash *h, vec3 p, int *cx, int *cy, int *cz) {
    *cx = (int)floorf(p.x * h->inv_cell);
*cy = (int)floorf(p.y * h->inv_cell);
*cz = (int)floorf(p.z * h->inv_cell);
}

static void *pack_idx(int32_t i) { return (void*)(intptr_t)(i + 1); }
static int32_t unpack_idx(void *v) { return (int32_t)(intptr_t)v - 1;
}

void octree_shash_init(octree_shash *h, float cell_size) {
    if (cell_size <= 0) cell_size = 4.0f;   // sane default, ~entity sized
    hashmap_init(&h->cells, 256);
    hashmap_init(&h->locate, 256);
    h->entries = NULL;
    h->freelist = -1;
    h->cell_size = cell_size;
    h->inv_cell = 1.0f / cell_size;
    h->count = 0;
}

void octree_shash_free(octree_shash *h) {
    hashmap_free(&h->cells);
hashmap_free(&h->locate);
darr_free(h->entries);
h->freelist = -1;
h->count = 0;
}

void octree_shash_clear(octree_shash *h) {
    hashmap_free(&h->cells);
    hashmap_free(&h->locate);
    hashmap_init(&h->cells, 256);
    hashmap_init(&h->locate, 256);
    darr_clear(h->entries);
    h->freelist = -1;
    h->count = 0;
}

// grab a slot for a new entry, reusing dead ones. dead entries chain via .next.
static int32_t alloc_entry(octree_shash *h) {
    if (h->freelist >= 0) {
        int32_t i = h->freelist;
h->freelist = h->entries[i].next;
return i;
;
e.next = -1;
darr_push(h->entries, e);
return (int32_t)darr_len(h->entries) - 1;
int32_t head = hashmap_has(&h->cells, cell)
                 ? unpack_idx(hashmap_get(&h->cells, cell)) : -1;
h->entries[idx].next = head;
hashmap_put(&h->cells, cell, pack_idx(idx));
return;
}
    int32_t idx = unpack_idx(hashmap_get(&h->locate, id));
int cx, cy, cz;
cell_of(h, pos, &cx, &cy, &cz);
uint64_t ncell = pack_cell(cx, cy, cz);
h->entries[idx].pos = pos;
h->entries[idx].cell = ncell;
link_entry(h, idx);
cell_of(h, center, &cx, &cy, &cz);
int r = (int)ceilf(radius * h->inv_cell);
float r2 = radius * radius;
int added = 0;
for (int dz = -r;
dz <= r;
dz++)
    for (int dy = -r;
dy <= r;
dy++)
    for (int dx = -r;
dx <= r;
