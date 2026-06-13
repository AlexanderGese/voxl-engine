#include "biome_deco.h"
#include "biome_table.h"
#include "biome_noise.h"
#include "biome_blend.h"

// we lack dedicated grass-blade / flower blocks in the atlas, so ground cover
// resolves to leaves/torch stand-ins like the gen2 deco does. trees are real
// trunk+canopy stamps though.

void biome_place_reset(biome_place_buf *buf) {
    if (buf) buf->count = 0;
}

int biome_place_add(biome_place_buf *buf, int x, int y, int z, block_id b) {
    if (!buf || buf->count >= BIOME_PLACE_MAX) return 0;
    biome_place *p = &buf->items[buf->count++];
    p->x = x; p->y = y; p->z = z; p->block = b;
    return 1;
}

biome_deco_kind biome_deco_pick(const biome_column *col) {
    if (col->height < col->sea_level) return BIOME_DECO_NONE;   // underwater

    const biome_def *d = biome_table_get(col->biome);

    // tree roll first, gated by density. suppress on biome borders.
    float tree_roll = biome_hash01(col->wx, col->wz, col->seed ^ 0x77EEu);
    float edge = biome_blend_edge(col->wx, col->wz, col->seed);
    float tree_chance = d->tree_density * (1.0f - 0.7f * edge);

    if (tree_roll < tree_chance) {
        switch (col->biome) {
            case BIOME_KIND_TAIGA:
            case BIOME_KIND_SNOWY_PEAKS:
                return BIOME_DECO_TREE_PINE;
            case BIOME_KIND_BEACH:
            case BIOME_KIND_SAVANNA:
                return BIOME_DECO_TREE_PALM;
            default:
                return BIOME_DECO_TREE_OAK;
        }
    }

    // ground cover roll, second hash so it doesnt correlate with trees
    float g_roll = biome_hash01(col->wx, col->wz, col->seed ^ 0x11DDu);
    if (g_roll < d->grass_density * 0.5f) {
        uint32_t k = biome_hash2(col->wx, col->wz, col->seed ^ 0xF10Au);
        switch (col->biome) {
            case BIOME_KIND_DESERT:
                return (k & 7u) == 0 ? BIOME_DECO_CACTUS : BIOME_DECO_NONE;
            case BIOME_KIND_BADLANDS:
            case BIOME_KIND_TUNDRA:
                return (k & 7u) == 0 ? BIOME_DECO_DEADBUSH : BIOME_DECO_NONE;
            case BIOME_KIND_PLAINS:
                return (k & 3u) == 0 ? BIOME_DECO_FLOWER : BIOME_DECO_GRASS;
            default:
                return (k & 1u) ? BIOME_DECO_BUSH : BIOME_DECO_GRASS;
        }
    }
    return BIOME_DECO_NONE;
}

// --- feature stamps ------------------------------------------------------

// a simple trunk + blob canopy. height varies a little per column.
static int stamp_tree(biome_place_buf *buf, const biome_column *col,
                      int trunk_lo, int trunk_hi, block_id leaf, int conifer) {
    int x = col->wx, z = col->wz, base = col->height + 1;
    uint32_t h = biome_hash2(x, z, col->seed ^ 0xACE5u);
    int trunk = trunk_lo + (int)(h % (uint32_t)(trunk_hi - trunk_lo + 1));
    int added = 0;

    for (int i = 0; i < trunk; i++)
        added += biome_place_add(buf, x, base + i, z, BLOCK_WOOD);

    int top = base + trunk;
    if (conifer) {
        // tapered cone of leaves, widest at the bottom of the crown
        for (int layer = 0; layer < 3; layer++) {
            int ly = top - layer;
            int rad = layer;             // 0 at tip, wider going down
            for (int dx = -rad; dx <= rad; dx++)
                for (int dz = -rad; dz <= rad; dz++) {
                    if (dx == 0 && dz == 0 && layer == 0) continue;
                    if (dx * dx + dz * dz > (rad + 1) * (rad + 1)) continue;
                    added += biome_place_add(buf, x + dx, ly, z + dz, leaf);
                }
        }
        added += biome_place_add(buf, x, top + 1, z, leaf);   // spike
    } else {
        // roughly spherical canopy, radius 2
        for (int dy = -2; dy <= 1; dy++)
            for (int dx = -2; dx <= 2; dx++)
                for (int dz = -2; dz <= 2; dz++) {
                    int r2 = dx * dx + dy * dy + dz * dz;
                    if (r2 > 5) continue;
                    if (dx == 0 && dz == 0 && dy < 0) continue;  // dont fill trunk
                    added += biome_place_add(buf, x + dx, top + dy, z + dz, leaf);
                }
    }
    return added;
}

int biome_deco_emit(biome_place_buf *buf, const biome_column *col) {
    biome_deco_kind k = biome_deco_pick(col);
    if (k == BIOME_DECO_NONE) return 0;

    int x = col->wx, z = col->wz, y = col->height + 1;

    switch (k) {
        case BIOME_DECO_TREE_OAK:
            return stamp_tree(buf, col, 4, 6, BLOCK_LEAVES, 0);
        case BIOME_DECO_TREE_PINE:
            return stamp_tree(buf, col, 6, 9, BLOCK_LEAVES, 1);
        case BIOME_DECO_TREE_PALM: {
            // tall bare trunk, tiny leaf nub on top
            int trunk = 5 + (int)(biome_hash2(x, z, col->seed) % 3u);
            int added = 0;
            for (int i = 0; i < trunk; i++)
                added += biome_place_add(buf, x, y + i, z, BLOCK_WOOD);
            added += biome_place_add(buf, x, y + trunk, z, BLOCK_LEAVES);
            for (int d = 0; d < 4; d++) {
                int dx = (d == 0) - (d == 1);
                int dz = (d == 2) - (d == 3);
                added += biome_place_add(buf, x + dx, y + trunk, z + dz, BLOCK_LEAVES);
            }
            return added;
        }
        case BIOME_DECO_CACTUS: {
            int hgt = 2 + (int)(biome_hash2(x, z, col->seed) % 2u);
            int added = 0;
            for (int i = 0; i < hgt; i++)
                added += biome_place_add(buf, x, y + i, z, BLOCK_LEAVES);  // stand-in
            return added;
        }
        case BIOME_DECO_BUSH:
        case BIOME_DECO_FLOWER:
            return biome_place_add(buf, x, y, z, BLOCK_LEAVES);
        case BIOME_DECO_DEADBUSH:
            return biome_place_add(buf, x, y, z, BLOCK_LEAVES);
        case BIOME_DECO_GRASS:
        default:
            // no grass-blade block; pick is still useful metadata for cross-quad
            // rendering later, but nothing solid to place.
            return 0;
    }
}
