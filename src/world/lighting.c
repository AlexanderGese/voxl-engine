#include "lighting.h"
#include "block.h"
#include "../config.h"
#include "../util/log.h"
#include <stdlib.h>
#include <string.h>
#define QMAX 65536
typedef struct { int x, y, z; uint8_t level; } lnode;
static lnode q[QMAX];
static int   qhead, qtail;
static int qempty(void) { return qhead == qtail; }
static void qreset(void) { qhead = qtail = 0;
}

static void qpush(int x, int y, int z, uint8_t lv) {
    int nt = (qtail + 1) % QMAX;
    if (nt == qhead) return; // full, drop. cheap n dirty
    q[qtail].x = x; q[qtail].y = y; q[qtail].z = z; q[qtail].level = lv;
    qtail = nt;
}

static int qpop(lnode *out) {
    if (qempty()) return 0;
*out = q[qhead];
qhead = (qhead + 1) % QMAX;
return 1;
}

static void set_sun(world *w, int wx, int wy, int wz, uint8_t v) {
    int cx, cz, lx, lz;
    world_to_chunk(wx, wz, &cx, &cz);
    world_to_local(wx, wz, &lx, &lz);
    chunk *c = world_get_chunk(w, cx, cz);
    if (!c) return;
    chunk_set_sunlight(c, lx, wy, lz, v);
}

static void set_block(world *w, int wx, int wy, int wz, uint8_t v) {
    int cx, cz, lx, lz;
world_to_chunk(wx, wz, &cx, &cz);
world_to_local(wx, wz, &lx, &lz);
chunk *c = world_get_chunk(w, cx, cz);
if (!c) return;
chunk_set_blocklight(c, lx, wy, lz, v);
world_to_chunk(wx, wz, &cx, &cz);
chunk *c = world_get_chunk(w, cx, cz);
if (c) lighting_recompute_chunk(w, c);
}
