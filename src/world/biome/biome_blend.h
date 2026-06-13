#ifndef WORLD_BIOME_BLEND_H
#define WORLD_BIOME_BLEND_H

#include <stdint.h>
#include "biome_def.h"

// neighborhood blending. we sample the picked biome on a small grid around a
// column and combine their per-biome scalars with distance falloff weights.
// this is what kills the hard cliff seams between biomes. heights, tints, and
// densities all go through here.

// a normalized mix of biome contributions for one column.
typedef struct {
    biome_kind dominant;       // the closest single biome
    int        count;          // how many distinct biomes contributed
    float      sharpness;      // 0 deep interior .. 1 right on a border
    float      height;         // blended terrain top y
    float      tree_density;   // blended
    float      grass_density;  // blended
    uint32_t   grass_tint;     // blended 0xRRGGBB
    uint32_t   water_tint;
} biome_blend;

// full blend at a world column. radius is in blocks, clamped to [0,8]. step
// lets you skip columns for speed; 1 == sample every block in the kernel.
void  biome_blend_at(int wx, int wz, uint32_t seed, int sea_level,
                     int radius, int step, biome_blend *out);

// just the blended height. cheaper path for the terrain pass which only needs
// the surface y and nothing else.
int   biome_blend_height(int wx, int wz, uint32_t seed, int sea_level,
                         int radius);

// how close this column is to a biome border, 0..1. used to gate things like
// suppressing trees right on a seam.
float biome_blend_edge(int wx, int wz, uint32_t seed);

#endif
