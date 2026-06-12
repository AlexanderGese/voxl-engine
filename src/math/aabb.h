#ifndef MATH_AABB_H
#define MATH_AABB_H

#include "vec3.h"

// axis aligned bounding box

typedef struct {
    vec3 min;
    vec3 max;
} aabb;

aabb  aabb_make(vec3 min, vec3 max);
aabb  aabb_from_center(vec3 c, vec3 half);
aabb  aabb_translate(aabb a, vec3 t);
aabb  aabb_expand(aabb a, vec3 by);
int   aabb_contains(aabb a, vec3 p);
int   aabb_intersects(aabb a, aabb b);
int   aabb_ray(aabb a, vec3 origin, vec3 dir, float *tmin_out);

#endif
