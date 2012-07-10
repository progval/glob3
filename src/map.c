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
 * \file map.c
 * \brief Handle map-related stuff.
 */

#include <stdlib.h>
#include "utils.h"
#include "map.h"

/**
 * \brief Create a simple map, for debugging purposes.
 * \param size_x Width of the map.
 * \param size_y Height of the map.
 *
 * Creates a simple map, filled with grass, a square of water in the middle,
 * and a terrain of each type in the top left corner.
 */
struct Map* map_create_simple_map(coordinate size_x, coordinate size_y) {
    // Creates a simple map, with grass and a square of water.
    struct Map* map = malloc(sizeof(struct Map));
    map->size_x = size_x;
    map->size_y = size_y;
    map->terrain = malloc(sizeof(struct Terrain)*size_x*size_y);
    int middle_x = size_x/2, middle_y = size_y/2;
    int half_square_border = (size_x < size_y) ? size_x/6 : size_y/6;
    struct Terrain *cell = map->terrain;
    for (int x=0; x<size_x; x++) {
        for (int y=0; y<size_y; y++) {
            if (max(abs(x-middle_x), abs(y-middle_y)) <= half_square_border)
                // On the border of the square
                cell->type = WATER;
            else
                cell->type = GRASS;
            cell->resource = NO_RESOURCE;
            cell += 1; // Increments cell of sizeof(struct Terrain)
        }
    }
    map->terrain[1].resource = WHEAT;
    map->terrain[2].resource = WOOD;
    map->terrain[3].resource = STONE;
    map->terrain[4].type = WATER;
    map->terrain[5].type = WATER;
    map->terrain[5].resource = ALGAE;
    return map;
}

/**
 * \brief Free resources used by a map instance.
 */
void map_free(struct Map *map) {
    if (map) {
        free(map->terrain);
        free(map);
    }
}
