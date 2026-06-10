#ifndef UTIL_JSON_STRING_H
#define UTIL_JSON_STRING_H

// string escaping helpers, shared by the lexer (decode) and the writer
// (encode). these are the only place that knows the \uXXXX / surrogate dance,
// so neither the parser nor the writer has to carry that knowledge around.

#include <stddef.h>
#include <stdint.h>

#include "../strbuf.h"
#include "json_types.h"

// encode one utf-8 codepoint into out (up to 4 bytes). returns bytes written,
// 0 for an out-of-range / surrogate-range codepoint. used when decoding \u.
int json_utf8_encode(uint32_t cp, char out[4]);

// decode the leading utf-8 sequence at s[0..n). on success returns the byte
// length consumed and writes the codepoint to *cp. returns 0 on a malformed or
// truncated sequence. the lexer uses this to validate raw multibyte content.
int json_utf8_decode(const char *s, size_t n, uint32_t *cp);

// append `src` to `out` with json-string escaping (quotes, backslashes,
// control chars -> \uXXXX, the usual short escapes for \n \t etc). does NOT add
// the surrounding quotes - the writer owns those.
void json_escape_into(strbuf *out, const char *src, size_t n);

// decode a json string body (the bytes between the quotes, escapes and all)
// into `out`. on a bad escape or stray control char returns the matching
// json_err and leaves whatever was decoded so far in out. JSON_OK on success.
json_err json_unescape_into(strbuf *out, const char *src, size_t n);

#endif
