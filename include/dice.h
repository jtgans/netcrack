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

#ifndef DICE_H
#define DICE_H

#include <tcod/libtcod.h>

/**
 * Structure that defines a dice.
 *
 * @see die_parse
 * @see die_to_str
 * @see roll_die
 */
typedef struct {
    int rolls;
    int sides;
    int offset;
} die;

bool die_parse(die *d, char *def);
char *die_to_str(die d);
int roll_die(die d);

#endif /* DICE_H */
