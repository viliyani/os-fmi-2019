// ### Libraries

#include <stdlib.h>
// Open
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
// Stat
#include <unistd.h>
// Errors
#include <err.h>
#include <errno.h>
// Print
#include <stdio.h>
// String
#include <string.h>
// Ints
#include <stdint.h>
// Signal
#include<signal.h> 
// Time
#include <time.h>
// Shared Memory
#include <sys/ipc.h> 
#include <sys/shm.h>
// Semaphore
#include <semaphore.h>

// ### Common
char * storage_file;

struct Memory {
	sem_t    sem_server;
    sem_t    sem_client;
    sem_t    t1;
    sem_t    t2;
	short has_client; // 0 or 1 (false or true) - show if has client to the server
	short account_id; // account id from the bank
	uint32_t account_balance; // account balance for given account id
	int16_t change_balance; // sum that will be + (added) or - (removed) from the current account balance
	short status; // Info:
					/*
						0 - no client
						1 - do update account balance
						2 - error - invalid account id
						3 - error - invalid change balance
						4 - success - everything is ok
					*/
};

// ### Shared Memory 
key_t ShmKEY = 20190617; // getting the unique identifier key from directory location;
int ShmID;
struct Memory * ShmPTR;
