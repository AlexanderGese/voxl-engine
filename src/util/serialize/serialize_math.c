#include "serialize_math.h"

void serialize_put_vec2(serialize_writer *w, vec2 v) {
    serialize_put_f32(w, v.x);
    serialize_put_f32(w, v.y);
}

vec2 serialize_get_vec2(serialize_reader *r) {
    vec2 v;
    v.x = serialize_get_f32(r);
    v.y = serialize_get_f32(r);
    return v;
}

void serialize_put_vec3(serialize_writer *w, vec3 v) {
    serialize_put_f32(w, v.x);
    serialize_put_f32(w, v.y);
    serialize_put_f32(w, v.z);
}

vec3 serialize_get_vec3(serialize_reader *r) {
    vec3 v;
    v.x = serialize_get_f32(r);
    v.y = serialize_get_f32(r);
    v.z = serialize_get_f32(r);
    return v;
}

void serialize_put_vec4(serialize_writer *w, vec4 v) {
    serialize_put_f32(w, v.x);
    serialize_put_f32(w, v.y);
    serialize_put_f32(w, v.z);
    serialize_put_f32(w, v.w);
}

vec4 serialize_get_vec4(serialize_reader *r) {
    vec4 v;
    v.x = serialize_get_f32(r);
    v.y = serialize_get_f32(r);
    v.z = serialize_get_f32(r);
    v.w = serialize_get_f32(r);
    return v;
}

void serialize_put_mat4(serialize_writer *w, const mat4 *m) {
    // column major, matches mat4_data layout exactly
    for (int c = 0; c < 4; c++)
        for (int r = 0; r < 4; r++)
            serialize_put_f32(w, m->m[c][r]);
}

mat4 serialize_get_mat4(serialize_reader *r) {
    mat4 m;
    for (int c = 0; c < 4; c++)
        for (int rr = 0; rr < 4; rr++)
            m.m[c][rr] = serialize_get_f32(r);
    return m;
}

void serialize_put_aabb(serialize_writer *w, aabb a) {
    serialize_put_vec3(w, a.min);
    serialize_put_vec3(w, a.max);
}

aabb serialize_get_aabb(serialize_reader *r) {
    aabb a;
    a.min = serialize_get_vec3(r);
    a.max = serialize_get_vec3(r);
    return a;
}

static int16_t quantize(float x, float range) {
    if (range <= 0.0f) return 0;
    float t = x / range;
    if (t >  1.0f) t =  1.0f;
    if (t < -1.0f) t = -1.0f;
    // round-to-nearest into a signed 15-bit-ish range
    float s = t * 32767.0f;
    return (int16_t)(s >= 0 ? s + 0.5f : s - 0.5f);
}

void serialize_put_vec3_q16(serialize_writer *w, vec3 v, float range) {
    serialize_put_i16(w, quantize(v.x, range));
    serialize_put_i16(w, quantize(v.y, range));
    serialize_put_i16(w, quantize(v.z, range));
}

vec3 serialize_get_vec3_q16(serialize_reader *r, float range) {
    float inv = range / 32767.0f;
    vec3 v;
    v.x = (float)serialize_get_i16(r) * inv;
    v.y = (float)serialize_get_i16(r) * inv;
    v.z = (float)serialize_get_i16(r) * inv;
    return v;
}
