#include "json_merge.h"
#include "json_value.h"
#include "json_array.h"
#include "json_object.h"

#include <math.h>
#include <string.h>

#include "../darray.h"

int json_merge(json_value *target, const json_value *patch) {
    if (!target || !patch) return 0;

    // patch isnt an object -> it replaces the target outright. clone so the
    // patch stays owned by its caller.
    if (patch->kind != JSON_OBJECT) {
        json_value c = json_clone(patch);
        json_free(target);
        *target = c;
        return 1;
    }

    // patch is an object but target isnt -> make target an empty object first,
    // then fall through to the key-by-key merge below.
    if (target->kind != JSON_OBJECT) {
        json_free(target);
        *target = json_object();
    }

    size_t n = json_object_len(patch);
    for (size_t i = 0; i < n; i++) {
        const char *key; const json_value *pv;
        if (!json_object_at(patch, i, &key, &pv)) continue;

        // a null patch value means "remove this key from the target".
        if (pv->kind == JSON_NULL) {
            json_object_remove(target, key);
            continue;
        }

        json_value *tv = json_object_get(target, key);
        if (tv && tv->kind == JSON_OBJECT && pv->kind == JSON_OBJECT) {
            // both objects: recurse so nested overrides compose.
            if (!json_merge(tv, pv)) return 0;
        } else {
            // otherwise set/replace with a clone of the patch value.
            json_value c = json_clone(pv);
            json_object_set(target, key, c);
        }
    }
    return 1;
}

json_value json_merged(const json_value *base, const json_value *patch) {
    json_value out = json_clone(base);
    if (patch) json_merge(&out, patch);
    return out;
}

int json_equal(const json_value *a, const json_value *b) {
    if (a == b) return 1;
    if (!a || !b) return 0;
    if (a->kind != b->kind) return 0;

    switch (a->kind) {
        case JSON_NULL:
            return 1;
        case JSON_BOOL:
            return (a->as.boolean != 0) == (b->as.boolean != 0);
        case JSON_NUMBER:
            // exact bit equality is too strict for floats that came from
            // different text; compare values directly. nan never equals nan,
            // which matches every other == in C.
            return a->as.number == b->as.number;
        case JSON_STRING:
            return strcmp(a->as.str ? a->as.str : "",
                          b->as.str ? b->as.str : "") == 0;
        case JSON_ARRAY: {
            size_t na = darr_len(a->as.arr), nb = darr_len(b->as.arr);
            if (na != nb) return 0;
            for (size_t i = 0; i < na; i++) {
                if (!json_equal(&a->as.arr[i], &b->as.arr[i])) return 0;
            }
            return 1;
        }
        case JSON_OBJECT: {
            size_t na = json_object_len(a), nb = json_object_len(b);
            if (na != nb) return 0;
            // key order doesnt matter: for each member of a, find the matching
            // key in b and compare. counts match so this also rules out dups.
            for (size_t i = 0; i < na; i++) {
                const char *k; const json_value *av;
                json_object_at(a, i, &k, &av);
                const json_value *bv = json_object_get_const(b, k);
                if (!bv || !json_equal(av, bv)) return 0;
            }
            return 1;
        }
    }
    return 0;
}
