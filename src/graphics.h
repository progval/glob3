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
 * \file graphics.h
 * \brief graphics.c headers.
 */

#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

enum GraphicsResourceType {
    TERRAIN, RESOURCE, UNIT, BUILDING, 
};
enum GraphicsDirection {
    BOTTOMRIGHT=0, BOTTOM, BOTTOMLEFT, LEFT, TOPLEFT, TOP, TOPRIGHT, RIGHT,
};

char* graphics_get_resource_name(enum GraphicsResourceType resource_type, int resource, enum GraphicsDirection direction, int variant);

#endif
