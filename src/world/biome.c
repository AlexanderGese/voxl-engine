#include "biome.h"
#include "../math/noise.h"

static const biome_info g_biomes[BIOME_COUNT] = {
    { "plains",    BLOCK_GRASS, BLOCK_DIRT,  BLOCK_AIR,  0.5f,   0.0f,  1.0f, 0 },
    { "forest",    BLOCK_GRASS, BLOCK_DIRT,  BLOCK_AIR,  1.5f,   1.0f,  1.2f, 0 },
    { "desert",    BLOCK_SAND,  BLOCK_SAND,  BLOCK_AIR,  0.02f, -2.0f,  0.6f, 0 },
    { "mountains", BLOCK_STONE, BLOCK_STONE, BLOCK_SNOW, 0.2f,  14.0f,  2.5f, 1 },
    { "tundra",    BLOCK_SNOW,  BLOCK_DIRT,  BLOCK_AIR,  0.1f,  -1.0f,  0.8f, 1 },
    { "swamp",     BLOCK_GRASS, BLOCK_DIRT,  BLOCK_AIR,  0.8f,  -3.0f,  0.5f, 0 },
    { "beach",     BLOCK_SAND,  BLOCK_SAND,  BLOCK_AIR,  0.0f,  -6.0f,  0.3f, 0 },
};

const biome_info *biome_get(biome_id id) {
    if ((int)id < 0 || id >= BIOME_COUNT) return &g_biomes[BIOME_PLAINS];
    return &g_biomes[id];
}

biome_id biome_pick(float t, float h) {
    // rough zone decision. not pretty
    if (t < 0.2f)        return BIOME_TUNDRA;
    if (t > 0.8f && h < 0.3f) return BIOME_DESERT;
    if (t > 0.6f && h > 0.7f) return BIOME_SWAMP;
    if (h > 0.65f)       return BIOME_FOREST;
    if (t > 0.85f)       return BIOME_MOUNTAINS;
    return BIOME_PLAINS;
}

void biome_sample(int wx, int wz, unsigned seed, float *t_out, float *h_out) {
    // two cheap noise fields, offset so they dont correlate
    (void)seed;
    float s = 1.0f / 220.0f;
    float t = noise2d(wx * s + 1000.0f, wz * s - 500.0f);
    float h = noise2d(wx * s - 3333.0f, wz * s + 7777.0f);
    // remap from [-1, 1] to [0, 1]
    *t_out = 0.5f * (t + 1.0f);
    *h_out = 0.5f * (h + 1.0f);
}
