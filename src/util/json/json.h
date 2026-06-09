#ifndef UTIL_JSON_H
#define UTIL_JSON_H

// minimal json reader/writer for the engine. umbrella include - pull this one
// line and you get the dom, the parser, the query helpers and the writer.
//
// json_value doc;
// json_loc   loc;
// if (json_parse_file("config/world.json", &doc, &loc) != JSON_OK) {
// char tmp[128];
// log_warn("config: %s", json_loc_str(&loc, tmp, sizeof tmp));
// } else {
// int seed = (int)json_get_int(&doc, "world.seed", 0);
// json_free(&doc);
// }
//
// the design goal is "small enough to read in one sitting, correct enough to
// trust with a config file". it is not a streaming parser and it will happily
// hold the whole document in memory. for our config sizes that's a non-issue.
//
// not supported, on purpose: comments, trailing commas, single quotes, NaN /
// Infinity. if a tool writes those it's not writing json and i'm not catching
// its mess.

#include "json_types.h"
#include "json_error.h"
#include "json_value.h"
#include "json_array.h"
#include "json_object.h"
#include "json_number.h"
#include "json_string.h"
#include "json_token.h"
#include "json_parse.h"
#include "json_query.h"
#include "json_write.h"
#include "json_merge.h"
#include "json_build.h"

// read + parse a file off disk in one call. uses util/file.h under the hood.
// returns JSON_OK on success (caller json_frees *out), or an error - including
// a synthetic JSON_ERR_EOF when the file couldnt be opened at all.
json_err json_parse_file(const char *path, json_value *out, json_loc *loc);

// stringify and write to disk. `pretty` toggles indentation. returns 0 on
// success, nonzero on a write failure or oom. handy mirror of json_parse_file.
int json_write_file(const char *path, const json_value *v, int pretty);

#endif
