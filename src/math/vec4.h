#ifndef MATH_VEC4_H
#define MATH_VEC4_H

#include "vec3.h"

typedef struct { float x, y, z, w; } vec4;

vec4 vec4_new(float x, float y, float z, float w);
vec4 vec4_from_vec3(vec3 v, float w);
vec4 vec4_add(vec4 a, vec4 b);
vec4 vec4_sub(vec4 a, vec4 b);
vec4 vec4_scale(vec4 a, float s);
float vec4_dot(vec4 a, vec4 b);
vec4 vec4_normalize(vec4 a);

#endif
