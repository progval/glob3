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
 * \file os.h
 * \brief Headers of os abstraction functions.
 *
 * \seen os_prosix.c
 */

#ifndef __OS_H__
#define __OS_H__

#ifdef _WIN32
#include "os_win32.h"
#else
#include "os_posix.h"
#endif

char* os_path_join(int parts, ...);
char* os_get_config_root();

#endif
