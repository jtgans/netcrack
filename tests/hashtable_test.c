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

#include <hashtable.h>

#include "testhelpers.h"

const char *alphabet    = "abcdefghijklmnopqrstuvwxyz_";
const char alphabet_len = 27;

#define INT_VAL    112358
#define DOUBLE_VAL 3.14159
#define STR_VAL    "string"
#define BOOL_VAL   true


int test_creation_deletion(void)
{
    hashtable *tbl = NULL;

    TEST_START();

    tbl = hashtable_new(10);
    if (tbl) {
        TEST_OK();
    } else {
        TEST_FAIL();
        TEST_COMPLETE_FAIL("hashtable_new(10) returned %p\n", tbl);
    }

    hashtable_del(tbl);
    TEST_OK();

    TEST_COMPLETE_OK();
}

int test_insertion(void)
{
    hashtable *tbl = NULL;
    hashtable_bucket *bucket = NULL;

    TEST_START();

    tbl = hashtable_new(10);
    if (tbl) {
        TEST_OK();
    } else {
        TEST_FAIL();
        TEST_COMPLETE_FAIL("hashtable_new(10) returned %p\n", tbl);
    }

    if (hashtable_set(tbl, "int", INT, INT_VAL)) {
        TEST_OK();
    } else {
        TEST_FAIL();
        TEST_COMPLETE_FAIL("hashtable_set(int) returned %s.\n", "NULL");
    }

    if (hashtable_set(tbl, "double", DOUBLE, DOUBLE_VAL)) {
        TEST_OK();
    } else {
        TEST_FAIL();
        TEST_COMPLETE_FAIL("hashtable_set('double') returned %s.\n", "NULL");
    }

    if (hashtable_set(tbl, "string", STR, STR_VAL)) {
        TEST_OK();
    } else {
        TEST_FAIL();
        TEST_COMPLETE_FAIL("hashtable_set('str') returned %s.\n", "NULL");
    }

    if (hashtable_set(tbl, "bool", BOOL, BOOL_VAL)) {
        TEST_OK();
    } else {
        TEST_FAIL();
        TEST_COMPLETE_FAIL("hashtable_set('int') returned %s.\n", "NULL");
    }

    bucket = hashtable_get(tbl, "int");
    if (bucket) {
        TEST_OK();

        if (bucket->type == INT) {
            TEST_OK();

            if (bucket->data.i == INT_VAL) {
                TEST_OK();
            } else {
                TEST_FAIL();
                TEST_COMPLETE_FAIL("Data was not stored as an integer.\n"
                                   "Got %d instead of %d.",
                                   bucket->data.i, INT_VAL);
            }
        } else {
            TEST_FAIL();
            TEST_COMPLETE_FAIL("Data was stored as type %d.\n",
                               bucket->type);
        }
    } else {
        TEST_FAIL();
        TEST_COMPLETE_FAIL("hashtable_get(int) returned %s.\n", "NULL");
    }

    bucket = hashtable_get(tbl, "double");
    if (bucket) {
        TEST_OK();

        if (bucket->type == DOUBLE) {
            TEST_OK();

            if (bucket->data.d == DOUBLE_VAL) {
                TEST_OK();
            } else {
                TEST_FAIL();
                TEST_COMPLETE_FAIL("Data was not stored as a float.\n"
                                   "Got %f instead of %f.",
                                   bucket->data.d, DOUBLE_VAL);
            }
        } else {
            TEST_FAIL();
            TEST_COMPLETE_FAIL("Data was stored as type %d.\n",
                               bucket->type);
        }
    } else {
        TEST_FAIL();
        TEST_COMPLETE_FAIL("hashtable_get(double) returned %s.\n", "NULL");
    }

    bucket = hashtable_get(tbl, "string");
    if (bucket) {
        TEST_OK();

        if (bucket->type == STR) {
            TEST_OK();

            if (strcmp(bucket->data.s, STR_VAL) == 0) {
                TEST_OK();
            } else {
                TEST_FAIL();
                TEST_COMPLETE_FAIL("Data was not stored as a string.\n"
                                   "Didn't get %s", STR_VAL);
            }
        } else {
            TEST_FAIL();
            TEST_COMPLETE_FAIL("Data was stored as type %d.\n",
                               bucket->type);
        }
    } else {
        TEST_FAIL();
        TEST_COMPLETE_FAIL("hashtable_get(string) returned %s.\n", "NULL");
    }

    bucket = hashtable_get(tbl, "bool");
    if (bucket) {
        TEST_OK();

        if (bucket->type == BOOL) {
            TEST_OK();

            if (bucket->data.b == BOOL_VAL) {
                TEST_OK();
            } else {
                TEST_FAIL();
                TEST_COMPLETE_FAIL("Data was not stored as a bool.\n"
                                   "%s", "");
            }
        } else {
            TEST_FAIL();
            TEST_COMPLETE_FAIL("Data was stored as type %d.\n",
                               bucket->type);
        }
    } else {
        TEST_FAIL();
        TEST_COMPLETE_FAIL("hashtable_get(bool) returned %s.\n", "NULL");
    }

    hashtable_del(tbl);
    TEST_OK();

    TEST_COMPLETE_OK();
}

int main(void)
{
    if (!test_creation_deletion()) return -1;
    if (!test_insertion()) return -1;

    return 0;
}
