#ifndef MATH_VEC3_H
#define MATH_VEC3_H
typedef struct {
    float x, y, z;
} vec3;
;
;
;
;
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
#endif
