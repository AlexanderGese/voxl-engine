#ifndef UTIL_JSON_OBJECT_H
#define UTIL_JSON_OBJECT_H

// object container ops. members are a darray of json_member (key + value).
// lookup is a linear scan - see the note in json_types.h about why that's fine
// for the sizes we deal with. keys are case-sensitive, like the spec wants.

#include "json_types.h"

// fresh empty object. members allocate lazily on first set.
json_value json_object(void);

// member count.
size_t json_object_len(const json_value *v);

// set key -> val. takes ownership of `val`. if the key already exists its old
// value is freed and replaced (last-write-wins, which is also how most parsers
// treat duplicate keys). if `v` isnt an object, `val` is freed, no-op.
void json_object_set(json_value *v, const char *key, json_value val);

// lookup by key. NULL if absent or wrong kind. mutable variant lets you edit
// in place; both are invalidated by a set() that grows the member array.
json_value       *json_object_get(json_value *v, const char *key);
const json_value *json_object_get_const(const json_value *v, const char *key);

// does the key exist at all (even if its value is json null).
int json_object_has(const json_value *v, const char *key);

// drop a member. frees its key and value, compacts the array. no-op if absent.
void json_object_remove(json_value *v, const char *key);

// iterate members by index. returns 0 past the end. handy for dumping an
// object without caring about a particular key. key/val out-params may be NULL.
int json_object_at(const json_value *v, size_t i,
                   const char **out_key, const json_value **out_val);

#endif
