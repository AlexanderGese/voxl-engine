#include "bvh_query.h"
#include "bvh_box.h"
#include <stddef.h>
#include <math.h>
typedef int (*box_overlap_fn)(aabb node_or_prim, const void *shape);
typedef struct {
    const bvh   *b;
    box_overlap_fn overlap;
    const void  *shape;
    uint32_t     tag_mask;
    bvh_visit_fn fn;
    void        *user;
    int          visited;
    int          stop;
} query_ctx;
static void walk(query_ctx *q) {
    if (!q->b->built || q->b->root < 0) return;

    int32_t stack[BVH_MAX_DEPTH * 2 + 4];
    int sp = 0;
    stack[sp++] = q->b->root;

    while (sp > 0 && !q->stop) {
        int32_t idx = stack[--sp];
        const bvh_node *n = bvh_node_at_c(&q->b->store, idx);

        if (!q->overlap(n->bounds, q->shape)) continue;

        if (n->count > 0) {
            uint32_t first = n->u.first_prim;
            for (uint32_t k = 0; k < n->count && !q->stop; k++) {
                const bvh_prim *p = &q->b->store.prims[first + k];
                if (q->tag_mask && !(p->tag & q->tag_mask)) continue;
                if (!q->overlap(p->box, q->shape)) continue;
                q->visited++;
                if (q->fn && q->fn(p->id, p->tag, p->box, q->user))
                    q->stop = 1;
            }
        } else {
            // push both children. order doesnt matter for an overlap query.
            stack[sp++] = idx + 1;
            stack[sp++] = (int32_t)n->u.second_child;
        }
    }
}

// --- box overlap ---
static int ov_box(aabb box, const void *shape) {
    const aabb *q = (const aabb *)shape;
return aabb_intersects(box, *q);
return aabb_contains(box, *p);
float r2;
} sphere;
;
walk(&q);
return q.visited;
bvh_query_aabb(b, query, tag_mask, any_visit, &hit);
return hit;
float best2 = max_dist > 0 ? max_dist * max_dist : 1e30f;
int found = 0;
uint32_t best_id = 0, best_tag = 0;
aabb best_box = b->store.nodes[b->root].bounds;
struct { int32_t idx; float d2; } stack[BVH_MAX_DEPTH * 2 + 4];
int sp = 0;
stack[sp].idx = b->root;
stack[sp].d2  = bvh_point_box_dist2(point, b->store.nodes[b->root].bounds);
sp++;
if (out_id)  *out_id  = best_id;
if (out_tag) *out_tag = best_tag;
if (out_box) *out_box = best_box;
return sqrtf(best2);
}
