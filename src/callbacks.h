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

#ifndef __CALLBACKS_H__
#define __CALLBACKS_H__

#include "protocol.h"

struct Game;
struct Unit;
struct Player;
struct Map;
enum TerrainType;

typedef void (*cb_game_start) (struct Game *game, void *data);
typedef void (*cb_game_tick) (struct Game *game, void *data);
typedef void (*cb_game_end) (struct Game *game, void *data);

typedef void (*cb_map_change) (coordinate x, coordinate y, struct Map *map, enum TerrainType previous_terrain);

typedef void (*cb_unit_spawn) (coordinate x, coordinate y, struct Map *map, struct Unit *unit);
typedef void (*cb_unit_die) (coordinate x, coordinate y, struct Map *map, struct Unit *unit, struct Unit *killer);

#endif
