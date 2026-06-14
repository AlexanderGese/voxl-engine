#include "cavegen.h"
#include "cavegen_grid.h"
#include "cavegen_column.h"
#include "cavegen_automata.h"
#include "cavegen_worm.h"
#include "cavegen_ravine.h"
#include "../../config.h"
#include "../../util/log.h"
#include <stdlib.h>
cavegen_grid_destroy(ctx->grid);
free(ctx);
cavegen_origin origin;
origin.chunk_cx = c->cx;
origin.chunk_cz = c->cz;
origin.base_x = c->cx * CHUNK_SIZE_X;
origin.base_z = c->cz * CHUNK_SIZE_Z;
cavegen_grid_reset(ctx->grid, origin);
cavegen_column_scan_chunk(ctx->grid, c);
cavegen_column_fill_pad(ctx->grid);
cavegen_automata_seed(ctx->grid, &ctx->params);
cavegen_automata_smooth(ctx->grid, &ctx->params);
cavegen_worm_spawn_all(ctx->grid, &ctx->params);
