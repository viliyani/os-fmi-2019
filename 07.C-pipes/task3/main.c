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
        errx(1, "No string is given!");
    }

    const int READ = 0;
    const int WRITE = 1;

    int pd[2];

    if (pipe(pd) == -1) {
        err(1, "Pipe problem!");
    }

    pid_t pid = fork();
    if (pid == -1) {
        err(1, "Fork problem!");
    }

    if (pid == 0) {
        // Child
        close(pd[WRITE]);

        dup2(pd[READ], 0);
        if (execlp("wc", "wc", "-c", (char *) NULL) == -1)
        {
            err(1, "Exec for wc problem!");
        }
    }

    // Parent
    close(pd[READ]);

    printf("Your string '%s' has length: \n", argv[1]);
    write(pd[WRITE], argv[1], strlen(argv[1]));

    close(pd[WRITE]);

    wait(NULL);

    exit(0);
}