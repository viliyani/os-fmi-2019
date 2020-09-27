#include <stdlib.h>
// Open
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
// Stat
#include <unistd.h>
// Errors
#include <err.h>
// Print
#include <stdio.h>
// Wait
#include <sys/wait.h>
// String
#include <string.h>
// Ints
#include <stdint.h>

void createFile1() {
	// int fd = open("f1.bin", O_WRONLY | O_CREAT | O_TRUNC, 0644);

	// uint8_t nums[] = {245, 196, 177, 89, 204, 128, 226, 239, 193, 199, 201, 154, 47, 176, 13, 140, 60, 131, 111, 237, 107, 70, 9, 210, 144, 223, 207, 30, 154, 60, 31, 5, 5, 249, 76, 41, 253, 88, 165, 241, 203, 123, 201, 208, 178, 52, 35, 152, 53, 175, 107, 230, 90, 113, 178, 58, 14, 141, 8, 222, 222, 242, 33, 76};

	// write(fd, &nums, sizeof(nums));

	// close(fd);
}

void createFile2() {
	// int fd = open("f2.bin", O_WRONLY | O_CREAT | O_TRUNC, 0644);

	// uint8_t nums[] = {245, 196, 89, 89, 204, 128, 226, 239, 193, 199, 201, 154, 47, 176, 13, 140, 60, 131, 111, 237, 107, 70, 9, 210, 144, 223, 207, 30, 154, 60, 31, 5, 5, 249, 76, 41, 253, 88, 165, 241, 203, 123, 201, 208, 178, 52, 35, 152, 175, 175, 107, 230, 90, 113, 178, 58, 14, 141, 8, 222, 222, 242, 33, 76};

	// write(fd,&nums, sizeof(nums));

	// close(fd);
}

int main(void) {

	// # FILE 1
	int fd1 = open("f1.bin", O_RDONLY);
	if(fd1 == -1) {
		err(1, "File 1 open problem!");
	}

	// Get count of total numbers in file 1
	int total_in_f1 = lseek(fd1, 0, SEEK_END);
	lseek(fd1, 0, SEEK_SET);

	// Extract numbers from file 1
	uint8_t * nums1 = (uint8_t *) malloc(total_in_f1);

	uint8_t int_buff[64];
	size_t idx = 0;
	ssize_t read_size;
	while((read_size = read(fd1, &int_buff, sizeof(int_buff))) > 0){
		int j = 0;
		for (j = 0; j < read_size; j++) {
			nums1[idx++] = int_buff[j];
		}
	}

	// Print numbers in file 1
	/*int i = 0;
	for (; i < total_in_f1; i++) {
		printf("Number: %d\n", nums1[i]);
	}*/

	// # FILE 2
	int fd2 = open("f2.bin", O_RDONLY);
	if(fd2 == -1) {
		err(1, "File 1 open problem!");
	}

	// Get count of total numbers in file 1
	int total_in_f2 = lseek(fd2, 0, SEEK_END);
	lseek(fd2, 0, SEEK_SET);

	// Extract numbers from file 1
	uint8_t * nums2 = (uint8_t *) malloc(total_in_f2);

	uint8_t int_buff2[64];
	size_t idx2 = 0;
	ssize_t read_size2;
	while((read_size2 = read(fd2, &int_buff2, sizeof(int_buff2))) > 0){
		int j = 0;
		for (j = 0; j < read_size2; j++) {
			nums2[idx2++] = int_buff2[j];
		}
	}

	// Print numbers in file 2
	/*int i2 = 0;
	for (; i2 < total_in_f2; i2++) {
		printf("Number: %d\n", nums2[i2]);
	}*/

	// # FILE 3 -> Patch file with results
	int fd3 = open("patch.bin", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd3 == -1) {
		err(1, "File 3 open error!");
	}

	// Struct for each result
	struct element {
		uint16_t offset;
		uint8_t orig_byte;
		uint8_t new_byte;
	};

	struct element result;

	// Print numbers from the 2 files
	int i = 0;
	char f[3];
	for (; i < total_in_f1; i++) {
		f[0] = ' ';
		f[1] = ' ';
		f[2] = '\0';
		if(nums1[i] != nums2[i]) {
			f[0] = 'D';
			f[1] = 'A';
			f[2] = '\0';

			// Save in patch
			result.offset = i;
			result.orig_byte = nums1[i];
			result.new_byte = nums2[i];
			write(fd3, &result, sizeof(result));

		} 
		printf("%d -> %d --- %d    %s\n", i, nums1[i], nums2[i], f);
	}

	// Delete dynamic memory
	free(nums1);
	free(nums2);

	// Close file descriptiors
	close(fd1);
	close(fd2);
	close(fd3);

    return 0;
}