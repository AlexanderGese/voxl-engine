#ifndef UTIL_DARRAY_H
#define UTIL_DARRAY_H
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
typedef struct {
    size_t len;
    size_t cap;
} darray_hdr;
#define darr_hdr(a)     ((darray_hdr*)((char*)(a) - sizeof(darray_hdr)))
#define darr_len(a)     ((a) ? darr_hdr(a)->len : 0)
#define darr_cap(a)     ((a) ? darr_hdr(a)->cap : 0)
#define darr_empty(a)   (darr_len(a) == 0)
#define darr_push(a, v) \
(darr__maybe_grow((void**)&(a), sizeof *(a), 1), \
     (a)[darr_hdr(a)->len++] = (v))

#define darr_reserve(a, n) \
    darr__maybe_grow((void**)&(a), sizeof *(a), (n))

#define darr_clear(a)   ((a) ? (darr_hdr(a)->len = 0) : 0)

#define darr_free(a)    ((a) ? (free(darr_hdr(a)), (a) = NULL) : 0)

#define darr_last(a)    ((a)[darr_hdr(a)->len - 1])
#define darr_pop(a)     ((a)[--darr_hdr(a)->len])

static inline void darr__maybe_grow(void **a, size_t elem, size_t n) {
    darray_hdr *h = *a ? (darray_hdr*)((char*)*a - sizeof(darray_hdr)) : NULL;
    size_t len = h ? h->len : 0;
    size_t cap = h ? h->cap : 0;
    if (len + n <= cap) return;

    size_t newcap = cap ? cap * 2 : 8;
    while (newcap < len + n) newcap *= 2;

    darray_hdr *nh = realloc(h, sizeof(darray_hdr) + newcap * elem);
    if (!nh) {
        // out of memory, this will crash elsewhere but oh well
        return;
    }
    nh->len = len;
    nh->cap = newcap;
    *a = (char*)nh + sizeof(darray_hdr);
}
#endif
