#include "cavegen_column.h"

#include "../../config.h"
#include "../block.h"

int cavegen_column_surface_y(const chunk *c, int lx, int lz) {
    for (int y = CHUNK_SIZE_Y - 1; y >= 0; y--) {
        block_id id = chunk_get_block(c, lx, y, lz);
        // water doesnt count as surface — we want the rock/ground top so caves
        // under a lake still keep their crust.
        if (id != BLOCK_AIR && id != BLOCK_WATER)
            return y;
    }
    return -1;
}

void cavegen_column_scan_chunk(cavegen_grid *g, const chunk *c) {
    for (int lz = 0; lz < CHUNK_SIZE_Z; lz++) {
        for (int lx = 0; lx < CHUNK_SIZE_X; lx++) {
            int y = cavegen_column_surface_y(c, lx, lz);
            if (y < 0) y = WORLD_SEA_LEVEL;   // open water column, fake a surface
            cavegen_grid_set_height(g, lx + CAVEGEN_PAD, lz + CAVEGEN_PAD, y);
        }
    }
}

void cavegen_column_fill_pad(cavegen_grid *g) {
    // left/right skirt columns copy the nearest interior column.
    for (int z = 0; z < CAVEGEN_DIM_Z; z++) {
        int src_z = z;
        if (src_z < CAVEGEN_PAD) src_z = CAVEGEN_PAD;
        if (src_z >= CAVEGEN_DIM_Z - CAVEGEN_PAD) src_z = CAVEGEN_DIM_Z - CAVEGEN_PAD - 1;

        cavegen_grid_set_height(g, 0, z,
            cavegen_grid_height(g, CAVEGEN_PAD, src_z));
        cavegen_grid_set_height(g, CAVEGEN_DIM_X - 1, z,
            cavegen_grid_height(g, CAVEGEN_DIM_X - 1 - CAVEGEN_PAD, src_z));
    }
    // top/bottom skirt rows.
    for (int x = 0; x < CAVEGEN_DIM_X; x++) {
        int src_x = x;
        if (src_x < CAVEGEN_PAD) src_x = CAVEGEN_PAD;
        if (src_x >= CAVEGEN_DIM_X - CAVEGEN_PAD) src_x = CAVEGEN_DIM_X - CAVEGEN_PAD - 1;

        cavegen_grid_set_height(g, x, 0,
            cavegen_grid_height(g, src_x, CAVEGEN_PAD));
        cavegen_grid_set_height(g, x, CAVEGEN_DIM_Z - 1,
            cavegen_grid_height(g, src_x, CAVEGEN_DIM_Z - 1 - CAVEGEN_PAD));
    }
}
