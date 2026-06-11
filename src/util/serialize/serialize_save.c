#include "serialize_save.h"
#include "serialize_doc.h"
#include "serialize_tag.h"
#include "serialize_chunk.h"
#include "../file.h"
#include "../log.h"

#include <stdlib.h>
#include <string.h>

#define META_VERSION 1

static void write_meta(serialize_writer *w, const serialize_save_meta *m) {
    serialize_tag_scope s = serialize_tag_begin(w, SERIALIZE_FOURCC('M','E','T','A'),
                                                META_VERSION, SERIALIZE_TAG_F_CRC);
    serialize_put_u64(w, m->seed);
    serialize_put_f32(w, m->spawn_x);
    serialize_put_f32(w, m->spawn_y);
    serialize_put_f32(w, m->spawn_z);
    serialize_put_f64(w, m->play_seconds);
    serialize_put_u32(w, m->chunk_count);
    serialize_tag_end(w, &s);
}

static int read_meta(serialize_reader *r, serialize_save_meta *m) {
    serialize_tag_hdr h;
    if (serialize_tag_find(r, SERIALIZE_FOURCC('M','E','T','A'), &h) != 0) {
        LOGW("serialize: no META section");
        return -1;
    }
    if (h.version > META_VERSION) {
        LOGW("serialize: META version %u too new", h.version);
        return -1;
    }
    m->seed         = serialize_get_u64(r);
    m->spawn_x      = serialize_get_f32(r);
    m->spawn_y      = serialize_get_f32(r);
    m->spawn_z      = serialize_get_f32(r);
    m->play_seconds = serialize_get_f64(r);
    m->chunk_count  = serialize_get_u32(r);
    return r->err == SERIALIZE_OK ? 0 : -1;
}

// --- builder -------------------------------------------------------------

void serialize_save_open(serialize_save *s, const serialize_save_meta *meta) {
    serialize_writer_init_cap(&s->w, 1 << 16);   // 64k, a save grows fast
    s->committed = 0;
    serialize_doc_begin(&s->w, SERIALIZE_SAVE_USER_VERSION);
    write_meta(&s->w, meta);
}

void serialize_save_add_chunk(serialize_save *s, const chunk *c, int with_light) {
    serialize_chunk_write(&s->w, c, with_light);
}

// splice a pre-built section writer in verbatim. lets callers compose item /
// container sections elsewhere and dump them here without re-encoding.
void serialize_save_add_raw(serialize_save *s, serialize_writer *section_src) {
    if (section_src->err != SERIALIZE_OK) {
        s->w.err = section_src->err;
        return;
    }
    serialize_put_bytes(&s->w, section_src->data, section_src->len);
}

int serialize_save_commit(serialize_save *s, const char *path) {
    serialize_doc_finish(&s->w);
    if (s->w.err != SERIALIZE_OK) {
        LOGE("serialize: save build failed: %s", serialize_strerror(s->w.err));
        serialize_writer_free(&s->w);
        return -1;
    }
    size_t len;
    uint8_t *data = serialize_writer_take(&s->w, &len);
    if (!data) return -1;

    int rc = file_write_all(path, data, len);
    free(data);
    if (rc != 0) {
        LOGE("serialize: cant write save %s", path);
        return -1;
    }
    s->committed = 1;
    LOGI("serialize: wrote %zu bytes to %s", len, path);
    return 0;
}

void serialize_save_abort(serialize_save *s) {
    serialize_writer_free(&s->w);
}

// --- loading -------------------------------------------------------------

int serialize_save_load(const char *path, serialize_save_meta *meta,
                        uint8_t **out_data, size_t *out_len, size_t *body_at) {
    size_t len = 0;
    char *raw = file_read_all(path, &len);
    if (!raw) return -1;

    serialize_reader r;
    serialize_reader_init(&r, raw, len);

    serialize_doc_hdr hdr;
    if (serialize_doc_open(&r, &hdr) != 0) {
        LOGE("serialize: %s is not a valid save (%s)", path,
             serialize_strerror(r.err));
        free(raw);
        return -1;
    }
    if (read_meta(&r, meta) != 0) {
        free(raw);
        return -1;
    }

    *out_data = (uint8_t *)raw;
    *out_len  = len;
    // start section iteration right after the doc header. callers re-scan
    // from here (the META find above ran on this same reader, but the helper
    // each_chunk reopens its own reader at body_at so order doesnt matter).
    // header is magic(4)+format(2)+user(2)+endian(1)+reserved(3) = 12 bytes.
    *body_at  = 12;
    return 0;
}

int serialize_save_each_chunk(const uint8_t *data, size_t len, size_t body_at,
                              void (*cb)(const chunk *c, void *ud), void *ud) {
    serialize_reader r;
    serialize_reader_init(&r, data + body_at, len - body_at);

    int count = 0;
    serialize_tag_hdr h;
    while (serialize_tag_next(&r, &h) == 0) {
        if (h.tag == SERIALIZE_DOC_TAG_END) break;
        if (h.tag == SERIALIZE_FOURCC('C','H','N','K')) {
            chunk c;
            memset(&c, 0, sizeof c);
            if (serialize_chunk_read(&r, &c) == 0) {
                if (cb) cb(&c, ud);
                count++;
                // chunk_read consumed exactly the block/light streams; let the
                // tag layer reconcile the body length + crc.
                if (serialize_tag_skip_body(&r, &h) != 0) return -1;
            } else {
                return -1;
            }
        } else {
            if (serialize_tag_skip_body(&r, &h) != 0) return -1;
        }
    }
    return r.err == SERIALIZE_OK ? count : -1;
}
