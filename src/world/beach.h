#ifndef WORLD_BEACH_H
#define WORLD_BEACH_H

#include "chunk.h"

// postprocess: turn the top layers near water into sand.
void beach_smooth(chunk *c, unsigned seed);

#endif
