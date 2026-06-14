#ifndef WORLD_BIOME_WORLD_H
#define WORLD_BIOME_WORLD_H

// umbrella header for the world/biome subsystem. include this from worldgen
// and you get the whole pipeline: climate sampling, biome lookup, neighborhood
// blending, surface material rules, climate tints, decoration, and the column
// cache. nothing here depends on chunks or render, so it can be exercised
// headless.
//
// rough call order from the worldgen driver:
// 1. one biome_cache per worldgen worker (biome_cache_init).
// 2. for each column, biome_cache_get -> resolved biome_column.
// 3. fill the vertical strip with biome_surface_block / _fill.
// 4. roll deco with biome_deco_emit, drain the place buffer into the chunk.
// 5. push grass/water tints from biome_color_* into the mesh vertex colors.

#include "biome_def.h"
#include "biome_climate.h"
#include "biome_noise.h"
#include "biome_table.h"
#include "biome_lookup.h"
#include "biome_height.h"
#include "biome_blend.h"
#include "biome_column.h"
#include "biome_surface.h"
#include "biome_color.h"
#include "biome_deco.h"
#include "biome_cache.h"

#endif
