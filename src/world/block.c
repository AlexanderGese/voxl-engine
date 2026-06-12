#include "block.h"

// block registry. hand populated. order matches enum in header.
static const block_info g_blocks[BLOCK_COUNT] = {
    // name          solid opaque emit lum  top  bot  side
    { "air",           0,    0,    0,   0,   0,   0,   0 },
    { "stone",         1,    1,    0,   0,   1,   1,   1 },
    { "dirt",          1,    1,    0,   0,   2,   2,   2 },
    { "grass",         1,    1,    0,   0,   0,   2,   3 },
    { "sand",          1,    1,    0,   0,   4,   4,   4 },
    { "wood",          1,    1,    0,   0,   5,   5,   6 },
    { "leaves",        1,    0,    0,   0,   7,   7,   7 },
    { "planks",        1,    1,    0,   0,   8,   8,   8 },
    { "cobble",        1,    1,    0,   0,   9,   9,   9 },
    { "bedrock",       1,    1,    0,   0,  10,  10,  10 },
    { "glass",         1,    0,    0,   0,  11,  11,  11 },
    { "water",         0,    0,    0,   0,  12,  12,  12 }, // not solid on purpose
    { "torch",         0,    0,    1,  14,  13,  13,  13 },
    { "brick",         1,    1,    0,   0,  14,  14,  14 },
    { "snow",          1,    1,    0,   0,  15,  15,  15 },
    { "ice",           1,    0,    0,   0,  16,  16,  16 },
};

const block_info *block_get(block_id id) {
    if (id >= BLOCK_COUNT) return &g_blocks[BLOCK_AIR];
    return &g_blocks[id];
}

int block_is_solid(block_id id)   { return block_get(id)->solid; }
int block_is_opaque(block_id id)  { return block_get(id)->opaque; }
int block_is_air(block_id id)     { return id == BLOCK_AIR; }
int block_is_transparent(block_id id) { return !block_get(id)->opaque; }

int block_face_tile(block_id id, int face) {
    const block_info *b = block_get(id);
    switch (face) {
    case 2: return b->tile_top;
    case 3: return b->tile_bot;
    default: return b->tile_side;
    }
}
