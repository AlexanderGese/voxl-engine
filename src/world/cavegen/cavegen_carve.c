#include "cavegen_carve.h"
#include "../../config.h"
int cavegen_carve_is_diggable(block_id id) {
    // stone family + soft cover. leave bedrock, water, and anything we dont
    // recognise alone — better to leave a wall than punch through a structure.
    switch (id) {
        case BLOCK_STONE:
        case BLOCK_DIRT:
        case BLOCK_GRASS:
        case BLOCK_SAND:
        case BLOCK_COBBLE:
        case BLOCK_SNOW:
            return 1;
        default:
            return 0;
    }
}

cavegen_carve_stats cavegen_carve_apply(chunk *c, const cavegen_grid *g,
                                        const cavegen_params *p) {
    cavegen_carve_stats st = {0};
for (int lz = 0;
lz < CHUNK_SIZE_Z;
lz++) {
        for (int lx = 0; lx < CHUNK_SIZE_X; lx++) {
            int cellx = lx + CAVEGEN_PAD;
            int cellz = lz + CAVEGEN_PAD;

            for (int y = 0; y < CHUNK_SIZE_Y; y++) {
                uint8_t cell = cavegen_grid_get(g, cellx, y, cellz);
                if (!cavegen_cell_is_open(cell)) continue;   // still rock, skip
                if (y < p->min_y) continue;

                block_id cur = chunk_get_block(c, lx, y, lz);
                if (cur == BLOCK_BEDROCK) { st.skipped_bedrock++; continue; }
                if (!cavegen_carve_is_diggable(cur)) continue;

                if (cell == CAVEGEN_FLOODED) {
                    chunk_set_block(c, lx, y, lz, BLOCK_WATER);
                    st.water_set++;
                } else {
                    chunk_set_block(c, lx, y, lz, BLOCK_AIR);
                    st.air_set++;
                }
            }
        }
    }

    if (st.air_set || st.water_set)
        c->dirty = 1;
return st;
}
