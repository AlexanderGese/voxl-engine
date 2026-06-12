#include "vec3.h"

#include <math.h>

vec3 vec3_new(float x, float y, float z) {
    vec3 v = {x, y, z};
    return v;
}

vec3 vec3_add(vec3 a, vec3 b) {
    return (vec3){a.x + b.x, a.y + b.y, a.z + b.z};
}

vec3 vec3_sub(vec3 a, vec3 b) {
    return (vec3){a.x - b.x, a.y - b.y, a.z - b.z};
}

vec3 vec3_scale(vec3 a, float s) {
    return (vec3){a.x * s, a.y * s, a.z * s};
}

vec3 vec3_mul(vec3 a, vec3 b) {
    return (vec3){a.x * b.x, a.y * b.y, a.z * b.z};
}

vec3 vec3_neg(vec3 a) {
    return (vec3){-a.x, -a.y, -a.z};
}

float vec3_dot(vec3 a, vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

vec3 vec3_cross(vec3 a, vec3 b) {
    return (vec3){
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

float vec3_length_sq(vec3 a) {
    return a.x * a.x + a.y * a.y + a.z * a.z;
}

float vec3_length(vec3 a) {
    return sqrtf(vec3_length_sq(a));
}

float vec3_distance(vec3 a, vec3 b) {
    return vec3_length(vec3_sub(a, b));
}

vec3 vec3_normalize(vec3 a) {
    float len = vec3_length(a);
    if (len < 1e-6f) return VEC3_ZERO;
    float inv = 1.0f / len;
    return (vec3){a.x * inv, a.y * inv, a.z * inv};
}

vec3 vec3_lerp(vec3 a, vec3 b, float t) {
    return (vec3){
        a.x + (b.x - a.x) * t,
        a.y + (b.y - a.y) * t,
        a.z + (b.z - a.z) * t
    };
}

vec3 vec3_min(vec3 a, vec3 b) {
    return (vec3){
        a.x < b.x ? a.x : b.x,
        a.y < b.y ? a.y : b.y,
        a.z < b.z ? a.z : b.z
    };
}

vec3 vec3_max(vec3 a, vec3 b) {
    return (vec3){
        a.x > b.x ? a.x : b.x,
        a.y > b.y ? a.y : b.y,
        a.z > b.z ? a.z : b.z
    };
}

vec3 vec3_floor(vec3 a) {
    return (vec3){floorf(a.x), floorf(a.y), floorf(a.z)};
}

int vec3_equals(vec3 a, vec3 b) {
    return a.x == b.x && a.y == b.y && a.z == b.z;
}
