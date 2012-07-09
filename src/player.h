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
 * \file player.h
 * \brief player.c headers.
 */

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "protocol.h"
#include "callbacks.h"

/**
 * \brief Available player types.
 */
enum PlayerType {
    HUMAN, AI,
    UNKNOWN_PLAYER,
};

/**
 * Container for game callbacks.
 * \see game_run
 */
struct PlayerCallbacks {
    /**
     * \see gui_on_game_start
     */
    cb_game_start game_start;
    /**
     * \see gui_on_game_tick
     */
    cb_game_tick game_tick;
    /**
     * \see gui_on_game_end
     */
    cb_game_end game_end;
    cb_map_change map_change;
    cb_unit_die unit_die;
    cb_unit_spawn unit_spawn;
};

struct Player {
    char *name;
    enum PlayerType type;
    void *handler; // Data structure handling the user (gui, connection, AI, ...) passed to callbacks
    struct PlayerCallbacks *callbacks;
};

struct PlayerList {
    struct PlayerList *next;
    struct Player *player;
};

void player_free(struct Player *player);

#endif
