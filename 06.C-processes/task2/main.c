#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <err.h>

int main(int argc, char * argv[]) {
    if (argc != 4) {
    	errx(1, "Give only 4 arguments, please!");
    }

    int status1, status2, status3;

    // Command 1 -> Do fork!
    pid_t pid1 = fork();

    if (pid1 == 0) {
    	// Child
    	execlp(argv[1], argv[1], (char *) NULL);
    } 

	// Parent
   	wait(&status1);
   	printf("--> We run command: %s. PID: %d, Status: %d \n", argv[1], pid1, status1);

    // Command 2 -> Do fork!
    pid_t pid2 = fork();

    if (pid2 == 0) {
        // Child
        execlp(argv[2], argv[2], (char *) NULL);
    } 

    // Parent
    wait(&status2);
    printf("--> We run command: %s. PID: %d, Status: %d \n", argv[2], pid2, status2);

    // Command 3 -> Do fork!
    pid_t pid3 = fork();

    if (pid3 == 0) {
        // Child
        execlp(argv[3], argv[3], (char *) NULL);
    } 

    // Parent
    wait(&status3);
    printf("--> We run command: %s. PID: %d, Status: %d \n", argv[3], pid3, status3);


    return 0;
}