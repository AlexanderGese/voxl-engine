#ifndef WORLD_BIOME_DECO_H
#define WORLD_BIOME_DECO_H

#include <stdint.h>
#include "biome_column.h"
#include "../block.h"

// surface decoration: trees and ground cover. this decides *what* and *where*,
// not how to splat it into a chunk; we emit placements into a small buffer the
// worldgen driver drains. keeps this module free of chunk dependencies so it
// stays unit-testable on its own.

typedef enum {
    BIOME_DECO_NONE = 0,
    BIOME_DECO_GRASS,
    BIOME_DECO_BUSH,
    BIOME_DECO_DEADBUSH,
    BIOME_DECO_FLOWER,
    BIOME_DECO_CACTUS,
    BIOME_DECO_TREE_OAK,
    BIOME_DECO_TREE_PINE,
    BIOME_DECO_TREE_PALM,
    BIOME_DECO_KIND_COUNT
} biome_deco_kind;

// one block to place. the buffer is a fixed array; small features only.
typedef struct { int x, y, z; block_id block; } biome_place;

#define BIOME_PLACE_MAX  96
typedef struct {
    biome_place items[BIOME_PLACE_MAX];
    int count;
} biome_place_buf;

void biome_place_reset(biome_place_buf *buf);
int  biome_place_add(biome_place_buf *buf, int x, int y, int z, block_id b);

// decide the deco kind for a column. trees gated by tree_density, ground cover
// by grass_density, both rolled from per-column hashes. border columns get
// trees suppressed so forests dont leak trunks into the desert next door.
biome_deco_kind biome_deco_pick(const biome_column *col);

// expand the picked deco into block placements. returns how many were added.
int biome_deco_emit(biome_place_buf *buf, const biome_column *col);

#endif
