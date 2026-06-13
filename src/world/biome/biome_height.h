#ifndef WORLD_BIOME_HEIGHT_H
#define WORLD_BIOME_HEIGHT_H

#include <stdint.h>
#include "biome_def.h"

// per-biome terrain height. each biome's def carries a base offset and an amp;
// here we turn those plus noise into an actual top-y. erosion flattens things:
// high erosion squashes the amplitude so worn lowlands stay calm while peaks
// biomes keep their relief.

int biome_height_for(const biome_def *def, int wx, int wz,
                     int sea_level, uint32_t seed, float erosion);

// the raw, unbiomed continental base height. biomes ride on top of this so
// neighboring biomes share a baseline and only differ by their relief.
int biome_height_base(int wx, int wz, int sea_level, uint32_t seed);

#endif
