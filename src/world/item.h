#ifndef WORLD_ITEM_H
#define WORLD_ITEM_H

#include "../math/vec3.h"
#include "block.h"
#include "world.h"

// dropped block items. float in the world until picked up.

#define MAX_ITEMS 256

typedef struct {
    int      alive;
    block_id block;
    vec3     pos;
    vec3     vel;
    float    age;
    float    spin;
} dropped_item;

typedef struct {
    dropped_item list[MAX_ITEMS];
    int          count;
} item_world;

void item_world_init(item_world *iw);
void item_spawn(item_world *iw, vec3 pos, block_id id);
void item_update(item_world *iw, world *w, float dt);
int  item_try_pickup(item_world *iw, vec3 player_pos, block_id *out);

#endif
