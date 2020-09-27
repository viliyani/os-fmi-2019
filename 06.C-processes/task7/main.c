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
    int wstatus;
    
    if (argc != 4) {
        errx(1, "Give 4 arguments, please!");
    }

    int fd = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (fd == -1) {
        err(1, "Problem with openning the file %s", argv[3]);
    }

    pid_t pid1 = fork();

    if (pid1 < 0) {
        err(1, "Fork problem");
    }

    if (pid1 == 0) {
        // Child

        if(execlp(argv[1], argv[1], (char *) NULL) == -1) {
            err(1, "Problem with exec for %s", argv[1]);
        }
    }

    wait(&wstatus);

    printf("status is: %d", wstatus);

    if(WIFEXITED(wstatus)) {
        write(fd, argv[1], strlen(argv[1]));
    } else {
        printf("Command %s failed", argv[1]);
    }

    pid_t pid2 = fork();

    if(pid2 < 0) {
        err(2, "Error second fork");
    }

    if(pid2 == 0) {
        // Child -> command 2

        if(execlp(argv[2], argv[2], (char *) NULL) == -1) {
            err(1, "Problem with exec for %s", argv[2]);
        }
    }

    wait(&wstatus);

    return 0;
}