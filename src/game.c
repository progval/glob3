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

#include <assert.h>
#include <stdlib.h>
#include "callbacks.h"
#include "game.h"
#include "player.h"

void game_run(struct Game *game) {
    assert(game->state == NOT_STARTED);
    game->state = INITIALIZING;
    for (struct PlayerList *players = game->players; players; players=players->next)
        if (players->player->callbacks->game_start)
            players->player->callbacks->game_start(game, players->player);

    game->state = SYNCING;
    while (game->state != ENDING) {
        game->state = TICKING;
        for (struct PlayerList *players = game->players; players; players=players->next)
            if (players->player->callbacks->game_tick)
                players->player->callbacks->game_tick(game, players->player);
    }

    for (struct PlayerList *players = game->players; players; players=players->next)
        if (players->player->callbacks->game_end)
            players->player->callbacks->game_end(game, players->player);
}

void game_free(struct Game *game) {
    struct PlayerList *players;
    if (game) {
        while (game->players) {
            players = game->players->next;
            free(game->players);
            game->players = players;
        }
        free(game);
    }
}
