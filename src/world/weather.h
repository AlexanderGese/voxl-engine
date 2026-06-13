#ifndef WORLD_WEATHER_H
#define WORLD_WEATHER_H

typedef enum {
    WEATHER_CLEAR = 0,
    WEATHER_RAIN,
    WEATHER_SNOW,
} weather_state;

typedef struct {
    weather_state state;
    float         intensity;  // 0..1
    float         timer;
    float         next_change;
} weather;

void weather_init(weather *w);
void weather_update(weather *w, float dt);
const char *weather_name(weather_state s);

#endif
