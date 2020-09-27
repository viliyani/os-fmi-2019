// cat /etc/passwd | cut -d: -f1 | sort -r

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


int main(void) {
    const int READ = 0;
    const int WRITE = 1;
    
    int pd1[2];

    if (pipe(pd1) == -1) {
        err(1, "Pipe 1 error!");
    }

    pid_t pid1 = fork();
    if (pid1 == -1) {
        err(1, "Fork 1 error!");
    }

    if (pid1 == 0) {
        // Child 1 -> do cat /etc/passwd
        close(pd1[READ]);
        dup2(pd1[WRITE], 1);
        if (execlp("cat", "cat", "/etc/passwd", (char *) NULL) == -1){
            err(1, "Exec cat problem!");
        }
    }

    // Parent
    wait(NULL); // wait child 1 to do the "cat /etc/passwd"
    close(pd1[WRITE]);

    int pd2[2];

    if(pipe(pd2) == -1) {
        err(1, "Pipe 2 error!");
    }

    pid_t pid2 = fork();
    if (pid2 == -1) {
        err(1, "Fork 2 error!");
    } 

    if (pid2 == 0) {
        // Child 2 -> do "cut"
        close(pd2[READ]);
        dup2(pd1[READ], 0);
        dup2(pd2[WRITE], 1);
        if (execlp("cut", "cut", "-d:", "-f1", (char *) NULL) == -1) {
            err(1, "Exec cut problem!");
        }
    }

    // Parent
    wait(NULL);
    close(pd2[WRITE]);

    pid_t pid3 = fork();
    if (pid3 == -1) {
        err(1, "Fork 3 error!");
    }

    if (pid3 == 0) {
        // Child 3 -> do sort and print to stdout
        dup2(pd2[READ], 0);

        if (execlp("sort", "sort", "-r", (char *) NULL)) {
            err(1, "Exec sort problem!");
        }
    }

    // Parent
    wait(NULL);

    printf("\nThanks for watching! :)\n");
    exit(0);
}