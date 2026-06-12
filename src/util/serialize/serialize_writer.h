#ifndef UTIL_SERIALIZE_WRITER_H
#define UTIL_SERIALIZE_WRITER_H

#include <stddef.h>
#include <stdint.h>

#include "serialize.h"

// append-only byte writer. owns a malloc'd buffer that doubles when full.
// every primitive goes out little-endian, byte by byte, so a save written on
// a big-endian box reads fine on a little-endian one and vice versa.
//
// once err != OK every put is a silent no-op. check w->err at the end.

typedef struct {
    uint8_t      *data;
    size_t        len;
    size_t        cap;
    serialize_err err;
} serialize_writer;

void serialize_writer_init(serialize_writer *w);
void serialize_writer_init_cap(serialize_writer *w, size_t cap);
void serialize_writer_free(serialize_writer *w);

// hand the buffer to the caller and reset the writer to empty. caller frees.
// returns NULL (and frees internally) if the writer is in an error state.
uint8_t *serialize_writer_take(serialize_writer *w, size_t *out_len);

// raw bytes. used by the higher layers and the blob copies.
void serialize_put_bytes(serialize_writer *w, const void *p, size_t n);

// fixed width little-endian primitives.
void serialize_put_u8 (serialize_writer *w, uint8_t  v);
void serialize_put_u16(serialize_writer *w, uint16_t v);
void serialize_put_u32(serialize_writer *w, uint32_t v);
void serialize_put_u64(serialize_writer *w, uint64_t v);
void serialize_put_i8 (serialize_writer *w, int8_t  v);
void serialize_put_i16(serialize_writer *w, int16_t v);
void serialize_put_i32(serialize_writer *w, int32_t v);
void serialize_put_i64(serialize_writer *w, int64_t v);

// floats go out as their ieee-754 bit pattern in a u32/u64. this is the only
// portable way; we are not going to support a host without ieee floats.
void serialize_put_f32(serialize_writer *w, float  v);
void serialize_put_f64(serialize_writer *w, double v);

// varint, leb128 unsigned. cheap for the small counts we write everywhere.
void serialize_put_varint(serialize_writer *w, uint64_t v);

// length-prefixed (varint) byte string. n==0 writes just the prefix.
void serialize_put_blob(serialize_writer *w, const void *p, size_t n);

// length-prefixed (varint) c-string, no trailing nul stored.
void serialize_put_str(serialize_writer *w, const char *s);

// --- patch points --------------------------------------------------------
// reserve a u32 now, get its offset, fill it in later once you know the value.
// used for total-length fields you cant compute up front.
size_t serialize_writer_reserve_u32(serialize_writer *w);
void   serialize_writer_patch_u32(serialize_writer *w, size_t at, uint32_t v);

#endif
