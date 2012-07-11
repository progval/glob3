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
 * \file building.c
 * \brief Handle buildings-related stuff.
 */

#include <stdlib.h>
#include "building.h"
#include "player.h"
#include "map.h"

const coordinate building_size[][MAX_BUILDING_LEVEL][2] = {
    {{2, 2}, {2, 2}, {3, 3}}, // Inn
    {{3, 3}, {0, 0}, {0, 0}}, // Swarm
};
const int building_health[][MAX_BUILDING_LEVEL] = {
    {200, 500, 700}, // Inn
    {700,   0,   0}, // Swarm
};

/**
 * \brief Create a building and return an pointer to it.
 * \param player The owner of the building.
 * \param type The type of the building.
 */
struct Building* building_new(struct Player *player, enum BuildingType type) {
    struct Building *building = malloc(sizeof(struct Building));
    building->owner = player;
    building->type = type;
    building->health = building_health[type][1];
    building->level = 0;
    building->in_progress = false;
    return building;
}

/**
 * \brief Get a pointer to the building at given coordinates.
 * \return NULL if no building is found at this place.
 */
struct Building* building_get(struct Map *map, coordinate x, coordinate y) {
    struct BuildingList *buildings;
    for (int i=0; i<UNKNOWN_BUILDING; i++) {
        buildings = map->buildings[i];
        while (buildings) {
            const coordinate *size = building_size[i][buildings->building->level];
            if ((x >= buildings->x) && (x < buildings->x+size[0]) && (y >= buildings->y) && (y < buildings->y+size[1]))
                return buildings->building;
            buildings = buildings->next;
        }
    }
    return NULL;
}

/**
 * \brief Return the width of a building.
 * \see building_get_height
 */
coordinate building_get_width(struct Building *building) {
    return building_size[building->type][building->level][0];
}

/**
 * \brief Return the height of a building.
 * \see building_get_width
 */
coordinate building_get_height(struct Building *building) {
    return building_size[building->type][building->level][1];
}
