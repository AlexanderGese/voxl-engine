#ifndef UTIL_STRBUF_H
#define UTIL_STRBUF_H

// mutable string buffer. null terminated. grows.

#include <stdarg.h>
#include <stddef.h>

typedef struct {
    char  *data;
    size_t len;
    size_t cap;
} strbuf;

void  strbuf_init(strbuf *s);
void  strbuf_free(strbuf *s);
void  strbuf_clear(strbuf *s);
void  strbuf_reserve(strbuf *s, size_t n);
void  strbuf_append(strbuf *s, const char *str);
void  strbuf_append_n(strbuf *s, const char *str, size_t n);
void  strbuf_append_char(strbuf *s, char c);
void  strbuf_appendf(strbuf *s, const char *fmt, ...);
void  strbuf_vappendf(strbuf *s, const char *fmt, va_list ap);

#endif
