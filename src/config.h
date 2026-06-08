#ifndef CONFIG_H
#define CONFIG_H
#define WIN_TITLE          "voxl"
#define WIN_WIDTH          1280
#define WIN_HEIGHT           720
#define CHUNK_SIZE_X        16
#define CHUNK_SIZE_Z        16
#define CHUNK_SIZE_Y       128
#define CHUNK_VOLUME       (CHUNK_SIZE_X * CHUNK_SIZE_Z * CHUNK_SIZE_Y)
#define RENDER_DISTANCE      6
#define LOAD_DISTANCE        (RENDER_DISTANCE + 2)
#define PLAYER_EYE_HEIGHT   1.7f
