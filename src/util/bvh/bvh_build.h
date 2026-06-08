#ifndef UTIL_BVH_BUILD_H
#define UTIL_BVH_BUILD_H

// the builder. takes the caller's primitive list and produces a flat SAH bvh
// inside the tree's storage. this is the heavy one - O(N log N) with the SAH
// sweep at each node - so callers should build once and query many, or refit
// (bvh_refit.c) when the prims only nudged.

#include "bvh.h"

// build a tree over `prims` (n of them). copies the prims internally and
// reorders the copy, so the caller's array is left alone. returns 0 on success,
// -1 on oom or n <= 0. an empty input leaves an unbuilt tree (not an error to
// query, it just never hits).
int bvh_build(bvh *b, const bvh_prim *prims, int32_t n);

// like bvh_build but forces a plain longest-axis median split instead of the
// SAH. faster to build, looser tree. handy when you're rebuilding every frame
// and traversal quality matters less than build time.
int bvh_build_median(bvh *b, const bvh_prim *prims, int32_t n);

#endif
