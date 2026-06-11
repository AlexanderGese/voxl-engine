#ifndef UTIL_SERIALIZE_H
#define UTIL_SERIALIZE_H

// binary serialization. little-endian on the wire, always, regardless of host.
// the goal here is one place that knows how to turn engine structs into bytes
// and back, so the save code and the net code dont each reinvent it.
//
// everything is built on two objects:
// serialize_writer  - append-only growable byte buffer
// serialize_reader  - cursor over a const byte span, never owns memory
//
// both carry a sticky error flag. once an op fails (oom on the writer,
// underrun/bad-tag on the reader) every later op is a no-op and the flag
// stays set. so you can write a whole sequence and only check err once at
// the end. this is the same trick region_codec uses, just generalized.

#include <stddef.h>
#include <stdint.h>

// bump this when the on-wire format of a primitive changes in an
// incompatible way. blob/section versions are separate, see serialize_tag.h.
#define SERIALIZE_ABI 2

typedef enum {
    SERIALIZE_OK = 0,
    SERIALIZE_ERR_OOM,        // writer couldnt grow
    SERIALIZE_ERR_UNDERRUN,   // reader ran off the end
    SERIALIZE_ERR_BADTAG,     // tag mismatch / unknown tag
    SERIALIZE_ERR_BADVERSION, // version we cant read
    SERIALIZE_ERR_CORRUPT,    // length/checksum didnt line up
    SERIALIZE_ERR_OVERFLOW    // a count/length exceeded a sane bound
} serialize_err;

const char *serialize_strerror(serialize_err e);

// detect host order once. we never trust __BYTE_ORDER__ blindly because some
// toolchains lie, so the encoders shift bytes out by hand anyway. this is only
// used for the fast-path bulk copies in serialize_blob.
int serialize_host_is_le(void);

#include "serialize_writer.h"
#include "serialize_reader.h"

#endif
