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
        errx(1, "Give string!");
    }

    const int READ = 0;
    const int WRITE = 1;

    int pd[2];

    if(pipe(pd) == -1) {
        err(1, "Pipe error!");
    }

    pid_t pid = fork();
    if (pid == -1) {
        err(1, "Fork problem!");
    }

    if (pid == 0) {
        // Child
        close(pd[WRITE]);
        char c;

        while (read(pd[READ], &c, 1) > 0) {
            write(1, &c, 1);
        }

        exit(0);
    }

    // Parent
    close(pd[READ]);

    write(pd[WRITE], argv[1], strlen(argv[1]));

    close(pd[WRITE]);

    wait(NULL);

    exit(0);
}