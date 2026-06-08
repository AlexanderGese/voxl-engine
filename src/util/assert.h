#ifndef UTIL_ASSERT_H
#define UTIL_ASSERT_H

#include "log.h"

// crashes hard on failure. gcc __builtin_trap, or just a fatal log.

#define VX_ASSERT(expr)                                                  \
    do {                                                                 \
        if (!(expr)) {                                                   \
            log_msg(LOG_FATAL, __FILE__, __LINE__,                       \
                    "assertion failed: %s", #expr);                      \
            __builtin_trap();                                            \
        }                                                                \
    } while (0)

#define VX_ASSERT_M(expr, msg)                                           \
    do {                                                                 \
        if (!(expr)) {                                                   \
            log_msg(LOG_FATAL, __FILE__, __LINE__,                       \
                    "assertion failed: %s (%s)", #expr, msg);            \
            __builtin_trap();                                            \
        }                                                                \
    } while (0)

#endif
