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

#ifndef PROTO_H
#define PROTO_H

#include <utils.h>
#include <hashtable.h>

/** \file
 * \section Protocol API
 *
 * The protocol API is fairly simple. It works mostly like printf and friends,
 * for the most part. In this case, it actually takes the command type, followed
 * by the field values, as defined below. From that call, it creates a
 * proto_packet instance with associated fields which can then be serialized to
 * the string format defined below, and then written out to the socket. An
 * example of how to use it:
 *
 * \code
 * void connect(FILE *sock)
 * {
 *     int ver[] = [ 1, 2, 3 ];
 *     char *buf[256];
 *     proto_packet *pkt;
 *
 *     pkt = proto_new_packet(HELLO, ver[0], ver[1], ver[2]);
 *     fwrite(sock, proto_packet_to_str(pkt));
 *
 *     memset(buf, 0, 256);
 *     fnread(sock, buf, 256);
 *     pkt = proto_str_to_packet(pkt);
 *
 *     if (proto_get_packet_type(pkt) == ACK) {
 *         printf("Hello success.\n");
 *     }
 * }
 * \endcode
 *
 * \section Wire Protocol
 *
 * The wire protocol is about as simple as possible. It's simple and clean, and
 * human readable as well, much like SMTP, IMAP, POP, HTML, etc. The general
 * idea is that it's a simple command language. Essentially, every command has
 * the following EBNF:
 *
 * \verbatim
 *     command: (command_type ' ' params '\n') | command_type '\n'
 *     command_type: [A-Z]+
 *     params: (parameter ' ' params) | parameter
 *     parameter: integer | string
 *     integer: [0-9]+
 *     string: '"' escaped_string '"'
 *     escaped_string: (escaped_char escaped_string) | escaped_char
 *     escaped_char: [^"] | '\"'
 * \endverbatim
 *
 * After each command, either the client or server should respond with an
 * appropriate \c ACK or \c NAK command response in the same format.
 *
 * \section Commands and Responses
 *
 * Some commands are client to server only, others are server to client, and
 * finally there are a few which can come from client and server. These are
 * mostly defined in the following subsections.
 *
 * \note \c \<s:blah\> and \c \<i:blah\> mean string and integer, respectively.
 *
 * \subsection Client to Server
 *
 * These commands are expected to be followed by a response from the server,
 * either as an \c ACK or \c NAK. They have the following formats:
 *
 *   - \c CHECK \<s:name\>
 *   - \c REGISTER \<s:name\> \<s:pass\> \<i:str\> \<i:end\> \<i:int\> \<i:chr\> \<i:dex\>
 *   - \c LOGIN \<s:name\> \<s:pass\>
 *   - \c LOGOUT
 *
 * \c CHECK and \c REGISTER are primarily used during the initial character
 * registration/generation phase of the client. \c CHECK is used to check to see
 * if a name has been taken or not, while \c REGISTER is used to actually
 * register a character. Note that either can result in \c NAK responses, so
 * the client should attempt to check for this eventuality.
 *
 * Note that LOGIN is unique in that it causes the server to then spout out a
 * set of additional commands to the client. In this case, they spawn an
 * additional MAP command, followed by multiple ENTITY commands.
 *
 * \todo determine the following command formats:
 *   - \c COMMAND
 *
 * \subsection Server to Client
 *
 * Again, these commands are expected to be followed by \c NAK or \c ACK command
 * responses. These commands have the following formats:
 *
 *   - \c MAP \<i:width\> \<i:height\> \<i:data\>
 *
 * \todo determine the following command formats:
 *   - \c ENTITY
 *
 * \subsection Bidirectional Commands
 *
 * These commands may be sent from either client or server. They have the
 * following formats:
 *
 *   - \c MESSAGE \<s:name\> \<s:message\>
 *   - \c ACK
 *   - \c NAK
 *
 * \c MESSAGE is used to send a text message to all players on the same map. It
 * can be assumed that any \c MESSAGE commands coming from the server are
 * properly filtered for the map that the player is on.
 *
 * Note that in the case of the client sending a \c MESSAGE command to the
 * server that the name part is really not needed, but included for
 * completeness.
 */

/**
 * The protocol command type.
 *
 * Each of these command types are described above.
 */
enum proto_cmd_type {
    HELLO,         /* C -> S | major, minor, teeny */
    CHECK,         /* C -> S | name */
    REGISTER,      /* C -> S | name, pass, str, end, int, chr, dex */
    LOGIN,         /* C -> S | name, pass */
    LOGOUT,        /* C -> S */

    MAP,           /* C <- S | width, height, data */

    MESSAGE,       /* C <> S | name, message */
    ACK,           /* C <> S */
    NAK,           /* C <> S */

    /* TODO(proto): Define COMMAND and ENTITY */
    COMMAND,       /* C <- S */
    ENTITY,        /* C <- S */
};

/**
 * A packet representation.
 *
 * The appropriate fields described above are stored in a hashtable called
 * fields.
 */
typedef struct {
    enum proto_cmd_type type;
    hashtable *fields;
} proto_packet;

/** Default number of buckets to initialize the hashtable with. */
#define PROTO_PACKET_BUCKETS 100

proto_packet *proto_new_packet(enum proto_cmd_type type, ...);
char *proto_packet_to_str(proto_packet *pkt);
proto_packet *proto_str_to_packet(char *str);
void proto_del_packet(proto_packet *pkt);

#endif /* PROTO_H */
