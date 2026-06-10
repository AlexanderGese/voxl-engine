#ifndef UTIL_JSON_PARSE_H
#define UTIL_JSON_PARSE_H

// the parser. recursive descent over the token stream from json_token.h,
// building a json_value dom. one entry point; everything below it is static.

#include <stddef.h>

#include "json_types.h"
#include "json_error.h"

// parse a buffer of `len` bytes (need not be nul terminated) into *out.
// on success returns JSON_OK and *out owns a dom the caller must json_free.
// on failure *out is left JSON_NULL and *loc (if non-NULL) carries the spot.
//
// trailing non-whitespace after the top value is an error (JSON_ERR_TRAILING);
// thats usually a sign of two concatenated documents or a stray brace.
json_err json_parse(const char *src, size_t len, json_value *out, json_loc *loc);

// convenience wrapper for a nul-terminated c-string.
json_err json_parse_cstr(const char *src, json_value *out, json_loc *loc);

#endif
