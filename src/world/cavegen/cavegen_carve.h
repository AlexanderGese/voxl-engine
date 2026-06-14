#ifndef WORLD_CAVEGEN_CARVE_H
#define WORLD_CAVEGEN_CARVE_H

#include "cavegen_grid.h"
#include "cavegen_types.h"
#include "../chunk.h"

// the bridge from scratch grid to actual world. everything above this file is
// pure number-crunching; this is where we finally touch a chunk and replace
// stone with air (or water). only the centre footprint of the grid maps onto
// the chunk — the pad cells were just there so the automata and worms had
// context past the chunk border.

// counts so the driver can log what it did. nice for spotting "carved 0 blocks"
// regressions, which means a param got fat-fingered.
typedef struct {
    int air_set;       // cells turned to air
    int water_set;     // cells turned to water
    int skipped_bedrock;
} cavegen_carve_stats;

// stamp the finished grid into the chunk. only carves blocks that are currently
// solid-ish (stone/dirt/etc) — wont eat ores or player builds if those got
// placed first, and never touches bedrock. flooded cells become water.
cavegen_carve_stats cavegen_carve_apply(chunk *c, const cavegen_grid *g,
                                        const cavegen_params *p);

// true for blocks the carver is allowed to dig through.
int cavegen_carve_is_diggable(block_id id);

#endif
