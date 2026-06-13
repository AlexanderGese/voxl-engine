#include "time_of_day.h"
#include "../render/sun.h"

void tod_init(time_of_day *t, float start_hour) {
    daynight_init(&t->dn, start_hour);
    tod_update(t, 0);
}

void tod_update(time_of_day *t, float dt) {
    daynight_update(&t->dn, dt);
    daynight_sky_color(&t->dn, &t->sky_r, &t->sky_g, &t->sky_b);
    sun_color(t->dn.hours, &t->sun_r, &t->sun_g, &t->sun_b);
    t->sun_strength = daynight_sun_factor(&t->dn);
}
