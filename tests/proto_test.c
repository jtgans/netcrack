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

#include <netcrack.h>
#include <hashtable.h>
#include <proto.h>

#include "testhelpers.h"

int test_creation_deletion(void)
{
    proto_packet *pkt = NULL;
    hashtable_bucket *bucket = NULL;

    TEST_START();

    pkt = proto_new_packet(HELLO, g_version[0], g_version[1], g_version[2]);
    if (pkt) {
        TEST_OK();
    } else {
        TEST_FAIL();
        TEST_COMPLETE_FAIL("proto_new_packet returned %p\n", pkt);
    }

    if (pkt->type == HELLO) {
        TEST_OK();
    } else {
        TEST_FAIL();
        TEST_COMPLETE_FAIL("proto_new_packet stored HELLO type as %d instead.\n",
                           pkt->type);
    }

    if ((bucket = hashtable_get(pkt->fields, "major"))) {
        TEST_OK();

        if (bucket->type == INT) {
            TEST_OK();

            if (bucket->data.i == g_version[0]) {
                TEST_OK();
            } else {
                TEST_FAIL();
                TEST_COMPLETE_FAIL("proto_new_packet stored major as %d\n",
                                   bucket->data.i);
            }
        } else {
            TEST_FAIL();
            TEST_COMPLETE_FAIL("proto_new_packet stored major type as %d.\n",
                               bucket->type);
        }
    } else {
        TEST_FAIL();
        TEST_COMPLETE_FAIL("proto_new_packet failed to store the '%s' tag.\n",
                           "major");
    }

    proto_del_packet(pkt);
    TEST_OK();

    TEST_COMPLETE_OK();
}

#define SERIALIZED_PACKET "HELLO 0 0 0"

int test_serialize(void)
{
    proto_packet *pkt = NULL;
    char *str = NULL;

    TEST_START();

    pkt = proto_new_packet(HELLO, g_version[0], g_version[1], g_version[2]);
    if (pkt) {
        TEST_OK();
    } else {
        TEST_FAIL();
        TEST_COMPLETE_FAIL("proto_new_packet returned %s.\n", "NULL");
    }

    str = proto_packet_to_str(pkt);
    if (str) {
        TEST_OK();
    } else {
        TEST_FAIL();
        TEST_COMPLETE_FAIL("proto_packet_to_str returned %s.\n", "NULL");
    }

    if (strcmp(str, SERIALIZED_PACKET) == 0) {
        TEST_OK();
    } else {
        TEST_FAIL();
        TEST_COMPLETE_FAIL("proto_packet_to_str generated '%s' instead of %s.\n",
                           str, SERIALIZED_PACKET);
    }

    TEST_COMPLETE_OK();
}

int test_deserialize(void)
{
    proto_packet *pkt = NULL;

    TEST_START();

    pkt = proto_str_to_packet(SERIALIZED_PACKET);
    if (pkt) {
        TEST_OK();
    } else {
        TEST_FAIL();
        TEST_COMPLETE_FAIL("proto_str_to_packet returned %s.\n", "NULL");
    }

    if (pkt->type == HELLO) {
        TEST_OK();
    } else {
        TEST_FAIL();
        TEST_COMPLETE_FAIL("proto_str_to_packet set type as %d.\n", pkt->type);
    }

    if (hashtable_get(pkt->fields, "major")) {
        TEST_OK();
    } else {
        TEST_FAIL();
        TEST_COMPLETE_FAIL("proto_str_to_pocket failed to set major field.%s\n",
                           "");
    }

    if (hashtable_get(pkt->fields, "major")->type == INT) {
        TEST_OK();
    } else {
        TEST_FAIL();
        TEST_COMPLETE_FAIL("proto_str_to_pocket set the wrong type for %s.\n",
                           "major");
    }

    if (hashtable_get(pkt->fields, "major")->data.i == 0) {
        TEST_OK();
    } else {
        TEST_FAIL();
        TEST_COMPLETE_FAIL("proto_str_to_pocket set %d for major field.\n",
                           hashtable_get(pkt->fields, "major")->data.i);
    }

    TEST_COMPLETE_OK();
}

int main(void)
{
    if (!test_creation_deletion()) return -1;
    if (!test_serialize()) return -1;
    if (!test_deserialize()) return -1;

    return 0;
}
