#include "daynight.h"

#include <math.h>

#define PI 3.14159265358979f

void daynight_init(daynight *d, float start_hour) {
    d->hours = start_hour;
    d->rate  = 1.0f / 30.0f;   // 30 real seconds per in-game hour
}

void daynight_update(daynight *d, float dt) {
    d->hours += dt * d->rate;
    while (d->hours >= 24.0f) d->hours -= 24.0f;
}

float daynight_sun_factor(const daynight *d) {
    // 06:00 sunrise, 18:00 sunset, with smooth dawn/dusk transitions
    float h = d->hours;
    if (h < 5.0f || h > 21.0f) return 0.0f;
    if (h >= 8.0f && h <= 18.0f) return 1.0f;
    if (h < 8.0f) return (h - 5.0f) / 3.0f;
    return (21.0f - h) / 3.0f;
}

void daynight_sky_color(const daynight *d, float *r, float *g, float *b) {
    float k = daynight_sun_factor(d);
    // day color
    float dr = 0.55f, dg = 0.74f, db = 0.95f;
    // night color
    float nr = 0.04f, ng = 0.05f, nb = 0.10f;
    *r = nr + (dr - nr) * k;
    *g = ng + (dg - ng) * k;
    *b = nb + (db - nb) * k;
}

int daynight_is_night(const daynight *d) {
    return daynight_sun_factor(d) < 0.1f;
}
