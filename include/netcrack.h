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

#ifndef NETCRACK_H
#define NETCRACK_H

/** \mainpage
 *
 * \section intro_sec Introduction
 *
 * This project is an attempt at a multiplayer roguelike set on a partially
 * terraformed planet somewhere out in space in the future. It has a slight
 * cyberpunk-like feel (due to the use of "decks", "chips", and "code
 * fragments") with some other modifications wherever it was felt it needed to
 * change.
 *
 * Platforms targeted for the client are Windows and Linux, while the server is
 * Linux only.
 *
 * \section goals_sec Goals
 *
 *  -# Equip yourself with weapons and armor.
 *  -# Dive into the mines nearby the colony.
 *  -# ???
 *  -# Profit!
 *
 * Storyline TBD.
 *
 * \section soft_req_sec Software Requirements
 *
 * Currently the netcrack codebase is slowly evolving, but has some minimal
 * requirements:
 *
 *   - The Doryen Library (aka libtcod)
 *   - SLang
 *
 * \section why_sec Why another roguelike?
 *
 * Why not? The idea was hacked together during
 * Thanksgiving of '08 while a pair of siblings were looking for something to do
 * together. They tried mAngband, ToME, and others looking for a simple, easy to
 * play roguelike that was multiplayer. Unfortunately, none of the
 * aforementioned roguelikes were up to the task, so they decided to hack their
 * own way. Netcrack is their result.
 *
 * \section code_layout_sec Code Layout
 *
 * The code layout is pretty simple:
 *
 *   - doc
 *     - html
 *   - include
 *   - lib
 *   - share
 *   - src
 *     - server
 *     - client
 *   - tests
 *
 * Documentation goes into <code>doc</code>, and is mostly consisting of
 * plaintext files. The <code>html</code> subdir is where the Doxygen generated
 * output goes as well.
 *
 * <code>include</code> is where the headers for the
 * <code>libnetcrack.{a,lib}</code> library are stored. Note that these must be
 * publicly visible functions and variables that are shared between the client
 * and server code. Things like generic dice rolling, map representations and so
 * on are stored here.
 *
 * <code>lib</code> is where the source C files for the shared library are
 * stored.
 *
 * <code>share</code> contains any non-source code that has to be shared between
 * client and server. Things such as fonts, weapon definitions, scripts, and so
 * on are stored here. It's roughly equivalent to the <code>/usr/share</code>
 * directory in the LFS.
 *
 * <code>src</code> is split into two pieces: <code>client</code> and
 * <code>server</code>. These two directories contain the source for each part
 * of the game, respectively. Private headers required for compilation are
 * stored in each directory directly.
 *
 * <code>tests</code> contains a set of C scripts that perform unit tests
 * against the shared library. The general rule here is that the filename must
 * be called <code>&lt;test_category&gt;_test.c</code>, and must contain a set
 * of tests.
 */

/** Netcrack version number */
extern int g_version[3];

#endif /* NETCRACK_H */
