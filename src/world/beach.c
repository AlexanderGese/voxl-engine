#include "beach.h"
#include "block.h"
#include "../config.h"

void beach_smooth(chunk *c, unsigned seed) {
    (void)seed;
    for (int lz = 0; lz < CHUNK_SIZE_Z; lz++) {
        for (int lx = 0; lx < CHUNK_SIZE_X; lx++) {
            for (int y = WORLD_SEA_LEVEL - 1; y <= WORLD_SEA_LEVEL + 2; y++) {
                block_id id = chunk_get_block(c, lx, y, lz);
                if (id == BLOCK_GRASS || id == BLOCK_DIRT) {
                    chunk_set_block(c, lx, y, lz, BLOCK_SAND);
                }
            }
        }
    }
}
