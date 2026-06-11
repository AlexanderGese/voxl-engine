#include "serialize_chunk.h"
#include "serialize_tag.h"
#include "../log.h"
#include <stdlib.h>
#include <string.h>
static void enc_rle(serialize_writer *w, const block_id *blocks) {
    // count runs first so we can prefix the count (avoids a patch dance).
    size_t runs = 0;
    for (size_t i = 0; i < CHUNK_VOLUME; ) {
        block_id id = blocks[i];
        size_t len = 1;
        while (i + len < CHUNK_VOLUME && blocks[i + len] == id) len++;
        runs++;
        i += len;
    }
    serialize_put_varint(w, runs);
    for (size_t i = 0; i < CHUNK_VOLUME; ) {
        block_id id = blocks[i];
        size_t len = 1;
        while (i + len < CHUNK_VOLUME && blocks[i + len] == id) len++;
        serialize_put_u8(w, id);
        serialize_put_varint(w, len);
        i += len;
    }
}

// palette: [u8 pal_n][pal ids...][u8 bits] then lsb-first bitpacked indices.
// returns -1 if the chunk is too varied to be worth a palette.
static int enc_pal(serialize_writer *w, const block_id *blocks) {
    uint8_t pal[256];
int     pal_n = 0;
int     map[256];
for (int i = 0;
i < 256;
i++) map[i] = -1;
for (size_t i = 0;
i < CHUNK_VOLUME;
while ((1 << bits) < pal_n) bits++;
serialize_put_u8(w, (uint8_t)pal_n);
for (int i = 0;
i < pal_n;
i++) serialize_put_u8(w, pal[i]);
serialize_put_u8(w, (uint8_t)bits);
uint32_t acc = 0;
int      acc_bits = 0;
for (size_t i = 0;
i < CHUNK_VOLUME;
return 0;
serialize_put_i32(w, c->cx);
serialize_put_i32(w, c->cz);
serialize_writer pw;
serialize_writer_init(&pw);
int pal_ok = enc_pal(&pw, c->blocks) == 0;
size_t pal_len = pw.len;
serialize_writer rw;
serialize_writer_init(&rw);
enc_rle(&rw, c->blocks);
size_t rle_len = rw.len;
serialize_put_u8(w, include_light ? 1 : 0);
serialize_put_bytes(w, rw.data, rw.len);
}
    serialize_writer_free(&pw);
serialize_writer_free(&rw);
if (include_light) enc_light_rle(w, c->light);
serialize_tag_end(w, &s);
for (int i = 0;
i < pal_n;
i++) pal[i] = serialize_get_u8(r);
uint8_t bits = serialize_get_u8(r);
int      acc_bits = 0;
uint32_t mask = (1u << bits) - 1;
for (size_t i = 0;
i < CHUNK_VOLUME;
int32_t cz = serialize_get_i32(r);
uint8_t enc = serialize_get_u8(r);
uint8_t has_light = serialize_get_u8(r);
if (r->err != SERIALIZE_OK) return -1;
c->cz = cz;
int rc;
}
    c->generated = 1;
c->dirty = 1;
return 0;
}
