#ifndef CONFIG_H
#define CONFIG_H

// global tunables

#define WIN_TITLE          "voxl"
#define WIN_WIDTH          1280
#define WIN_HEIGHT           720

#define CHUNK_SIZE_X        16
#define CHUNK_SIZE_Z        16
#define CHUNK_SIZE_Y       128
#define CHUNK_VOLUME       (CHUNK_SIZE_X * CHUNK_SIZE_Z * CHUNK_SIZE_Y)

// how many chunks around the player we keep loaded
#define RENDER_DISTANCE      6
#define LOAD_DISTANCE        (RENDER_DISTANCE + 2)

// player
#define PLAYER_EYE_HEIGHT   1.7f
#define PLAYER_WIDTH        0.6f
#define PLAYER_HEIGHT       1.8f
#define PLAYER_REACH        5.0f
#define PLAYER_MOVE_SPEED   4.3f
#define PLAYER_SPRINT_MULT  1.4f
#define PLAYER_JUMP_VEL     7.5f
#define GRAVITY            -22.0f
#define TERMINAL_VELOCITY  -60.0f

// world
#define WORLD_SEA_LEVEL     64
#define MAX_LIGHT           15

// atlas (16x16 tiles of 16px each = 256 tile ids)
#define ATLAS_TILES_X       16
#define ATLAS_TILES_Y       16

// hotbar
#define HOTBAR_SLOTS         9

// save
#define SAVE_MAGIC          0x564F584Cu  /* 'VOXL' */
#define SAVE_VERSION        1

// debug
// #define DEBUG_WIREFRAME
// #define DEBUG_DISABLE_CULLING

#endif
