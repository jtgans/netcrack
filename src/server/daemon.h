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

#ifndef DAEMON_H
#define DAEMON_H

/** Set nonzero to daemonize; defaults to 1 */
int g_daemonize;

/** Set nonzero to fake a fork call; defaults to 0 */
int g_debug_daemonize;

/** Set nonzero to write the pidfile; defaults to 1 */
int g_write_pid;

/**
 * The path to the pidfile to write out when daemonizing. Defaults to
 * "/var/run/netcrackd.pid"
 */
char *g_daemon_pid_path;

void daemonize(void);
void daemon_shutdown(void);

#endif /* DAEMON_H */
