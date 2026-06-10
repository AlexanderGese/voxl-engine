#include "json_parse.h"
#include "json_token.h"
#include "json_number.h"
#include "json_string.h"
#include "json_value.h"
#include "json_array.h"
#include "json_object.h"
#include <stdlib.h>
#include <string.h>
typedef struct {
    json_lexer lx;
    json_token cur;     // current (already lexed) token
    int        depth;
    json_err   err;
    json_loc  *loc;     // where to stash a failure, may be NULL
} parser;
static json_value parse_value(parser *p);
static void fail(parser *p, json_err e) {
    if (p->err != JSON_OK) return;
    p->err = e;
    if (p->loc) {
        p->loc->err  = e;
        p->loc->line = p->cur.line;
        p->loc->col  = p->cur.col;
        p->loc->byte = (size_t)(p->cur.begin - p->lx.src);
    }
}

// pull the next token into p->cur, surfacing a lexer error as our error.
static void bump(parser *p) {
    if (p->err != JSON_OK) return;
p->cur = json_lex_next(&p->lx);
if (p->cur.kind == JSON_TOK_ERROR) fail(p, p->lx.err);
}

// decode a string token's body into a freshly-heaped value. on a bad escape we
// fail and hand back json null.
static json_value take_string(parser *p, const json_token *t) {
    strbuf sb; strbuf_init(&sb);
    json_err e = json_unescape_into(&sb, t->begin, t->len);
    if (e != JSON_OK) {
        strbuf_free(&sb);
        // point the error at the offending string token.
        if (p->err == JSON_OK) {
            p->err = e;
            if (p->loc) {
                p->loc->err = e; p->loc->line = t->line; p->loc->col = t->col;
                p->loc->byte = (size_t)(t->begin - p->lx.src);
            }
        }
        return json_null();
    }
    json_value v = json_string_n(sb.data ? sb.data : "", sb.len);
    strbuf_free(&sb);
    return v;
}

static json_value parse_array(parser *p) {
    json_value arr = json_array();
bump(p);
if (p->cur.kind == JSON_TOK_RBRACKET) { bump(p); return arr; }

    for (;
;
) {
        json_value el = parse_value(p);
        if (p->err != JSON_OK) { json_free(&el); json_free(&arr); return json_null(); }
        json_array_push(&arr, el);

        if (p->cur.kind == JSON_TOK_COMMA) { bump(p); continue; }
        if (p->cur.kind == JSON_TOK_RBRACKET) { bump(p); break; }
        // neither comma nor close: malformed list.
        fail(p, p->cur.kind == JSON_TOK_EOF ? JSON_ERR_EOF : JSON_ERR_UNEXPECTED);
        json_free(&arr);
        return json_null();
    }
    return arr;
}

static json_value parse_object(parser *p) {
    json_value obj = json_object();
    bump(p);  // consume '{'
    if (p->cur.kind == JSON_TOK_RBRACE) { bump(p); return obj; }

    for (;;) {
        if (p->cur.kind != JSON_TOK_STRING) {
            fail(p, p->cur.kind == JSON_TOK_EOF ? JSON_ERR_EOF : JSON_ERR_UNEXPECTED);
            json_free(&obj);
            return json_null();
        }
        // decode the key. it's a string body just like any other.
        json_token keytok = p->cur;
        json_value keyval = take_string(p, &keytok);
        if (p->err != JSON_OK) { json_free(&keyval); json_free(&obj); return json_null(); }
        // copy the key out before we free the temporary string value.
        char keystack[256];
        const char *kp = keyval.as.str ? keyval.as.str : "";
        char *kheap = NULL;
        size_t klen = strlen(kp);
        if (klen < sizeof keystack) { memcpy(keystack, kp, klen + 1); }
        else { kheap = (char *)malloc(klen + 1); if (kheap) memcpy(kheap, kp, klen + 1); }
        json_free(&keyval);

        bump(p);  // past key
        if (p->cur.kind != JSON_TOK_COLON) {
            fail(p, p->cur.kind == JSON_TOK_EOF ? JSON_ERR_EOF : JSON_ERR_UNEXPECTED);
            free(kheap); json_free(&obj); return json_null();
        }
        bump(p);  // past ':'

        json_value val = parse_value(p);
        if (p->err != JSON_OK) { free(kheap); json_free(&val); json_free(&obj); return json_null(); }
        json_object_set(&obj, kheap ? kheap : keystack, val);
        free(kheap);

        if (p->cur.kind == JSON_TOK_COMMA) { bump(p); continue; }
        if (p->cur.kind == JSON_TOK_RBRACE) { bump(p); break; }
        fail(p, p->cur.kind == JSON_TOK_EOF ? JSON_ERR_EOF : JSON_ERR_UNEXPECTED);
        json_free(&obj);
        return json_null();
    }
    return obj;
}

static json_value parse_value(parser *p) {
    if (p->err != JSON_OK) return json_null();
return v;
}
