#ifndef WORLD_SPAWN_MOBS_H
#define WORLD_SPAWN_MOBS_H

#include "../entity/mob.h"
#include "world.h"

// ambient mob spawning driven by time of day. called on a slow tick.
void spawn_mobs_tick(mob_registry *mr, world *w, float day_hour, float dt);

#endif
