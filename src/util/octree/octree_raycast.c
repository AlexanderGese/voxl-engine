#include "octree_raycast.h"

#include "octree_node.h"
#include "../darray.h"
#include <math.h>
#include <float.h>

// local slab test that hands back BOTH tmin and tmax. aabb.c only returns tmin,
// and for ordered traversal we need the exit param too, so we keep our own.
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
    vec3   inv;        // 1/dir per axis, precomputed once
    float  max_t;
    uint32_t mask;     // 0 = no filter
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
    // already have a hit before this node even starts? skip the whole subtree.
    if (rc->best.hit && rc->best.t <= tmin) return;

    test_items(rc, n);
    if (n->leaf) return;

    int32_t order[8]; float od[8]; int m = 0;
    for (int c = 0; c < 8; c++) {
        if (n->child[c] < 0) continue;
        float a, b;
        if (!ray_box(rc->t->pool.nodes[n->child[c]].bounds, rc->origin, rc->inv, &a, &b))
            continue;
        // insertion sort by entry t
        int j = m++;
        order[j] = n->child[c]; od[j] = a;
        while (j > 0 && od[j-1] > od[j]) {
            float td = od[j]; od[j] = od[j-1]; od[j-1] = td;
            int32_t ti = order[j]; order[j] = order[j-1]; order[j-1] = ti;
            j--;
        }
    }
    for (int k = 0; k < m; k++) {
        if (rc->best.hit && rc->best.t <= od[k]) break;
        ray_walk(rc, order[k]);
    }
}

static octree_hit do_raycast(const octree *t, vec3 origin, vec3 dir,
                             float max_t, uint32_t mask) {
    ray_ctx rc;
    rc.t = t;
    rc.origin = origin;
    // guard against zero components - a flat 1/0 gives inf which the slab test
    // handles fine, but a literal 0 dir would nan. nudge it.
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

    if (rc.best.hit) {
        rc.best.point = vec3_add(origin, vec3_scale(dir, rc.best.t));
    }
    return rc.best;
}

octree_hit octree_raycast(const octree *t, vec3 origin, vec3 dir, float max_t) {
    return do_raycast(t, origin, dir, max_t, 0);
}

octree_hit octree_raycast_tagged(const octree *t, vec3 origin, vec3 dir,
                                 float max_t, uint32_t tag_mask) {
    return do_raycast(t, origin, dir, max_t, tag_mask);
}

int octree_segment_blocked(const octree *t, vec3 origin, vec3 target) {
    vec3 d = vec3_sub(target, origin);
    float len = vec3_length(d);
    if (len < 1e-6f) return 0;
    // raycast along the segment; any hit with t<=1 (in segment units) blocks.
    octree_hit h = do_raycast(t, origin, d, 1.0f, 0);
    return h.hit;
}
