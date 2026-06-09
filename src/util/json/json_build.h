#ifndef UTIL_JSON_BUILD_H
#define UTIL_JSON_BUILD_H
// little builders for assembling a dom by hand, which the save/export code does
// a lot. without these you end up writing the same json_object() + a pile of
// json_object_set() lines everywhere; these collapse the common shapes (writing
// engine math types out as arrays, mostly) into one call.
//
// these all take a pointer to an object/array value and append to it, returning
// the same pointer so calls chain:
//
// json_value o = json_object();
#include "json_types.h"
#include "../../math/vec3.h"
#include "../../math/vec2.h"
json_value *json_oset_null  (json_value *o, const char *key);
json_value *json_oset_bool  (json_value *o, const char *key, int b);
json_value *json_oset_num   (json_value *o, const char *key, double n);
json_value *json_oset_int   (json_value *o, const char *key, long long n);
json_value *json_oset_str   (json_value *o, const char *key, const char *s);
json_value *json_oset_vec3  (json_value *o, const char *key, vec3 v);
json_value *json_oset_vec2  (json_value *o, const char *key, vec2 v);
json_value *json_apush_num  (json_value *a, double n);
json_value *json_apush_int  (json_value *a, long long n);
json_value *json_apush_str  (json_value *a, const char *s);
vec3 json_read_vec3(const json_value *v, vec3 fallback);
vec2 json_read_vec2(const json_value *v, vec2 fallback);
#endif
