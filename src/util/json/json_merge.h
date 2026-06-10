#ifndef UTIL_JSON_MERGE_H
#define UTIL_JSON_MERGE_H

// deep merge / patch. the engine loads a baked-in default config and then lets
// a user config override pieces of it; rather than writing that overlay logic
// at every call site, it lives here once. semantics follow rfc 7386 (json merge
// patch) closely enough for our needs:
//
// - objects merge key by key, recursing into nested objects
// - a null in the patch deletes the key from the target
// - any non-object patch value (scalar, array) replaces the target value
// wholesale - arrays are not merged element-wise, that way lies madness
//
// the patch is consumed read-only; the target is mutated in place.

#include "json_types.h"

// apply `patch` onto `target` in place. both should be valid values. if target
// isnt an object and patch is, target is replaced by a clone of patch. returns
// 1 on success, 0 on an allocation failure mid-merge (target left usable but
// partially patched - it's config, we move on).
int json_merge(json_value *target, const json_value *patch);

// non-destructive variant: returns a fresh merged value, leaving both inputs
// untouched. handy when you want to keep the defaults around. caller json_frees
// the result. returns JSON_NULL on oom.
json_value json_merged(const json_value *base, const json_value *patch);

// structural equality. two values are equal if same kind and same contents;
// object key order does NOT matter, array order DOES. used by tests and by the
// save code to skip writing config that matches the default.
int json_equal(const json_value *a, const json_value *b);

#endif
