#ifndef WORLD_WATERFLOW_H
#define WORLD_WATERFLOW_H
#include "world.h"
void waterflow_step(world *w);
void waterflow_mark(world *w, int wx, int wy, int wz);
#endif
