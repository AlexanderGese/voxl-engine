#include "vec2.h"
#include <math.h>

vec2 vec2_new(float x, float y) { return (vec2){x, y}; }
vec2 vec2_add(vec2 a, vec2 b) { return (vec2){a.x + b.x, a.y + b.y}; }
vec2 vec2_sub(vec2 a, vec2 b) { return (vec2){a.x - b.x, a.y - b.y}; }
vec2 vec2_scale(vec2 a, float s) { return (vec2){a.x * s, a.y * s}; }
float vec2_dot(vec2 a, vec2 b) { return a.x * b.x + a.y * b.y; }
float vec2_length(vec2 a) { return sqrtf(a.x * a.x + a.y * a.y); }

vec2 vec2_normalize(vec2 a) {
    float l = vec2_length(a);
    if (l < 1e-6f) return (vec2){0, 0};
    return (vec2){a.x / l, a.y / l};
}

vec2 vec2_lerp(vec2 a, vec2 b, float t) {
    return (vec2){a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t};
}
