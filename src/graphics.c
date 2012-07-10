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
 * \file graphics.c
 * \brief Graphics handling.
 *
 * This file differs of gui.c in that it may be used by interfaces that
 * are not gui.c: web interface, CLI interface, ...
 */

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "graphics.h"
#include "utils.h"
#include "unit.h"
#include "map.h"
#include "log.h"

/**
 * \brief Get the name of a resource.
 * \param resource_type The type of the resource (terrain, unit, ...)
 * \param resource The resource itself (for example, if resource_type is UNIT, it may
 * be WORKER, EXPLORER, or WARRIOR).
 * \param UnitDirection If this is a unit: its direction.
 * \param variant The number of the frame wanted, if the resource is an animation.
 * \return The path to the resource. NULL if the resource is not found.
 */
char* graphics_get_resource_name(enum GraphicsResourceType resource_type, int resource, enum GraphicsDirection direction, int variant) {
    char *prefix = NULL;
    switch (resource_type) {
        case TERRAIN:
            prefix = "terrain";
            switch (resource) {
                case GRASS:
                    if ((variant < 0) || (variant > 16))
                        return NULL;
                    break;
                case WATER:
                    variant += 256;
                    if ((variant < 256) || (variant > 271))
                        return NULL;
                    break;
                default:
                    log_msg(LOG_WARNING, "graphics", 1, "Asking for unknown TERRAIN resource.");
                    return NULL;
            }
            break;
        case RESOURCE:
            prefix = "ressource"; // Glob2 authors typoed it.
            if ((variant < 0) || (variant > 7)) {
                log_msg(LOG_WARNING, "graphics", 1, "Asking for unknown RESOURCE variant.");
                return NULL;
            }
            switch (resource) {
                case NO_RESOURCE:
                    return NULL;
                case WOOD:
                    break;
                case WHEAT:
                    variant += 10;
                    break;
                case STONE:
                    variant += 30;
                    break;
                case ALGAE:
                    variant += 40;
                    break;
                default:
                    log_msg(LOG_WARNING, "graphics", 1, "Asking for unknown RESOURCE resource.");
                    return NULL;
            }
            break;
        case UNIT:
            prefix = "unit";
            if ((variant < 0) || (variant > 7)) {
                log_msg(LOG_WARNING, "graphics", 1, "Asking for unknown UNIT variant.");
                return NULL;
            }
            switch (resource) {
                case EXPLORER:
                    variant += 8*direction;
                    break;
                case WORKER:
                    variant += 64 + 8*direction;
                    // TODO: handle swimming workers.
                    break;
                case WARRIOR:
                    variant += 256 + 8*direction;
                    // TODO: handle swimming warriors.
                    break;
            }
            break;
        case BUILDING:
            // TODO: implement this.
            break;
    }
    char variant_string[4]; // The variant never exceeds 447.
    sprintf(variant_string, "%i", variant);
    char *result = strcat_realloc(prefix, variant_string);
    return result;
}

