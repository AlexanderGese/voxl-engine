#ifndef UTIL_BVH_RAYCAST_H
#define UTIL_BVH_RAYCAST_H

// ray traversal. the bvh's headline feature: shoot a ray, get the nearest prim
// it crosses, fast, by pruning whole subtrees the ray misses and walking the
// children near-to-far so the first hit usually lets us cull the rest.

#include "bvh.h"

// nearest hit along the ray within [0, max_t]. dir need not be normalized; t is
// in dir units. returns a bvh_hit with hit=0 if nothing was crossed.
bvh_hit bvh_raycast(const bvh *b, vec3 origin, vec3 dir, float max_t);

// same but only prims whose tag matches tag_mask (0 = any) are considered.
bvh_hit bvh_raycast_tagged(const bvh *b, vec3 origin, vec3 dir, float max_t,
                           uint32_t tag_mask);

// is the straight segment origin->target blocked by anything (tag_mask, 0=any)?
// line-of-sight / occlusion test. cheaper than the full raycast because it
// stops at the first prim it finds rather than searching for the nearest.
int     bvh_segment_blocked(const bvh *b, vec3 origin, vec3 target,
                            uint32_t tag_mask);

// callback for the all-hits variant. `t` is the ray entry param of this prim.
// return 0 to keep collecting, non-zero to stop early.
typedef int (*bvh_ray_visit_fn)(uint32_t id, uint32_t tag, float t, void *user);

// visit EVERY prim the ray crosses within [0, max_t], not just the nearest.
// unordered (we dont sort hits) - if you need them sorted, collect and sort on
// the caller side. useful for translucent spans, damage-through, beam effects.
// returns the number of prims hit.
int     bvh_raycast_all(const bvh *b, vec3 origin, vec3 dir, float max_t,
                        uint32_t tag_mask, bvh_ray_visit_fn fn, void *user);

#endif
