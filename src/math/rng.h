#ifndef MATH_RNG_H
#define MATH_RNG_H

#include <stdint.h>

// xorshift64*. fast, ok quality, seedable.

typedef struct { uint64_t s; } rng;

void     rng_init(rng *r, uint64_t seed);
uint64_t rng_u64(rng *r);
uint32_t rng_u32(rng *r);
float    rng_float01(rng *r);
int      rng_range(rng *r, int lo, int hi);    // inclusive
float    rng_frange(rng *r, float lo, float hi);

#endif
