#ifndef WORLD_SAVE_H
#define WORLD_SAVE_H

#include "chunk.h"

// disk format:
// u32 magic  ('VOXL')
// u32 version
// i32 cx
// i32 cz
// u8  blocks[CHUNK_VOLUME]
//
// file path: saves/cXX_cZZ.chunk

int  save_load_chunk(chunk *c);  // returns 1 if loaded
int  save_save_chunk(chunk *c);  // returns 0 on success

#endif
