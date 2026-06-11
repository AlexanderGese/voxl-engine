#ifndef UTIL_OCTREE_REGION_H
#define UTIL_OCTREE_REGION_H
// world-facing glue. the bare octree doesnt know what a chunk is on purpose,
// but in practice everyone wires it to chunk/block space, so the boilerplate
// for that lives here instead of being copy-pasted into entity/ and world/.
//
#include "octree.h"
aabb octree_chunk_bounds(int cx, int cz);
aabb octree_block_bounds(int bx, int by, int bz);
void octree_init_region(octree *t, int center_cx, int center_cz, int radius);
int  octree_insert_box(octree *t, uint32_t id, uint32_t tag, vec3 center, vec3 half);
void octree_world_to_chunk(int bx, int bz, int *cx, int *cz);
#endif
