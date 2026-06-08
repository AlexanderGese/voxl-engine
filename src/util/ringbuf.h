#ifndef UTIL_RINGBUF_H
#define UTIL_RINGBUF_H

// fixed capacity ring buffer of void*. cheap queue.

#include <stddef.h>

typedef struct {
    void **data;
    size_t cap;
    size_t head;
    size_t tail;
} ringbuf;

void   ringbuf_init(ringbuf *r, size_t cap);
void   ringbuf_free(ringbuf *r);
int    ringbuf_push(ringbuf *r, void *v);   // 0 on success, -1 if full
void  *ringbuf_pop(ringbuf *r);             // NULL if empty
size_t ringbuf_len(const ringbuf *r);
int    ringbuf_empty(const ringbuf *r);

#endif
