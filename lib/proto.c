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
#include <string.h>
#include <malloc.h>

#include <netcrack.h>
#include <hashtable.h>
#include <utils.h>
#include <proto.h>

struct packet_format {
    char *cmd_string;
    char *format;
};

static struct packet_format packet_formats[] = {
    { "HELLO",    "i:major i:minor i:teeny" },
    { "CHECK",    "s:name" },
    { "REGISTER", "s:name s:pass i:str i:end i:int i:chr i:dex" },
    { "LOGIN",    "s:name s:pass" },
    { "LOGOUT",   NULL },
    { "MAP",      "i:width i:height s:data" },
    { "MESSAGE",  "s:name s:message" },
    { "ACK",      NULL },
    { "NAK",      NULL },

    /* TODO(proto): Define COMMAND and ENTITY */
    { "COMMAND",  NULL },   /* TODO(proto): Define this */
    { "ENTITY",   NULL },   /* TODO(proto): Define this */

    /* Terminator */
    { NULL,       NULL }
};


/**
 * Allocate a new, empty proto_packet.
 *
 * The type is copied across, and the fields hashtable is initalized. Use
 * proto_del_packet to deallocate the packet. This is a static function, because
 * it shouldn't be necessary to use -- use proto_new_packet instead.
 *
 * @see proto_new_packet
 * @see proto_del_packet
 * @param type The type of packet to create.
 * @returns a pointer to a freshly initialized proto_packet.
 */
static proto_packet *proto_new_empty_packet(enum proto_cmd_type type)
{
    proto_packet *pkt = calloc(sizeof(proto_packet), 1);

    if (!pkt) return NULL;

    pkt->type = type;
    pkt->fields = hashtable_new(PROTO_PACKET_BUCKETS);

    if (!pkt->fields) {
        free(pkt);
        return NULL;
    }

    return pkt;
}

/**
 * Create a new proto_packet with the appropriate fields.
 *
 * @see proto_del_packet
 * @param type The type of packet to create. Note that this also defines the
 * format for the varadic arguments.
 * @returns a newly initialized proto_packet.
 */
proto_packet *proto_new_packet(enum proto_cmd_type type, ...)
{
    struct packet_format fmt = packet_formats[type];
    proto_packet *pkt = proto_new_empty_packet(type);
    va_list ap;
    char *format = fmt.format;
    char *field = NULL;
    char typechr = '\0';
    int next_pos = 0;

    if (!pkt) return NULL;

    va_start(ap, type);

    while ((format = strchr(format, ':'))) {
        if (strchr(format, ' ')) {
            next_pos = (int)(strchr(format, ' ') - 1 - format);
        } else {
            next_pos = strlen(format);
        }

        typechr = (format - 1)[0];
        field = strndup(format + 1, next_pos);
        format++;  /* skip past the colon */

        switch (typechr) {
            case 's':
                if (!hashtable_set(pkt->fields, field, STR,
                                   va_arg(ap, char *))) {

                    proto_del_packet(pkt);
                    free(field);
                    va_end(ap);

                    return NULL;
                }
                break;

            case 'i':
                if (!hashtable_set(pkt->fields, field, INT,
                                   va_arg(ap, int))) {

                    proto_del_packet(pkt);
                    free(field);
                    va_end(ap);

                    return NULL;
                }
                break;

            default:
                proto_del_packet(pkt);
                free(field);
                va_end(ap);

                return NULL;
                    break;
        }

        free(field);
    }
    va_end(ap);

    return pkt;
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
static char *proto_escape_string(char *str)
{
    char *buf;
    int i, j, quote_count;

    for (i=0; i<strlen(str); i++) {
        if (str[i] == '"') {
            quote_count++;
        }
    }

    buf = calloc(sizeof(char *), strlen(str) + quote_count);
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
 * Serialize a proto_packet into it's string representation for sending across
 * the wire.
 *
 * \note Caller is repsonsible for freeing the returned string.
 *
 * @see proto_new_packet
 * @param pkt The packet to serialize.
 * @returns A newly allocated string representation of the packet.
 */
char *proto_packet_to_str(proto_packet *pkt)
{
    char *result = NULL;
    hashtable_bucket *val = NULL;
    char *key = NULL;
    char *buf;
    int i, len;

    if (!pkt) return NULL;

    result = strdup(packet_formats[pkt->type].cmd_string);

    HASHTABLE_FOREACH(pkt->fields, i, key, val) {
        if (val->type == INT) {
            buf = calloc(sizeof(char), 256);

            if (!buf) {
                free(result);
                return NULL;
            }

            len = snprintf(buf, 256, " %d", val->data.i);
            result = strncatalloc(result, buf, len);

            free(buf);

            if (!result) {
                return NULL;
            }

        } else if (val->type == STR) {
            buf = proto_escape_string(val->data.s);

            if (!buf) {
                free(result);
                return NULL;
            }

            len = strlen(buf);
            result = realloc(result, strlen(result) + len + 6);

            if (!result) {
                free(buf);
                return NULL;
            }

            strcpy(result - len + 6, " \"");
            strcpy(result - len + 3, buf);
            strcpy(result - len + 1, "\"");

            free(buf);

        } else {
            free(result);
            return NULL;
        }
    }

    return result;
}

/**
 * Deserialize a string into a proto_packet.
 *
 * If the string given does not match one of the possible serialized formats,
 * this function will return NULL. Otherwise, the caller is responsible for
 * freeing the returned proto_packet instance.
 *
 * @see proto_packet_to_str
 * @see proto_new_packet
 * @see proto_del_packet
 * @param str The string to deserialize.
 * @returns A newly allocated proto_packet, or NULL if str does not contain a
 * packet.
 */
proto_packet *proto_str_to_packet(char *str)
{
    return NULL;
}

/**
 * Deallocate a previously allocated proto_packet.
 *
 * @see proto_new_packet
 * @param pkt The packet to deallocate.
 */
void proto_del_packet(proto_packet *pkt)
{
    if (!pkt) return;

    hashtable_del(pkt->fields);
    free(pkt);
}
