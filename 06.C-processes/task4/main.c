#include <stdlib.h>
// Open
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
// Stat
#include <unistd.h>
// Errors
#include <err.h>
// Print
#include <stdio.h>
// Wait
#include <sys/wait.h>

int main() {
    
    int fd = open("foo", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    int wstatus;

    if (fd == -1) {
        err(1, "File open problem!");
    }

    pid_t pid1 = fork();

    if (pid1 < 0) {
        err(1, "Problem with fork 1");
    }

    if (pid1 == 0) {
        // Child
        write(fd, "foo", 3);
        close(fd);
    }

    waitpid(pid1, &wstatus, 0);

    pid_t pid2 = fork();

    if (pid2 < 0) {
        err(1, "Problem with fork 1");
    }

    if (pid2 == 0) {
        // Child
        write(fd, "bar", 3);
        close(fd);
    }

    wait(NULL);

    close(fd);

    return 0;
}