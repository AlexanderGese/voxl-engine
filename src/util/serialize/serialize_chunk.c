#include "serialize_chunk.h"
#include "serialize_tag.h"
#include "../log.h"
#include <stdlib.h>
#include <string.h>
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
