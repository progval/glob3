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
 * \file building.h
 * \brief building.c headers.
 */

#ifndef __BUILDING_H__
#define __BUILDING_H__

#include <stdbool.h>
#include "player.h"
#include "protocol.h"
#include "resource.h"

#define MAX_BUILDING_LEVEL 3

enum BuildingType {
    INN, SWARM,
    UNKNOWN_BUILDING,
};

struct Building {
    enum BuildingType type;
    struct Player *owner;
    unsigned int health;
    unsigned int level;
    /** Amount of resources in the building (including materials). */
    unsigned int contents[UNKNOWN_RESOURCE];
    /** Determine whether or not the construction is in progress. */
    bool in_progress;
};

struct BuildingList {
    struct BuildingList *next;
    coordinate x, y;
    struct Building *building;
};

struct Building* building_new(struct Player *player, enum BuildingType type);
struct Building* building_get(struct Map *map, coordinate x, coordinate y);
coordinate building_get_width(struct Building *building);
coordinate building_get_height(struct Building *building);

#endif
