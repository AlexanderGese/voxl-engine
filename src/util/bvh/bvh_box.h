#ifndef UTIL_BVH_BOX_H
#define UTIL_BVH_BOX_H
// aabb helpers the bvh leans on that math/aabb.h doesnt carry. the engine's
// aabb is just min/max + a slab raycast; the builder needs surface area, union,
// centroid, the longest-axis pick and an "empty" sentinel to grow from. rather
// than bolt these onto the shared aabb (and risk a rebuild of half the tree) we
// keep our own little kit here.
#include "../../math/aabb.h"
// an aabb that swallows anything unioned into it. min = +inf, max = -inf. start
// here, fold prims in, and you end with their tight bound.
aabb  bvh_box_empty(void);
// union of two boxes. component-wise min of mins, max of maxs.
aabb  bvh_box_union(aabb a, aabb b);
// grow a box to include a point.
aabb  bvh_box_include(aabb a, vec3 p);
// centroid (midpoint) of a box.
vec3  bvh_box_centroid(aabb a);
// half the diagonal vector, i.e. the extents from center to a corner.
vec3  bvh_box_half(aabb a);
// surface area. the SAH currency. a degenerate/empty box returns 0 so it never
// poisons a cost sum with a negative.
float bvh_box_area(aabb a);
// longest axis: 0=x, 1=y, 2=z. ties resolve toward x then y. used as the median
// fallback split axis when SAH gives up.
int   bvh_box_max_axis(aabb a);
// does `outer` fully contain `inner`? inclusive on the edges. used when deciding
// whether a refit even needs to touch a parent (child still inside its old box).
int   bvh_box_contains_box(aabb outer, aabb inner);
// volume of the overlap region of two boxes, 0 if they dont overlap. a softer
// signal than a boolean intersects when you want to rank candidates.
float bvh_box_overlap_volume(aabb a, aabb b);
// pad a box outward by a uniform margin on every axis. handy for building a bvh
// over moving prims with a velocity-sized skin so small motion doesnt force a
// refit (the classic "fattened aabb" trick).
aabb  bvh_box_pad(aabb a, float margin);
// per-axis component read without a switch at every call site.
static inline float bvh_axis_get(vec3 v, int axis) {
    return axis == 0 ? v.x : (axis == 1 ? v.y : v.z);
}
#endif
