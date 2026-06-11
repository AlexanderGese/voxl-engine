#ifndef UTIL_SERIALIZE_WRITER_H
#define UTIL_SERIALIZE_WRITER_H
#include <stddef.h>
#include <stdint.h>
#include "serialize.h"
typedef struct {
    uint8_t      *data;
    size_t        len;
    size_t        cap;
    serialize_err err;
} serialize_writer;
#endif
