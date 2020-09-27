#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include <stdio.h>

void copy_file_to_dir(char * src_filename, char * dest_dirname) {
	struct stat src_stats;

	if (stat(src_filename, &src_stats) == -1) {
		err(2, "Cant stat src file %s", src_filename);
	}

	char * dest_filename = (char *) malloc(strlen(dest_dirname) + 1 + strlen(src_filename) + 1);

	strncpy(dest_filename, dest_dirname, strlen(dest_dirname));
	strncat(dest_filename, "/", 1);
	strncat(dest_filename, src_filename, strlen(src_filename));

	printf("Copying file %s to dest %s \n", src_filename, dest_filename);

	int fd1 = open(src_filename, O_RDONLY);
	if(fd1 == -1) {
		err(2, "File open problem for %s", src_filename);
	}

	int fd2 = open(dest_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if(fd2 == -1) {
		close(fd1);
		err(2, "File open problem for %s", dest_filename);
	}

	char buff[4096];
	ssize_t readSize = 0;

	while ((readSize = read(fd1, &buff, sizeof(buff))) > 0) {
		if ((write(fd2, &buff, readSize)) != readSize) {
			close(fd1);
			close(fd2);
			err(2, "Problem with writing in %s", dest_filename);
		}
	}

	close(fd1);
	close(fd2);
	free(dest_filename);
}

int main(int argc, char* argv[]) {
	if (argc < 3) {
		errx(1, "Give minimum 2 arguments!");
	}

	// There are 2 cases:

	// 1. Only 2 arguments -> Copy arg1 in arg2
	if (argc == 3) {
		int readSize = 0;
		char buff[4096];

		int fd1 = open(argv[1], O_RDONLY);
		if(fd1 == -1) {
			err(1, "Open file %s problem.", argv[1]);
		}

		int fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if(fd2 == -1) {
			close(fd1);
			err(1, "Open file %s problem.", argv[2]);
		}

		// Read from fd1 and write to fd2
		while((readSize = read(fd1, &buff, sizeof(buff))) > 0){
			if((write(fd2, &buff, readSize)) != readSize) {
				close(fd1);
				close(fd2);
				err(1, "Problem with writing in %s", argv[2]);
			}
		}
	} else {
		// 2. More than 2 arguments -> Copy arg1,..., arg(N-1) in directory argN
		char * destDir = argv[argc-1];

		struct stat dirStat;

		stat(destDir, &dirStat);

		if(!S_ISDIR(dirStat.st_mode)) {
			err(1, "The argument %s is not a directory!", destDir);
		}
		
		int i = 0;

		for (i = 1; i < argc-1; i++) {
			copy_file_to_dir(argv[i],destDir);
		}

	}

	exit(0);
}
