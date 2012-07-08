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

#ifndef __GUI_H__
#define __GUI_H__

#include <SDL/SDL.h>
#include "game.h"

#define GUI_TERRAIN_BORDER 32
#define GUI_MINIMAP_MARGIN 10

enum GuiMode {
    MAIN_MENU, IN_GAME
};

struct GuiCamera {
    coordinate x, y;
};

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
