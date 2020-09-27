#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
	if ( argc != 2 ) {
		write(2, "Error. Give one file.\n", 30);
		exit(1);
	}

	int fd;
	int chars = 0;
	int words = 0;
	int lines = 0;
	char c;
	
	fd=open(argv[1], O_RDONLY);
	
	if (fd == -1) {
		write(2, "Error with file open\n", 20);
		exit(1);
	}
	
	while (read(fd, &c, 1)) {
		if (c == '\n') {
			lines = lines + 1;
		}
		
		if (c == ' ') {
			words = words + 1;
		}

		chars = chars + 1;

	}
	
	printf("Total lines: %d, Total words: %d, Total chars: %d", lines, words, chars);

	close(fd);
	exit(0);
}
