#ifndef UTIL_HASHMAP_H
#define UTIL_HASHMAP_H

// open addressing hashmap with 64 bit keys (we store chunk coords packed
// into u64). linear probing, grow at 0.75 load factor.
// used by world/ to replace the linked list of chunks. the old linked list
// version lived in world.c for a while before i swapped it out.

#include <stdint.h>
#include <stddef.h>

typedef struct {
    uint64_t key;
    void    *val;
    uint8_t  state;     // 0 empty, 1 used, 2 tomb
} hm_slot;

typedef struct {
    hm_slot *slots;
    size_t   cap;
    size_t   len;
} hashmap;

void   hashmap_init(hashmap *m, size_t initial_cap);
void   hashmap_free(hashmap *m);
void   hashmap_put(hashmap *m, uint64_t key, void *val);
void  *hashmap_get(const hashmap *m, uint64_t key);
int    hashmap_has(const hashmap *m, uint64_t key);
void   hashmap_del(hashmap *m, uint64_t key);
size_t hashmap_len(const hashmap *m);

// iterator
typedef struct {
    const hashmap *m;
    size_t i;
} hm_iter;

void  hm_iter_init(hm_iter *it, const hashmap *m);
int   hm_iter_next(hm_iter *it, uint64_t *key, void **val);

// packs two 32 bit ints into a key
static inline uint64_t hm_pack2(int32_t a, int32_t b) {
    return ((uint64_t)(uint32_t)a) | (((uint64_t)(uint32_t)b) << 32);
}

#endif
