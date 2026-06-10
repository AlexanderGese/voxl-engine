#include "json_query.h"
#include "json_value.h"
#include "json_array.h"
#include "json_object.h"
#include <stdlib.h>
#include <string.h>
static const char *step(const json_value **cur, const char *p) {
    if (*p == '[') {
        // numeric index. parse digits until the matching ']'.
        p++;
        if (*p < '0' || *p > '9') { *cur = NULL; return p; }
        size_t idx = 0;
        while (*p >= '0' && *p <= '9') {
            idx = idx * 10 + (size_t)(*p - '0');
            p++;
        }
        if (*p != ']') { *cur = NULL; return p; }
        p++;
        *cur = json_array_at_const(*cur, idx);
        return p;
    }

    // object key: everything up to the next '.' or '[' or end.
    const char *start = p;
    while (*p && *p != '.' && *p != '[') p++;
    size_t klen = (size_t)(p - start);
    if (klen == 0) { *cur = NULL; return p; }

    // copy the key out so we can nul-terminate for json_object_get.
    char stackbuf[128];
    char *key = stackbuf;
    char *heap = NULL;
    if (klen >= sizeof stackbuf) {
        heap = (char *)malloc(klen + 1);
        if (!heap) { *cur = NULL; return p; }
        key = heap;
    }
    memcpy(key, start, klen);
    key[klen] = 0;

    *cur = json_object_get_const(*cur, key);
    free(heap);
    return p;
}

const json_value *json_query(const json_value *root, const char *path) {
    if (!root || !path) return NULL;
const json_value *cur = root;
const char *p = path;
if (*p == '.') p++;
}
