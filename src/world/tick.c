#include "tick.h"
#include "block.h"
#include "../math/rng.h"
#include "../config.h"

static float acc = 0;
static rng   g_rng;

void tick_init(void) {
    rng_init(&g_rng, 0xbeefcafe);
    acc = 0;
}

static void tick_block(world *w, chunk *c, int lx, int y, int lz) {
    block_id id = chunk_get_block(c, lx, y, lz);

    // grass spreads to adjacent dirt if there's air above
    if (id == BLOCK_GRASS) {
        if (y + 1 < CHUNK_SIZE_Y && chunk_get_block(c, lx, y + 1, lz) != BLOCK_AIR) {
            // grass suffocates and becomes dirt
            chunk_set_block(c, lx, y, lz, BLOCK_DIRT);
            return;
        }
        static const int ox[4] = { 1,-1, 0, 0};
        static const int oz[4] = { 0, 0, 1,-1};
        for (int i = 0; i < 4; i++) {
            int nx = lx + ox[i];
            int nz = lz + oz[i];
            if (nx < 0 || nx >= CHUNK_SIZE_X) continue;
            if (nz < 0 || nz >= CHUNK_SIZE_Z) continue;
            if (chunk_get_block(c, nx, y, nz) == BLOCK_DIRT &&
                (y + 1 >= CHUNK_SIZE_Y || chunk_get_block(c, nx, y + 1, nz) == BLOCK_AIR)) {
                if (rng_float01(&g_rng) < 0.3f) {
                    chunk_set_block(c, nx, y, nz, BLOCK_GRASS);
                }
            }
        }
    }

    // snow melts if adjacent to a torch-lit block
    if (id == BLOCK_SNOW) {
        int bl = chunk_get_blocklight(c, lx, y, lz);
        if (bl > 7 && rng_float01(&g_rng) < 0.4f) {
            chunk_set_block(c, lx, y, lz, BLOCK_AIR);
        }
    }
}

void tick_update(world *w, float dt) {
    acc += dt;
    if (acc < 0.25f) return;
    acc = 0;

    for (world_node *n = w->head; n; n = n->next) {
        chunk *c = n->c;
        // try ticking a handful of random blocks in the chunk
        for (int i = 0; i < 8; i++) {
            int lx = rng_range(&g_rng, 0, CHUNK_SIZE_X - 1);
            int lz = rng_range(&g_rng, 0, CHUNK_SIZE_Z - 1);
            int y  = rng_range(&g_rng, 1, CHUNK_SIZE_Y - 2);
            tick_block(w, c, lx, y, lz);
        }
    }
}
