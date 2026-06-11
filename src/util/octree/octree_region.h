#ifndef UTIL_OCTREE_REGION_H
#define UTIL_OCTREE_REGION_H

// world-facing glue. the bare octree doesnt know what a chunk is on purpose,
// but in practice everyone wires it to chunk/block space, so the boilerplate
// for that lives here instead of being copy-pasted into entity/ and world/.
//
// chunk coords are (cx, cz). a chunk is CHUNK_SIZE_X * CHUNK_SIZE_Y *
// CHUNK_SIZE_Z blocks. we offer helpers to turn those into world aabbs and to
// stand up an octree sized to a loaded-region square.

#include "octree.h"

// world aabb of a whole chunk column at (cx, cz).
aabb octree_chunk_bounds(int cx, int cz);

// world aabb of a single block at world coords (bx, by, bz).
aabb octree_block_bounds(int bx, int by, int bz);

// build a tree that covers a square of chunks centered on (center_cx, center_cz)
// extending `radius` chunks each way. handy for an entity broadphase scoped to
// the loaded area. y spans the full world height.
void octree_init_region(octree *t, int center_cx, int center_cz, int radius);

// insert an entity-ish thing by center + half-extents. wraps the box math so
// callers dont rebuild an aabb every frame.
int  octree_insert_box(octree *t, uint32_t id, uint32_t tag, vec3 center, vec3 half);

// world coord -> owning chunk coord. floor division, handles negatives right.
void octree_world_to_chunk(int bx, int bz, int *cx, int *cz);

#endif
