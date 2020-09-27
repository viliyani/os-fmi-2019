#include <stdlib.h>
// Open
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
// Stat
#include <unistd.h>
// Errors
#include <err.h>
#include <errno.h>
// Print
#include <stdio.h>
// Wait
#include <sys/wait.h>
// String
#include <string.h>
// Ints
#include <stdint.h>

void printLine(int line, int check) {
	if (check == 1) {
		char c = (++line) + '0';
		write(1, &c, 1);
		write(1, " ", 1);	
	}
}

int main(int argc, char * argv[]) {

	if (argc < 2) {
		errx(1, "Give arguments!");
	}

	int i;
	int show_lines = 0;
	int line = 0;

	if (strcmp(argv[1], "-n") == 0) {
		i = 2;
		show_lines = 1;
	} else {
		i = 1;
	}

	char buff[4096];
	ssize_t read_size;
	int j, k;
	int fd;

	

	for (; i < argc; i++) {

		if (strcmp("-", argv[i]) == 0) {
			//printFromStdin();
			printLine(line++, show_lines);

			ssize_t read_size;
			
			while ((read_size = read(0, &buff, sizeof(buff))) > 0) {

				for (j = 0; j < read_size; j++) {
					if (buff[j - 1] == '\n') {
						printLine(line++, show_lines);
					}
					write(1, &buff[j], 1);
				}
			}
		} else {
			// Print from file
			fd = open(argv[i], O_RDONLY);
			if (fd == -1) {
				err(1, "Error with file open for %s", argv[i]);
			}

			printLine(line++, show_lines);

			while ((read_size = read(fd, &buff, sizeof(buff))) > 0) {
				for (k = 0; k < read_size; k++) {
					if (buff[k - 1] == '\n') {
						printLine(line++, show_lines);
					}
					write(1, &buff[k], 1);
				}
			}

			close(fd);
		}
	}

	return 0;
}