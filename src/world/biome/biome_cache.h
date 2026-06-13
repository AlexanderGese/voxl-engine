#ifndef WORLD_BIOME_CACHE_H
#define WORLD_BIOME_CACHE_H

#include <stdint.h>
#include "biome_column.h"

// resolving a column isn't free (climate sample + blend kernel), and the chunk
// mesher hits the same columns over and over. so we cache resolved columns in
// an open-addressed table with linear probing. fixed capacity, evict-on-insert
// when a slot collides past the probe limit. good enough, no allocation churn.

#define BIOME_CACHE_BITS  12
#define BIOME_CACHE_SIZE  (1u << BIOME_CACHE_BITS)
#define BIOME_CACHE_MASK  (BIOME_CACHE_SIZE - 1u)
#define BIOME_CACHE_PROBE 6

typedef struct {
    int32_t      wx, wz;     // key
    uint8_t      used;
    uint32_t     stamp;      // last-access tick for stats / debugging
    biome_column col;        // cached payload
} biome_cache_slot;

typedef struct {
    biome_cache_slot slots[BIOME_CACHE_SIZE];
    uint32_t seed;
    int      sea_level;
    int      radius;         // blend radius used when filling
    uint32_t tick;           // bumps every query
    uint64_t hits, misses;
} biome_cache;

void biome_cache_init(biome_cache *cache, uint32_t seed, int sea_level, int radius);
void biome_cache_clear(biome_cache *cache);

// fetch the resolved column for (wx,wz), building + inserting on miss. the
// returned pointer is stable until the slot is evicted; copy if you hold it.
const biome_column *biome_cache_get(biome_cache *cache, int wx, int wz);

// quick biome-only query that goes through the cache.
biome_kind biome_cache_biome(biome_cache *cache, int wx, int wz);

// hit-rate as a percentage, for the debug overlay.
float biome_cache_hitrate(const biome_cache *cache);

#endif
