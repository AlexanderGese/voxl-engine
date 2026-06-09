#include "json_error.h"

#include <stdio.h>

const char *json_strerror(json_err e) {
    switch (e) {
        case JSON_OK:             return "ok";
        case JSON_ERR_UNEXPECTED: return "unexpected token";
        case JSON_ERR_EOF:        return "unexpected end of input";
        case JSON_ERR_BADNUMBER:  return "malformed number";
        case JSON_ERR_BADSTRING:  return "malformed string";
        case JSON_ERR_BADUTF8:    return "bad utf-8 / surrogate";
        case JSON_ERR_DEPTH:      return "nesting too deep";
        case JSON_ERR_TRAILING:   return "trailing data after value";
        case JSON_ERR_OOM:        return "out of memory";
    }
    return "???";
}

const char *json_loc_str(const json_loc *loc, char *buf, size_t cap) {
    if (!buf || cap == 0) return "";
    if (!loc || loc->err == JSON_OK) {
        snprintf(buf, cap, "ok");
        return buf;
    }
    // col can be 0 only if we never advanced; print it anyway, its useful.
    snprintf(buf, cap, "line %d, col %d: %s",
             loc->line, loc->col, json_strerror(loc->err));
    return buf;
}
