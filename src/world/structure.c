#include "structure.h"
#include "block.h"
#include "worldgen.h"
#include "../math/rng.h"
#include "../config.h"

static void place_hut(chunk *c, int lx, int ly, int lz) {
    // 5x5 footprint, 3 high. planks walls, door slot, roof.
    if (lx < 1 || lx + 5 >= CHUNK_SIZE_X) return;
    if (lz < 1 || lz + 5 >= CHUNK_SIZE_Z) return;
    if (ly + 4 >= CHUNK_SIZE_Y) return;

    for (int dz = 0; dz < 5; dz++) {
        for (int dx = 0; dx < 5; dx++) {
            chunk_set_block(c, lx + dx, ly, lz + dz, BLOCK_PLANKS);
            chunk_set_block(c, lx + dx, ly + 3, lz + dz, BLOCK_PLANKS);
        }
    }
    for (int h = 1; h <= 2; h++) {
        for (int k = 0; k < 5; k++) {
            chunk_set_block(c, lx + k, ly + h, lz,     BLOCK_PLANKS);
            chunk_set_block(c, lx + k, ly + h, lz + 4, BLOCK_PLANKS);
            chunk_set_block(c, lx,     ly + h, lz + k, BLOCK_PLANKS);
            chunk_set_block(c, lx + 4, ly + h, lz + k, BLOCK_PLANKS);
        }
    }
    // door hole
    chunk_set_block(c, lx + 2, ly + 1, lz, BLOCK_AIR);
    chunk_set_block(c, lx + 2, ly + 2, lz, BLOCK_AIR);
    // torch inside
    chunk_set_block(c, lx + 2, ly + 2, lz + 2, BLOCK_TORCH);
}

void structure_maybe_place(chunk *c, unsigned seed) {
    rng r;
    rng_init(&r, seed ^ ((uint64_t)c->cx * 49979687u) ^ ((uint64_t)c->cz * 86028121u));
    if (rng_float01(&r) > 0.05f) return;   // ~5% of chunks get one

    int lx = rng_range(&r, 2, CHUNK_SIZE_X - 6);
    int lz = rng_range(&r, 2, CHUNK_SIZE_Z - 6);
    int wx = c->cx * CHUNK_SIZE_X + lx + 2;
    int wz = c->cz * CHUNK_SIZE_Z + lz + 2;
    int h  = worldgen_height_at(wx, wz, seed);
    if (h < WORLD_SEA_LEVEL + 2) return;
    place_hut(c, lx, h + 1, lz);
}
