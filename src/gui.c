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
#include <SDL/SDL.h>
#include <math.h>
#include "player.h"
#include "utils.h"
#include "game.h"
#include "map.h"
#include "gui.h"
#include "log.h"

Uint32 gui_get_terrain_color(SDL_PixelFormat *format, enum TerrainType terrain) {
    char *type = malloc(sizeof(enum TerrainType)*2);
    switch(terrain) {
        case GRASS:  return SDL_MapRGB(format,  20, 200, 100);
        case WHEAT:  return SDL_MapRGB(format, 200, 200,   0);
        case FOREST: return SDL_MapRGB(format,   0, 200,   0);
        case STONE:  return SDL_MapRGB(format, 100, 100, 100);
        case WATER:  return SDL_MapRGB(format,   0,   0, 200);
        case ALGAE:  return SDL_MapRGB(format,   0, 100,  70);
        default:
            sprintf(type, "%x", terrain);
            log_msg(LOG_WARNING, "gui", 2, "Asking for color of unknown terrain type: 0x", type);
            free(type);
            return SDL_MapRGB(format, 0, 0, 0);
    }
}

float gui_get_camera_width(struct Gui *gui) {
    return ((float) gui->size_x - gui->menu_width)/GUI_TERRAIN_BORDER;
}
float gui_get_camera_height(struct Gui *gui) {
    return ((float) gui->size_y)/GUI_TERRAIN_BORDER;
}

void gui_draw(struct Game *game, struct Player *player) {
    // Redraw the entire camera (view + minimap)
    struct Gui* gui = (struct Gui*) player->handler;
    SDL_Rect coord, minimap_coord, minimap_rectangle;
    SDL_Surface *surface;
    Uint32 color;
    enum TerrainType *cell = game->map->terrain;
    int relative_x, relative_y;

    // Some handy pre-computed values
    const int minimap_width = gui->menu_width-2*GUI_MINIMAP_MARGIN;
    const float minimap_element_size = ((float) minimap_width)/game->map->size_x;
    const int minimap_height = game->map->size_y*minimap_element_size;
    const int minimap_corner1_x = gui->size_x-gui->menu_width+GUI_MINIMAP_MARGIN;
    const int minimap_corner1_y = GUI_MINIMAP_MARGIN;
    const int minimap_camera_corner1_x = minimap_corner1_x + gui->camera->x*minimap_element_size;
    const int minimap_camera_corner1_y = minimap_corner1_y + gui->camera->y*minimap_element_size;
    const int minimap_camera_corner2_x = minimap_camera_corner1_x + (((float) (gui->size_x - gui->menu_width))/GUI_TERRAIN_BORDER)*minimap_element_size;
    const int minimap_camera_corner2_y = minimap_camera_corner1_y + (((float) gui->size_y)/GUI_TERRAIN_BORDER)*minimap_element_size;

    // Draw the camera and the minimap
    for (int x=0; x<(game->map->size_x); x++) {
        relative_x = x - gui->camera->x;
        coord.x = relative_x*GUI_TERRAIN_BORDER;
        minimap_coord.x = minimap_corner1_x + x*minimap_element_size;
        for (int y=0; y<game->map->size_y; y++) {
            color = gui_get_terrain_color(gui->screen->format, *cell);

            // Draw on camera
            relative_y = y - gui->camera->y;
            coord.y = relative_y*GUI_TERRAIN_BORDER;
            surface = SDL_CreateRGBSurface(SDL_HWSURFACE, GUI_TERRAIN_BORDER, GUI_TERRAIN_BORDER, 32, 0, 0, 0, 0);
            SDL_FillRect(surface, NULL, color);
            if (relative_x >= 0 && coord.x+GUI_TERRAIN_BORDER < gui->size_x-gui->menu_width && relative_y >= 0 && coord.y+GUI_TERRAIN_BORDER < gui->size_x) {
                // Display only if needed.
                SDL_BlitSurface(surface, NULL, gui->screen, &coord);
            }
            SDL_FreeSurface(surface);

            // Draw on minimap
            minimap_coord.y = GUI_MINIMAP_MARGIN + y*minimap_element_size;
            surface = SDL_CreateRGBSurface(SDL_HWSURFACE, ceil(minimap_element_size), ceil(minimap_element_size), 32, 0, 0, 0, 0);
            SDL_FillRect(surface, NULL, color);
            SDL_BlitSurface(surface, NULL, gui->screen, &minimap_coord);
            SDL_FreeSurface(surface);

            cell += 1;
        }
    }
    
    // Draw rectangle on the minimap
    surface = SDL_CreateRGBSurface(SDL_HWSURFACE, ceil(minimap_element_size), ceil(minimap_element_size), 32, 0, 0, 0, 0);
    SDL_FillRect(surface, NULL, SDL_MapRGB(gui->screen->format, 0, 0, 0));
    for (minimap_coord.x=minimap_camera_corner1_x; minimap_coord.x<=minimap_camera_corner2_x; minimap_coord.x++) {
        minimap_coord.y = minimap_camera_corner1_y;
        SDL_BlitSurface(surface, NULL, gui->screen, &minimap_coord);
        minimap_coord.y = minimap_camera_corner2_y;
        SDL_BlitSurface(surface, NULL, gui->screen, &minimap_coord);
    }
    for (minimap_coord.y=minimap_camera_corner1_y; minimap_coord.y<=minimap_camera_corner2_y; minimap_coord.y++) {
        minimap_coord.x = minimap_camera_corner1_x;
        SDL_BlitSurface(surface, NULL, gui->screen, &minimap_coord);
        minimap_coord.x = minimap_camera_corner2_x;
        SDL_BlitSurface(surface, NULL, gui->screen, &minimap_coord);
    }
    SDL_FreeSurface(surface);

    SDL_Flip(gui->screen);
}

struct Gui* gui_init(int size_x, int size_y, int menu_width) {
    if (SDL_Init(SDL_INIT_VIDEO) == -1)
        log_msg(LOG_WARNING, "gui", 2, "SDL error: ", SDL_GetError());
    else {
        log_msg(LOG_DEBUG, "gui", 1, "SDL successfully initialized");
        struct Gui *gui = malloc(sizeof(struct Gui));
        gui->size_x = size_x;
        gui->size_y = size_y;
        gui->menu_width = menu_width;
        gui->screen = SDL_SetVideoMode(size_x, size_y, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
        gui->camera = malloc(sizeof(struct GuiCamera));
        SDL_WM_SetCaption("Globulation 3", NULL);
        SDL_EnableKeyRepeat(25, 25);
        return gui;
    }
}

void gui_on_game_start(struct Game *game, struct Player *player) {
    log_msg(LOG_DEBUG, "gui", 1, "Game started.");
    struct Gui* gui = (struct Gui*) player->handler;
    gui_draw(game, player);
    log_msg(LOG_DEBUG, "gui", 1, "Map drawn.");
}
void gui_on_game_tick(struct Game *game, struct Player *player) {
    struct Gui* gui = (struct Gui*) player->handler;
    assert(game->state == TICKING);
    SDL_Event event;
    if (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                log_msg(LOG_DEBUG, "gui", 1, "Window closed or SIGINT received.");
                game->state = ENDING;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        if (gui->camera->y > 0)
                            gui->camera->y--;
                        break;
                    case SDLK_DOWN:
                        if (gui->camera->y + (gui->size_y/GUI_TERRAIN_BORDER) < game->map->size_y)
                            gui->camera->y++;
                        break;
                    case SDLK_LEFT:
                        if (gui->camera->x > 0)
                            gui->camera->x--;
                        break;
                    case SDLK_RIGHT:
                        if (gui->camera->x + ((gui->size_x - gui->menu_width)/GUI_TERRAIN_BORDER ) < game->map->size_x)
                            gui->camera->x++;
                        break;
                }
                gui_draw(game, player);
                break;
        }
    }
}
void gui_on_game_end(struct Game *game, struct Player *player) {
    struct Gui* gui = (struct Gui*) player->handler;
    log_msg(LOG_DEBUG, "gui", 1, "Game ended.");
}

void gui_free(struct Gui *gui) {
    free(gui);
    SDL_Quit();
    log_msg(LOG_DEBUG, "gui", 1, "SDL unloaded.");
}
