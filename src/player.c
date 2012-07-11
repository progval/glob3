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
 * \file player.c
 * \brief Handle player-related stuff.
 */

#include <stdlib.h>
#include "player.h"

/**
 * \brief Recursive CallbackList freer.
 */

void player_callbacks_free(struct CallbackList *list) {
    if (list)
        player_callbacks_free(list->next);
    free(list);
}

/**
 * \brief Free the resources uses by the player instance.
 * \param gui The player instance.
 */
void player_free(struct Player *player) {
    player_callbacks_free(player->callbacks->game_start);
    player_callbacks_free(player->callbacks->game_tick);
    player_callbacks_free(player->callbacks->game_end);
    player_callbacks_free(player->callbacks->map_change);
    player_callbacks_free(player->callbacks->unit_die);
    player_callbacks_free(player->callbacks->unit_spawn);
    free(player->callbacks);
    free(player);
}
