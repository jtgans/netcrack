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
#include <stdarg.h>
#include <malloc.h>
#include <string.h>

#include <netcrack.h>
#include <hashtable.h>


bool has_key(hashtable *table, char *key)
{
    int i;

    for (i=0; i<table->num_keys; i++) {
        if (strcmp(table->keys[i], key) == 0) {
            return true;
        }
    }

    return false;
}

bool add_key(hashtable *table, char *key)
{
    char **old_keys = NULL;
    int old_num_keys = 0;

    if (!table) return false;
    if (has_key(table, key)) return true;

    old_keys = table->keys;
    old_num_keys = table->num_keys;

    /* Setup a new keys vector. */
    table->num_keys++;
    table->keys = calloc(sizeof(char *), table->num_keys);

    if (!table->keys) {
        table->keys = old_keys;
        table->num_keys = old_num_keys;

        return false;
    }

    /* Copy across the old keys (if there were any) and free the old vector. */
    if (old_keys) {
        memcpy(table->keys, old_keys, sizeof(char *) * old_num_keys);
        free(old_keys);
    }

    /* Don't add one to the number of keys -- arrays are 0 based */
    table->keys[table->num_keys - 1] = strdup(key);

    return true;
}

bool remove_key(hashtable *table, char *key)
{
    char **old_keys = NULL;
    int old_keycount;
    int i;

    if (!table) return false;
    if (!has_key(table, key)) return false;

    /* Shortcut case to make the vector nonexistant as add_key expects. */
    if (table->num_keys == 1) {
        free(table->keys);

        table->keys = NULL;
        table->num_keys = 0;

        return true;
    }

    /* Remove the key from the list of keys -- this involves reallocating the
       keys vector to a different size and reassigning the pointers. Ugly, but
       it works. */

    old_keys = table->keys;
    old_keycount = table->num_keys;

    table->num_keys--;
    table->keys = calloc(sizeof(char *), table->num_keys);

    if (!table->keys) {
        table->keys = old_keys;
        table->num_keys = old_keycount;

        return false;
    }

    /* Find the index of the key we want to remove. No sense checking to see if
       we're at the end of the list -- has_key will tell us if we have a key in
       the first place, in which case we just return. */

    for (i=0; i<old_keycount; i++) {
        if (strcmp(old_keys[i], key) == 0) break;
    }

    if (i == 0) {                     /* start */
        memcpy(table->keys, old_keys + 1, sizeof(char *) * table->num_keys);
    } else if (i == old_keycount) {   /* end */
        memcpy(table->keys, old_keys, sizeof(char *) * table->num_keys);
    } else {                          /* middle */
        /* Copy to the left */
        memcpy(table->keys,
               old_keys,
               sizeof(char *) * (i - 1));

        /* Copy to the right */
        memcpy(table->keys,
               old_keys + i + 1,
               sizeof(char *) * (old_keycount - i));
    }

    free(old_keys[i]);
    free(old_keys);

    return true;
}

static hashtable_bucket *set_bucket_data(hashtable_bucket *bucket,
                                         enum hashtable_bucket_type type,
                                         va_list ap)
{
    if (!bucket) return NULL;

    switch (type) {
        case INT:
            bucket->data.i = va_arg(ap, int);
            break;

        case DOUBLE:
            bucket->data.d = va_arg(ap, double);
            break;

        case STR:
            if (bucket->data.s) free(bucket->data.s);
            bucket->data.s = strdup(va_arg(ap, char*));
            break;

        case BOOL:
            bucket->data.b = va_arg(ap, bool);
            break;

        default:
            free(bucket);
            bucket = NULL;
    }

    return bucket;
}

static hashtable_bucket *new_bucket(enum hashtable_bucket_type type,
                                    char *key, va_list ap)
{
    hashtable_bucket *bucket = calloc(sizeof(hashtable_bucket), 1);

    if (!bucket) return NULL;

    bucket->type = type;
    bucket->key  = strdup(key);
    set_bucket_data(bucket, type, ap);

    return bucket;
}

static void del_bucket(hashtable_bucket *bucket)
{
    if (!bucket) return;
    if (bucket->type == STR) free(bucket->data.s);

    free(bucket->key);
    free(bucket);
}

long hashtable_default_hashfunc(hashtable *table, char *key)
{
    return 0;
}

hashtable *hashtable_new(int num_buckets)
{
    hashtable *table = NULL;

    table = calloc(sizeof(hashtable), 1);
    if (!table) return NULL;

    table->buckets = calloc(sizeof(hashtable_bucket *), num_buckets);

    if (!table->buckets) {
        free(table);
        return NULL;
    }

    table->num_buckets = num_buckets;
    table->hashfunc = hashtable_default_hashfunc;

    return table;
}

hashtable *hashtable_new_with_func(int num_buckets, hashtable_func func)
{
    hashtable *table = hashtable_new(num_buckets);

    if (!table) return NULL;
    if (!func)  return NULL;

    table->hashfunc = func;

    return table;
}

char **hashtable_get_keys(hashtable *table)
{
    if (!table) return NULL;

    return table->keys;
}

hashtable_bucket *hashtable_get(hashtable *table, char *key)
{
    long hash;
    hashtable_bucket *bucket = NULL;

    if (!table) return NULL;

    hash = table->hashfunc(table, key);

    /*
      Iterate through the collisions until we find a key that matches. If no
      key matches, we'll run into the NULL at the end of the list, and end up
      returning that.
    */

    for (bucket = table->buckets[hash];
         (bucket != NULL) && (strcmp(bucket->key, key) != 0);
         bucket = bucket->next);

    return bucket;
}

hashtable_bucket *hashtable_set(hashtable *table, char *key,
                                enum hashtable_bucket_type type, ...)
{
    hashtable_bucket *last   = NULL;
    hashtable_bucket *cursor = NULL;
    hashtable_bucket *bucket = NULL;
    long hash;
    va_list ap;

    if (!table) return NULL;

    hash = table->hashfunc(table, key);

    if (table->buckets[hash] != NULL) {
        /* Iterate through the buckets looking for the key */
        for (cursor = table->buckets[hash];
             (cursor != NULL) && (strcmp(cursor->key, key) != 0);
             last = cursor, cursor = cursor->next);

        if (cursor != NULL) {
            /* We found the key -- set the old bucket's data to the new type. */
            cursor->type = type;
            set_bucket_data(cursor, type, ap);
        } else {
            /* We've reached the end of the list and not found the key -- append
               a new bucket to the end of the list. */

            va_start(ap, type);
            if (!(bucket = new_bucket(type, key, ap))) return NULL;
            va_end(ap);

            if (!add_key(table, key)) {
                del_bucket(bucket);
                return NULL;
            }

            last->next = bucket;
        }
    } else {
        /* TODO(hashes): Make this less redundant with the above identical code
           fragment. */

        va_start(ap, type);
        if (!(bucket = new_bucket(type, key, ap))) return NULL;
        va_end(ap);

        if (!add_key(table, key)) {
            del_bucket(bucket);
            return NULL;
        }

        table->buckets[hash] = bucket;
    }

    return bucket;
}

bool hashtable_unset(hashtable *table, char *key)
{
    hashtable_bucket *cursor = NULL;
    hashtable_bucket *last = NULL;
    long hash;

    if (table == NULL) return false;

    hash = table->hashfunc(table, key);

    if (table->buckets[hash] == NULL) return false;

    /* Iterate the bucket list and attempt to find the bucket with the key we
       were asked to eliminate. */
    for (cursor = table->buckets[hash];
         (cursor != NULL) && (strcmp(cursor->key, key) != 0);
         last = cursor, cursor = cursor->next);

    /* We couldn't find the key in the buckets -- unlikely, but we have to
       cover the case that the hashing function is broken. */
    if (cursor == NULL) return false;

    /* Remove the bucket from the list. */
    if (last == NULL) {
        /* Catch the case where we're at the beginning of the list */
        table->buckets[hash] = cursor->next;
    } else {
        last->next = cursor->next;
    }

    del_bucket(cursor);
    remove_key(table, key);

    return true;
}

void hashtable_del(hashtable *table)
{
    if (!table) return;

    while (table->num_keys != 0) {
        hashtable_unset(table, table->keys[0]);
    }

    free(table->buckets);
    free(table);
}
