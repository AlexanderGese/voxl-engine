#include "biome_deco.h"
#include "biome_table.h"
#include "biome_noise.h"
#include "biome_blend.h"
biome_place *p = &buf->items[buf->count++];
p->x = x;
p->y = y;
p->z = z;
p->block = b;
return 1;
uint32_t h = biome_hash2(x, z, col->seed ^ 0xACE5u);
int trunk = trunk_lo + (int)(h % (uint32_t)(trunk_hi - trunk_lo + 1));
int added = 0;
for (int i = 0;
i < trunk;
