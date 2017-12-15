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

#include <hashtable.h>

int main(void)
{
    hashtable *table = hashtable_new(100);
    hashtable_bucket *val;
    char *key;
    int i;

    hashtable_set(table, "foo", INT, 1);
    hashtable_set(table, "bar", INT, 2);
    hashtable_set(table, "baz", INT, 3);

    HASHTABLE_FOREACH(table, i, key, val) {
        printf("Iteration %d: %s -> %d\n",
               i, key, val->data.i);
    }

    hashtable_del(table);

    return 0;
}
