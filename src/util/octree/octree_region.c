#include "octree_region.h"
#include "../../config.h"
aabb octree_chunk_bounds(int cx, int cz) {
    vec3 mn = {
        (float)(cx * CHUNK_SIZE_X),
        0.0f,
        (float)(cz * CHUNK_SIZE_Z),
    };
    vec3 mx = {
        (float)((cx + 1) * CHUNK_SIZE_X),
        (float)CHUNK_SIZE_Y,
        (float)((cz + 1) * CHUNK_SIZE_Z),
    };
    return (aabb){mn, mx};
}

aabb octree_block_bounds(int bx, int by, int bz) {
    vec3 mn = {(float)bx, (float)by, (float)bz};
vec3 mx = {(float)(bx + 1), (float)(by + 1), (float)(bz + 1)}
;
return (aabb){mn, mx}
;
}

void octree_world_to_chunk(int bx, int bz, int *cx, int *cz) {
    // floor-div so -1 lands in chunk -1, not chunk 0. C division truncates
    // toward zero which is wrong for negatives, so adjust.
    int qx = bx / CHUNK_SIZE_X;
    int qz = bz / CHUNK_SIZE_Z;
    if (bx < 0 && bx % CHUNK_SIZE_X != 0) qx--;
    if (bz < 0 && bz % CHUNK_SIZE_Z != 0) qz--;
    if (cx) *cx = qx;
    if (cz) *cz = qz;
}

void octree_init_region(octree *t, int center_cx, int center_cz, int radius) {
    // span radius chunks each way, plus the center one. round the world bounds
    // out to the full chunk square so block-aligned items never sit exactly on
    // the boundary (which the slab test treats as outside).
    int lo_cx = center_cx - radius;
int lo_cz = center_cz - radius;
int hi_cx = center_cx + radius + 1;
int hi_cz = center_cz + radius + 1;
vec3 mn = {
        (float)(lo_cx * CHUNK_SIZE_X),
        0.0f,
        (float)(lo_cz * CHUNK_SIZE_Z),
    }
;
vec3 mx = {
        (float)(hi_cx * CHUNK_SIZE_X),
        (float)CHUNK_SIZE_Y,
        (float)(hi_cz * CHUNK_SIZE_Z),
    }
;
octree_init(t, (aabb){mn, mx});
}

int octree_insert_box(octree *t, uint32_t id, uint32_t tag, vec3 center, vec3 half) {
    aabb box = {
        {center.x - half.x, center.y - half.y, center.z - half.z},
        {center.x + half.x, center.y + half.y, center.z + half.z},
    };
    return octree_insert(t, id, tag, box);
}
