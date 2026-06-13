#include "biome_color.h"

// all the math is in plain 0..255 channels. nothing fancy, just nudging the
// base tint by climate so the world doesnt look like one flat shade of green.

static int clampi(int v, int lo, int hi) {
    return v < lo ? lo : (v > hi ? hi : v);
}

static void unpack(uint32_t c, int *r, int *g, int *b) {
    *r = (c >> 16) & 0xFF;
    *g = (c >> 8)  & 0xFF;
    *b =  c        & 0xFF;
}

static uint32_t pack(int r, int g, int b) {
    return ((uint32_t)clampi(r, 0, 255) << 16)
         | ((uint32_t)clampi(g, 0, 255) << 8)
         |  (uint32_t)clampi(b, 0, 255);
}

uint32_t biome_color_mix(uint32_t a, uint32_t b, float t) {
    if (t <= 0.0f) return a;
    if (t >= 1.0f) return b;
    int ar, ag, ab, br, bg, bb;
    unpack(a, &ar, &ag, &ab);
    unpack(b, &br, &bg, &bb);
    int r = ar + (int)((br - ar) * t);
    int g = ag + (int)((bg - ag) * t);
    int bl = ab + (int)((bb - ab) * t);
    return pack(r, g, bl);
}

uint32_t biome_color_grass(uint32_t base, const biome_climate *c) {
    int r, g, b;
    unpack(base, &r, &g, &b);

    // dry + hot -> push toward yellow/brown (more red, less green/blue).
    // wet -> deepen green. centered so a mid climate is a no-op.
    float dry = 1.0f - c->humidity;          // 0 wet .. 1 dry
    float heat = c->temperature;

    r += (int)(dry * heat * 40.0f);
    g -= (int)(dry * 18.0f);
    b -= (int)(dry * 12.0f);

    // wet boost to green
    g += (int)(c->humidity * 14.0f);

    return pack(r, g, b);
}

uint32_t biome_color_foliage(uint32_t base, const biome_climate *c) {
    // start from the grass response then darken and cool it a hair
    uint32_t gtone = biome_color_grass(base, c);
    int r, g, b;
    unpack(gtone, &r, &g, &b);
    r = (int)(r * 0.88f);
    g = (int)(g * 0.92f);
    b = (int)(b * 0.90f) + 6;
    return pack(r, g, b);
}

uint32_t biome_color_water(uint32_t base, const biome_climate *c) {
    int r, g, b;
    unpack(base, &r, &g, &b);
    // warm stagnant water greens out, cold water stays blue
    float warm = c->temperature;
    g += (int)(warm * 20.0f);
    b -= (int)(warm * 16.0f);
    return pack(r, g, b);
}
