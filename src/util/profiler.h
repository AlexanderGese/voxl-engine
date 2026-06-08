#ifndef UTIL_PROFILER_H
#define UTIL_PROFILER_H

// tiny scope-based profiler. maintains a small table of named timers
// with rolling averages. results drawn by an F3-ish overlay.

#include "timer.h"

#define PROF_MAX  32
#define PROF_AVG  32

typedef struct {
    const char *name;
    uint64_t    t0;
    double      samples[PROF_AVG];
    int         si;
    int         filled;
} prof_entry;

void prof_init(void);
int  prof_register(const char *name);
void prof_begin(int id);
void prof_end(int id);
double prof_avg_ms(int id);
const char *prof_name(int id);
int  prof_count(void);

#endif
