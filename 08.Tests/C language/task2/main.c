// cut -d: -f7 /etc/passwd | sort | uniq -c | sort -n

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
// Ints
#include <stdint.h>


int main(void) {

	const int READ = 0;
	const int WRITE = 1;

	int pd1[2];
	int pd2[2];
	int pd3[2];

	pid_t cut_pid;
	pid_t sort_pid;
	pid_t uniq_pid;

	if (pipe(pd1) == -1) {
		err(1, "pipe error");
	}

	if ((cut_pid = fork()) == -1) {
		err(1, "cut fork");
	}

	if (cut_pid == 0) {
		// Child
		close(pd1[READ]);
		dup2(pd1[WRITE], 1);
		if (execlp("cut", "cut", "-d:", "-f7", "/etc/passwd", (char *) NULL) == -1) {
			err(1, "cut exec");
		}
	}

	// Parent
	close(pd1[WRITE]);

	if (pipe(pd2) == -1) {
		err(1, "pipe 2 error");
	}

	if ((sort_pid = fork()) == -1) {
		err(1, "sort fork");
	}

	if (sort_pid == 0) {
		// Child
		close(pd2[READ]);
		dup2(pd1[READ], 0);
		dup2(pd2[WRITE], 1);
		if (execlp("sort", "sort", (char *) NULL) == -1) {
			err(1, "sort exec");
		}
	}

	// Parent
	close(pd2[WRITE]);

	if (pipe(pd3) == -1) {
		err(1, "pipe 3 error");
	}

	if ((uniq_pid = fork()) == -1) {
		err(1, "uniq fork");
	}

	if (uniq_pid == 0) {
		// Child
		close(pd3[READ]);
		dup2(pd2[READ], 0);
		dup2(pd3[WRITE], 1);
		if (execlp("uniq", "uniq", "-c", (char *) NULL) == -1) {
			err(1, "uniq exec");
		}
	}

	// Parent
	close(pd3[WRITE]);

	dup2(pd3[READ], 0);

	if (execlp("sort", "sort", "-n", (char *) NULL) == -1) {
		err(1, "sort exec");
	}
}