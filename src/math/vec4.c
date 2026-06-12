#include "vec4.h"
#include <math.h>

vec4 vec4_new(float x, float y, float z, float w) { return (vec4){x, y, z, w}; }
vec4 vec4_from_vec3(vec3 v, float w) { return (vec4){v.x, v.y, v.z, w}; }

vec4 vec4_add(vec4 a, vec4 b) {
    return (vec4){a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
}
vec4 vec4_sub(vec4 a, vec4 b) {
    return (vec4){a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
}
vec4 vec4_scale(vec4 a, float s) {
    return (vec4){a.x * s, a.y * s, a.z * s, a.w * s};
}
float vec4_dot(vec4 a, vec4 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}
vec4 vec4_normalize(vec4 a) {
    float l = sqrtf(vec4_dot(a, a));
    if (l < 1e-6f) return (vec4){0, 0, 0, 0};
    return (vec4){a.x / l, a.y / l, a.z / l, a.w / l};
}
