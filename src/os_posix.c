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
 * \file os_posix.c
 * \brief Implements os.h function for POSIX-compliant systems.
 */

#ifndef _WIN32

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "os.h"
#include "utils.h"

char* os_path_join(int parts, ...) {
    size_t size;
    va_list args;
    va_start(args, parts);
    char *path = va_arg(args, char*);
    char *part;
    for (int i=1; i<parts; i++) {
        part = va_arg(args, char*);
        // FIXME: Memory leaks here
        path = strcat_realloc(path, "/");
        path = strcat_realloc(path, part);
    }
    va_end(args);
    return path;
}

char* os_get_config_root() {
    return "~/.config/glob3/";
}

#endif
