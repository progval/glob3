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

#include <stdlib.h>
#include "utils.h"
#include "map.h"

struct Map* map_create_simple_map(coordinate size_x, coordinate size_y) {
    // Creates a simple map, with grass and a square of water.
    struct Map* map = malloc(sizeof(struct Map));
    map->size_x = size_x;
    map->size_y = size_y;
    map->terrain = malloc(sizeof(enum TerrainType)*size_x*size_y);
    int middle_x = size_x/2, middle_y = size_y/2;
    int half_square_border = (size_x < size_y) ? size_x/6 : size_y/6;
    enum TerrainType *cell = map->terrain;
    for (int x=0; x<size_x; x++) {
        for (int y=0; y<size_y; y++) {
            if (x < UNKNOWN_TERRAIN && y == 0)
                *cell = x;
            else if (max(abs(x-middle_x), abs(y-middle_y)) <= half_square_border)
                // On the border of the square
                *cell = WATER;
            else
                *cell = GRASS;
            cell += 1; // Increments cell of sizeof(enum TerrainType)
        }
    }
    return map;
}

void map_free(struct Map *map) {
    if (map) {
        free(map->terrain);
        free(map);
    }
}
