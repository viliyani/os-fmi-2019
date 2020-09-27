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

    if(argc != 3) {
        errx(1, "Give 2 arguments, please!");
    }

    pid_t pid1 = fork();

    if (pid1 < 0) {
        err(1, "Problem with fork");
    }

    if (pid1 == 0) {
        // child
        sleep(1); // comment 

        if (execlp(argv[1], argv[1], (char *) NULL)) {
            err(1, "Exec problem for %s", argv[1]);
        }
    }

    pid_t pid2 = fork();

    if (pid2 < 0) {
        err(2, "Problem with fork");
    }

    if (pid2 == 0) {
        // child

        if (execlp(argv[2], argv[2], (char *) NULL)) {
            err(2, "Exec problem for %s", argv[2]);
        }
    }

    pid_t firstFinished = wait(NULL);
    pid_t secondFinished = wait(NULL);

    printf("--> p1 (%s): %d \n", argv[1], pid1);    
    printf("--> p2 (%s): %d \n", argv[2], pid2);    
    printf("--> First: %d \n", firstFinished);    
    printf("--> Second: %d \n", secondFinished);    
}