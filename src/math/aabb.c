#include "aabb.h"
#include <math.h>

aabb aabb_make(vec3 min, vec3 max) { return (aabb){min, max}; }

aabb aabb_from_center(vec3 c, vec3 half) {
    return (aabb){
        {c.x - half.x, c.y - half.y, c.z - half.z},
        {c.x + half.x, c.y + half.y, c.z + half.z}
    };
}

aabb aabb_translate(aabb a, vec3 t) {
    a.min = vec3_add(a.min, t);
    a.max = vec3_add(a.max, t);
    return a;
}

aabb aabb_expand(aabb a, vec3 by) {
    a.min = vec3_sub(a.min, by);
    a.max = vec3_add(a.max, by);
    return a;
}

int aabb_contains(aabb a, vec3 p) {
    return p.x >= a.min.x && p.x <= a.max.x
        && p.y >= a.min.y && p.y <= a.max.y
        && p.z >= a.min.z && p.z <= a.max.z;
}

int aabb_intersects(aabb a, aabb b) {
    return a.min.x <= b.max.x && a.max.x >= b.min.x
        && a.min.y <= b.max.y && a.max.y >= b.min.y
        && a.min.z <= b.max.z && a.max.z >= b.min.z;
}

static float max3f(float a, float b, float c) {
    float m = a > b ? a : b;
    return m > c ? m : c;
}
static float min3f(float a, float b, float c) {
    float m = a < b ? a : b;
    return m < c ? m : c;
}

int aabb_ray(aabb a, vec3 origin, vec3 dir, float *tmin_out) {
    // slab test
    float tx1 = (a.min.x - origin.x) / dir.x;
    float tx2 = (a.max.x - origin.x) / dir.x;
    float ty1 = (a.min.y - origin.y) / dir.y;
    float ty2 = (a.max.y - origin.y) / dir.y;
    float tz1 = (a.min.z - origin.z) / dir.z;
    float tz2 = (a.max.z - origin.z) / dir.z;

    float txmin = fminf(tx1, tx2), txmax = fmaxf(tx1, tx2);
    float tymin = fminf(ty1, ty2), tymax = fmaxf(ty1, ty2);
    float tzmin = fminf(tz1, tz2), tzmax = fmaxf(tz1, tz2);

    float tmin = max3f(txmin, tymin, tzmin);
    float tmax = min3f(txmax, tymax, tzmax);
    if (tmax < 0 || tmin > tmax) return 0;
    if (tmin_out) *tmin_out = tmin > 0 ? tmin : 0;
    return 1;
}
