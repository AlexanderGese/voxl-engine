#ifndef UTIL_SERIALIZE_DOC_H
#define UTIL_SERIALIZE_DOC_H

// the outermost wrapper around a save: a fixed header then a stream of tagged
// sections. the header reuses SAVE_MAGIC / SAVE_VERSION from config.h so the
// loader and this code agree on what "a voxl save" is.
//
// u32  magic     (SAVE_MAGIC, 'VOXL')
// u16  format    (SERIALIZE_ABI, bumps when section framing changes)
// u16  user_ver  (SAVE_VERSION, the gameplay-level save version)
// u8   endian    (1 = little, the only value we ever write)
// u8   reserved[3]
// ...  sections (serialize_tag.h)
// 'END' sentinel section, zero body
//
// open() validates the header and leaves the reader parked at the first
// section. begin()/finish() bracket the writer.

#include "serialize_writer.h"
#include "serialize_reader.h"
#include "serialize_tag.h"

#define SERIALIZE_DOC_TAG_END SERIALIZE_FOURCC('E','N','D','\0')

typedef struct {
    uint32_t magic;
    uint16_t format;
    uint16_t user_ver;
    int      little_endian;
} serialize_doc_hdr;

// writer: stamp the header. caller then writes sections, then calls finish.
void serialize_doc_begin(serialize_writer *w, uint16_t user_ver);
void serialize_doc_finish(serialize_writer *w);

// reader: validate header. returns 0 on success, fills *out. on a bad magic
// or a format from the future this fails rather than guessing.
int  serialize_doc_open(serialize_reader *r, serialize_doc_hdr *out);

// did we hit the END sentinel? call after the last real section is consumed.
int  serialize_doc_at_end(serialize_reader *r);

#endif
