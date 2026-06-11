#ifndef UTIL_SERIALIZE_MATH_H
#define UTIL_SERIALIZE_MATH_H

// serializers for the math types. these are pure component-wise f32 dumps,
// no tags, no version. if you want versioned/tagged storage wrap them in a
// blob (serialize_tag.h). keeping them dumb means the mesher cache and the
// entity save code can share the exact same bytes.

#include "serialize_writer.h"
#include "serialize_reader.h"

#include "../../math/vec3.h"
#include "../../math/vec4.h"
#include "../../math/vec2.h"
#include "../../math/mat4.h"
#include "../../math/aabb.h"

void serialize_put_vec2(serialize_writer *w, vec2 v);
vec2 serialize_get_vec2(serialize_reader *r);

void serialize_put_vec3(serialize_writer *w, vec3 v);
vec3 serialize_get_vec3(serialize_reader *r);

void serialize_put_vec4(serialize_writer *w, vec4 v);
vec4 serialize_get_vec4(serialize_reader *r);

// column-major, same order mat4 stores. 16 floats.
void serialize_put_mat4(serialize_writer *w, const mat4 *m);
mat4 serialize_get_mat4(serialize_reader *r);

void serialize_put_aabb(serialize_writer *w, aabb a);
aabb serialize_get_aabb(serialize_reader *r);

// quantized vec3: pack each component into an i16 over [-range, range].
// loses precision on purpose. used for velocities / normals where 1/2^15 of
// the range is plenty and we want a third of the bytes.
void serialize_put_vec3_q16(serialize_writer *w, vec3 v, float range);
vec3 serialize_get_vec3_q16(serialize_reader *r, float range);

#endif
