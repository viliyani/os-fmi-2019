#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
	if ( argc != 2 ) {
		write(2, "Error. Give one file.\n", 30);
		exit(1);
	}

	int fd;
	int i = 0;
	char c;
	
	fd=open(argv[1], O_RDONLY);
	
	if (fd == -1) {
		write(2, "Error with file open\n", 20);
		exit(1);
	}
	
	while (read(fd, &c, 1)) {
		if (c == '\n') {
			i=i+1;
		}

		write(1, &c, 1);

		if (i == 10) {
			break;
		}
	}

	close(fd);
	exit(0);
}
