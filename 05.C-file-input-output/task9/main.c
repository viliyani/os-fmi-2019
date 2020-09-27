#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

int main(int argc, char * argv[]) {

	if (argc != 3) {
		errx(1, "Give 2 arguments!");
	}

	int fd = open("dump", O_RDONLY);

	if (fd == -1) {
		err(1, "File open problem");
	}

	uint16_t nums[4096];
	read(fd, &nums, sizeof(nums));

	int i;
	int len = 4096;

	if (strcmp(argv[1], "--min") == 0) {
		// Find min
		uint16_t min = nums[0];

		for(i = 1; i < len; i++) {
			if (nums[i] < min) {
				min = nums[i];
			}
		}

		printf("Minimum is: %d\n", min);

	} else if (strcmp(argv[1], "--max") == 0) {
		// Find max
		uint16_t max = nums[0];

		for(i = 1; i < len; i++) {
			if (nums[i] > max) {
				max = nums[i];
			}
		}

		printf("Maximum is: %d\n", max);

	} else if (strcmp(argv[1], "--print") == 0) {
		// Print

		for(i = 0; i < len; i++) {
			printf("%d\n", nums[i]);
		}
	} else {
		close(fd);
		errx(1, "Invalid operation!");
	}

	close(fd);

	exit(0);
}
