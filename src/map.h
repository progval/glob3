/*
  Copyright (C) 2012 Valentin Lorentz

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

/** 
 * \file map.h
 * \brief map.c headers.
 */

#ifndef __MAP_H__
#define __MAP_H__

#include <stdint.h>
#include "building.h"
#include "protocol.h"

/**
 * \brief Available terrain types.
 */
enum TerrainType {
    GRASS, WHEAT, FOREST, STONE,
    WATER, ALGAE,
    UNKNOWN_TERRAIN,
};

/**
 * \brief Represents the map of a game.
 */
struct Map {
    coordinate size_x, size_y;
    enum TerrainType *terrain; // two-dimensional array.
    struct BuildingList buildings[UNKNOWN_TERRAIN];
};

struct Map* map_create_simple_map(coordinate size_x, coordinate size_y);
void map_free(struct Map *map);

#endif
