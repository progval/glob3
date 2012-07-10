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
 * \file utils.c
 * \brief Utilities.
 */

#include <stdlib.h>
#include <string.h>
#include "utils.h"

/**
 * \brief Variant of strcat that allocates the first parameter.
 */
char* strcat_realloc(const char *str1, const char *str2) {
    char *newstr = malloc(strlen(str1) + strlen(str2) + 1);
    strcpy(newstr, str1);
    strcat(newstr, str2);
    return newstr;
}

/**
 * \brief Variant of strcat_realloc that frees the first parameter.
 */
char* strcat_realloc_free(char *str1, const char *str2) {
    char *result = strcat_realloc(str1, str2);
    free(str1);
    return result;
}

/**
 * \brief Wrapper arround rand() that selects only values between 0 (included)
 * and the upper bound (excluded).
 */
int randmax(int upperbound) {
    // Greater multiple of maximum that is lesser than RAND_MAX
    int greater_multiple = RAND_MAX - (RAND_MAX%upperbound);
    int result;
    do {
        result = rand();
    } while (result >= greater_multiple); // Otherwise, lower numbers are more likely to be returned
    return result % upperbound;
}
