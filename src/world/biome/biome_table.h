#ifndef WORLD_BIOME_TABLE_H
#define WORLD_BIOME_TABLE_H

#include "biome_def.h"

// the registry. one static def per biome_kind, looked up by id. the table is
// the single source of truth for climate envelopes and surface materials.

const biome_def *biome_table_get(biome_kind kind);

// total registered biomes (always BIOME_KIND_COUNT, but go through this so
// callers dont hardcode the enum tail).
int  biome_table_count(void);

// convenience name lookup, returns "?" for out-of-range.
const char *biome_table_name(biome_kind kind);

// iterate all defs in id order. returns NULL once index is past the end.
const biome_def *biome_table_at(int index);

#endif
