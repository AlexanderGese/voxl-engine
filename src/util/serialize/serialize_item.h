#ifndef UTIL_SERIALIZE_ITEM_H
#define UTIL_SERIALIZE_ITEM_H

// dropped items and containers. small fixed-ish structs, nothing fancy.
// dropped items get a 'ITMS' section, containers a 'CONT' section. both store
// only the live entries, not the whole fixed array, so an empty world is a
// couple of bytes.

#include "serialize_writer.h"
#include "serialize_reader.h"
#include "../../world/item.h"
#include "../../world/container.h"

#define SERIALIZE_ITEM_VERSION 1
#define SERIALIZE_CONT_VERSION 1

// writes a 'ITMS' section for every alive dropped item in iw.
void serialize_items_write(serialize_writer *w, const item_world *iw);

// reads a 'ITMS' body back into iw (positioned at body start). returns 0 ok.
int  serialize_items_read(serialize_reader *r, item_world *iw);

// a single container at world position (wx,wy,wz). the position is part of the
// body so the loader can re-key it into the side hashmap.
void serialize_container_write(serialize_writer *w, int wx, int wy, int wz,
                               const container *c);
int  serialize_container_read(serialize_reader *r, int *wx, int *wy, int *wz,
                              container *c);

#endif
