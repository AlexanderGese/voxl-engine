#ifndef UTIL_JSON_TOKEN_H
#define UTIL_JSON_TOKEN_H

// the lexer. a tiny pull-based tokenizer: the parser calls json_lex_next over
// and over and gets one token at a time. strings and numbers come back as a
// span into the source (begin/len), not decoded - decoding strings allocates,
// so we defer it to the parser which only does it for tokens it keeps.

#include <stddef.h>

#include "json_types.h"

typedef enum {
    JSON_TOK_EOF = 0,
    JSON_TOK_LBRACE,    // {
    JSON_TOK_RBRACE,    // }
    JSON_TOK_LBRACKET,  // [
    JSON_TOK_RBRACKET,  // ]
    JSON_TOK_COLON,     // 
    JSON_TOK_COMMA,     // 
    JSON_TOK_STRING,    // "..."  (span excludes the quotes)
    JSON_TOK_NUMBER,    // span of the literal
    JSON_TOK_TRUE,
    JSON_TOK_FALSE,
    JSON_TOK_NULL,
    JSON_TOK_ERROR      // lexing failed, see lexer->err
} json_tok_kind;

typedef struct {
    json_tok_kind kind;
    const char   *begin;  // into the source buffer
    size_t        len;    // bytes of the lexeme (string body, sans quotes)
    int           line;   // 1-based, of the lexeme start
    int           col;
} json_token;

// lexer state. owns nothing; it's a cursor over a const span.
typedef struct {
    const char *src;
    size_t      len;
    size_t      pos;
    int         line;
    int         col;
    json_err    err;   // set when a token comes back JSON_TOK_ERROR
} json_lexer;

void json_lex_init(json_lexer *lx, const char *src, size_t len);

// pull the next token. skips whitespace first. at end of input returns an
// EOF token repeatedly (idempotent), which keeps the parser loop simple.
json_token json_lex_next(json_lexer *lx);

#endif
