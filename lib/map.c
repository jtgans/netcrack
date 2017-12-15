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
#include <map.h>

#define BSP_DEPTH 4
#define MIN_ROOM_SIZE 4

/** \file
 * Simple library routines to define new maps, load and save to disk, and
 * generate random warrens.
 *
 * Utilizes libtcod's random number generator and bsp trees.
 */

/**
 * Generate a new empty map.
 *
 * Caller is responsible for freeing the map with map_del.
 *
 * @param width The width of the map.
 * @param height The height of the map.
 * @see map_del
 * @see map_new_random
 * @see map_load_from_disk
 * @return An initialized map. NULL on error.
 */
map *map_new_empty(int width, int height)
{
    map *m = malloc(sizeof(map));
    int grid_size = width * height * sizeof(char);

    if (!m) return NULL;

    m->width = width;
    m->height = height;
    m->grid = malloc(grid_size);

    if (!m->grid) {
        free(m);
        return NULL;
    }

    // We want to tunnel
    memset(m->grid, '#', grid_size);

    return m;
}

// draw a vertical line
static void vline(map *m, int x, int y1, int y2) {
	int y = y1;
	int dy = (y1 > y2 ? -1 : 1);

	m->grid[x + (m->width * y)] = ' ';

	if (y1 == y2) return;

	do {
		y += dy;
		m->grid[x + (m->width * y)] = ' ';
	} while (y != y2);
}

// draw a vertical line up until we reach an empty space
static void vline_up(map *m, int x, int y) {
	while (y >= 0 && m->grid[x + (m->width * y)] != ' ') {
		m->grid[x + (m->width * y)] = ' ';
		y--;
	}
}

// draw a vertical line down until we reach an empty space
static void vline_down(map *m, int x, int y) {
	while (y < m->height && m->grid[x + (m->width * y)] != ' ') {
		m->grid[x + (m->width * y)] = ' ';
		y++;
	}
}

// draw a horizontal line
static void hline(map *m, int x1, int y, int x2) {
	int x = x1;
	int dx = (x1 > x2 ? -1 : 1);

	m->grid[x + (m->width * y)] = ' ';

	if (x1 == x2) return;

	do {
		x += dx;
		m->grid[x + (m->width * y)] = ' ';
	} while (x != x2);
}

// draw a horizontal line left until we reach an empty space
static void hline_left(map *m, int x, int y) {
	while (x >= 0 && m->grid[x + (m->width * y)] != ' ') {
		m->grid[x + (m->width * y)] = ' ';
		x--;
	}
}

// draw a horizontal line right until we reach an empty space
static void hline_right(map *m, int x, int y) {
	while (x < m->width && m->grid[x + (m->width * y)] != ' ') {
		m->grid[x + (m->width * y)] = ' ';
		x++;
	}
}

static bool random_map_callback(TCOD_bsp_t *node, void *data)
{
    map *m = (map *)(data);
    int minx = node->x + 1;
    int maxx = node->x + node->w - 1;
    int miny = node->y + 1;
    int maxy = node->y + node->h - 1;
    int x, y;
    TCOD_bsp_t *left, *right;

    if (TCOD_bsp_is_leaf(node)) {
        // Update the min and maxes with randomization
        minx = TCOD_random_get_int(NULL, minx, maxx - MIN_ROOM_SIZE + 1);
        miny = TCOD_random_get_int(NULL, miny, maxy - MIN_ROOM_SIZE + 1);
        maxx = TCOD_random_get_int(NULL, minx + MIN_ROOM_SIZE - 1, maxx);
        maxy = TCOD_random_get_int(NULL, miny + MIN_ROOM_SIZE - 1, maxy);

        if (maxx == m->width - 1) maxx--;
        if (maxy == m->height - 1) maxy--;

        // Shrink/expand the slice to match the room sizing
        node->x = minx;
        node->y = miny;
        node->w = maxx - minx + 1;
        node->h = maxy - miny + 1;

        // Clear out the room space
        for (x = minx; x <= maxx; x++) {
            for (y = miny; y <= maxy; y++) {
                m->grid[x + (y * m->width)] = ' ';
            }
        }
    } else {
        // We're a branch -- draw a corridor instead
        left = TCOD_bsp_left(node);
        right = TCOD_bsp_right(node);

        node->x = MIN(left->x, right->x);
        node->y = MIN(left->y, right->y);
        node->w = MAX(left->x + left->w, right->x + right->w) - node->x;
        node->h = MAX(left->y + left->h, right->y + right->h) - node->y;

        if (node->horizontal) {
            if ((left->x + left->w - 1 < right->x) ||
                (right->x + right->w - 1 < left->x)) {
				// no overlapping zone. we need a Z shaped corridor
				int x1 = TCOD_random_get_int(NULL, left->x,
                                             left->x + left->w - 1);
				int x2 = TCOD_random_get_int(NULL, right->x,
                                             right->x + right->w - 1);
				int y = TCOD_random_get_int(NULL, left->y + left->h, right->y);

				vline_up(m, x1, y-1);
				hline(m, x1, x2, y);
				vline_down(m, x2, y+1);
			} else {
				// straight vertical corridor
				int minx = MAX(left->x, right->x);
				int maxx = MIN(left->x + left->w - 1, right->x + right->w - 1);
				int x = TCOD_random_get_int(NULL, minx, maxx);

				vline_down(m, x, right->y);
				vline_up(m, x, right->y - 1);
			}
		} else {
			// horizontal corridor
			if (left->y + left->h - 1 < right->y ||
                right->y + right->h - 1 < left->y ) {
				// no overlapping zone. we need a Z shaped corridor
				int y1 = TCOD_random_get_int(NULL, left->y,
                                             left->y + left->h - 1);
				int y2 = TCOD_random_get_int(NULL, right->y,
                                             right->y + right->h - 1);
				int x = TCOD_random_get_int(NULL, left->x + left->w, right->x);

				hline_left(m, x - 1, y1);
				vline(m, x, y1, y2);
				hline_right(m, x + 1, y2);
			} else {
				// straight horizontal corridor
				int miny = MAX(left->y, right->y);
				int maxy = MIN(left->y + left->h - 1, right->y + right->h - 1);
				int y = TCOD_random_get_int(NULL, miny, maxy);

				hline_left(m, right->x - 1, y);
				hline_right(m, right->x, y);
			}
		}
    }

    return true;
}

/**
 * Generate a new random map with a defined size.
 *
 * Caller is responsible for freeing the map using map_del.
 *
 * @param width The width of the map.
 * @param height The height of the map.
 * @see map_new_empty
 * @see map_load_from_disk
 * @see map_del
 * @return A map pointer, or NULL on error.
 */
map *map_new_random(int width, int height)
{
    map *m = map_new_empty(width, height);
    TCOD_bsp_t *bsp = TCOD_bsp_new_with_size(0, 0, width, height);

    TCOD_bsp_split_recursive(bsp, NULL, BSP_DEPTH,
                             MIN_ROOM_SIZE, MIN_ROOM_SIZE, 1.5f, 1.5f);
    TCOD_bsp_traverse_inverted_level_order(bsp, random_map_callback, m);
    TCOD_bsp_delete(bsp);

    return m;
}

/**
 * Delete a previously allocated map.
 *
 * @param m The previously allocated map.
 * @see map_new_empty
 * @see map_new_random
 * @see map_load_from_disk
 */
void map_del(map *m)
{
    if (m == NULL) return;

    free(m->grid);
    free(m);
}

/**
 * Save a map to disk for loading later.
 *
 * @param m The previously allocated map.
 * @param filename The filename to save the map to.
 * @see map_new_empty
 * @see map_new_random
 * @return true on success, false on error.
 */
bool map_save_to_disk(map *m, char *filename)
{
    FILE *fp = fopen(filename, "w");
    int grid_size = m->width * m->height;

    if (!fp) return false;

    if (m == NULL) {
        fclose(fp);
        return false;
    }

    if (fwrite(&m->width, sizeof(int), 1, fp) != 1) {
        fclose(fp);
        return false;
    }

    if (fwrite(&m->height, sizeof(int), 1, fp) != 1) {
        fclose(fp);
        return false;
    }

    if (fwrite(m->grid, sizeof(char), grid_size, fp) != grid_size) {
        fclose(fp);
        return false;
    }

    fclose(fp);
    return true;
}

/**
 * Load and allocate a map prevoiusly saved to disk.
 *
 * Caller is responsible for deallocating the map with map_del.
 *
 * @param filename The filename to load the map from.
 * @see map_save_to_disk
 * @return A pointer to the new map, or NULL on error.
 */
map *map_load_from_disk(char *filename)
{
    FILE *fp = fopen(filename, "r");
    map *m = NULL;
    int width, height, grid_size;

    if (!fp) return NULL;

    if (fread(&width, sizeof(int), 1, fp) != 1) {
        fclose(fp);
        return NULL;
    }

    if (fread(&height, sizeof(int), 1, fp) != 1) {
        fclose(fp);
        return NULL;
    }

    grid_size = width * height;
    m = map_new_empty(width, height);

    if (!m) {
        fclose(fp);
        return NULL;
    }

    if (fread(m->grid, sizeof(char), grid_size, fp) != grid_size) {
        fclose(fp);
        return NULL;
    }

    fclose(fp);
    return m;
}
