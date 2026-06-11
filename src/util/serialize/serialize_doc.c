#include "serialize_doc.h"
#include "../log.h"
#include "../../config.h"

void serialize_doc_begin(serialize_writer *w, uint16_t user_ver) {
    serialize_put_u32(w, SAVE_MAGIC);
    serialize_put_u16(w, (uint16_t)SERIALIZE_ABI);
    serialize_put_u16(w, user_ver);
    serialize_put_u8(w, 1);              // little endian on the wire, always
    serialize_put_u8(w, 0);
    serialize_put_u8(w, 0);
    serialize_put_u8(w, 0);
}

void serialize_doc_finish(serialize_writer *w) {
    // sentinel section so a streaming reader knows it reached a clean stop
    // and didnt just run out of bytes mid-file.
    serialize_tag_scope s = serialize_tag_begin(w, SERIALIZE_DOC_TAG_END, 0,
                                                SERIALIZE_TAG_F_NONE);
    serialize_tag_end(w, &s);
}

int serialize_doc_open(serialize_reader *r, serialize_doc_hdr *out) {
    out->magic    = serialize_get_u32(r);
    out->format   = serialize_get_u16(r);
    out->user_ver = serialize_get_u16(r);
    uint8_t endian = serialize_get_u8(r);
    serialize_skip(r, 3);                // reserved
    if (r->err != SERIALIZE_OK) {
        LOGW("serialize: doc header truncated");
        return -1;
    }

    if (out->magic != SAVE_MAGIC) {
        LOGW("serialize: bad magic %08x (want %08x)", out->magic, SAVE_MAGIC);
        r->err = SERIALIZE_ERR_BADTAG;
        return -1;
    }
    if (out->format > SERIALIZE_ABI) {
        LOGW("serialize: doc format %u newer than we speak (%u)",
             out->format, SERIALIZE_ABI);
        r->err = SERIALIZE_ERR_BADVERSION;
        return -1;
    }
    out->little_endian = (endian == 1);
    // we only ever emit little-endian, but if some tool wrote big we have no
    // path to swap on the fly, so refuse rather than silently mangle.
    if (!out->little_endian) {
        LOGW("serialize: doc claims big-endian, unsupported");
        r->err = SERIALIZE_ERR_BADVERSION;
        return -1;
    }
    return 0;
}

int serialize_doc_at_end(serialize_reader *r) {
    if (r->left == 0) return 1;          // ran out, treat as end
    // peek a tag without consuming the stream permanently
    serialize_reader probe = *r;
    serialize_tag_hdr h;
    if (serialize_tag_next(&probe, &h) != 0) return 1;
    return h.tag == SERIALIZE_DOC_TAG_END;
}
