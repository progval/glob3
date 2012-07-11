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
 * \file gui.c
 * \brief Graphical user interface.
 *
 * This file handles the main game interface, shown to the player.
 * Depends on the SDL.
 */

#include <assert.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL.h>
#include <math.h>
#include "building.h"
#include "graphics.h"
#include "player.h"
#include "config.h"
#include "utils.h"
#include "game.h"
#include "map.h"
#include "gui.h"
#include "log.h"
#include "os.h"

SDL_Surface* gui_terrain_backgrounds_cache[UNKNOWN_TERRAIN_TYPE][4][4];
SDL_Surface* gui_terrain_foregrounds_cache[UNKNOWN_RESOURCE][MAX_TERRAIN_RESOURCES+1];
SDL_Surface* gui_buildings_cache[UNKNOWN_BUILDING][MAX_BUILDING_LEVEL][2]; // 2 stands for the two building->in_progress states.

/**
 * \brief Return the SDL color matching the terrain.
 * \param format The SDL color format. Usually accessible via screen->format.
 * \param terrain The terrain type, as stored in the map.
 * \return An SDL color.
 */

Uint32 gui_get_terrain_color(SDL_PixelFormat *format, struct Terrain terrain) {
    char *type;
    switch (terrain.resource) {
        case WHEAT:  return SDL_MapRGB(format, 200, 200,   0);
        case WOOD:   return SDL_MapRGB(format,   0, 100,   0);
        case STONE:  return SDL_MapRGB(format, 100, 100, 100);
        case ALGAE:  return SDL_MapRGB(format,  50, 100,  70);
        case NO_RESOURCE:
            switch(terrain.type) {
                case GRASS:  return SDL_MapRGB(format,  20, 200, 100);
                case WATER:  return SDL_MapRGB(format,   0,   0, 200);
            }
        default:
            type = malloc(sizeof(enum TerrainType)*2);
            sprintf(type, "%x", terrain);
            log_msg(LOG_WARNING, "gui", 2, "Asking for color of unknown terrain type: 0x", type);
            free(type);
            return SDL_MapRGB(format, 0, 0, 0);
    }
}

/**
 * \brief Get a sprite of the background of the given terrain.
 * \param format Usually accessible via screen->format
 * \param terrain The terrain you want the sprite.
 * \param x,y Coordinates of the terrain. Used for variants and surface smoothing.
 * \return The sprite.
 * \see gui_get_terrain_foreground.
 */
SDL_Surface* gui_get_terrain_background(SDL_PixelFormat *format, struct Terrain terrain, coordinate x, coordinate y) {
    if (gui_terrain_backgrounds_cache[terrain.type][x%4][y%4])
        return gui_terrain_backgrounds_cache[terrain.type][x%4][y%4];
    char *resource = graphics_get_resource_name(TERRAIN, terrain.type, 0, (x%4) + 4*(y%4));
    assert(resource != NULL);
    resource = strcat_realloc_free(resource, ".png");
    char *path = os_path_join(3, RESOURCES_PREFIX, "graphics", resource);
    SDL_Surface *surface = IMG_Load(path);
    if (!surface)
        log_msg(LOG_ERROR, "gui", 2, "Failed to load resource ", path);
    free(resource);
    free(path);
    gui_terrain_backgrounds_cache[terrain.type][x%4][y%4] = surface;
    return surface;
        
}

/**
 * \brief Get a sprite of the foreground of the given terrain.
 * \param format Usually accessible via screen->format
 * \param terrain The terrain you want the sprite.
 * \return The sprite.
 * \see gui_get_terrain_background.
 * \see gui_get_building
 */
SDL_Surface* gui_get_terrain_foreground(SDL_PixelFormat *format, struct Terrain terrain) {
    if (terrain.resource == NO_RESOURCE)
        return NULL;
    if (gui_terrain_foregrounds_cache[terrain.resource][terrain.resource_amount])
        return gui_terrain_foregrounds_cache[terrain.resource][terrain.resource_amount];
    char *resource;
    int variant;
    assert(terrain.resource != NO_RESOURCE || terrain.resource_amount == 0);
    assert(terrain.resource == NO_RESOURCE || terrain.resource_amount != 0);
    if (terrain.resource == WHEAT || terrain.resource == WOOD || terrain.resource == STONE || terrain.resource == ALGAE)
        variant = terrain.resource_amount-1;
    else
        variant = 0;
    assert(variant >= 0);
    resource = graphics_get_resource_name(RESOURCE, terrain.resource, 0, variant);
    resource = strcat_realloc_free(resource, ".png");
    char *path = os_path_join(3, RESOURCES_PREFIX, "graphics", resource);
    SDL_Surface *surface = IMG_Load(path);
    if (!surface)
        log_msg(LOG_ERROR, "gui", 2, "Failed to load resource ", path);
    else
        log_msg(LOG_DEBUG, "gui", 2, "Loading resource ", path);
    free(resource);
    free(path);
    gui_terrain_foregrounds_cache[terrain.resource][terrain.resource_amount] = surface;
    return surface;
        
}

/**
 * \brief Get a sprite for the given building.
 * \see gui_get_terrain_foreground
 */
SDL_Surface* gui_get_building(SDL_PixelFormat *format, const struct Building *building) {
    if (gui_buildings_cache[building->type][building->level][building->in_progress])
        return gui_buildings_cache[building->type][building->level][building->in_progress];
    char *resource, *resource2;
    resource = graphics_get_resource_name(BUILDING, building->type, 0, building->level);
    resource2 = malloc(strlen(resource)+7);
    strcpy(resource2, resource);
    if (building->in_progress)
        strcat(resource2, "c0");
    else
        strcat(resource2, "b0");
    if (building->type == SWARM)
        strcat(resource2, "r");
    strcat(resource2, ".png");
    free(resource);
    resource = resource2;
    char *path = os_path_join(3, RESOURCES_PREFIX, "graphics", resource);
    SDL_Surface *surface = IMG_Load(path);
    if (!surface)
        log_msg(LOG_ERROR, "gui", 2, "Failed to load resource ", path);
    else
        log_msg(LOG_DEBUG, "gui", 2, "Loading resource ", path);
    free(resource);
    free(path);
    gui_buildings_cache[building->type][building->level][building->in_progress] = surface;
    return surface;
}


/**
 * \brief Shortcut for getting the camera width.
 * \param gui The gui the camera belongs to.
 * \return Width of the camera, as a number of terrain elements.
 * \see gui_get_camera_height
 */

float gui_get_camera_width(const struct Gui *gui) {
    return ((float) gui->size_x - gui->menu_width)/GUI_TERRAIN_BORDER;
}

/**
 * \brief Shortcut for getting the camera height.
 * \param gui The gui the camera belongs to.
 * \return Height of the camera, as a number of terrain elements.
 * \see gui_get_camera_width
 */
float gui_get_camera_height(const struct Gui *gui) {
    return ((float) gui->size_y)/GUI_TERRAIN_BORDER;
}

void gui_draw_terrain_on_camera(const struct Gui *gui, const struct Map *map, const coordinate x, const coordinate y, bool redraw);
/**
 * \brief Draw a building on the camera.
 * \see gui_draw
 * \see gui_draw_building_on_camera
 */
void gui_draw_building_on_camera(const struct Gui *gui, const struct Map *map, const coordinate x, const coordinate y, struct Building *building) {
    const int relative_x = x - gui->camera->x, relative_y = y - gui->camera->y;
    SDL_Rect camera_coord;
    camera_coord.x = relative_x*GUI_TERRAIN_BORDER;
    camera_coord.y = relative_y*GUI_TERRAIN_BORDER;
    SDL_Surface *surface;
    if (relative_x >= 0 && camera_coord.x+GUI_TERRAIN_BORDER < gui->size_x-gui->menu_width && relative_y >= 0 && camera_coord.y+GUI_TERRAIN_BORDER < gui->size_x) {
        // Redraw the terrain behind, so we do not loose border smoothing.
        for (int x2=x; x2 < x+building_get_width(building); x2++)
            for (int y2=y; y2 < y+building_get_height(building); y2++)
                gui_draw_terrain_on_camera(gui, map, x2, y2, (x2==x || y2==y));

        surface = gui_get_building(gui->screen->format, building);
        if (surface) {
            if (building->type == SWARM) {
                camera_coord.x -= 5;
                camera_coord.y -= GUI_TERRAIN_BORDER/2;
            }
            SDL_BlitSurface(surface, NULL, gui->screen, &camera_coord);
        }
    }
}

/**
 * \brief Draw a terrain on the camera, if needed.
 * \param redraw Determines whether or not this function will try to fix glitches with close cells.
 * \see gui_draw
 * \see gui_draw_terrain_on_minimap
 * \see gui_draw_building_on_camera
 */
void gui_draw_terrain_on_camera(const struct Gui *gui, const struct Map *map, const coordinate x, const coordinate y, bool redraw) {
    const int relative_x = x - gui->camera->x, relative_y = y - gui->camera->y;
    SDL_Rect camera_coord;
    SDL_Surface *background, *foreground;
    camera_coord.x = relative_x*GUI_TERRAIN_BORDER;
    camera_coord.y = relative_y*GUI_TERRAIN_BORDER;
    const struct Terrain *cell = &map->terrain[x*map->size_y + y];
    if (relative_x >= 0 && camera_coord.x+GUI_TERRAIN_BORDER < gui->size_x-gui->menu_width && relative_y >= 0 && camera_coord.y+GUI_TERRAIN_BORDER < gui->size_x) {
        background = gui_get_terrain_background(gui->screen->format, *cell, x, y);
        SDL_BlitSurface(background, NULL, gui->screen, &camera_coord);
        foreground = gui_get_terrain_foreground(gui->screen->format, *cell);
        if (foreground) {
            SDL_BlitSurface(foreground, NULL, gui->screen, &camera_coord);
            // Do not free the surface, it is cached.

            if (redraw && relative_x>0) {
                // Fix the left cell
                camera_coord.x -= GUI_TERRAIN_BORDER;
                foreground = gui_get_terrain_foreground(gui->screen->format, *(cell-map->size_y));
                if (foreground)
                    SDL_BlitSurface(foreground, NULL, gui->screen, &camera_coord);
                camera_coord.x += GUI_TERRAIN_BORDER;
            }
            if (redraw && relative_y>0) {
                // Fix the top cell
                camera_coord.y -= GUI_TERRAIN_BORDER;
                foreground = gui_get_terrain_foreground(gui->screen->format, *(cell-1));
                if (foreground)
                    SDL_BlitSurface(foreground, NULL, gui->screen, &camera_coord);
                camera_coord.y += GUI_TERRAIN_BORDER;
            }
            if (redraw && relative_x>0 && relative_y>0) {
                // Fix the top left cell
                camera_coord.x -= GUI_TERRAIN_BORDER;
                camera_coord.y -= GUI_TERRAIN_BORDER;
                foreground = gui_get_terrain_foreground(gui->screen->format, *(cell-1-map->size_y));
                if (foreground)
                    SDL_BlitSurface(foreground, NULL, gui->screen, &camera_coord);
            }
            if (redraw) {
                struct BuildingList *swarms = map->buildings[SWARM];
                while (swarms) {
                    gui_draw_building_on_camera(gui, map, swarms->x, swarms->y, swarms->building);
                    swarms = swarms->next;
                }
            }
        }
    }
}

/**
 * \brief Draw a terrain on the minimap.
 * \see gui_draw
 * \see gui_draw_terrain_on_camera
 */
void gui_draw_terrain_on_minimap(const struct Gui *gui, const struct Map *map, const coordinate x, const coordinate y) {
    const int minimap_width = gui->menu_width-2*GUI_MINIMAP_MARGIN;
    const float minimap_element_size = ((float) minimap_width)/map->size_x;
    const int minimap_height = map->size_y*minimap_element_size;
    const int minimap_corner1_x = gui->size_x-gui->menu_width+GUI_MINIMAP_MARGIN;
    const int minimap_corner1_y = GUI_MINIMAP_MARGIN;
    const struct Terrain *cell = &map->terrain[x*map->size_y + y];
    SDL_Surface *surface;
    SDL_Rect minimap_coord;
    Uint32 color = gui_get_terrain_color(gui->screen->format, *cell);
    minimap_coord.x = minimap_corner1_x + x*minimap_element_size;
    minimap_coord.y = GUI_MINIMAP_MARGIN + y*minimap_element_size;
    surface = SDL_CreateRGBSurface(SDL_HWSURFACE, ceil(minimap_element_size), ceil(minimap_element_size), 32, 0, 0, 0, 0);
    SDL_FillRect(surface, NULL, color);
    SDL_BlitSurface(surface, NULL, gui->screen, &minimap_coord);
    SDL_FreeSurface(surface);
}

/**
 * \brief Draw the rectangle on the minimap.
 * \see gui_draw
 * \see gui_draw_terrain_on_camera
 */
void gui_draw_rectangle_on_minimap(const struct Gui *gui, const struct Map *map) {
    const int minimap_width = gui->menu_width-2*GUI_MINIMAP_MARGIN;
    const float minimap_element_size = ((float) minimap_width)/map->size_x;
    const int minimap_height = map->size_y*minimap_element_size;
    const int minimap_corner1_x = gui->size_x-gui->menu_width+GUI_MINIMAP_MARGIN;
    const int minimap_corner1_y = GUI_MINIMAP_MARGIN;
    const int minimap_camera_corner1_x = minimap_corner1_x + gui->camera->x*minimap_element_size;
    const int minimap_camera_corner1_y = minimap_corner1_y + gui->camera->y*minimap_element_size;
    const int minimap_camera_corner2_x = minimap_camera_corner1_x + gui_get_camera_width(gui)*minimap_element_size;
    const int minimap_camera_corner2_y = minimap_camera_corner1_y + gui_get_camera_height(gui)*minimap_element_size;
    SDL_Surface *surface;
    SDL_Rect minimap_coord;
    surface = SDL_CreateRGBSurface(SDL_HWSURFACE, min(1, floor(minimap_element_size)), min(1, floor(minimap_element_size)), 32, 0, 0, 0, 0);
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
}

/**
 * \brief (Re)draw the entire interface.
 * \param game The game instance.
 * \param player The player who is using this gui instance.
 */
void gui_draw(struct Game *game, struct Player *player) {
    // Redraw the entire camera (view + minimap)
    struct Gui* gui = (struct Gui*) player->handler;
    struct Terrain *cell = game->map->terrain;

    // Some handy pre-computed values

    // Draw the camera and the minimap
    for (int x=game->map->size_x-1; x>=0; x--) {
        for (int y=game->map->size_y-1; y>=0; y--) {
            gui_draw_terrain_on_camera(gui, game->map, x, y, false); // No need to fix glitches
            gui_draw_terrain_on_minimap(gui, game->map, x, y);

            cell += 1;
        }
    }

    struct BuildingList *buildings;
    for (int i=0; i<UNKNOWN_BUILDING; i++) {
        buildings = game->map->buildings[i];
        while (buildings) {
            gui_draw_building_on_camera(gui, game->map, buildings->x, buildings->y, buildings->building);
            buildings = buildings->next;
        }
    }
    
    gui_draw_rectangle_on_minimap(gui, game->map);

    SDL_Flip(gui->screen);
}

/**
 * \brief Initialize the user interface.
 * \param size_x Width of the window.
 * \param size_y Height of the window.
 * \param menu_width Width of the right menu.
 * \return A gui instance.
 */
struct Gui* gui_init(int size_x, int size_y, int menu_width) {
    if (SDL_Init(SDL_INIT_VIDEO) == -1)
        log_msg(LOG_WARNING, "gui", 2, "SDL error: ", SDL_GetError());
    else {
        log_msg(LOG_DEBUG, "gui", 1, "SDL successfully initialized");
        struct Gui *gui = malloc(sizeof(struct Gui));
        gui->size_x = size_x;
        gui->size_y = size_y;
        gui->menu_width = menu_width - ((size_x-menu_width-GUI_MINIMAP_MARGIN) % GUI_TERRAIN_BORDER);
        gui->screen = SDL_SetVideoMode(size_x, size_y, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
        gui->camera = malloc(sizeof(struct GuiCamera));
        SDL_WM_SetCaption("Globulation 3", NULL);
        SDL_EnableKeyRepeat(25, 25);
        memset(gui_terrain_backgrounds_cache, 0, sizeof(SDL_Surface*)*UNKNOWN_TERRAIN_TYPE*4*4);
        memset(gui_terrain_foregrounds_cache, 0, sizeof(SDL_Surface*)*UNKNOWN_RESOURCE*(MAX_TERRAIN_RESOURCES+1));
        memset(gui_buildings_cache, 0, sizeof(SDL_Surface*)*UNKNOWN_BUILDING*(MAX_BUILDING_LEVEL+1)*2);
        return gui;
    }
}

/**
 * \brief Callback for the game start.
 * \param game The game instance
 * \param player The player who is using this gui instance.
 * \see game_run
 * \see PlayerCallbacks
 */
void gui_on_game_start(struct Game *game, struct Player *player) {
    log_msg(LOG_DEBUG, "gui", 1, "Game started.");
    struct Gui* gui = (struct Gui*) player->handler;
    gui_draw(game, player);
    log_msg(LOG_DEBUG, "gui", 1, "Map drawn.");
}

/**
 * \brief Callback on a game tick.
 * \param game The game instance
 * \param player The player who is using this gui instance.
 * \see game_run
 * \see PlayerCallbacks
 */
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
                    default:
                        return;
                }
                gui_draw(game, player);
                break;
        }
    }
}

/**
 * \brief Callback for map changes.
 * \param game The game instance
 * \param player The player who is using this gui instance.
 * \param coord Coordinates of changed terrains.
 */
void gui_on_map_change(struct Game *game, struct Player *player, const coordinate** coord) {
    coordinate x, y;
    for (int i=0; coord[i]; i++) {
        x = coord[i][0];
        y = coord[i][1];
        gui_draw_terrain_on_camera((struct Gui*) player->handler, game->map, x, y, true);
        gui_draw_terrain_on_minimap((struct Gui*) player->handler, game->map, x, y);
        gui_draw_rectangle_on_minimap((struct Gui*) player->handler, game->map);
    }
    SDL_Flip(((struct Gui*) player->handler)->screen);
}

/**
 * \brief Callback for the game end.
 * \param game The game instance
 * \param player The player who is using this gui instance.
 * \see game_run
 * \see PlayerCallbacks
 */
void gui_on_game_end(struct Game *game, struct Player *player) {
    struct Gui* gui = (struct Gui*) player->handler;
    log_msg(LOG_DEBUG, "gui", 1, "Game ended.");
}

/**
 * \brief Free the resources uses by the gui instance, and exists the SDL.
 * \param gui The gui instance.
 */
void gui_free(struct Gui *gui) {
    free(gui->camera);
    free(gui);
    SDL_Quit();
    log_msg(LOG_DEBUG, "gui", 1, "SDL unloaded.");
}
