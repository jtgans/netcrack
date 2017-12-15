/*
 * Netcrack -- a multiplayer roguelike
 * Copyright (C) 2008  June R. Tate-Gans, Jonathan L. Tate
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <tcod/libtcod.h>

#include <netcrack.h>
#include <dice.h>

/** \file
 * Library routines for parsing dice strings and rolling them.
 *
 * Primarily used for parsing strings like 1d10+3 into a die struct. Also allows
 * for rolling the dice in useful ways via the use of libTCOD's functions.
 */

/**
 * Parse a string of the form NNNNdNNNN+NNNN into a die struct.
 *
 * @param d A pointer to the die structure you wish to initialize.
 * @param def A string pointer containing the dice definition.
 * @see die_to_str
 * @return True on successful parsing, otherwise False.
 */
bool die_parse(die *d, char *def)
{
    // Format: NNNNdNNNN+NNNN

    die temp = { 0, 0, 0 };
    int fields_read = 0;

    if (d == NULL) return false;
    if (def == NULL) return false;

    fields_read = sscanf(def, "%id%i+%i",
                         &temp.rolls, &temp.sides, &temp.offset);

    if ((fields_read < 2) ||
        (fields_read > 3)) {
        return false;
    }

    d->rolls  = temp.rolls;
    d->sides  = temp.sides;
    d->offset = temp.offset;

    return true;
}

/**
 * Convert a die struct into a string representation.
 * Note that you must free the returned string later.
 *
 * @param d A die structure to convert to a string.
 * @see die_parse
 * @return A string.
 */
char *die_to_str(die d)
{
    char buf[256];

    if (d.offset > 0) {
        snprintf(buf, sizeof(char)*256, "%id%i+%i",
                 d.rolls, d.sides, d.offset);
    } else {
        snprintf(buf, sizeof(char)*256, "%id%i",
                 d.rolls, d.sides);
    }

    return strdup(buf);
}

/**
 * Roll a dice using the default TCOD random number generator.
 *
 * @param d The die to roll.
 * @see die_to_str
 * @see die_parse
 * @return The random number generated according to the die.
 */
int die_roll(die d)
{
    int i;
    int total = 0;

    for (i=0; i<d.rolls; i++) {
        total += TCOD_random_get_int(NULL, 0, d.sides) + d.offset;
    }

    return total;
}
