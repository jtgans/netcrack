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
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#endif

#if defined(_WIN32) && defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

void winmemcheck(void)
{
	printf("\n\nWinmemcheck:\n");
	if(!_CrtDumpMemoryLeaks())
		printf("No leaks found.\n");
    else
		printf("Leaks found. See VC output window.\n");
	getch();
}

#else
#include <stdlib.h>
#endif

#include <map.h>


int main(void)
{
    map *m = NULL;
    map *m2 = NULL;
    bool result = 0;
    int x, y;
	char tempfilename[256];
	char *envtemp;

#if defined(_WIN32) && defined(_DEBUG)
	atexit(winmemcheck);
#endif

    printf("Creating a new random map of 80x25 size... ");
    fflush(stdout);
    m = map_new_random(80, 25);

    if (!m) {
        printf("Failed. Got null result.\n");
        fflush(stdout);
        return -1;
    } else {
        printf("\n");

        for (y = 0; y < m->height; y++) {
            for (x = 0; x < m->width; x++) {
                putchar(m->grid[x + (y * m->width)]);
            }
            printf("\n");
        }

        printf("Done.\n");
        fflush(stdout);
    }

    map_del(m);

    printf("Creating a new empty map of 10x10 size... ");
    fflush(stdout);
    m = map_new_empty(10, 10);

    if (!m) {
        printf("Failed. Got null result.\n");
        fflush(stdout);
        return -1;
    } else {
        printf("Ok.\n");
        fflush(stdout);
    }


	envtemp = getenv("temp");
	if(envtemp != NULL)
		strcpy(tempfilename, envtemp);
	else
		strcpy(tempfilename, "/tmp");

	strcat(tempfilename, "/map-test.map");
	printf("Saving to \"%s\"... ", tempfilename);
    fflush(stdout);
    result = map_save_to_disk(m, tempfilename);

    if (!result) {
        printf("Failed. Got -1 result.\n");
        fflush(stdout);
        return -1;
    } else {
        printf("Ok.\n");
        fflush(stdout);
    }

    printf("Loading from \"%s\"... ", tempfilename);
    fflush(stdout);
    m2 = map_load_from_disk(tempfilename);

    if (!m2) {
        printf("Failed. Got null result.\n");
        fflush(stdout);
        return -1;
    } else {
        printf("Ok.\n");
        fflush(stdout);
    }

    printf("Verifying data is the same... ");
    fflush(stdout);

    if ((m->width == m2->width) &&
        (m->height == m2->height) &&
        (memcmp(m->grid, m2->grid, m->width * m->height) == 0)) {
        printf("Ok.\n");
        fflush(stdout);
    } else {
        printf("Failed. Data did not match.\n");
        fflush(stdout);

        map_del(m);
        map_del(m2);

        return -1;
    }

    map_del(m);
    map_del(m2);

    return 0;
}

