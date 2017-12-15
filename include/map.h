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

#ifndef MAP_H
#define MAP_H

#include <tcod/libtcod.h>

/**
 * A structure to define a map with.
 *
 * Maps are simple structures, containing a character grid and the width and
 * height of the map. Nothing is assumed about the map's inner structure --
 * that's up to the caller to determine.
 *
 * @see map_new_empty
 * @see map_new_random
 * @see map_del
 * @see map_save_to_disk
 * @see map_load_from_disk
 */
typedef struct map_t {
    int width;
    int height;
    char *grid;
} map;

map *map_new_empty(int width, int height);
map *map_new_random(int width, int height);
void map_del(map *m);
bool map_save_to_disk(map *m, char *filename);
map *map_load_from_disk(char *filename);

#endif /* MAP_H */
