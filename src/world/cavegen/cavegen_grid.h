#ifndef WORLD_CAVEGEN_GRID_H
#define WORLD_CAVEGEN_GRID_H

#include <stdint.h>
#include "cavegen_types.h"

// the scratch grid the whole generator scribbles on. one byte per cell holding
// a cavegen_cell value. allocated once, reset per region. its big (about
// CHUNK_SIZE_X+2 * 128 * CHUNK_SIZE_Z+2 bytes) but it lives on the heap once and
// gets reused, so no churn.

typedef struct {
    uint8_t      *cells;       // CAVEGEN_CELLS bytes
    cavegen_origin origin;
    int           heightmap[CAVEGEN_DIM_X * CAVEGEN_DIM_Z]; // surface y per col
} cavegen_grid;

cavegen_grid *cavegen_grid_create(void);
void          cavegen_grid_destroy(cavegen_grid *g);

// wipe to all-solid and reset the origin. heightmap is left for the caller to
// fill via cavegen_grid_set_height.
void cavegen_grid_reset(cavegen_grid *g, cavegen_origin origin);

// flat index. caller is expected to stay in bounds; cavegen_grid_in_bounds
// exists for the cases where you cant be bothered to check by hand.
int  cavegen_grid_idx(int x, int y, int z);
int  cavegen_grid_in_bounds(int x, int y, int z);

uint8_t cavegen_grid_get(const cavegen_grid *g, int x, int y, int z);
void    cavegen_grid_set(cavegen_grid *g, int x, int y, int z, uint8_t v);

// "is this cell open space of any kind". worms/flooded/air all count.
int  cavegen_cell_is_open(uint8_t v);

// surface heightmap access (per x,z column, pad-inclusive coords)
void cavegen_grid_set_height(cavegen_grid *g, int x, int z, int y);
int  cavegen_grid_height(const cavegen_grid *g, int x, int z);

// map a cell coord to a world coord and back. the pad shifts everything.
void cavegen_cell_to_world(const cavegen_grid *g, int x, int y, int z,
                           int *wx, int *wy, int *wz);

// count cells in a given state. handy for stats and tests.
int  cavegen_grid_count(const cavegen_grid *g, uint8_t state);

#endif
