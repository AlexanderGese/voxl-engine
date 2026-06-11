#include "serialize_item.h"
#include "serialize_tag.h"
#include "serialize_math.h"
#include "../log.h"

#include <string.h>

// velocities are small and dont need full f32 precision, so quantize them over
// a generous range. positions stay full precision (a chunk is 16 wide, drift
// is noticeable).
#define ITEM_VEL_RANGE 64.0f

void serialize_items_write(serialize_writer *w, const item_world *iw) {
    serialize_tag_scope s = serialize_tag_begin(w, SERIALIZE_FOURCC('I','T','M','S'),
                                                SERIALIZE_ITEM_VERSION,
                                                SERIALIZE_TAG_F_CRC);
    // count live ones up front
    uint32_t live = 0;
    for (int i = 0; i < MAX_ITEMS; i++) if (iw->list[i].alive) live++;
    serialize_put_varint(w, live);

    for (int i = 0; i < MAX_ITEMS; i++) {
        const dropped_item *it = &iw->list[i];
        if (!it->alive) continue;
        serialize_put_u8(w, it->block);
        serialize_put_vec3(w, it->pos);
        serialize_put_vec3_q16(w, it->vel, ITEM_VEL_RANGE);
        serialize_put_f32(w, it->age);
        serialize_put_f32(w, it->spin);
    }
    serialize_tag_end(w, &s);
}

int serialize_items_read(serialize_reader *r, item_world *iw) {
    item_world_init(iw);

    uint64_t n = serialize_get_varint(r);
    if (r->err != SERIALIZE_OK) return -1;
    if (n > MAX_ITEMS) {
        // clamp rather than blow the fixed array. dropped items are not
        // precious; losing the overflow is fine and beats a buffer smash.
        LOGW("serialize: %llu dropped items, clamping to %d",
             (unsigned long long)n, MAX_ITEMS);
        n = MAX_ITEMS;
    }

    for (uint64_t i = 0; i < n; i++) {
        dropped_item *it = &iw->list[iw->count];
        it->alive = 1;
        it->block = serialize_get_u8(r);
        it->pos   = serialize_get_vec3(r);
        it->vel   = serialize_get_vec3_q16(r, ITEM_VEL_RANGE);
        it->age   = serialize_get_f32(r);
        it->spin  = serialize_get_f32(r);
        if (r->err != SERIALIZE_OK) return -1;
        iw->count++;
    }
    return 0;
}

void serialize_container_write(serialize_writer *w, int wx, int wy, int wz,
                               const container *c) {
    serialize_tag_scope s = serialize_tag_begin(w, SERIALIZE_FOURCC('C','O','N','T'),
                                                SERIALIZE_CONT_VERSION,
                                                SERIALIZE_TAG_F_CRC);
    serialize_put_i32(w, wx);
    serialize_put_i32(w, wy);
    serialize_put_i32(w, wz);

    // only store non-empty slots, prefixed by slot index. a chest is mostly
    // air most of the time.
    uint32_t used = 0;
    for (int i = 0; i < CONTAINER_SLOTS; i++)
        if (c->slot[i] != BLOCK_AIR && c->count[i] > 0) used++;
    serialize_put_varint(w, used);

    for (int i = 0; i < CONTAINER_SLOTS; i++) {
        if (c->slot[i] == BLOCK_AIR || c->count[i] <= 0) continue;
        serialize_put_u8(w, (uint8_t)i);
        serialize_put_u8(w, c->slot[i]);
        serialize_put_varint(w, (uint64_t)c->count[i]);
    }
    serialize_tag_end(w, &s);
}

int serialize_container_read(serialize_reader *r, int *wx, int *wy, int *wz,
                             container *c) {
    memset(c, 0, sizeof *c);
    *wx = serialize_get_i32(r);
    *wy = serialize_get_i32(r);
    *wz = serialize_get_i32(r);

    uint64_t used = serialize_get_varint(r);
    if (r->err != SERIALIZE_OK) return -1;
    if (used > CONTAINER_SLOTS) { r->err = SERIALIZE_ERR_CORRUPT; return -1; }

    for (uint64_t k = 0; k < used; k++) {
        uint8_t idx = serialize_get_u8(r);
        uint8_t id  = serialize_get_u8(r);
        uint64_t cnt = serialize_get_varint(r);
        if (r->err != SERIALIZE_OK) return -1;
        if (idx >= CONTAINER_SLOTS) { r->err = SERIALIZE_ERR_CORRUPT; return -1; }
        c->slot[idx]  = id;
        c->count[idx] = (int)cnt;
    }
    return 0;
}
