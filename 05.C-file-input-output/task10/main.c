#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

// int main(int argc, char * argv[]) {
int main(void) {

	uint16_t a = 300;
	uint8_t * p = (uint8_t *) &a;

	printf("Hello! -> %d --> %x\n", a, a);
	printf("Hello! -> %d --> %x\n", p[0], p[0]);
	printf("Hello! -> %d --> %x\n", p[1], p[1]);
	// if (argc != 3) {
	// 	errx(1, "Give 2 arguments!");
	// }

	// int fd = open("dump", O_RDONLY);

	// if (fd == -1) {
	// 	err(1, "File open problem");
	// }

	// uint16_t nums[4096];
	// read(fd, &nums, sizeof(nums));

	// int i;
	// int len = 4096;

	// // Print

	// for(i = 0; i < len; i++) {
	// 	printf("%d\n", nums[i]);
	// }
	

	// close(fd);

	exit(0);
}
