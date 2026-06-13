#include "waterflow.h"
#include "block.h"
#include "../config.h"

#define WATER_MAX_SPREAD 6

void waterflow_mark(world *w, int wx, int wy, int wz) {
    (void)w; (void)wx; (void)wy; (void)wz;
    // no-op for now. the step function just walks all water blocks
    // each tick. TODO: use a queue of recently-changed positions.
}

void waterflow_step(world *w) {
    // walk every loaded chunk, spreading water one step per call
    for (world_node *n = w->head; n; n = n->next) {
        chunk *c = n->c;
        for (int y = 1; y < CHUNK_SIZE_Y; y++) {
            for (int z = 0; z < CHUNK_SIZE_Z; z++) {
                for (int x = 0; x < CHUNK_SIZE_X; x++) {
                    if (chunk_get_block(c, x, y, z) != BLOCK_WATER) continue;

                    // fall
                    if (y > 0 && chunk_get_block(c, x, y - 1, z) == BLOCK_AIR) {
                        chunk_set_block(c, x, y - 1, z, BLOCK_WATER);
                        continue;
                    }

                    // sideways (within bounds only, cross-chunk flow skipped)
                    static const int dx[4] = { 1,-1, 0, 0};
                    static const int dz[4] = { 0, 0, 1,-1};
                    for (int i = 0; i < 4; i++) {
                        int nx = x + dx[i];
                        int nz = z + dz[i];
                        if (nx < 0 || nx >= CHUNK_SIZE_X) continue;
                        if (nz < 0 || nz >= CHUNK_SIZE_Z) continue;
                        if (chunk_get_block(c, nx, y, nz) == BLOCK_AIR) {
                            chunk_set_block(c, nx, y, nz, BLOCK_WATER);
                        }
                    }
                }
            }
        }
    }
}
