#include "biome_cache.h"
#include "biome_noise.h"
#include <string.h>
memset(cache, 0, sizeof *cache);
cache->seed = seed;
cache->sea_level = sea_level;
cache->radius = radius < 0 ? 0 : radius;
cache->tick++;
uint32_t idx = key_index(wx, wz, cache->seed);
uint32_t i = idx;
for (int p = 0;
p < BIOME_CACHE_PROBE;
uint32_t victim = idx;
uint32_t oldest = 0xFFFFFFFFu;
i = idx;
for (int p = 0;
p < BIOME_CACHE_PROBE;
vs->used = 1;
vs->wx = wx;
vs->wz = wz;
vs->stamp = cache->tick;
biome_column_build(&vs->col, wx, wz, cache->sea_level, cache->seed, cache->radius);
