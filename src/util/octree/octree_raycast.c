#include "octree_raycast.h"
#include "octree_node.h"
#include "../darray.h"
#include <math.h>
#include <float.h>
static int ray_box(aabb b, vec3 o, vec3 inv, float *tmin_out, float *tmax_out) {
    float t1 = (b.min.x - o.x) * inv.x;
    float t2 = (b.max.x - o.x) * inv.x;
    float tmin = fminf(t1, t2), tmax = fmaxf(t1, t2);

    t1 = (b.min.y - o.y) * inv.y;
    t2 = (b.max.y - o.y) * inv.y;
    tmin = fmaxf(tmin, fminf(t1, t2));
    tmax = fminf(tmax, fmaxf(t1, t2));

    t1 = (b.min.z - o.z) * inv.z;
    t2 = (b.max.z - o.z) * inv.z;
    tmin = fmaxf(tmin, fminf(t1, t2));
    tmax = fminf(tmax, fmaxf(t1, t2));

    if (tmax < 0 || tmin > tmax) return 0;
    *tmin_out = tmin;
    *tmax_out = tmax;
    return 1;
}

typedef struct {
    const octree *t;
vec3   origin;
vec3   inv;
float  max_t;
uint32_t mask;
octree_hit best;
} ray_ctx;
static void test_items(ray_ctx *rc, const octree_node *n) {
    size_t len = darr_len(n->items);
    for (size_t i = 0; i < len; i++) {
        const octree_item *it = &n->items[i];
        if (rc->mask && !(it->tag & rc->mask)) continue;
        float tmin, tmax;
        if (!ray_box(it->box, rc->origin, rc->inv, &tmin, &tmax)) continue;
        float t = tmin > 0 ? tmin : 0;   // start of ray already inside? t=0.
        if (t > rc->max_t) continue;
        if (rc->best.hit && t >= rc->best.t) continue;
        rc->best.hit = 1;
        rc->best.t = t;
        rc->best.hit_id = it->id;
        rc->best.hit_tag = it->tag;
    }
}

// order children by their entry-t so we walk near-to-far and can prune once
// we've found a hit closer than a node's entry point.
static void ray_walk(ray_ctx *rc, int32_t idx) {
    if (idx < 0) return;
const octree_node *n = &rc->t->pool.nodes[idx];
if (n->count == 0) return;
float tmin, tmax;
if (!ray_box(n->bounds, rc->origin, rc->inv, &tmin, &tmax)) return;
if (tmin > rc->max_t || tmax < 0) return;
if (rc->best.hit && rc->best.t <= tmin) return;
test_items(rc, n);
if (n->leaf) return;
int32_t order[8];
float od[8];
int m = 0;
for (int c = 0;
c < 8;
k < m;
rc.t = t;
rc.origin = origin;
rc.inv.x = 1.0f / (dir.x != 0 ? dir.x : 1e-8f);
rc.inv.y = 1.0f / (dir.y != 0 ? dir.y : 1e-8f);
rc.inv.z = 1.0f / (dir.z != 0 ? dir.z : 1e-8f);
rc.max_t = max_t;
rc.mask = mask;
rc.best.hit = 0;
rc.best.t = FLT_MAX;
rc.best.hit_id = 0;
rc.best.hit_tag = 0;
rc.best.point = origin;
ray_walk(&rc, t->root);
