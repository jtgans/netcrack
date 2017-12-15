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

#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include <netcrack.h>
#include <utils.h>


#ifndef __GNU__
/**
 * Duplicate copy of strndup from the GNU C library. Creates a duplicate of a
 * slice of a string.
 *
 * Only used in the case where the GNU C library is unavailable.
 *
 * @param str The string to duplicate from.
 * @param size The number of bytes to duplicate.
 * @returns A pointer to a duplicate string, or NULL on malloc issues.
 */
char *strndup(const char *str, int size)
{
    char *temp = calloc(sizeof(char), size + 1);

    if (!temp) return NULL;

    if (!strncpy(temp, str, size)) {
        free(temp);
        return NULL;
    }

    return temp;
}
#endif

/**
 * strcat, but without the idiocy.
 *
 * Concatenate a slice of src onto the end of dest, reallocating dest if
 * necessary.
 *
 * \note If size is greater than the length of src, strncatalloc assumes you
 * \note want the whole string.
 *
 * \note If dest is NULL, strncatalloc will allocate a new string for you, much
 * \note like strndup.
 *
 * \note If source is not null-terminated, this routine can cause a segfault.
 *
 * @see strndup
 * @param dest The destination string. If NULL, a new string is allocated and
 * returned.
 * @param src The source null-terminated string.
 * @param size The number of bytes to copy out of src into dest.
 * @returns A pointer to dest.
 */
char *strncatalloc(char *dest, const char *src, int size)
{
    int destlen = 0;

    if (!src)  return NULL;

    if (size > strlen(src)) {
        size = strlen(src);
    }

    if (!dest) {
        dest = malloc(sizeof(char) * size);
    } else {
        destlen = strlen(dest);
        dest = realloc(dest, sizeof(char) * (destlen + size));
    }

    if (!dest) return NULL;

    strncpy(dest + destlen, src, size);
    dest[destlen + size] = '\0';

    return dest;
}

/**
 * Escape any quotes in a string with \\".
 *
 * Returns a newly allocated, duplicated string with all quotes replaced with
 * \\".
 *
 * \note The caller is responsible for freeing the returned string.
 *
 * @param str The string to escape.
 * @returns The newly allocated, escaped string.
 */
char *escape_string(const char *str)
{
    char *buf;
    int i, j, quote_count;

    for (i=0; i<strlen(str); i++) {
        if (str[i] == '"') {
            quote_count++;
        }
    }

    buf = calloc(sizeof(char), strlen(str) + quote_count);
    if (!buf) return NULL;

    for (i = 0, j = 0;
         i < strlen(str);
         i++, j++) {
        if (str[i] == '"') {
            buf[++j] = '\\';
            buf[j] = '"';
        } else {
            buf[j] = str[i];
        }
    }

    return buf;
}

/**
 * Unescape any \\" in a string with quotes.
 *
 * Returns a newly allocated, duplicated string with all escaped quotes replaced
 * with standard quotes.
 *
 * \note The callre is responsible for freeing the returned string.
 *
 * @param str The string to unescape.
 * @returns The newly allocated, unescaped string.
 */
char *unescape_string(const char *str)
{
    char *buf;
    int i, j, quote_count;

    for (i=0; i<strlen(str); i++) {
        if ((str[i] == '\\') && (str[i+1] == '"')) {
            quote_count++;
        }
    }

    buf = calloc(sizeof(char), strlen(str) + quote_count);
    if (!buf) return NULL;

    for (i = 0, j = 0;
         i < strlen(str);
         i++, j++) {
        if ((str[i] == '\\') && (str[i+1] == '"')) {
            buf[j] = '"';
            i++;
        } else {
            buf[j] = str[i];
        }
    }

    return buf;
}
