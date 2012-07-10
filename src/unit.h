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
 * \file unit.h
 * \brief unit.c headers.
 */

#ifndef __UNIT_H__
#define __UNIT_H__

#include <stdint.h>

typedef uint8_t unit_level;
typedef uint8_t unit_hungry;

enum UnitType {
    WORKER, EXPLORER, WARRIOR,
};

enum UnitDeathType {
    STARVING,
    KILLED, // By a warrior or an explorer
    SYSTEM, // For example on map editting
    UNKNOWN_DEATH,
};

struct Unit {
    enum UnitType type;
    struct Player *owner;
};

struct UnitList {
    struct UnitList *next;
    struct Unit *player;
};

#endif

