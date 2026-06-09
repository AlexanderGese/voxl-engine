#include "bvh_region.h"
#include "bvh_build.h"
#include "../../config.h"

#include <stddef.h>
#include <stdlib.h>

aabb bvh_chunk_bounds(int cx, int cz) {
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
    return aabb_make(mn, mx);
}

aabb bvh_block_bounds(int bx, int by, int bz) {
    vec3 mn = { (float)bx, (float)by, (float)bz };
    vec3 mx = { (float)(bx + 1), (float)(by + 1), (float)(bz + 1) };
    return aabb_make(mn, mx);
}

uint32_t bvh_pack_chunk_id(int cx, int cz) {
    // bias by 32768 so the int16 range maps into unsigned without sign games.
    uint32_t ux = (uint32_t)((cx + 32768) & 0xFFFF);
    uint32_t uz = (uint32_t)((cz + 32768) & 0xFFFF);
    return (ux << 16) | uz;
}

void bvh_unpack_chunk_id(uint32_t id, int *cx, int *cz) {
    int ux = (int)((id >> 16) & 0xFFFF);
    int uz = (int)(id & 0xFFFF);
    if (cx) *cx = ux - 32768;
    if (cz) *cz = uz - 32768;
}

bvh_prim bvh_prim_box(uint32_t id, uint32_t tag, vec3 center, vec3 half) {
    bvh_prim p;
    p.id = id;
    p.tag = tag;
    p.box = aabb_from_center(center, half);
    return p;
}

int bvh_build_chunk_region(bvh *b, int center_cx, int center_cz, int radius) {
    if (radius < 0) radius = 0;
    int side = radius * 2 + 1;
    int32_t n = side * side;

    // scratch prim list. one per chunk column. freed after the build copies it.
    bvh_prim *prims = malloc((size_t)n * sizeof(bvh_prim));
    if (!prims) return -1;

    int32_t k = 0;
    for (int dz = -radius; dz <= radius; dz++) {
        for (int dx = -radius; dx <= radius; dx++) {
            int cx = center_cx + dx;
            int cz = center_cz + dz;
            prims[k].id  = bvh_pack_chunk_id(cx, cz);
            prims[k].tag = 0;
            prims[k].box = bvh_chunk_bounds(cx, cz);
            k++;
        }
    }

    int rc = bvh_build(b, prims, n);
    free(prims);
    return rc;
}
