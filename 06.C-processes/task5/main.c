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

int main(int argc, char * argv[]) {
    
    if (argc != 3) {
        errx(1, "Only 3 arguments, please!");
    }

    int wstatus;
    pid_t pid1 = fork();

    if (pid1 < 0) {
        err(1, "Problem with fork");
    }

    if (pid1 == 0) {
        // Child

        if( execlp(argv[1], argv[1], (char *) NULL) == -1 ) {
            err(1, "Problem with exec for %s", argv[1]);
        }

    }

    waitpid(pid1, &wstatus, 0);

    if (WIFEXITED(wstatus)) {
        pid_t pid2 = fork();

        if (pid2 < 0) {
            err(2, "Problem with second fork");
        }

        if (pid2 == 0) {
            // Child 

            if( execlp(argv[2], argv[2], (char *) NULL) == -1 ) {
                err(2, "Problem with exec for %s", argv[2]);
            }
        }

    } else{
        errx(-1, "child is failed something");
    }

    return 0;
}