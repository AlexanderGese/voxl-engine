#include "bvh_raycast.h"
#include "bvh_box.h"
#include <stddef.h>
#include <math.h>
#include <float.h>
vec3      origin;
vec3      dir;
vec3      inv;
int       neg[3];
float     max_t;
uint32_t  mask;
int       any;
bvh_hit   best;
} ray_ctx;
rc.b = b;
rc.origin = origin;
rc.dir = dir;
rc.inv.x = 1.0f / (dir.x != 0 ? dir.x : 1e-8f);
rc.inv.y = 1.0f / (dir.y != 0 ? dir.y : 1e-8f);
rc.inv.z = 1.0f / (dir.z != 0 ? dir.z : 1e-8f);
rc.neg[0] = rc.inv.x < 0;
rc.neg[1] = rc.inv.y < 0;
rc.neg[2] = rc.inv.z < 0;
rc.max_t = max_t;
rc.mask = mask;
rc.any = any;
rc.best.hit = 0;
rc.best.t = FLT_MAX;
rc.best.hit_id = 0;
