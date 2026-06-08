#include "ringbuf.h"

#include <stdlib.h>

void ringbuf_init(ringbuf *r, size_t cap) {
    if (cap < 2) cap = 2;
    // round to power of two for cheap modulo
    size_t c = 1;
    while (c < cap) c <<= 1;
    r->data = malloc(c * sizeof(void*));
    r->cap  = c;
    r->head = 0;
    r->tail = 0;
}

void ringbuf_free(ringbuf *r) {
    free(r->data);
    r->data = NULL;
    r->cap = r->head = r->tail = 0;
}

int ringbuf_push(ringbuf *r, void *v) {
    size_t next = (r->tail + 1) & (r->cap - 1);
    if (next == r->head) return -1;
    r->data[r->tail] = v;
    r->tail = next;
    return 0;
}

void *ringbuf_pop(ringbuf *r) {
    if (r->head == r->tail) return NULL;
    void *v = r->data[r->head];
    r->head = (r->head + 1) & (r->cap - 1);
    return v;
}

size_t ringbuf_len(const ringbuf *r) {
    return (r->tail - r->head) & (r->cap - 1);
}

int ringbuf_empty(const ringbuf *r) { return r->head == r->tail; }
