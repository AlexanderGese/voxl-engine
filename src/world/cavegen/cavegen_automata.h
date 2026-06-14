#ifndef WORLD_CAVEGEN_AUTOMATA_H
#define WORLD_CAVEGEN_AUTOMATA_H

#include "cavegen_grid.h"
#include "cavegen_types.h"

// 3d cellular automata. the classic "random fill then smooth" cave shaping,
// lifted into 3d. we gate the initial fill through the cheese noise so caves
// only spawn in the regions the noise allows, otherwise the whole world turns
// to swiss cheese and you fall through the floor on spawn. ask me how i know.

// seed the grid: every eligible cell is randomly air-or-solid, weighted by
// params.fill_chance, but only where the cheese noise opens up and where we are
// far enough under the surface. cells outside that stay solid.
void cavegen_automata_seed(cavegen_grid *g, const cavegen_params *p);

// run params.ca_iterations smoothing passes. uses a 26-neighbour moorhood and
// the birth/survive thresholds. double buffered internally.
void cavegen_automata_smooth(cavegen_grid *g, const cavegen_params *p);

// count solid neighbours in the 3x3x3 around a cell (excluding the cell).
// oob counts as solid, which conveniently caps caves at the region skirt.
int  cavegen_automata_solid_neighbours(const cavegen_grid *g, int x, int y, int z);

#endif
