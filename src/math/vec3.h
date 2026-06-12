#ifndef MATH_VEC3_H
#define MATH_VEC3_H

// 3d vector. float. plain struct.
// i tried a union once with .v[3] array access but it got gross so i went back.

typedef struct {
    float x, y, z;
} vec3;

static const vec3 VEC3_ZERO  = {0, 0, 0};
static const vec3 VEC3_UP    = {0, 1, 0};
static const vec3 VEC3_RIGHT = {1, 0, 0};
static const vec3 VEC3_FWD   = {0, 0, 1};

vec3  vec3_new(float x, float y, float z);
vec3  vec3_add(vec3 a, vec3 b);
vec3  vec3_sub(vec3 a, vec3 b);
vec3  vec3_scale(vec3 a, float s);
vec3  vec3_mul(vec3 a, vec3 b);
vec3  vec3_neg(vec3 a);

float vec3_dot(vec3 a, vec3 b);
vec3  vec3_cross(vec3 a, vec3 b);

float vec3_length(vec3 a);
float vec3_length_sq(vec3 a);
float vec3_distance(vec3 a, vec3 b);

vec3  vec3_normalize(vec3 a);
vec3  vec3_lerp(vec3 a, vec3 b, float t);
vec3  vec3_min(vec3 a, vec3 b);
vec3  vec3_max(vec3 a, vec3 b);
vec3  vec3_floor(vec3 a);

int   vec3_equals(vec3 a, vec3 b);

#endif
