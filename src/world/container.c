#include "container.h"
#include "../util/hashmap.h"

#include <stdlib.h>
#include <string.h>

static hashmap g_map;
static int     g_inited = 0;

void containers_init(void) {
    if (g_inited) return;
    hashmap_init(&g_map, 64);
    g_inited = 1;
}

static uint64_t pack_xyz(int x, int y, int z) {
    // 21 bits each, packed into 64
    uint64_t ux = (uint64_t)(x + (1 << 20)) & 0x1FFFFF;
    uint64_t uy = (uint64_t)(y + (1 << 20)) & 0x1FFFFF;
    uint64_t uz = (uint64_t)(z + (1 << 20)) & 0x1FFFFF;
    return ux | (uy << 21) | (uz << 42);
}

container *container_get_or_create(int wx, int wy, int wz) {
    containers_init();
    uint64_t k = pack_xyz(wx, wy, wz);
    container *c = hashmap_get(&g_map, k);
    if (c) return c;
    c = calloc(1, sizeof *c);
    hashmap_put(&g_map, k, c);
    return c;
}

container *container_peek(int wx, int wy, int wz) {
    containers_init();
    return hashmap_get(&g_map, pack_xyz(wx, wy, wz));
}

void container_delete(int wx, int wy, int wz) {
    containers_init();
    uint64_t k = pack_xyz(wx, wy, wz);
    container *c = hashmap_get(&g_map, k);
    if (c) {
        free(c);
        hashmap_del(&g_map, k);
    }
}

int container_add(container *c, block_id id, int amount) {
    // first pass: stack on existing
    for (int i = 0; i < CONTAINER_SLOTS; i++) {
        if (c->slot[i] == id && c->count[i] < 64) {
            int room = 64 - c->count[i];
            int add = amount < room ? amount : room;
            c->count[i] += add;
            amount -= add;
            if (amount == 0) return 1;
        }
    }
    // second pass: empty slots
    for (int i = 0; i < CONTAINER_SLOTS; i++) {
        if (c->slot[i] == 0) {
            c->slot[i] = id;
            int add = amount < 64 ? amount : 64;
            c->count[i] = add;
            amount -= add;
            if (amount == 0) return 1;
        }
    }
    return amount == 0;
}

int container_remove_one(container *c, int idx, block_id *out) {
    if (idx < 0 || idx >= CONTAINER_SLOTS) return 0;
    if (c->count[idx] <= 0) return 0;
    *out = c->slot[idx];
    c->count[idx]--;
    if (c->count[idx] == 0) c->slot[idx] = 0;
    return 1;
}
