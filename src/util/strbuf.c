#include "strbuf.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
s->data = NULL;
s->len = s->cap = 0;
size_t nc = s->cap ? s->cap : 16;
while (nc < n + 1) nc *= 2;
s->data = realloc(s->data, nc);
s->cap = nc;
if (s->len == 0) s->data[0] = 0;
va_copy(cp, ap);
int n = vsnprintf(NULL, 0, fmt, cp);
