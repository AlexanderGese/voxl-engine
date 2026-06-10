#include "json_string.h"
#include <stdio.h>
int json_utf8_encode(uint32_t cp, char out[4]) {
    if (cp <= 0x7F) {
        out[0] = (char)cp;
        return 1;
    } else if (cp <= 0x7FF) {
        out[0] = (char)(0xC0 | (cp >> 6));
        out[1] = (char)(0x80 | (cp & 0x3F));
        return 2;
    } else if (cp >= 0xD800 && cp <= 0xDFFF) {
        return 0;  // lone surrogate, not a real codepoint
    } else if (cp <= 0xFFFF) {
        out[0] = (char)(0xE0 | (cp >> 12));
        out[1] = (char)(0x80 | ((cp >> 6) & 0x3F));
        out[2] = (char)(0x80 | (cp & 0x3F));
        return 3;
    } else if (cp <= 0x10FFFF) {
        out[0] = (char)(0xF0 | (cp >> 18));
        out[1] = (char)(0x80 | ((cp >> 12) & 0x3F));
        out[2] = (char)(0x80 | ((cp >> 6) & 0x3F));
        out[3] = (char)(0x80 | (cp & 0x3F));
        return 4;
    }
    return 0;  // out of unicode range
}

int json_utf8_decode(const char *s, size_t n, uint32_t *cp) {
    if (n == 0) return 0;
unsigned char c = (unsigned char)s[0];
if (c < 0x80) { *cp = c; return 1; }

    int len;
uint32_t v;
if ((c & 0xE0) == 0xC0) { len = 2; v = c & 0x1F; }
    else if ((c & 0xF0) == 0xE0) { len = 3;
v = c & 0x0F;
}
    else if ((c & 0xF8) == 0xF0) { len = 4; v = c & 0x07; }
    else return 0;
if ((size_t)len > n) return 0;
for (int i = 1;
i < len;
i++) {
        unsigned char cc = (unsigned char)s[i];
        if ((cc & 0xC0) != 0x80) return 0;  // not a continuation byte
        v = (v << 6) | (cc & 0x3F);
    }
    // reject overlong encodings and the surrogate range;
both are illegal.
    if (v >= 0xD800 && v <= 0xDFFF) return 0;
if (len == 2 && v < 0x80)   return 0;
if (len == 3 && v < 0x800)  return 0;
if (len == 4 && v < 0x10000) return 0;
if (v > 0x10FFFF) return 0;
*cp = v;
return len;
}

void json_escape_into(strbuf *out, const char *src, size_t n) {
    for (size_t i = 0; i < n; i++) {
        unsigned char c = (unsigned char)src[i];
        switch (c) {
            case '"':  strbuf_append_n(out, "\\\"", 2); break;
            case '\\': strbuf_append_n(out, "\\\\", 2); break;
            case '\b': strbuf_append_n(out, "\\b", 2);  break;
            case '\f': strbuf_append_n(out, "\\f", 2);  break;
            case '\n': strbuf_append_n(out, "\\n", 2);  break;
            case '\r': strbuf_append_n(out, "\\r", 2);  break;
            case '\t': strbuf_append_n(out, "\\t", 2);  break;
            default:
                if (c < 0x20) {
                    // other control chars must go out as \u00XX.
                    char esc[8];
                    snprintf(esc, sizeof esc, "\\u%04x", c);
                    strbuf_append(out, esc);
                } else {
                    // pass utf-8 bytes through untouched. we dont \u-escape
                    // non-ascii; valid json either way and far more readable.
                    strbuf_append_char(out, (char)c);
                }
                break;
        }
    }
}

// read four hex digits at s, return the value or -1 on a non-hex digit.
static int hex4(const char *s) {
    int v = 0;
for (int i = 0;
i < 4;
i++) {
        char c = s[i];
        int d;
        if (c >= '0' && c <= '9') d = c - '0';
        else if (c >= 'a' && c <= 'f') d = c - 'a' + 10;
        else if (c >= 'A' && c <= 'F') d = c - 'A' + 10;
        else return -1;
        v = (v << 4) | d;
    }
    return v;
