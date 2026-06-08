#ifndef UTIL_BVH_SAH_H
#define UTIL_BVH_SAH_H

// the surface area heuristic split chooser. given a span of primitives, the
// builder asks "where do i cut, on which axis, to minimize expected ray cost?".
// this file owns that decision and nothing else - it's pure cost arithmetic, no
// tree mutation. the builder (bvh_build.c) calls bvh_sah_best_split then does
// the actual partition.

#include "bvh_types.h"

// a candidate split: which axis, the bucket boundary, and the SAH cost. cost is
// FLT_MAX-ish if no useful split exists (all prim centroids coincide), in which
// case the builder makes a leaf or falls back to a median split.
typedef struct {
    int   axis;        // 0/1/2, -1 if no split found
    int   bucket;      // split after this bucket index (0..BUCKETS-2)
    float cost;        // estimated SAH cost of this partition
    float leaf_cost;   // cost of just making everything one leaf, for comparison
} bvh_split;

// one SAH bucket: a running bound + a count. the sweep fills BVH_SAH_BUCKETS of
// these per axis from the centroids, then evaluates the BUCKETS-1 boundaries.
typedef struct {
    aabb  bounds;
    int   count;
} bvh_bucket;

// evaluate the SAH over [first, first+count) of `prims` and return the best
// split. `centroid_bounds` is the precomputed bound over the prim centroids
// (the builder already has it, cheaper to pass than recompute). reads prims,
// never writes them.
bvh_split bvh_sah_best_split(const bvh_prim *prims, int32_t first, int32_t count,
                             aabb centroid_bounds);

// partition [first, first+count) in place so prims whose centroid falls in the
// chosen split's lower buckets come first. returns the index of the first prim
// on the high side (the "mid"). a thin wrapper over a hoare-ish partition keyed
// on the bucket the centroid lands in.
int32_t   bvh_sah_partition(bvh_prim *prims, int32_t first, int32_t count,
                            const bvh_split *split, aabb centroid_bounds);

#endif
