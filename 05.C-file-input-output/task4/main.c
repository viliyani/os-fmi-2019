#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>

int openFile(const char * fname) {
	struct stat statf;
	stat(fname, &statf);

	if (!S_ISREG(statf.st_mode)) {
		err(1, "The file %s isn't a regular file", fname);
	}

	int fd = open(fname, O_RDWR);

	return fd;
}

void copyContent(int fdReadFrom, int fdWriteTo, int fdOther) {
	char buff[4096];
	
	int readSize = 0;

	while ((readSize = read(fdReadFrom, &buff, sizeof(buff))) > 0) {
		printf("Readsize: %d \n", readSize);
		if (write(fdWriteTo, &buff, readSize) != readSize) {
			close(fdReadFrom);
			close(fdOther);
			close(fdWriteTo);
			err(1, "Error while writing");
		} 
	}

	lseek(fdReadFrom, 0, SEEK_SET);
	lseek(fdWriteTo, 0, SEEK_SET);
}

int main(int argc, char* argv[]) {
	if (argc != 3) {
		err(1, "Give 2 arguments, please!");
	}

	// Open and validate file 1
	int fd1 = openFile(argv[1]);

	if (fd1 == -1) {
		err(1, "Problem with file open for %s", argv[1]);
	}

	// Open and validate file 2
	int fd2 = openFile(argv[2]);

	if (fd2 == -1) {
		close(fd1);
		err(1, "Problem with file open for %s", argv[1]);
	}

	// Open file 3 -> temp file for swap
	int fd3 = open("temp", O_RDWR | O_CREAT | O_TRUNC, 0644);

	// Save f1 in temp -> Read f1 and Write in temp
	copyContent(fd1, fd3, fd2);

	// Save f2 in f1 -> Read f2 and Write in f1
	copyContent(fd2, fd1, fd3);

	// Save temp in f2 -> Read temp and Write in f2
	copyContent(fd3, fd2, fd1);

	exit(0);
}
