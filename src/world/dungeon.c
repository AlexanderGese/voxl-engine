#include "dungeon.h"
#include "block.h"
#include "container.h"
#include "../math/rng.h"
#include "../config.h"

void dungeon_maybe_place(chunk *c, unsigned seed) {
    rng r;
    rng_init(&r, seed ^ ((uint64_t)c->cx * 11400714819323198485ull) ^ (uint64_t)c->cz);
    if (rng_float01(&r) > 0.02f) return;

    int lx = rng_range(&r, 2, CHUNK_SIZE_X - 7);
    int lz = rng_range(&r, 2, CHUNK_SIZE_Z - 7);
    int ly = rng_range(&r, 8, 28);

    // hollow cobble box 5x4x5
    for (int dz = 0; dz < 5; dz++) {
        for (int dx = 0; dx < 5; dx++) {
            for (int dy = 0; dy < 4; dy++) {
                int x = lx + dx, y = ly + dy, z = lz + dz;
                int is_wall = (dx == 0 || dx == 4 || dz == 0 || dz == 4 || dy == 0 || dy == 3);
                chunk_set_block(c, x, y, z, is_wall ? BLOCK_COBBLE : BLOCK_AIR);
            }
        }
    }
    // torch in corner
    chunk_set_block(c, lx + 1, ly + 1, lz + 1, BLOCK_TORCH);

    // register a chest-equivalent container at worldspace center. we dont
    // have a chest block yet, so this is stored dangling in the side map.
    int wx = c->cx * CHUNK_SIZE_X + lx + 2;
    int wz = c->cz * CHUNK_SIZE_Z + lz + 2;
    int wy = ly + 1;
    container *box = container_get_or_create(wx, wy, wz);
    container_add(box, BLOCK_COBBLE, 16);
    container_add(box, BLOCK_BRICK, 8);
    container_add(box, BLOCK_TORCH, 4);
}
