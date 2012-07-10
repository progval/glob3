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
 * \file main.c
 * \brief Starting point of the program.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "callbacks.h"
#include "player.h"
#include "config.h"
#include "game.h"
#include "map.h"
#include "log.h"

#if WITH_GUI
#include "gui.h"
#endif

int main(int argc, char* argv[])
{
    bool net=WITH_NET, daemon=(!WITH_GUI);
    char *self = argv[0];

    for (int i=1; i<argc; i++) {
        if (strcmp(argv[i], "--disable-net")==0)
            net = false;
        else if (strcmp(argv[i], "--loglevel")==0) {
            if (i+1 == argc) {
                log_msg(LOG_CRITICAL, "boot", 1, "Log level not given.");
                return EXIT_FAILURE;
            }
            if (log_set_level_from_verbose(argv[i+1]))
                i++;
            else {
                log_msg(LOG_CRITICAL, "boot", 3, "'", argv[i+1], "' is not a valid log level.");
                return EXIT_FAILURE;
            }
        }
        else if (strcmp(argv[i], "--daemon")==0) {
            if (!WITH_SERVER) {
                log_msg(LOG_CRITICAL, "boot", 1, "Cannot run server: not built.");
                return EXIT_FAILURE;
            }
            else
                daemon = true;
        }
        else
            log_msg(LOG_WARNING, "boot", 3, "Unrecognized option: ", argv[i], ".");
    }

    struct PlayerCallbacks *callbacks = malloc(sizeof(struct PlayerCallbacks));
    struct Game *game = malloc(sizeof(struct Game));
    game->state = NOT_STARTED;
    game->map = map_create_simple_map(128, 128);
    struct Player *player = malloc(sizeof(struct Player));
    player->name = "test";
    player->callbacks = callbacks;
    game->players = malloc(sizeof(struct PlayerList));
    game->players->next = NULL;
    game->players->player = player;

#if WITH_GUI
    struct Gui* gui = gui_init(800, 600, 200);
    gui->camera->x = 0;
    gui->camera->y = 0;
#endif
    if (daemon) {
        log_msg(LOG_INFO, "boot", 1, "Running as daemon.");
        if (!net) {
            log_msg(LOG_CRITICAL, "boot", 1, "Cannot run as daemon if network support is disabled.");
            return EXIT_FAILURE;
        }
        callbacks->game_start = NULL;
        callbacks->game_tick = NULL;
        callbacks->game_end = NULL;
    }
#if WITH_GUI
    else {
        log_msg(LOG_INFO, "boot", 1, "Starting the GUI.");
        if (net)
            log_msg(LOG_INFO, "boot", 1, "Running with network support.");
        else
            log_msg(LOG_INFO, "boot", 1, "Running without network support.");
        player->handler = gui;
        callbacks->game_start = malloc(sizeof(struct CallbackList));
        callbacks->game_start->next = NULL;
        callbacks->game_start->callback = (cb_game_start) gui_on_game_start;

        callbacks->game_tick = malloc(sizeof(struct CallbackList));
        callbacks->game_tick->next = NULL;
        callbacks->game_tick->callback = (cb_game_tick) gui_on_game_tick;

        if (!net) { // There is no server to handle growing
            callbacks->game_tick->next = malloc(sizeof(struct CallbackList));
            callbacks->game_tick->next->next = NULL;
            callbacks->game_tick->next->callback = (cb_game_tick) map_grow_resources_on_game_tick;
        }

        callbacks->map_change = malloc(sizeof(struct CallbackList));
        callbacks->map_change->next = NULL;
        callbacks->map_change->callback = (cb_map_change) gui_on_map_change;

        callbacks->game_end = malloc(sizeof(struct CallbackList));
        callbacks->game_end->next = NULL;
        callbacks->game_end->callback = (cb_game_end) gui_on_game_end;
    }
#endif

    callbacks->unit_die = NULL;
    callbacks->unit_spawn = NULL;

    game_run(game);
    map_free(game->map);
    game_free(game);
    player_free(player);
#if WITH_GUI
    if (!daemon)
        gui_free(gui);
#endif

    return EXIT_SUCCESS;
}

