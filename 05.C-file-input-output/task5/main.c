#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>


int main(int argc, char* argv[]) {
	if ( argc != 3 ) {
		write(2, "Error. Give source file and destination file\n", 50);
		exit(1);
	}

	int fd1;
	int fd2;
	char c;
	
	fd1=open(argv[1], O_RDONLY);
	
	if (fd1 == -1) {
		write(2, "Error with first file open\n", 20);
		exit(1);
	}
	
	fd2=open(argv[2], O_CREAT|O_WRONLY, 0777);

	if(fd2 == -1) {
		close(fd1);
		write(2, "Error with second file open\n", 20);
		exit(2);
	}

	while (read(fd1, &c, 1)) {
		write(fd2, &c, 1);
	}
	
	close(fd1);
	close(fd2);

	exit(0);
}
