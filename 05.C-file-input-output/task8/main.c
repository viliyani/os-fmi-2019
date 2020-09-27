#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <err.h>

int main() {
	int fd1 = open("/etc/passwd", O_RDONLY);

	if(fd1 == -1) {
		err(1, "Failed to open /etc/passwd");
	}

	int fd2 = open("my_passwd", O_WRONLY | O_CREAT | O_TRUNC, 0644);

	if(fd2 == -1) {
		err(1, "Failed to open my_passwd");
	}

	char buff[4096];
	ssize_t readSize;
	int i;

	while ((readSize = read(fd1, &buff, sizeof(buff))) > 0) {

		for (i = 0; i < readSize; i++) {
			if (buff[i] == ':') {
				buff[i] = '@';
			}
		}

		if ((write(fd2, &buff, readSize)) != readSize) {
			close(fd2);
			err(1, "Problem with file write");
		}
		
	}

	close(fd2);

	exit(0);
}
