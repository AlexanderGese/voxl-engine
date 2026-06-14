#ifndef WORLD_CAVEGEN_H
#define WORLD_CAVEGEN_H

#include "cavegen_types.h"
#include "cavegen_connect.h"
#include "cavegen_carve.h"
#include "../chunk.h"

// top-level cave generator. the rest of worldgen calls cavegen_carve_chunk()
// after surface terrain is laid down but before decoration, and out fall the
// caves. the pipeline per chunk:
//
// 1. scan the chunk for its surface heightmap          (column)
// 2. random-fill + smooth a 3d cellular automata        (automata)
// 3. walk perlin worms to link the rooms with tunnels   (worm)
// 4. flood-fill connectivity, seal tiny dead bubbles     (connect)
// 5. stamp the result back into the chunk as air/water   (carve)
//
// steps 2-4 work on a padded scratch grid so caves line up across chunk seams.
// one cavegen_ctx owns the reusable scratch buffer; create it once per worldgen
// worker, not per chunk.

typedef struct {
    cavegen_params  params;
    cavegen_grid   *grid;     // reused scratch, owned
} cavegen_ctx;

// rolled-up stats from one chunk's carve. mostly for logging/tests.
typedef struct {
    cavegen_connect_stats connect;
    cavegen_carve_stats   carve;
} cavegen_report;

cavegen_ctx *cavegen_create(uint32_t world_seed);
void         cavegen_destroy(cavegen_ctx *ctx);

// override the defaults if you want to tune. copies the params in.
void cavegen_set_params(cavegen_ctx *ctx, const cavegen_params *p);

// run the whole pipeline against one chunk, in place. safe to call on a chunk
// that already has terrain; it only carves diggable solids. returns a report.
cavegen_report cavegen_carve_chunk(cavegen_ctx *ctx, chunk *c);

#endif
