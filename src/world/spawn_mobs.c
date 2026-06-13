#include "spawn_mobs.h"
#include "worldgen.h"
#include "../math/rng.h"
#include "../config.h"

#include <stddef.h>

static float acc = 0;
static rng srng;
static int srng_inited = 0;

void spawn_mobs_tick(mob_registry *mr, world *w, float day_hour, float dt) {
    if (!srng_inited) { rng_init(&srng, 0x5ACED); srng_inited = 1; }
    acc += dt;
    if (acc < 5.0f) return;
    acc = 0;

    if (mr->count >= MAX_MOBS - 8) return;

    // night spawns hostile, day spawns passive
    int hostile = (day_hour < 6.0f || day_hour > 20.0f);
    for (int i = 0; i < 2; i++) {
        int tries = 5;
        while (tries--) {
            if (w->count == 0) break;
            int target = rng_range(&srng, 0, w->count - 1);
            chunk *c = NULL;
            int k = 0;
            for (world_node *n = w->head; n; n = n->next) {
                if (k == target) { c = n->c; break; }
                k++;
            }
            if (!c) break;
            int lx = rng_range(&srng, 1, CHUNK_SIZE_X - 2);
            int lz = rng_range(&srng, 1, CHUNK_SIZE_Z - 2);
            int wx = c->cx * CHUNK_SIZE_X + lx;
            int wz = c->cz * CHUNK_SIZE_Z + lz;
            int h  = worldgen_height_at(wx, wz, w->seed);
            vec3 p = { (float)wx + 0.5f, (float)h + 1.5f, (float)wz + 0.5f };
            entity_type t = hostile
                ? (entity_type)(ET_ZOMBIE + rng_range(&srng, 0, 2))
                : (entity_type)(ET_COW    + rng_range(&srng, 0, 1));
            mob_spawn(mr, t, p);
            break;
        }
    }
}
