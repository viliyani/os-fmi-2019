#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <err.h>

int main(int argc, char * argv[]) {
    if (argc != 2) {
    	errx(1, "Give only 1 argument, please!");
    }

    pid_t pid = fork();

    if (pid == 0) {
    	// Child
    	execlp(argv[1], argv[1], (char *) NULL);
    } else {
    	// Parent
    	wait(NULL);
    	printf("--> We run command: %s \n", argv[1]);
    }

    return 0;
}