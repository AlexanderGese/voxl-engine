#ifndef WORLD_TICK_H
#define WORLD_TICK_H

#include "world.h"

// slow tick system: every N ms, a random set of blocks per loaded chunk
// get a chance to tick. used for grass spreading, water flow, snow melt, etc.

void tick_init(void);
void tick_update(world *w, float dt);

#endif
