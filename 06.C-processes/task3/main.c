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
    if (argc != 2) {
    	errx(1, "Give only 1 argument, please!");
    }

    char c;
    int counter = 0;

    // struct stat statFile;

    // Check file -> In this task this is not necessary.
    /*stat(argv[1], &statFile);

    if(!S_ISREG(statFile.st_mode)) {
        errx(2, "No regular file!");
    }*/
    
    // Open file
    

    pid_t pid = fork();

    if (pid == 0) {
        // Child
        int fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);

        if (fd < 0) {
            err(3, "Error with open file: %s \n", argv[1]);
        }
        write(fd, "foobar", 6);

        close(fd);

        exit(0);
    }

    // Parent
    wait(NULL);

    int fd = open(argv[1], O_RDONLY, 0644);

    if (fd < 0) {
        err(3, "Error with open file: %s \n", argv[1]);
    }

    while(read(fd, &c, 1) > 0)
    {
        if (counter % 2 == 0) {
            write(1, "\n", 2);
        }
        write(1, &c, 1);
        counter++;
    }
    
    
    close(fd);

    return 0;
}