#ifndef UTIL_JSON_VALUE_H
#define UTIL_JSON_VALUE_H

// value constructors, type checks and leaf accessors. the array/object
// container ops live in json_array.h / json_object.h so this file stays about
// individual values and not about collections.

#include "json_types.h"

// constructors. these return values by copy (the value is small). strings are
// duplicated onto the heap so the caller keeps ownership of whatever they
// passed in. on oom a string ctor falls back to JSON_NULL rather than crash.
json_value json_null(void);
json_value json_bool(int b);
json_value json_number(double n);
json_value json_int(long long n);     // sets is_int so it writes back clean
json_value json_string(const char *s);
json_value json_string_n(const char *s, size_t n);

// recursively free a value's owned memory and leave it as JSON_NULL. safe to
// call on a value you got back by copy - it frees the heap stuff (str/arr/obj),
// the little struct itself is the caller's.
void json_free(json_value *v);

// deep copy. allocates; the result is independent of the source. returns a
// JSON_NULL on oom partway through (and frees what it made).
json_value json_clone(const json_value *v);

// type predicates. cheap, just compare the tag.
static inline int json_is_null  (const json_value *v) { return v && v->kind == JSON_NULL;   }
static inline int json_is_bool  (const json_value *v) { return v && v->kind == JSON_BOOL;   }
static inline int json_is_number(const json_value *v) { return v && v->kind == JSON_NUMBER; }
static inline int json_is_string(const json_value *v) { return v && v->kind == JSON_STRING; }
static inline int json_is_array (const json_value *v) { return v && v->kind == JSON_ARRAY;  }
static inline int json_is_object(const json_value *v) { return v && v->kind == JSON_OBJECT; }

// leaf accessors with a default. these never fault: wrong-kind or NULL returns
// the supplied fallback. that's deliberate - reading config shouldnt need a
// type check on every line.
int          json_as_bool  (const json_value *v, int fallback);
double       json_as_number(const json_value *v, double fallback);
long long    json_as_int   (const json_value *v, long long fallback);
const char  *json_as_string(const json_value *v, const char *fallback);

#endif
