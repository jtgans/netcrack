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
#include <syslog.h>
#include <stdarg.h>

#include "logging.h"


int g_log_to_syslog = 1;
enum log_priority g_log_level = INFO;

/**
 * Array to convert the log priority to a human-readable string.
 */
static char *log_priority_to_str[] = {
    "FATAL",
    "ERROR",
    "WARN",
    "INFO",
    "DEBUG"
};

/**
 * Array to convert the log priority to a syslog-readable value.
 */
static int log_to_syslog_priority[] = {
    LOG_EMERG,
    LOG_ERR,
    LOG_WARNING,
    LOG_INFO,
    LOG_DEBUG
};


/**
 * Initialize the logging infrastructure.
 *
 * Called by main() once only.
 */
void log_start(void)
{
    if (g_log_to_syslog) {
        openlog(SYSLOG_IDENT, 0, LOG_DAEMON);
    }
}

/**
 * Shutdown the logging infrastructure.
 *
 * Called by main() before daemon_shutdown() is called.
 */
void log_end(void)
{
    if (g_log_to_syslog) {
        closelog();
    }
}

/**
 * Quickie method to log a string without formatters. See logstrf() for more
 * details.
 */
inline void logstr(enum log_priority priority, const char *str)
{
    logstrf(priority, str);
}

/**
 * Log a string.
 *
 * @param priority The priority at which to log the message.
 * @param fmt A printf() like string containing the formatted message.
 */
void logstrf(enum log_priority priority, const char *fmt, ...)
{
    va_list ap;

    if (priority <= g_log_level) {
        va_start(ap, fmt);

        if (g_log_to_syslog) {
            vsyslog(log_to_syslog_priority[priority],
                    fmt, ap);
        } else {
            fprintf(stderr, "%s<%s>: ",
                    SYSLOG_IDENT, log_priority_to_str[priority]);
            vfprintf(stderr, fmt, ap);
            fprintf(stderr, "\n");
        }

        va_end(ap);
    }
}
