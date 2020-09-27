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
// Regex
#include <regex.h>
// Pow
#include <math.h>

// Structures for each segment type

typedef struct seg_text_type {
	uint8_t segmentType;
    uint8_t metaInfo[7];
    char word_options[3][16];
    uint8_t padding[8]; // That's because we have 56 - 48 = 8 bytes that are not used 
} seg_text_type;

typedef struct seg_num_type {
	uint8_t segmentType;
    uint8_t metaInfo[7];
    uint32_t num_options[14];
} seg_num_type;

typedef struct seg_byte_type {
	uint8_t segmentType;
    uint8_t metaInfo[7];
    uint8_t byte_options[56];
} seg_byte_type;

// ### CONFIGURATION ###

// Segments order for our device
seg_text_type segment_text1;
seg_text_type segment_text2;
seg_num_type segment_num;
seg_byte_type segment_byte;
seg_text_type segment_text3;

// Parameters -> Info is divided in 3 arrays for parameter name, segment id, and position
// Total: 15 parameters in 5 segments

// parameter; segment; position; valid values
char param_names[15][20] = {
							"device_name", "rom_revision", "serial_number",
							"bd_addr_part0", "bd_addr_part1", "bd_pass_part0",
							"serial_baudrate", "audio_bitrate", "sleep_period",
							"serial_parity", "serial_data_bit", "serail_stop_bit",
							"bd_pass_part1", "rom_checksum_part0", "rom_checksum_part1"
							};
int param_segments[15] = 	{0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4};
int param_positions[15] = 	{0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 2};
// For example: 
// - Parameter: device_name; 0; 0; ---> param_names[0] = "device_name", param_segments[0] = 0, param_positions[0] = 0;

// ### FUNCTIONS ###

// Source: https://www.quora.com/How-do-I-use-regular-expressions-in-the-C-programming-language
int regex_match(const char *string, const char *pattern)
{
    regex_t re;
    if (regcomp(&re, pattern, REG_EXTENDED|REG_NOSUB) != 0) return 0;
    int status = regexec(&re, string, 0, NULL, 0);
    regfree(&re);
    if (status != 0) return 0;
    return 1;
}

void extract_text_segment(int fd, seg_text_type * seg_text) {
	// Read 64 bytes from the file and store it to the given structure
	read(fd, seg_text, 64);
}

void extract_num_segment(int fd, seg_num_type * seg_num) {
	// Read 64 bytes from the file and store it to the given structure
	read(fd, seg_num, 64);
}

void extract_byte_segment(int fd, seg_byte_type * seg_byte) {
	// Read 64 bytes from the file and store it to the given structure
	read(fd, seg_byte, 64);
}

// Print TEXT Segment by given segment in structure
void print_text_segment(seg_text_type * seg_text, int segment_num) {
	printf("___ Segment: TEXT ___\n");

	printf("Meta Info Bytes: %d %d %d %d %d %d %d\n", seg_text->metaInfo[0], seg_text->metaInfo[1], seg_text->metaInfo[2], seg_text->metaInfo[3], seg_text->metaInfo[4], seg_text->metaInfo[5], seg_text->metaInfo[6]);

	// Show word options
    int i;
    for(i = 0; i < 3; i++) {
    	printf("Word option (%d): %s = %s\n",(i+1), param_names[(segment_num * 3) + i], seg_text->word_options[i]);
    }

}

// Print NUM Segment by given segment in structure
void print_num_segment(seg_num_type * seg_num, int segment_num) {
	printf("___ Segment: NUM ___\n");
	
	printf("Meta Info Bytes: %d %d %d %d %d %d %d\n", seg_num->metaInfo[0], seg_num->metaInfo[1], seg_num->metaInfo[2], seg_num->metaInfo[3], seg_num->metaInfo[4], seg_num->metaInfo[5], seg_num->metaInfo[6]);

    int i;
	// Show num options from the file
    for(i = 0; i < 3; i++) {
    	printf("Num option (%d): %s = %d\n",(i+1), param_names[(segment_num * 3) + i], seg_num->num_options[i]);
    }

}

// Print BYTE Segment by given segment in structure
void print_byte_segment(seg_byte_type * seg_byte, int segment_num) {
	printf("___ Segment: BYTE ___\n");
	
	printf("Meta Info Bytes: %d %d %d %d %d %d %d\n", seg_byte->metaInfo[0], seg_byte->metaInfo[1], seg_byte->metaInfo[2], seg_byte->metaInfo[3], seg_byte->metaInfo[4], seg_byte->metaInfo[5], seg_byte->metaInfo[6]);

    int i;
    // Show byte options from the file
    char inAscii;
    for(i = 0; i < 3; i++) {
    	inAscii = seg_byte->byte_options[i];
    	printf("Byte option (%d): %s = %d --> In Ascii: %s \n",(i+1), param_names[(segment_num * 3) + i], seg_byte->byte_options[i], &inAscii);
    }

}

// Text segment options validator
int text_segment_options_validator(const char * option_name, const char * option_value) {
	// This function do regex check and return 1 (true) and 0 (false)

	char reg_expr[20];

	// Regex expression for different options
	
	// For 1st Text Segment
	if (strcmp(option_name, "device_name") == 0) {
		// regex expression -> [a-zA-Z0-9_-]
		strcpy(reg_expr, "^[a-zA-Z0-9_-]+$");
	} else if (strcmp(option_name, "rom_revision") == 0) {
		// regex expression -> [a-zA-Z0-9_-\.]
		strcpy(reg_expr, "^[-a-zA-Z0-9_\\.]+$");
	} else if (strcmp(option_name, "serial_number") == 0) {
		// regex expression -> [A-Z0-9]
		strcpy(reg_expr, "^[A-Z0-9]+$");
	} 
	// For 2nd Text Segment
	else if (strcmp(option_name, "bd_addr_part0") == 0) {
		// regex expression -> [A-Z0-9:]
		strcpy(reg_expr, "^[A-Z0-9:]+$");
	} else if (strcmp(option_name, "bd_addr_part1") == 0) {
		// regex expression -> [A-Z0-9:]
		strcpy(reg_expr, "^[A-Z0-9:]+$");
	} else if (strcmp(option_name, "bd_pass_part0") == 0) {
		// regex expression -> [a-z0-9]
		strcpy(reg_expr, "^[a-z0-9]+$");
	} 
	// For 3rd Text Segment
	else if (strcmp(option_name, "bd_pass_part1") == 0) {
		// regex expression -> [a-z0-9]
		strcpy(reg_expr, "^[a-z0-9]+$");
	} else if (strcmp(option_name, "rom_checksum_part0") == 0) {
		// regex expression -> [a-z0-9]
		strcpy(reg_expr, "^[a-z0-9]+$");
	} else if (strcmp(option_name, "rom_checksum_part1") == 0) {
		// regex expression -> [a-z0-9]
		strcpy(reg_expr, "^[a-z0-9]+$");
	} else {
		printf("--> ERROR! --> Option name: %s is not found.\n", option_name);
		return 0;
	}	

	// Do regex check
	if (regex_match(option_value, reg_expr)) {
		return 1;
	}

	return 0;
}

// Numeric segment options validator
int num_segment_options_validator(const char * option_name, uint32_t option_value) {
	uint32_t valid_values[20];

	if (strcmp(option_name, "serial_baudrate") == 0) {
		valid_values[0] = 1200;
		valid_values[1] = 2400;
		valid_values[2] = 4800;
		valid_values[3] = 9600;
		valid_values[4] = 19200;
		valid_values[5] = 115200;
	} else if (strcmp(option_name, "audio_bitrate") == 0) {
		valid_values[0] = 32;
		valid_values[1] = 96;
		valid_values[2] = 128;
		valid_values[3] = 160;
		valid_values[4] = 192;
		valid_values[5] = 256;
		valid_values[6] = 320;
	} else if (strcmp(option_name, "sleep_period") == 0) {
		valid_values[0] = 100;
		valid_values[1] = 500;
		valid_values[2] = 1000;
		valid_values[3] = 5000;
		valid_values[4] = 10000;
	} else {
		printf("--> ERROR! --> Option name: %s is not found.\n", option_name);
		return 0;
	}


	// Check if option_value is in valid_values
	int i;
	int total = sizeof(valid_values)/sizeof(valid_values[0]);

	for (i = 0; i < total; i++) {
		if (option_value == valid_values[i]) {
			return 1;
		}
	}

	return 0;
}

// Byte segment options validator
int byte_segment_options_validator(const char * option_name, uint8_t option_value) {

	char c;

	if (strcmp(option_name, "serial_parity") == 0) {
		c = option_value;

		if ( c == 'N' ) {
			return 1;
		} else if ( c == 'E' ) {
			return 1;
		} else if ( c == 'O' ) {
			return 1;
		} else {
			return 0;
		}

	} else if (strcmp(option_name, "serial_data_bit") == 0) {

		if ( option_value == 5 ) {
			return 1;
		} else if ( option_value == 6 ) {
			return 1;
		} else if ( option_value == 7 ) {
			return 1;
		} else if ( option_value == 8 ) {
			return 1;
		} else {
			return 0;
		}

	} else if (strcmp(option_name, "serail_stop_bit") == 0) {
		
		if ( option_value == 0 ) {
			return 1;
		} else if ( option_value == 1 ) {
			return 1;
		} else {
			return 0;
		}

	} else {
		printf("--> ERROR! --> Option name: %s is not found.\n", option_name);
		return 0;
	}

	return 0;
}

// Extract all segments
void extract_all_segments(int fd) {
	// For our device we have total 5 segments in following order:
	// 0 - text, 1 - text, 2 - numeric, 3 - byte, 4 - text
	// So, do 5 segment extracts by types

	// Segment 0, type: Text
	printf("****** 0, TEXT ******\n");
	extract_text_segment(fd, &segment_text1);
	print_text_segment(&segment_text1, 0);

	// Segment 1, type: Text
	printf("****** 1, TEXT ******\n");
	extract_text_segment(fd, &segment_text2);
	print_text_segment(&segment_text2, 1);

	// Segment 2, type: Numeric
	printf("****** 2, NUMERIC ******\n");
	extract_num_segment(fd, &segment_num);
	print_num_segment(&segment_num, 2);

	// Segment 3, type: Byte
	printf("****** 3, BYTE ******\n");
	extract_byte_segment(fd, &segment_byte);
	print_byte_segment(&segment_byte, 3);

	// Segment 4, type: Text
	printf("****** 4, TEXT ******\n");
	extract_text_segment(fd, &segment_text3);
	print_text_segment(&segment_text3, 4);
}

uint8_t make_byte_from_bits(uint8_t * bits) {
	uint8_t result_byte = 0;

	int i;
	uint8_t temp_pow = 1;
    int k;

	for (i = 0; i < 8; i++) {
		temp_pow = 1;
		for (k = 1; k < (8-i); k++) {
			temp_pow *= 2;
		}
		result_byte += (bits[i]*temp_pow);
	}

	return result_byte;
}

// Function that makes Active given bit in meta info
void set_meta_bits(int segment_id, uint8_t bit_position)
{	
	uint8_t new_bits[56] = {0};
	uint8_t temp_byte = 0; 
    uint8_t temp_bits[8];
	int bit_temp;
	int idx = 0;
	int i, j;


	if (segment_id == 0)
	{
		// Convert meta info bytes to bits
	    for (i = 0; i < 7; i++) {
	    	// Get bits in reversed order because of Little-Endian
	    	for (j = 7; j >= 0; j--) {
	    		bit_temp = (segment_text1.metaInfo[i] >> j) & 1;
	    		new_bits[idx++] = bit_temp;
	    	}
	    }

	    // Set active the given bit
	    new_bits[bit_position] = 1;

	    // Convert back to bytes
	    for (i = 0; i < 7; i++) {
	    	temp_byte = 0; 
	    	
	    	for (j = 0; j < 8; j++) {
	    			temp_bits[j] = new_bits[(i*8)+j];
	    	}

	    	temp_byte = make_byte_from_bits(temp_bits);

	    	segment_text1.metaInfo[i] = temp_byte;
	    }
	} else if (segment_id == 1)
	{
		// Convert meta info bytes to bits
	    for (i = 0; i < 7; i++) {
	    	// Get bits in reversed order because of Little-Endian
	    	for (j = 7; j >= 0; j--) {
	    		bit_temp = (segment_text2.metaInfo[i] >> j) & 1;
	    		new_bits[idx++] = bit_temp;
	    	}
	    }

	    // Set active the given bit
	    new_bits[bit_position] = 1;

	    // Convert back to bytes
	    for (i = 0; i < 7; i++) {
	    	temp_byte = 0; 
	    	
	    	for (j = 0; j < 8; j++) {
	    			temp_bits[j] = new_bits[(i*8)+j];
	    	}

	    	temp_byte = make_byte_from_bits(temp_bits);

	    	segment_text2.metaInfo[i] = temp_byte;
	    }
	} else if (segment_id == 2)
	{
		// Convert meta info bytes to bits
	    for (i = 0; i < 7; i++) {
	    	// Get bits in reversed order because of Little-Endian
	    	for (j = 7; j >= 0; j--) {
	    		bit_temp = (segment_num.metaInfo[i] >> j) & 1;
	    		new_bits[idx++] = bit_temp;
	    	}
	    }

	    // Set active the given bit
	    new_bits[bit_position] = 1;

	    // Convert back to bytes
	    for (i = 0; i < 7; i++) {
	    	temp_byte = 0; 
	    	
	    	for (j = 0; j < 8; j++) {
	    			temp_bits[j] = new_bits[(i*8)+j];
	    	}

	    	temp_byte = make_byte_from_bits(temp_bits);

	    	segment_num.metaInfo[i] = temp_byte;
	    }
	} 
	else if (segment_id == 3)
	{
		// Convert meta info bytes to bits
	    for (i = 0; i < 7; i++) {
	    	// Get bits in reversed order because of Little-Endian
	    	for (j = 7; j >= 0; j--) {
	    		bit_temp = (segment_byte.metaInfo[i] >> j) & 1;
	    		new_bits[idx++] = bit_temp;
	    	}
	    }

	    // Set active the given bit
	    new_bits[bit_position] = 1;

	    // Convert back to bytes
	    for (i = 0; i < 7; i++) {
	    	temp_byte = 0; 
	    	
	    	for (j = 0; j < 8; j++) {
	    			temp_bits[j] = new_bits[(i*8)+j];
	    	}

	    	temp_byte = make_byte_from_bits(temp_bits);

	    	segment_byte.metaInfo[i] = temp_byte;
	    }
	} 
	else if (segment_id == 4)
	{
		// Convert meta info bytes to bits
	    for (i = 0; i < 7; i++) {
	    	// Get bits in reversed order because of Little-Endian
	    	for (j = 7; j >= 0; j--) {
	    		bit_temp = (segment_text3.metaInfo[i] >> j) & 1;
	    		new_bits[idx++] = bit_temp;
	    	}
	    }

	    // Set active the given bit
	    new_bits[bit_position] = 1;

	    // Convert back to bytes
	    for (i = 0; i < 7; i++) {
	    	temp_byte = 0; 
	    	
	    	for (j = 0; j < 8; j++) {
	    			temp_bits[j] = new_bits[(i*8)+j];
	    	}

	    	temp_byte = make_byte_from_bits(temp_bits);

	    	segment_text3.metaInfo[i] = temp_byte;
	    }
	}
}

// Function that makes Unactive given bit in meta info
void unset_meta_bits(int segment_id, uint8_t bit_position)
{	
	uint8_t new_bits[56] = {0};
	uint8_t temp_byte = 0; 
    uint8_t temp_bits[8];
	int bit_temp;
	int idx = 0;
	int i, j;


	if (segment_id == 0)
	{
		// Convert meta info bytes to bits
	    for (i = 0; i < 7; i++) {
	    	// Get bits in reversed order because of Little-Endian
	    	for (j = 7; j >= 0; j--) {
	    		bit_temp = (segment_text1.metaInfo[i] >> j) & 1;
	    		new_bits[idx++] = bit_temp;
	    	}
	    }

	    // Set active the given bit
	    new_bits[bit_position] = 0;

	    // Convert back to bytes
	    for (i = 0; i < 7; i++) {
	    	temp_byte = 0; 
	    	
	    	for (j = 0; j < 8; j++) {
	    			temp_bits[j] = new_bits[(i*8)+j];
	    	}

	    	temp_byte = make_byte_from_bits(temp_bits);

	    	segment_text1.metaInfo[i] = temp_byte;
	    }
	} else if (segment_id == 1)
	{
		// Convert meta info bytes to bits
	    for (i = 0; i < 7; i++) {
	    	// Get bits in reversed order because of Little-Endian
	    	for (j = 7; j >= 0; j--) {
	    		bit_temp = (segment_text2.metaInfo[i] >> j) & 1;
	    		new_bits[idx++] = bit_temp;
	    	}
	    }

	    // Set active the given bit
	    new_bits[bit_position] = 0;

	    // Convert back to bytes
	    for (i = 0; i < 7; i++) {
	    	temp_byte = 0; 
	    	
	    	for (j = 0; j < 8; j++) {
	    			temp_bits[j] = new_bits[(i*8)+j];
	    	}

	    	temp_byte = make_byte_from_bits(temp_bits);

	    	segment_text2.metaInfo[i] = temp_byte;
	    }
	} else if (segment_id == 2)
	{
		// Convert meta info bytes to bits
	    for (i = 0; i < 7; i++) {
	    	// Get bits in reversed order because of Little-Endian
	    	for (j = 7; j >= 0; j--) {
	    		bit_temp = (segment_num.metaInfo[i] >> j) & 1;
	    		new_bits[idx++] = bit_temp;
	    	}
	    }

	    // Set active the given bit
	    new_bits[bit_position] = 0;

	    // Convert back to bytes
	    for (i = 0; i < 7; i++) {
	    	temp_byte = 0; 
	    	
	    	for (j = 0; j < 8; j++) {
	    			temp_bits[j] = new_bits[(i*8)+j];
	    	}

	    	temp_byte = make_byte_from_bits(temp_bits);

	    	segment_num.metaInfo[i] = temp_byte;
	    }
	} 
	else if (segment_id == 3)
	{
		// Convert meta info bytes to bits
	    for (i = 0; i < 7; i++) {
	    	// Get bits in reversed order because of Little-Endian
	    	for (j = 7; j >= 0; j--) {
	    		bit_temp = (segment_byte.metaInfo[i] >> j) & 1;
	    		new_bits[idx++] = bit_temp;
	    	}
	    }

	    // Set active the given bit
	    new_bits[bit_position] = 0;

	    // Convert back to bytes
	    for (i = 0; i < 7; i++) {
	    	temp_byte = 0; 
	    	
	    	for (j = 0; j < 8; j++) {
	    			temp_bits[j] = new_bits[(i*8)+j];
	    	}

	    	temp_byte = make_byte_from_bits(temp_bits);

	    	segment_byte.metaInfo[i] = temp_byte;
	    }
	} 
	else if (segment_id == 4)
	{
		// Convert meta info bytes to bits
	    for (i = 0; i < 7; i++) {
	    	// Get bits in reversed order because of Little-Endian
	    	for (j = 7; j >= 0; j--) {
	    		bit_temp = (segment_text3.metaInfo[i] >> j) & 1;
	    		new_bits[idx++] = bit_temp;
	    	}
	    }

	    // Set active the given bit
	    new_bits[bit_position] = 0;

	    // Convert back to bytes
	    for (i = 0; i < 7; i++) {
	    	temp_byte = 0; 
	    	
	    	for (j = 0; j < 8; j++) {
	    			temp_bits[j] = new_bits[(i*8)+j];
	    	}

	    	temp_byte = make_byte_from_bits(temp_bits);

	    	segment_text3.metaInfo[i] = temp_byte;
	    }
	}
}

// Function that returns bit in given position
uint8_t get_meta_bit_by_position(int segment_id, uint8_t bit_position)
{	
	uint8_t new_bits[56] = {0};

	int bit_temp;
	int idx = 0;
	int i, j;


	if (segment_id == 0)
	{
		// Convert meta info bytes to bits
	    for (i = 0; i < 7; i++) {
	    	// Get bits in reversed order because of Little-Endian
	    	for (j = 7; j >= 0; j--) {
	    		bit_temp = (segment_text1.metaInfo[i] >> j) & 1;
	    		new_bits[idx++] = bit_temp;
	    	}
	    }

	    return new_bits[bit_position];
	} else if (segment_id == 1)
	{
		// Convert meta info bytes to bits
	    for (i = 0; i < 7; i++) {
	    	// Get bits in reversed order because of Little-Endian
	    	for (j = 7; j >= 0; j--) {
	    		bit_temp = (segment_text2.metaInfo[i] >> j) & 1;
	    		new_bits[idx++] = bit_temp;
	    	}
	    }

	    return new_bits[bit_position];
	} else if (segment_id == 2)
	{
		// Convert meta info bytes to bits
	    for (i = 0; i < 7; i++) {
	    	// Get bits in reversed order because of Little-Endian
	    	for (j = 7; j >= 0; j--) {
	    		bit_temp = (segment_num.metaInfo[i] >> j) & 1;
	    		new_bits[idx++] = bit_temp;
	    	}
	    }

	    return new_bits[bit_position];
	} 
	else if (segment_id == 3)
	{
		// Convert meta info bytes to bits
	    for (i = 0; i < 7; i++) {
	    	// Get bits in reversed order because of Little-Endian
	    	for (j = 7; j >= 0; j--) {
	    		bit_temp = (segment_byte.metaInfo[i] >> j) & 1;
	    		new_bits[idx++] = bit_temp;
	    	}
	    }

	    return new_bits[bit_position];
	} 
	else if (segment_id == 4)
	{
		// Convert meta info bytes to bits
	    for (i = 0; i < 7; i++) {
	    	// Get bits in reversed order because of Little-Endian
	    	for (j = 7; j >= 0; j--) {
	    		bit_temp = (segment_text3.metaInfo[i] >> j) & 1;
	    		new_bits[idx++] = bit_temp;
	    	}
	    }

	    return new_bits[bit_position];
	}

	return 0;
}

// Returns a index for a param_names array by given param name 
int get_idx_from_param_names_by_param_name(const char * param_name) {
	int idx = -1;
	int i = 0;
	int len = sizeof(param_names) / sizeof(param_names[0]);
	for (i = 0; i < len; i++) {
		if (strcmp(param_names[i], param_name) == 0) {
			idx = i;
			break;
		}
	}

	return idx;
}


// Change parameter value in text segment -> Used when is called for ex. -s device_name "DeviceN1"
void change_param_value_of_text_segment(const char * param_name, const char * param_value) {
	// 1. Find index in (param_names, param_segments, param_position) for this param name
	int param_names_index = get_idx_from_param_names_by_param_name(param_name);

	if (param_names_index == -1)
	{
		printf("--> ERROR! Parameter %s is not found in our segments!\n", param_name);
		return;
	}

	int segment_id = param_segments[param_names_index];
	int position_id = param_positions[param_names_index];

	// 2. Check if param value is valid value for the param
	int validation_check = text_segment_options_validator(param_name, param_value);
	if (validation_check == 0)
	{
		printf("--> ERROR! Value \"%s\" for parameter \"%s\" is NOT valid!\n", param_value, param_name);
		return;
	}

	// 3. Change value of the parameter in correct segment
	switch (segment_id) 
	{
		// We check only for 0, 1 or 4, because we know that only they are text segments
		case 0:
			strcpy(segment_text1.word_options[position_id], param_value);
			break;
		case 1:
			strcpy(segment_text2.word_options[position_id], param_value);
			break;
		case 4:
			strcpy(segment_text3.word_options[position_id], param_value);
			break;
		default:
			break;
	}
}

// Change parameter value in numeric segment
void change_param_value_of_num_segment(const char * param_name, uint32_t param_value) {
	// 1. Find index in (param_names, param_segments, param_position) for this param name
	int param_names_index = get_idx_from_param_names_by_param_name(param_name);

	if (param_names_index == -1)
	{
		printf("--> ERROR! Parameter %s is not found in our segments!\n", param_name);
		return;
	}

	int position_id = param_positions[param_names_index];

	// 2. Check if param value is valid value for the param
	int validation_check = num_segment_options_validator(param_name, param_value);
	if (validation_check == 0)
	{
		printf("--> ERROR! Value \"%d\" for parameter \"%s\" is NOT valid!\n", param_value, param_name);
		return;
	}

	// 3. Change value of the parameter in correct segment
	segment_num.num_options[position_id] = param_value;
}

// Change parameter value in numeric segment
void change_param_value_of_byte_segment(const char * param_name, uint8_t param_value) {
	// 1. Find index in (param_names, param_segments, param_position) for this param name
	int param_names_index = get_idx_from_param_names_by_param_name(param_name);

	if (param_names_index == -1)
	{
		printf("--> ERROR! Parameter %s is not found in our segments!\n", param_name);
		return;
	}

	int position_id = param_positions[param_names_index];

	// 2. Check if param value is valid value for the param
	int validation_check = byte_segment_options_validator(param_name, param_value);
	if (validation_check == 0)
	{
		printf("--> ERROR! Value \"%d\" for parameter \"%s\" is NOT valid!\n", param_value, param_name);
		return;
	}

	// 3. Change value of the parameter in correct segment
	segment_byte.byte_options[position_id] = param_value;
}

// Show param value by given param name
void show_param_value_by_param_name(const char * param_name) {
	// 1. Find index in (param_names, param_segments, param_position) for this param name
	int param_names_index = get_idx_from_param_names_by_param_name(param_name);

	if (param_names_index == -1)
	{
		printf("--> ERROR! Parameter %s is not found in our segments!\n", param_name);
		return;
	}

	int segment_id = param_segments[param_names_index];
	int position_id = param_positions[param_names_index];

	// 2. Find segment in which this param is, and then get value
	switch (segment_id) 
	{
		case 0: // text segment
			printf("Value of \"%s\" is \"%s\"\n", param_name, segment_text1.word_options[position_id]);
			break;
		case 1: // text segment
			printf("Value of \"%s\" is \"%s\"\n", param_name, segment_text2.word_options[position_id]);
			break;
		case 2: // num segment
			printf("Value of \"%s\" is \"%d\"\n", param_name, segment_num.num_options[position_id]);
			break;
		case 3: // byte segment
			if (segment_byte.byte_options[position_id] > 50) {
				// print ascii char
				printf("Value of \"%s\" is \"%c\"\n", param_name, segment_byte.byte_options[position_id]);
			} else {
				// print num
				printf("Value of \"%s\" is \"%d\"\n", param_name, segment_byte.byte_options[position_id]);
			}
			break;
		case 4: // text segment
			printf("Value of \"%s\" is \"%s\"\n", param_name,  segment_text3.word_options[position_id]);
			break;
		default:
			break;
	}
}

// Show all params that is used
void show_all_params(int only_actives) {
	if (only_actives == 1) {
		// Show only these params that are active; that are used by device

	} else {
		// Just show all params
		printf("___ List of All Parameters ___\n");

		int len = sizeof(param_names) / sizeof(param_names[0]);
		int i;

		for (i = 0; i < len; ++i)
		{
			printf("%d. %s\n", (i+1), param_names[i]);
		}
	}
}

// Save configuration data to new binary file
void export_data(const char * file_name) {
	int fd_new_file = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_new_file == -1) {
		err(1, "File open problem for %s", file_name);
	}

	write(fd_new_file, &segment_text1, sizeof(segment_text1));
	write(fd_new_file, &segment_text2, sizeof(segment_text2));
	write(fd_new_file, &segment_num, sizeof(segment_num));
	write(fd_new_file, &segment_byte, sizeof(segment_byte));
	write(fd_new_file, &segment_text3, sizeof(segment_text3));

	close(fd_new_file);
}

int main(int argc, char * argv[]) {

	// Arguments validation. Actually, validation needs to be more specific.
	if (argc < 2) 
	{
		errx(1, "Error! Please, give arguments!");
	}

	// Check if is Help
	if (strcmp(argv[1], "-h") == 0) {
		// Show help. Maybe it need to be little bit nice

		printf("--> Usage example: %s conf.bin -s device_name \"DeviceN1\"\n", argv[0]);
		printf("--> Usage example: %s conf.bin -S device_name \"DeviceN1\"\n", argv[0]);
		printf("--> Usage example: %s conf.bin -g device_name\n", argv[0]);
		printf("--> Usage example: %s conf.bin -G device_name\n", argv[0]);
		printf("--> Usage example: %s conf.bin -l\n", argv[0]);
		printf("--> Usage example: %s conf.bin -L\n", argv[0]);
		printf("--> Usage example: %s conf.bin -l device_name baudrate\n", argv[0]);
		printf("--> Usage example: %s conf.bin -L device_name baudrate\n", argv[0]);
		printf("--> Usage example: %s conf.bin -b device_name 1\n", argv[0]);
		exit(0);
	}

	// ### Open binary file
    int fd = open(argv[1], O_RDONLY);

    if (fd == -1) {
    	err(1, "File open problem!");
    }

    // ### Extract segments from the file
    printf("\n***** SEGMENTS INFORMATION *****\n\n");
    extract_all_segments(fd);

    close(fd);

    // ### Operations

    // Few new lines for more readability
    printf("\n\n");
    printf("\n***** Information from actions: *****\n\n");


    if (strcmp(argv[2], "-s") == 0) {
    	// conf.bin (argv [1]) -s (argv [2]) device_name (argv [3]) "DeviceN1" (argv [4])

    	int idx = get_idx_from_param_names_by_param_name(argv[3]);

    	switch (param_segments[idx]) {
    		case 0: // text
    			change_param_value_of_text_segment(argv[3], argv[4]);
    			set_meta_bits(0, param_positions[idx]);
    			break;
    		case 1: // text
    			change_param_value_of_text_segment(argv[3], argv[4]);
    			set_meta_bits(1, param_positions[idx]);
    			break;
    		case 2: // num
    			//change_param_value_of_num_segment(argv[3], argv[4]);
	    		// set_meta_bits(2, param_positions[idx]);
    			break;
			case 3: // byte
				//change_param_value_of_byte_segment(argv[3], argv[4]);
				// set_meta_bits(3, param_positions[idx]);
				break;
    		case 4: // text
    			change_param_value_of_text_segment(argv[3], argv[4]);
    			set_meta_bits(4, param_positions[idx]);
    			break;
    		default:
    			break;
    	}

    	// Save data to file
    	export_data(argv[1]);
    } else if (strcmp(argv[2], "-S") == 0) {
    	// conf.bin (argv [1]) -S (argv [2]) device_name (argv [3]) "DeviceN1" (argv [4])

    	int idx = get_idx_from_param_names_by_param_name(argv[3]);

    	switch (param_segments[idx]) {
    		case 0: // text
    			change_param_value_of_text_segment(argv[3], argv[4]);
    			break;
    		case 1: // text
    			change_param_value_of_text_segment(argv[3], argv[4]);
    			break;
    		case 2: // num
    			//change_param_value_of_num_segment(argv[3], argv[4]);
    			break;
			case 3: // byte
				//change_param_value_of_byte_segment(argv[3], argv[4]);
				break;
    		case 4: // text
    			change_param_value_of_text_segment(argv[3], argv[4]);
    			break;
    		default:
    			break;
    	}

    	// Save data to file
    	export_data(argv[1]);
    } else if (strcmp(argv[2], "-g") == 0) {
    	// conf.bin (argv [1]) -g (argv [2]) device_name (argv [3])
    	int idx = get_idx_from_param_names_by_param_name(argv[3]);

    	if (get_meta_bit_by_position(param_segments[idx], param_positions[idx]) == 1) {
    		show_param_value_by_param_name(argv[3]);
    	}
    } else if (strcmp(argv[2], "-G") == 0) {
    	// conf.bin (argv [1]) -G (argv [2]) device_name (argv [3])
    	show_param_value_by_param_name(argv[3]);
    } else if (strcmp(argv[2], "-l") == 0 && argc == 3) {
    	// conf.bin (argv [1]) -l (argv [2])
    	show_all_params(1);
    } else if (strcmp(argv[2], "-L") == 0 && argc == 3) {
    	// conf.bin (argv [1]) -L (argv [2])
    	show_all_params(2);
    } else if (strcmp(argv[2], "-l") == 0 && argc > 3) {
    	// conf.bin -l device_name baudrate
    	int i;
    	int idx;
    	for (i = 3; i < argc; i++) {
    		idx = get_idx_from_param_names_by_param_name(argv[i]);
    		if (get_meta_bit_by_position(param_segments[idx], param_positions[idx]) == 1) {
		    	show_param_value_by_param_name(argv[i]);
		    }
    	}
    } else if (strcmp(argv[2], "-L") == 0 && argc > 3) {
    	// conf.bin -L device_name baudrate
    	int i;
    	for (i = 3; i < argc; i++) {
	    	show_param_value_by_param_name(argv[i]);
    	}
    } else if (strcmp(argv[2], "-b") == 0) {
    	// conf.bin (argv [1]) -b (argv [2]) device_name (argv [3]) 1 (argv [4])

    	int idx = get_idx_from_param_names_by_param_name(argv[3]);

    	if (strcmp(argv[4], "1") == 0) {
    		// set bit
	    	set_meta_bits(param_segments[idx], param_positions[idx]);
    	} else if (strcmp(argv[4], "0") == 0){
    		// unset bit
	    	unset_meta_bits(param_segments[idx], param_positions[idx]);
    	}

    	export_data(argv[1]);
    } else if (strcmp(argv[2], "-c") == 0) {
    	// TODO
    } else {
    	printf("No valid operation!\n");
    }

    exit(0);
}