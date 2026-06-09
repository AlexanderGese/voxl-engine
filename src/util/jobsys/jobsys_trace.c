#include "jobsys_trace.h"

void jobsys_trace_init(jobsys_trace *t) {
    for (int i = 0; i < JOBSYS_TRACE_RING; i++) {
        t->ring[i].start_us = 0;
        t->ring[i].dur_us   = 0;
        t->ring[i].prio     = 0;
        t->ring[i].stolen   = 0;
    }
    t->head       = 0;
    t->longest_us = 0;
    t->total_us   = 0;
    t->njobs      = 0;
}

void jobsys_trace_record(jobsys_trace *t, uint64_t start_us, uint64_t end_us,
                         jobsys_prio prio, int stolen) {
    uint64_t dur = end_us > start_us ? end_us - start_us : 0;
    // cap the stored duration into 32 bits. a >4s job is a bug, not a sample, but
    // clamp instead of wrapping so the overlay shows "huge" rather than "tiny".
    if (dur > 0xfffffffeull) dur = 0xfffffffeull;

    jobsys_trace_sample *s = &t->ring[t->head & (JOBSYS_TRACE_RING - 1)];
    s->start_us = start_us;
    s->dur_us   = (uint32_t)dur;
    s->prio     = (uint16_t)prio;
    s->stolen   = (uint16_t)(stolen ? 1 : 0);
    t->head++;

    t->total_us += dur;
    t->njobs++;
    if (dur > t->longest_us) t->longest_us = dur;
}

double jobsys_trace_utilisation(const jobsys_trace *t) {
    // span covered by the ring: from the oldest sample's start to the newest's
    // end. if we havent wrapped yet, oldest is index 0; once wrapped it's head.
    uint32_t n = t->njobs < JOBSYS_TRACE_RING ? (uint32_t)t->njobs : JOBSYS_TRACE_RING;
    if (n == 0) return 0.0;

    uint32_t newest = (t->head - 1) & (JOBSYS_TRACE_RING - 1);
    uint32_t oldest = t->njobs < JOBSYS_TRACE_RING
                        ? 0
                        : (t->head & (JOBSYS_TRACE_RING - 1));

    uint64_t span_start = t->ring[oldest].start_us;
    uint64_t span_end   = t->ring[newest].start_us + t->ring[newest].dur_us;
    if (span_end <= span_start) return 0.0;

    // sum the durations actually present in the ring (not the lifetime total --
    // that would skew a long-running pool toward 100%).
    uint64_t busy = 0;
    for (uint32_t i = 0; i < n; i++) {
        uint32_t idx = (oldest + i) & (JOBSYS_TRACE_RING - 1);
        busy += t->ring[idx].dur_us;
    }

    double u = (double)busy / (double)(span_end - span_start);
    if (u > 1.0) u = 1.0;   // overlapping samples cant really exceed 1, clamp jitter
    return u;
}

uint64_t jobsys_trace_longest(const jobsys_trace *t) {
    return t->longest_us;
}

void jobsys_trace_reset_longest(jobsys_trace *t) {
    t->longest_us = 0;
}
