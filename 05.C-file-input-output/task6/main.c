#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <err.h>

int main(int argc, char* argv[]) {
	if ( argc < 2 ) {
		errx(1, "No files are given!");
	}

	int fd;
	char buff[4096];
	int i = 0;
	int readSize = 0;

	for (i = 1; i < argc; i++) {
		fd=open(argv[i], O_RDONLY);
	
		if (fd == -1) {
			err(2, "Open failed for file %s", argv[i]);
		}
	
		while ((readSize = read(fd, &buff, sizeof(buff))) > 0) {
			if((write(1, &buff, readSize)) != readSize) {
				err(3, "Problem with write in file %s", argv[i]);
				close(fd);
			}
		}
	
		close(fd);
	}

	exit(0);
}
