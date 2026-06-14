#include "biome_surface.h"
#include "biome_table.h"
#include "biome_noise.h"

// material stack from the top down:
// y > height           -> water (if below sea) or snow cap or air
// y == height          -> surface block (swapped if submerged / snowy)
// height-1 .. -depth    -> subsurface band
// below that            -> filler (stone) down to y==0 bedrock
//
// the little noise wobble on the subsurface depth keeps dirt bands from being
// suspiciously flat where two biomes meet.

block_id biome_surface_block(const biome_column *col, int y) {
    const biome_def *d = biome_table_get(col->biome);
    int top = col->height;
    int sea = col->sea_level;

    if (y < 0) return BLOCK_AIR;
    if (y == 0) return BLOCK_BEDROCK;

    // above the terrain
    if (y > top) {
        if (y <= sea) return BLOCK_WATER;   // ocean / lake column
        // snow layer sits one above the ground when cold + above water
        if (y == top + 1 && d->snow_capped &&
            col->climate.temperature < 0.30f && top >= sea) {
            return BLOCK_SNOW;
        }
        return BLOCK_AIR;
    }

    int depth = top - y;   // 0 at surface

    // wobble the subsurface band thickness a touch
    int wob = (int)(biome_hash01(col->wx, col->wz, col->seed ^ 0x50FAu) * 2.0f);
    int sub = d->subsurface_depth + wob;

    if (depth == 0) {
        // the top block, with submerged + frozen swaps
        if (top < sea) return d->underwater;          // beach/ocean floor
        if (top == sea && d->kind == BIOME_KIND_TUNDRA) return BLOCK_ICE;
        if (d->snow_capped && col->climate.temperature < 0.22f && top >= sea)
            return BLOCK_SNOW;
        return d->surface;
    }
    if (depth <= sub) {
        // submerged surfaces dont get grassy dirt, they stay sandy/stony
        if (top < sea && d->subsurface == BLOCK_DIRT) return BLOCK_SAND;
        return d->subsurface;
    }
    return d->filler;
}

block_id biome_surface_top(const biome_column *col) {
    if (col->height < col->sea_level) return BLOCK_WATER;
    return biome_surface_block(col, col->height);
}

int biome_surface_fill(const biome_column *col, int y0, int y1, block_id *out) {
    if (!out || y1 <= y0) return 0;
    int n = 0;
    for (int y = y0; y < y1; y++) {
        out[n++] = biome_surface_block(col, y);
    }
    return n;
}
