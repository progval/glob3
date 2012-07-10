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
#include <string.h>
#include <math.h>
#include <time.h>
#include "utils.h"
#include "map.h"
#include "log.h"

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
            cell->resource_amount = 0;
            cell += 1; // Increments cell of sizeof(struct Terrain)
        }
    }
    map->terrain[1].resource = WHEAT;
    map->terrain[1].resource_amount = 2;
    map->terrain[2].resource = WOOD;
    map->terrain[2].resource_amount = 2;
    map->terrain[3].resource = STONE;
    map->terrain[3].resource_amount = 3;
    map->terrain[4].type = WATER;
    map->terrain[5].type = WATER;
    map->terrain[5].resource = ALGAE;
    map->terrain[5].resource_amount = 2;
    return map;
}

/**
 * \brief Grows the resources on the map.
 * \return A null-terminated array of coordinates of grown resources (as coordinate[2]).
 *
 * First, this function evaluates the number of cells to test (let's call it N);
 * this is done by multiplicating the number of cells by the GROWTH_COEFFICIENT.
 *
 * Then, n cells are tested for growability: if there is already a resource, it grows;
 * and if not, the neighboring is tested.
 */
coordinate** map_grow_resources(struct Map *map) {
    float amount = ((float) map->size_x*map->size_y)*GROWTH_COEFFICIENT;
    int actually_grown = 0;
    coordinate x, y;
    coordinate** coord = malloc(sizeof(coordinate*)*(ceil(amount)+2));
    struct Terrain *cell, *cell2;
    bool found;
    static bool rand_initialized = false;
    if (!rand_initialized) {
        srand(time(NULL));
        rand_initialized = true;
    }
    if (amount < 1 && randmax(floor(1/amount))==0)
        amount = 1;
    for (int i=0; i<floor(amount); i++) {
        x = randmax(map->size_x);
        y = randmax(map->size_y);
        cell = &map->terrain[x*map->size_y + y];
        if (cell->resource_amount > 0 && cell->resource_amount < MAX_TERRAIN_RESOURCES) {
            cell->resource_amount++;
            coord[actually_grown] = malloc(sizeof(coordinate)*2);
            coord[actually_grown][0] = x;
            coord[actually_grown][1] = y;
            actually_grown++;
        }
        else if (cell->resource_amount == 0) {
            found = false;
            for (int x2=(x ? x-1 : x); !found && x2<=min(x+1, map->size_x-1); x2++) {
                for (int y2=(y ? y-1 : y); y2<=min(y+1, map->size_y-1); y2++) {
                    cell2 = &map->terrain[x2*map->size_y + y2];
                    if (((x2 != x) || (y2 != y)) && (cell2->resource_amount >= 2)) {
                        if (cell2->resource == STONE)
                            continue; // Stone does not grow
                        if ((cell2->resource == WHEAT) && (cell->type != GRASS))
                            continue;
                        if ((cell2->resource == WOOD) && (cell->type != GRASS))
                            continue;
                        if ((cell2->resource == ALGAE) && (cell->type != WATER))
                            continue;
                        cell->resource = cell2->resource;
                        cell->resource_amount = 1;
                        coord[actually_grown] = malloc(sizeof(coordinate)*2);
                        coord[actually_grown][0] = x;
                        coord[actually_grown][1] = y;
                        actually_grown++;
                        break;
                    }
                }
            }
        }

    }
    coord[actually_grown] = (coordinate*) NULL;
    return coord;
}

/**
 * \brief Wrapper for map_grow_resource, as a callback, and calling map_change callbacks.
 * \see cb_game_tick
 */
void map_grow_resources_on_game_tick(struct Game *game, struct Player *player) {
    coordinate **coord = map_grow_resources(game->map);
    for (struct PlayerList *players = game->players; players; players=players->next) {
        if (*coord)
            for (struct CallbackList *callbacks = players->player->callbacks->map_change; callbacks; callbacks=callbacks->next)
                ((cb_map_change) callbacks->callback)(game, players->player, coord);
    }
    free(coord);
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
