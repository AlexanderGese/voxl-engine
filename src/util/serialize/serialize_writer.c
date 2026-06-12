#include "serialize_writer.h"

#include <stdlib.h>
#include <string.h>

#define WRITER_MIN_CAP 64

void serialize_writer_init(serialize_writer *w) {
    serialize_writer_init_cap(w, 0);
}

void serialize_writer_init_cap(serialize_writer *w, size_t cap) {
    w->data = NULL;
    w->len  = 0;
    w->cap  = 0;
    w->err  = SERIALIZE_OK;
    if (cap) {
        w->data = malloc(cap);
        if (!w->data) { w->err = SERIALIZE_ERR_OOM; return; }
        w->cap = cap;
    }
}

void serialize_writer_free(serialize_writer *w) {
    free(w->data);
    w->data = NULL;
    w->len = w->cap = 0;
    w->err = SERIALIZE_OK;
}

static int writer_ensure(serialize_writer *w, size_t extra) {
    if (w->err != SERIALIZE_OK) return 0;
    if (w->len + extra <= w->cap) return 1;

    size_t nc = w->cap ? w->cap : WRITER_MIN_CAP;
    while (nc < w->len + extra) {
        // guard against the doubling wrapping around on huge inputs
        if (nc > (SIZE_MAX >> 1)) { w->err = SERIALIZE_ERR_OOM; return 0; }
        nc *= 2;
    }
    uint8_t *nd = realloc(w->data, nc);
    if (!nd) { w->err = SERIALIZE_ERR_OOM; return 0; }
    w->data = nd;
    w->cap  = nc;
    return 1;
}

uint8_t *serialize_writer_take(serialize_writer *w, size_t *out_len) {
    if (w->err != SERIALIZE_OK) {
        serialize_writer_free(w);
        if (out_len) *out_len = 0;
        return NULL;
    }
    uint8_t *d = w->data;
    if (out_len) *out_len = w->len;
    w->data = NULL;
    w->len = w->cap = 0;
    return d;
}

void serialize_put_bytes(serialize_writer *w, const void *p, size_t n) {
    if (!writer_ensure(w, n)) return;
    memcpy(w->data + w->len, p, n);
    w->len += n;
}

void serialize_put_u8(serialize_writer *w, uint8_t v) {
    if (!writer_ensure(w, 1)) return;
    w->data[w->len++] = v;
}

void serialize_put_u16(serialize_writer *w, uint16_t v) {
    if (!writer_ensure(w, 2)) return;
    w->data[w->len++] = (uint8_t)(v);
    w->data[w->len++] = (uint8_t)(v >> 8);
}

void serialize_put_u32(serialize_writer *w, uint32_t v) {
    if (!writer_ensure(w, 4)) return;
    w->data[w->len++] = (uint8_t)(v);
    w->data[w->len++] = (uint8_t)(v >> 8);
    w->data[w->len++] = (uint8_t)(v >> 16);
    w->data[w->len++] = (uint8_t)(v >> 24);
}

void serialize_put_u64(serialize_writer *w, uint64_t v) {
    if (!writer_ensure(w, 8)) return;
    for (int i = 0; i < 8; i++) w->data[w->len++] = (uint8_t)(v >> (i * 8));
}

void serialize_put_i8 (serialize_writer *w, int8_t  v) { serialize_put_u8 (w, (uint8_t)v); }
void serialize_put_i16(serialize_writer *w, int16_t v) { serialize_put_u16(w, (uint16_t)v); }
void serialize_put_i32(serialize_writer *w, int32_t v) { serialize_put_u32(w, (uint32_t)v); }
void serialize_put_i64(serialize_writer *w, int64_t v) { serialize_put_u64(w, (uint64_t)v); }

void serialize_put_f32(serialize_writer *w, float v) {
    uint32_t bits;
    memcpy(&bits, &v, 4);          // type-pun through memcpy, never a cast
    serialize_put_u32(w, bits);
}

void serialize_put_f64(serialize_writer *w, double v) {
    uint64_t bits;
    memcpy(&bits, &v, 8);
    serialize_put_u64(w, bits);
}

void serialize_put_varint(serialize_writer *w, uint64_t v) {
    // 7 bits per byte, high bit = continuation. at most 10 bytes for a u64.
    uint8_t tmp[10];
    int n = 0;
    do {
        uint8_t b = (uint8_t)(v & 0x7F);
        v >>= 7;
        if (v) b |= 0x80;
        tmp[n++] = b;
    } while (v);
    serialize_put_bytes(w, tmp, (size_t)n);
}

void serialize_put_blob(serialize_writer *w, const void *p, size_t n) {
    serialize_put_varint(w, (uint64_t)n);
    if (n) serialize_put_bytes(w, p, n);
}

void serialize_put_str(serialize_writer *w, const char *s) {
    size_t n = s ? strlen(s) : 0;
    serialize_put_blob(w, s, n);
}

size_t serialize_writer_reserve_u32(serialize_writer *w) {
    size_t at = w->len;
    serialize_put_u32(w, 0);
    return at;
}

void serialize_writer_patch_u32(serialize_writer *w, size_t at, uint32_t v) {
    if (w->err != SERIALIZE_OK) return;
    if (at + 4 > w->len) { w->err = SERIALIZE_ERR_CORRUPT; return; }
    w->data[at + 0] = (uint8_t)(v);
    w->data[at + 1] = (uint8_t)(v >> 8);
    w->data[at + 2] = (uint8_t)(v >> 16);
    w->data[at + 3] = (uint8_t)(v >> 24);
}
