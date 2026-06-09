#ifndef UTIL_JOBSYS_DEBUG_H
#define UTIL_JOBSYS_DEBUG_H

// debug overlay glue for the job system. dumps the live counters and per-worker
// deque depths into a strbuf the f3-style overlay can blit. same spirit as
// loader_debug -- the loader had its own; this is jobsys's. cheap to call, but
// it does read every deque size racily so dont spam it (once a frame is fine).

#include "jobsys.h"
#include "../strbuf.h"   // strbuf is an anon-struct typedef, cant forward-decl it

// append a multi-line summary to `out`. looks roughly like:
// jobs: 18234 run / 412 stolen / 7 spill / 33 main
// deq : [12 4 0 9 ..]  ovf 2
void jobsys_debug_dump(const jobsys_pool *p, strbuf *out);

// total work still sitting in queues (all deques + overflow). approximate. the
// overlay uses this to colour the "loader busy" indicator. >0 means in flight.
int64_t jobsys_debug_pending(const jobsys_pool *p);

// quick one-liner for the corner-of-screen counter: "jobs 14 / w8". writes into
// `buf` (size `n`) and returns it. truncates safely.
const char *jobsys_debug_oneline(const jobsys_pool *p, char *buf, int n);

#endif
