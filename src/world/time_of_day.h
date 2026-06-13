#ifndef WORLD_TIME_OF_DAY_H
#define WORLD_TIME_OF_DAY_H

#include "daynight.h"

// higher level "time of day" that combines day/night + sun color + sky color
// for the renderer. basically a facade over daynight.

typedef struct {
    daynight dn;
    float    sky_r, sky_g, sky_b;
    float    sun_r, sun_g, sun_b;
    float    sun_strength;
} time_of_day;

void tod_init(time_of_day *t, float start_hour);
void tod_update(time_of_day *t, float dt);

#endif
