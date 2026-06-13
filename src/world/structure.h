#ifndef WORLD_STRUCTURE_H
#define WORLD_STRUCTURE_H

#include "chunk.h"

// scatter a few "structures" (small huts, really) into chunks at low rate.
// not proper structure placement — just hand coded layouts.

void structure_maybe_place(chunk *c, unsigned seed);

#endif
