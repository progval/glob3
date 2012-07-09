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
 * \file gui.h
 * \brief gui.c headers.
 */

#ifndef __GUI_H__
#define __GUI_H__

#include <SDL/SDL.h>
#include "game.h"

#define GUI_TERRAIN_BORDER 32
#define GUI_MINIMAP_MARGIN 10

/**
 * \brief Different possible modes.
 */
enum GuiMode {
    MAIN_MENU, IN_GAME
};

/**
 * \brief Represents the area viewed by the player.
 *
 * Only the top left border coordinates are needed, because the width and 
 * the height are automatically computed from the gui settings.
 * \see gui_get_camera_height
 * \see gui_get_camera_width
 */
struct GuiCamera {
    coordinate x, y;
};

/**
 * \brief Represents the graphical user interface.
 * \see gui_init
 * \see gui_free
 */
struct Gui {
    enum GuiMode mode;
    int size_x, size_y, menu_width;
    SDL_Surface *screen;
    struct GuiCamera *camera;
};

struct Gui* gui_init(int size_x, int size_y, int menu_width);

void gui_on_game_start(struct Game *game, struct Player *player);
void gui_on_game_tick(struct Game *game, struct Player *player);
void gui_on_game_end(struct Game *game, struct Player *player);

void gui_free(struct Gui *gui);

#endif
