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

#ifndef LOGGING_H
#define LOGGING_H

/** Syslog identification string */
#define SYSLOG_IDENT "netcrackd"

/**
 * Logging priority levels.
 *
 * Note: If you change log_priority, make sure you update the entries in the
 * log_priority_to_str and log_to_syslog_priority arrays in logging.c and set
 * log_max_priority to match!
 */
enum log_priority {
    FATAL = 0,
    ERROR,
    WARN,
    INFO,
    DEBUG
};

/** Maximum defined priority level. */
#define LOG_MAX_PRIORITY 4

/** Set nonzero to log to syslog; defaults to 1 */
int g_log_to_syslog;

/** The logging level. Defaults to INFO. */
enum log_priority g_log_level;


void log_start(void);
void log_end(void);

inline void logstr(enum log_priority priority, const char *str);
void logstrf(enum log_priority priority, const char *fmt, ...);

#endif /* LOGGING_H */
