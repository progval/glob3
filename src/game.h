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

#ifndef __GAME_H__
#define __GAME_H__

#include <stdbool.h>
#include "map.h"
#include "player.h"

enum GameState {
    NOT_STARTED, INITIALIZING, SYNCING, TICKING, ENDING,
};

struct Game {
    enum GameState state;
    struct Map *map;
    struct PlayerList *players;
};

void game_run(struct Game *game);
void game_free(struct Game *game);

#endif
