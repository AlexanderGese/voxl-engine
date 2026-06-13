#ifndef WORLD_BIOME_DEF_H
#define WORLD_BIOME_DEF_H

#include <stdint.h>
#include "../block.h"
#include "biome_climate.h"

// our own biome ids. deliberately NOT the engine's biome_id from world/biome.h
// so the two systems can coexist while this one is being wired up. these cover
// a denser climate grid than the legacy 7.

typedef enum {
    BIOME_KIND_OCEAN = 0,
    BIOME_KIND_BEACH,
    BIOME_KIND_PLAINS,
    BIOME_KIND_FOREST,
    BIOME_KIND_RAINFOREST,
    BIOME_KIND_SWAMP,
    BIOME_KIND_SAVANNA,
    BIOME_KIND_DESERT,
    BIOME_KIND_BADLANDS,
    BIOME_KIND_TAIGA,
    BIOME_KIND_TUNDRA,
    BIOME_KIND_SNOWY_PEAKS,
    BIOME_KIND_STONE_PEAKS,
    BIOME_KIND_COUNT
} biome_kind;

// the climate centroid a biome wants to live at, plus a weight on each axis
// so we can say "deserts care a lot about humidity, less about weirdness".
// the lookup scores candidates by weighted squared distance to this point.
typedef struct {
    float temperature;
    float humidity;
    float erosion;
    float weirdness;
    float w_temp, w_humid, w_eros, w_weird;
} biome_envelope;

// static description of a biome. surface stacking + flavor knobs + tint refs.
typedef struct {
    const char *name;
    biome_kind  kind;
    biome_envelope env;

    block_id surface;     // very top block when dry land
    block_id subsurface;  // few blocks under surface
    block_id filler;      // deep filler, usually stone
    block_id underwater;  // top block if column sits below sea level

    float    base_height; // added to sea level (can be negative for oceans)
    float    height_amp;  // vertical variation magnitude
    int      subsurface_depth; // how many blocks of subsurface before filler

    float    tree_density;     // per-column trunk probability
    float    grass_density;    // per-column ground deco probability

    int      snow_capped;      // 1 if a snow layer can settle on top
    uint32_t grass_tint;       // packed 0xRRGGBB base, modulated by climate
    uint32_t water_tint;
} biome_def;

#endif
