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

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <tcod/libtcod.h>

/**
 * Bucket data type.
 */
enum hashtable_bucket_type {
    INT,
    DOUBLE,
    STR,
    BOOL,
};

/**
 * Bucket structure to contain the values stored in the hashtable.
 *
 * Contains a copy of the pointer to the key string.
 *
 * If there's a hash collision, next will be set to the bucket that collided
 * with the first. The key is stored here so we can differentiate the collisions
 * from one another.
 */
typedef struct hashtable_bucket_t {
    char *key;
    enum hashtable_bucket_type type;
    struct hashtable_bucket_t *next;

    union {
        int    i;
        double d;
        char   c;
        char  *s;
        bool   b;
    } data;
} hashtable_bucket;

/**
 * Base structure to represent a hash table.
 *
 * Unique property of these hash tables is that they also store the keys <i>in
 * the order they were inserted</i>, so hashtable_get_keys returns them in that
 * order, and HASHTABLE_FOREACH iterates in order as well. Note that the hashing
 * function can either use the default as defined in this library, or define one
 * that follows the definition for hashtable_func and set it on creation of the
 * hashtable via hashtable_new_with_func.
 *
 * @see hashtable_get_keys
 * @see hashtable_func
 * @see hashtable_new
 * @see hashtable_new_with_func
 */
typedef struct hashtable_t {
    int num_buckets;
    int num_keys;
    long (*hashfunc)(struct hashtable_t *, char *);

    char **keys;
    hashtable_bucket **buckets;
} hashtable;

/** Type for the hashtable function */
typedef long (*hashtable_func)(hashtable *, char *);

/**
 * Quick macro to emulate other interpreted languages' foreach keyword.
 *
 * \note Do not free any buckets used in this loop!
 *
 * \note Changes to any buckets given in the iteration will directly change
 * \note their values in the hashtable.
 *
 * \example hashtable_example.c
 *
 * @param table Must be a hashtable *.
 * @param i Must be an integer or long.
 * @param key Must be a char *.
 * @param val Must be a hashtable_bucket *.
 */
#define HASHTABLE_FOREACH(table, i, key, val)                           \
    for (i = 0, key = table->keys[i], val = hashtable_get(table, key);  \
         (i <= table->num_keys - 1) &&                                  \
         (key = table->keys[i]) &&                                      \
         (val = hashtable_get(table, key));                             \
         i++)


long hashtable_default_hashfunc(hashtable *table, char *key);

hashtable *hashtable_new(int num_buckets);
hashtable *hashtable_new_with_func(int num_buckets, hashtable_func func);
char **hashtable_get_keys(hashtable *table);
hashtable_bucket *hashtable_get(hashtable *table, char *key);
hashtable_bucket *hashtable_set(hashtable *table, char *key,
                                enum hashtable_bucket_type type, ...);
bool hashtable_unset(hashtable *table, char *key);
void hashtable_del(hashtable *table);

#endif /* HASHTABLE_H */
