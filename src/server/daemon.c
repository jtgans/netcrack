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
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "logging.h"
#include "daemon.h"


int g_daemonize = 1;
int g_debug_daemonize = 0;
char *g_daemon_pid_path = "/var/run/netcrackd.pid";


static void daemon_close_streams(void)
{
    FILE *fp = fopen("/dev/null", "w+");

    if (!fp) {
        logstrf(FATAL, "Unable to open /dev/null during daemonization: %s",
                strerror(errno));
        exit(1);
    }

    fclose(stdin);
    fclose(stdout);
    fclose(stderr);

    /* Set std streams to /dev/null to prevent segfaults. */
    stdin = fp;
    stdout = fp;
    stderr = fp;
}

static int daemon_fork(void)
{
    int pid = 0;

    if (g_debug_daemonize) {
        pid = 0;
    } else {
        pid = fork();
    }

    if (pid < 0) {
        logstrf(FATAL, "Failure to fork during daemonization: %s",
                strerror(errno));
        daemon_shutdown();

        exit(1);
    }

    return pid;
}

static void daemon_create_pidfile(void)
{
    int fd = open(g_daemon_pid_path, O_EXCL|O_CREAT|O_WRONLY, 0);
    FILE *fp = NULL;

    if (fd < 0) {
        logstrf(FATAL, "Daemon already running, or unable to create %s: %s",
                g_daemon_pid_path, strerror(errno));
        daemon_shutdown();
    }

    fp = fdopen(fd, "w");
    fprintf(fp, "%d\n", getpid());
    chmod(g_daemon_pid_path, 0644);
    fclose(fp);
}

static void daemon_signal_handler(int signum)
{
    if ((signum == SIGTERM) || (signum == SIGINT)) {
        logstrf(INFO, "Recieved %s", sys_siglist[signum]);
        daemon_shutdown();
    }
}

/**
 * Do the appropriate things to daemonize.
 *
 * Called once during main()'s initial startup. Sets the appropriate signal
 * handlers, cleans up the standard streams, and forks and writes out a
 * pidfile.
 */
void daemonize(void)
{
    int pid = 0;

    signal(SIGTERM, daemon_signal_handler);
    signal(SIGINT,  daemon_signal_handler);

    if (g_daemonize) {
        daemon_close_streams();
        pid = daemon_fork();

        if (pid == 0) {
            logstrf(DEBUG, "First fork complete. Forking again.");
            pid = daemon_fork();
        } else if (pid > 0) {
            exit(0);
        }

        if (pid == 0) {
            logstrf(DEBUG, "Second fork complete. Forking complete.");
        } else if (pid > 0) {
            exit(0);
        }

        daemon_create_pidfile();

        logstrf(DEBUG, "Daemonized.");
    } else {
        logstrf(DEBUG, "Running in foreground.");
    }
}

/**
 * Shutdown the daemon and do any additional cleanup.
 */
void daemon_shutdown(void)
{
    logstrf(INFO, "Shutting down.");
    unlink(g_daemon_pid_path);

    exit(0);
}
