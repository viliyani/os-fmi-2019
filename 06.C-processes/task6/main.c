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
    
    if (argc == 1) {
        errx(1, "Give arguments, please!");
    }

    int wstatus;
    int successCounter = 0;
    int errorCounter = 0;

    for (int i = 1; i < argc; i++) {
        if (fork() == 0) {
            if (execlp(argv[i], argv[i], (char *) NULL) == -1) {
                err(1, "Exec for %s failed", argv[i]);
            }
        }

        wait(&wstatus);
        if(WIFEXITED(wstatus)) {
            successCounter++;
        } else{
            errorCounter++;
        }
    }

    wait(NULL);

    printf("Total %d success and %d fails\n\n", successCounter, errorCounter);



    return 0;
}