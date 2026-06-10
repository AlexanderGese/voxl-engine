#ifndef UTIL_JSON_ERROR_H
#define UTIL_JSON_ERROR_H
// error reporting. nothing fancy: a result enum plus a position so the caller
// can print "line 4, col 12: bad escape" instead of just shrugging.
#include "json_types.h"
// where in the source a parse died. line/col are 1-based, byte is 0-based off
typedef struct {
    json_err err;
    int      line;
    int      col;
    size_t   byte;
} json_loc;
const char *json_strerror(json_err e);
const char *json_loc_str(const json_loc *loc, char *buf, size_t cap);
#endif
