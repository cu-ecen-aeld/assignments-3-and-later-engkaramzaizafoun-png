#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    // Open syslog
    openlog("writer", LOG_PID, LOG_USER);

    // Check arguments
    if (argc != 3) {
        syslog(LOG_ERR, "Invalid number of arguments");
        fprintf(stderr, "Usage: %s <file> <string>\n", argv[0]);
        closelog();
        return 1;
    }

    const char *file = argv[1];
    const char *string = argv[2];

    // Open file (create if not exists, truncate if exists)
    int fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        syslog(LOG_ERR, "Failed to open file %s: %s", file, strerror(errno));
        closelog();
        return 1;
    }

    // Write string to file
    ssize_t bytes_written = write(fd, string, strlen(string));
    if (bytes_written == -1) {
        syslog(LOG_ERR, "Failed to write to file %s: %s", file, strerror(errno));
        close(fd);
        closelog();
        return 1;
    }

    // Log success
    syslog(LOG_DEBUG, "Writing %s to %s", string, file);

    close(fd);
    closelog();
    return 0;
}

