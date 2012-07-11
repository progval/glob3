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
#include "resource.h"
#include "player.h"
#include "game.h"

/**
 * \brief Growth coefficient.
 * \see map_grow_resources
 */
#define GROWTH_COEFFICIENT 0.001

/**
 * \brief Maximum amount of resource in a terrain.
 */
#define MAX_TERRAIN_RESOURCES 4

/**
 * \brief Available terrain types.
 */
enum TerrainType {
    GRASS, WATER, UNKNOWN_TERRAIN_TYPE,
};

struct Terrain {
    enum TerrainType type;
    enum Resource resource;
    uint8_t resource_amount;
};

/**
 * \brief Represents the map of a game.
 */
struct Map {
    coordinate size_x, size_y;
    struct Terrain *terrain; // two-dimensional array.
    struct BuildingList* buildings[UNKNOWN_BUILDING]; // Storing buildings as a list makes searches faster, as the number of buildings is quite low.
};

struct Map* map_create_simple_map(struct Player *player, coordinate size_x, coordinate size_y);
coordinate** map_grow_resources(struct Map *map);
void map_free(struct Map *map);

void map_grow_resources_on_game_tick(struct Game *game, struct Player *player);

#endif
