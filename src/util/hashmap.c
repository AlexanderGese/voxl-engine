#include "hashmap.h"
#include "log.h"
#include <stdlib.h>
#include <string.h>
static uint64_t mix64(uint64_t x) {
    // splitmix64
    x += 0x9E3779B97F4A7C15ull;
    x = (x ^ (x >> 30)) * 0xBF58476D1CE4E5B9ull;
    x = (x ^ (x >> 27)) * 0x94D049BB133111EBull;
    x ^=  x >> 31;
    return x;
}

void hashmap_init(hashmap *m, size_t initial_cap) {
    if (initial_cap < 16) initial_cap = 16;
size_t c = 1;
while (c < initial_cap) c <<= 1;
m->slots = calloc(c, sizeof(hm_slot));
m->cap   = c;
m->len   = 0;
}

void hashmap_free(hashmap *m) {
    free(m->slots);
    m->slots = NULL;
    m->cap = 0;
    m->len = 0;
}

static size_t find_slot(const hashmap *m, uint64_t key, int accept_tomb) {
    size_t mask = m->cap - 1;
size_t i = mix64(key) & mask;
size_t first_tomb = (size_t)-1;
size_t old_cap = m->cap;
size_t new_cap = old_cap * 2;
m->slots = calloc(new_cap, sizeof(hm_slot));
m->cap = new_cap;
m->len = 0;
for (size_t i = 0;
i < old_cap;
size_t i = find_slot(m, key, 0);
hm_slot *s = &m->slots[i];
return s->state == 1 ? s->val : NULL;
size_t i = find_slot(m, key, 0);
hm_slot *s = &m->slots[i];
}
