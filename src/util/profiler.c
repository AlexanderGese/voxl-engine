#include "profiler.h"

#include <string.h>

static prof_entry g_entries[PROF_MAX];
static int        g_count = 0;

void prof_init(void) {
    memset(g_entries, 0, sizeof g_entries);
    g_count = 0;
}

int prof_register(const char *name) {
    for (int i = 0; i < g_count; i++) {
        if (strcmp(g_entries[i].name, name) == 0) return i;
    }
    if (g_count >= PROF_MAX) return -1;
    g_entries[g_count].name = name;
    return g_count++;
}

void prof_begin(int id) {
    if (id < 0 || id >= g_count) return;
    g_entries[id].t0 = timer_now_us();
}

void prof_end(int id) {
    if (id < 0 || id >= g_count) return;
    double dt_ms = (double)(timer_now_us() - g_entries[id].t0) / 1000.0;
    prof_entry *e = &g_entries[id];
    e->samples[e->si] = dt_ms;
    e->si = (e->si + 1) & (PROF_AVG - 1);
    if (!e->filled && e->si == 0) e->filled = 1;
}

double prof_avg_ms(int id) {
    if (id < 0 || id >= g_count) return 0;
    prof_entry *e = &g_entries[id];
    int n = e->filled ? PROF_AVG : e->si;
    if (n == 0) return 0;
    double s = 0;
    for (int i = 0; i < n; i++) s += e->samples[i];
    return s / (double)n;
}

const char *prof_name(int id) {
    if (id < 0 || id >= g_count) return "?";
    return g_entries[id].name;
}

int prof_count(void) { return g_count; }
