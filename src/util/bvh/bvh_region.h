#ifndef UTIL_BVH_REGION_H
#define UTIL_BVH_REGION_H

// world-facing glue. the bare bvh has no idea what a chunk or a block is, on
// purpose. but everyone wires it into chunk/block space, so the boilerplate for
// turning chunk coords into world boxes - and for stuffing a loaded region's
// chunk columns into a tree - lives here instead of being copy-pasted around.
//
// chunk coords are (cx, cz). a chunk spans CHUNK_SIZE_X * CHUNK_SIZE_Y *
// CHUNK_SIZE_Z blocks (see config.h). y is always the full world column.

#include "bvh.h"

// world aabb of a whole chunk column at (cx, cz).
aabb bvh_chunk_bounds(int cx, int cz);

// world aabb of a single block at world coords (bx, by, bz). a unit cube whose
// min corner sits at the integer coord.
aabb bvh_block_bounds(int bx, int by, int bz);

// build a bvh over every chunk column in the square of `radius` chunks around
// (center_cx, center_cz). each chunk becomes one prim, id packed from its (cx,
// cz) via bvh_pack_chunk_id. handy as a coarse "which chunk does this ray hit"
// accelerator. returns 0 ok, -1 on oom.
int  bvh_build_chunk_region(bvh *b, int center_cx, int center_cz, int radius);

// pack / unpack a chunk coord into a 32-bit prim id. 16 bits each, biased so
// negatives survive. lossy past +-32k chunks which is well past any sane world.
uint32_t bvh_pack_chunk_id(int cx, int cz);
void     bvh_unpack_chunk_id(uint32_t id, int *cx, int *cz);

// build a prim from a center + half-extents, so entity callers dont rebuild an
// aabb by hand every frame. fills *out, returns it too for chaining.
bvh_prim bvh_prim_box(uint32_t id, uint32_t tag, vec3 center, vec3 half);

#endif
