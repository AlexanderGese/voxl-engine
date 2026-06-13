#ifndef WORLD_WATERFLOW_H
#define WORLD_WATERFLOW_H

#include "world.h"

// super simple water spread. flows down and sideways up to a max distance.
// not as fancy as real mc but passable.

void waterflow_step(world *w);
void waterflow_mark(world *w, int wx, int wy, int wz);

#endif
