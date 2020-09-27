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
// String
#include <string.h>

int main(int argc, char * argv[]) {
    if (argc != 2) {
        errx(1, "No file is given!");
    }

    int fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (fd == -1) {
        err(1, "File open problem");
    }

    write(fd, "fo", 2);

    pid_t pid = fork();

    if (pid < 0) {
        err(1, "Fork problem");
    } else if (pid == 0) {
        // Child
        write(fd, "bar\n", 4);
        exit(0);
    }

    // Parent
    wait(NULL);
    write(fd, "o\n", 2);


    return 0;
}