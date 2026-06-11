#ifndef UTIL_OCTREE_RAYCAST_H
#define UTIL_OCTREE_RAYCAST_H

// ray traversal of the tree. ordered front-to-back so we can stop at the first
// hit instead of testing every item. this is the thing the player-reach pick
// and the projectile broadphase both lean on.

#include "octree.h"

// closest item the ray (origin + t*dir, t in [0, max_t]) hits. dir need not be
// normalized but t is measured in dir-lengths either way. returns the hit; if
// .hit == 0 nothing was found.
octree_hit octree_raycast(const octree *t, vec3 origin, vec3 dir, float max_t);

// tag-filtered variant. only items matching the mask are eligible.
octree_hit octree_raycast_tagged(const octree *t, vec3 origin, vec3 dir,
                                 float max_t, uint32_t tag_mask);

// does ANY item block the segment origin->target? early-outs on first hit, no
// distance sorting. cheaper than a full raycast when you only want yes/no
// (line of sight, can-this-mob-see-the-player, etc).
int octree_segment_blocked(const octree *t, vec3 origin, vec3 target);

#endif
