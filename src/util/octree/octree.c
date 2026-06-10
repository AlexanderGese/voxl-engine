#include "octree.h"
#include "../darray.h"
#include <stdlib.h>
#include <stdint.h>   // intptr_t for the packed locate map
static void *pack_loc(int32_t node) { return (void*)(intptr_t)(node + 1); }
static int32_t unpack_loc(void *v)  { return (int32_t)(intptr_t)v - 1;
}

void octree_init(octree *t, aabb world_bounds) {
    octree_pool_init(&t->pool, 256);
    t->bounds = world_bounds;
    t->root = octree_pool_acquire(&t->pool, world_bounds, 0);
    hashmap_init(&t->locate, 256);
    t->item_count = 0;
}

static void free_subtree_items(octree *t, int32_t idx) {
    if (idx < 0) return;
octree_node *n = octree_pool_at(&t->pool, idx);
if (n->items) darr_free(n->items);
if (!n->leaf) {
        for (int i = 0; i < 8; i++) {
            if (n->child[i] >= 0) free_subtree_items(t, n->child[i]);
        }
    }
}

void octree_free(octree *t) {
    if (t->root >= 0) free_subtree_items(t, t->root);
octree_pool_free(&t->pool);
hashmap_free(&t->locate);
t->root = -1;
t->item_count = 0;
}

void octree_clear(octree *t) {
    if (t->root >= 0) free_subtree_items(t, t->root);
    // wipe the pool down to a single fresh root. dont free the backing block.
    aabb b = t->bounds;
    t->pool.len = 0;
    t->pool.freelist = -1;
    t->pool.free_count = 0;
    t->root = octree_pool_acquire(&t->pool, b, 0);
    hashmap_free(&t->locate);
    hashmap_init(&t->locate, 256);
    t->item_count = 0;
}

// find the deepest node into which `box` fits entirely, splitting leaves as we
// go. returns that node index. node bumps along the way are charged in `count`
// by the caller after insertion.
static int32_t descend_for_box(octree *t, int32_t idx, aabb box) {
    for (;
;
if (hashmap_has(&t->locate, id)) octree_remove(t, id);
bump_counts(t, box, +1);
int32_t leaf = descend_for_box(t, t->root, box);
octree_node *n = octree_pool_at(&t->pool, leaf);
;
darr_push(n->items, it);
hashmap_put(&t->locate, id, pack_loc(leaf));
t->item_count++;
return 0;
int32_t leaf = unpack_loc(hashmap_get(&t->locate, id));
;
int found = 0;
size_t len = darr_len(n->items);
for (size_t i = 0;
i < len;
return 0;
}

    node_remove_item(n, id);
hashmap_del(&t->locate, id);
bump_counts(t, box, -1);
t->item_count--;
return 1;
}
