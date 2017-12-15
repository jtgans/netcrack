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

#include <dice.h>

typedef struct {
    char *spec;
    die dice;
} test_die;

test_die VALID_DICE[] = {
    { "1d2",   {  1,  2,  0 } },
    { "1d2+3", {  1,  2,  3 } },
    { NULL,    { -1, -1, -1 } }
};

int main(void)
{
    char *buf;
    die d = { 0, 0, 0 };
    int idx = 0;

    while (VALID_DICE[idx].spec != NULL) {
        printf("Testing %s... ", VALID_DICE[idx].spec);
        fflush(stdout);

        buf = die_to_str(VALID_DICE[idx].dice);

        if (strcmp(buf, VALID_DICE[idx].spec) != 0) {
            printf("Failed. Got '%s' instead.\n", buf);
            fflush(stdout);
            return 1;
        } else {
            printf("die_to_str ok. ");
            fflush(stdout);
        }

        if (!die_parse(&d, VALID_DICE[idx].spec)) {
            printf("Failed to parse.\n");
            fflush(stdout);
            return 1;
        }

        if ((d.rolls != VALID_DICE[idx].dice.rolls) ||
            (d.sides != VALID_DICE[idx].dice.sides) ||
            (d.offset != VALID_DICE[idx].dice.offset)) {
            printf("Failed. Got {%d, %d, %d} instead.\n",
                   d.rolls, d.sides, d.offset);
            fflush(stdout);
            return 1;
        } else {
            printf("die_parse ok.\n");
            fflush(stdout);
        }

        free(buf);

        d.rolls = 0;
        d.sides = 0;
        d.offset = 0;

        idx++;
    }

    return 0;
}
