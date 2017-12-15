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

#endif

#include <stdlib.h>

#include <tcod/libtcod.h>
#include <map.h>

#define FONT_FILE "../share/fonts/celtic_garamond_10x10_gs_tc.png"


int main(void)
{
    TCOD_console_t sconsole;
    TCOD_color_t dark_wall, dark_ground, current_wall;
    TCOD_key_t key;
    int x, y;
    char smap[80*25] =
        "################################################################################" // 1
        "##                                                                            ##" //19
        "##                                              ##                            ##" //20
        "##                                        ####  ##                            ##" //21
        "##                                   #####      ##                            ##" // 2
        "##                           #####       #      ##                            ##" // 3
        "##                         ##     ##     #      ##                            ##" // 4
        "##                  #### ##              #       ##                           ##" // 5
        "##              ####     ##              #       ##                           ##" // 6
        "##       #####  #          #####          #                                   ##" // 7
        "##  ######      #              #####      #      ##                           ##" // 8
        "##       #      #   ###             ##    #      ##                           ##" // 9
        "##       #      ####                ##    #                                   ##" //10
        "##       #       #         ##     ##                                          ##" //11
        "##       #       #           #####                                            ##" //12
        "##        #      #   ####                                                     ##" //13
        "##        #      ####                                                         ##" //14
        "##        #                                                                   ##" //15
        "##        #                                                                   ##" //16
        "##                                                                            ##" //17
        "##                                                                            ##" //18
        "##                                                                            ##" //22
        "##                                                                            ##" //23
        "##                                                                            ##" //24
        "################################################################################"; //25

    dark_wall.r = 64;
    dark_wall.g = 64;
    dark_wall.b = 0;
    dark_ground.r = 48;
    dark_ground.g = 48;
    dark_ground.b = 48;

	TCOD_console_set_custom_font(FONT_FILE, 10, 10,
                                 TCOD_FONT_TYPE_GREYSCALE|TCOD_FONT_LAYOUT_TCOD);

    TCOD_console_init_root(80, 60, "Press ESC to quit", false);

    sconsole = TCOD_console_new(80, 25);

    for (y=0; y<60; y++) {
        for (x=0; x<80; x++) {
            if (smap[y*80+x] == '#') {
                current_wall = dark_wall;
            } else {
                current_wall = dark_ground;
            }

            TCOD_console_set_back(sconsole, x, y, current_wall, TCOD_BKGND_SET);
        }
    }

	do {
		TCOD_console_blit(sconsole, 0, 0, 80, 25, NULL, 0, 0, 255);
		TCOD_console_flush();

		key = TCOD_console_check_for_keypress(TCOD_KEY_PRESSED);
	} while (!TCOD_console_is_window_closed() && (key.vk != TCODK_ESCAPE));

	return 0;
}
