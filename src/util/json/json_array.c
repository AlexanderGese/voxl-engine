#include "json_array.h"
#include "json_value.h"

#include <string.h>

#include "../darray.h"

json_value json_array(void) {
    json_value v = json_null();
    v.kind = JSON_ARRAY;
    v.as.arr = NULL;   // darray allocates on first push
    return v;
}

size_t json_array_len(const json_value *v) {
    if (!v || v->kind != JSON_ARRAY) return 0;
    return darr_len(v->as.arr);
}

void json_array_push(json_value *v, json_value child) {
    if (!v || v->kind != JSON_ARRAY) {
        // wrong target: dont leak the orphan we were handed.
        json_free(&child);
        return;
    }
    darr_push(v->as.arr, child);
}

json_value *json_array_at(json_value *v, size_t i) {
    if (!v || v->kind != JSON_ARRAY) return NULL;
    if (i >= darr_len(v->as.arr)) return NULL;
    return &v->as.arr[i];
}

const json_value *json_array_at_const(const json_value *v, size_t i) {
    if (!v || v->kind != JSON_ARRAY) return NULL;
    if (i >= darr_len(v->as.arr)) return NULL;
    return &v->as.arr[i];
}

void json_array_remove(json_value *v, size_t i) {
    if (!v || v->kind != JSON_ARRAY) return;
    size_t n = darr_len(v->as.arr);
    if (i >= n) return;
    json_free(&v->as.arr[i]);
    // shift the tail left by one. memmove handles the overlap.
    if (i + 1 < n) {
        memmove(&v->as.arr[i], &v->as.arr[i + 1],
                (n - i - 1) * sizeof(json_value));
    }
    darr_hdr(v->as.arr)->len = n - 1;
}
