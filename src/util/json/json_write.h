#ifndef UTIL_JSON_WRITE_H
#define UTIL_JSON_WRITE_H

// the writer. turns a dom back into text. two flavours: compact (no spaces,
// for saving on disk / sending over the wire) and pretty (indented, for human
// eyes and diffs). both append into a strbuf the caller owns.

#include "../strbuf.h"
#include "json_types.h"

// indentation knobs for the pretty writer.
typedef struct {
    int  indent;     // spaces per level
    int  sort_keys;  // emit object members in sorted key order if set
} json_write_opts;

// sensible defaults: two-space indent, keys left in insertion order.
json_write_opts json_write_defaults(void);

// compact form, no whitespace. appends to out.
void json_write(strbuf *out, const json_value *v);

// pretty form with the given options. appends to out, no trailing newline.
void json_write_pretty(strbuf *out, const json_value *v, json_write_opts opts);

// convenience: stringify into a fresh malloc'd, nul-terminated buffer the
// caller frees. `pretty` picks the formatter. returns NULL on oom.
char *json_to_string(const json_value *v, int pretty);

#endif
