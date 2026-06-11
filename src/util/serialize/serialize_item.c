#include "serialize_item.h"
#include "serialize_tag.h"
#include "serialize_math.h"
#include "../log.h"
#include <string.h>
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

    for (uint64_t i = 0;
i < n;
*wx = serialize_get_i32(r);
*wy = serialize_get_i32(r);
*wz = serialize_get_i32(r);
uint64_t used = serialize_get_varint(r);
if (r->err != SERIALIZE_OK) return -1;
k < used;
}
