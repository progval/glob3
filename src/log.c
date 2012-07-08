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

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include "log.h"

enum log_level LOG_LEVEL=LOG_DEBUG;

const char* log_level_verbose[5] = {"debug", "info", "warning", "error", "critical"};

bool log_set_level_from_verbose(char* level) {
    for (int i=LOG_DEBUG; i<LOG_CRITICAL; i++) {
        if (strcmp(log_level_verbose[i], level)==0) {
            LOG_LEVEL = i;
            return true;
        }
    }
    return false;
}

void log_msg(enum log_level level_code, char* component, int parts, ...) {
    va_list args;
    char *part;
    va_start(args, parts);
    if (level_code >= LOG_LEVEL) {
        printf("%s: %s: ", log_level_verbose[level_code], component);
        for (int i=0; i<parts; i++) {
            part = va_arg(args, char*);
            printf("%s", part);
        }
        putchar('\n');
    }
    va_end(args);
}
