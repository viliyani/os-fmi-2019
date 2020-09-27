// Common File
#include "comm.h"

// Remove shared memory
void remove_shared_memory() {
	// Remove shared memory
	shmctl(ShmID, IPC_RMID, NULL);
}

// Signal handling - basic version, only with SIGTERM and SIGINT
void handle_sigterm(int sig) 
{ 
	// Open file for logging interrupts of server
	int fd = open("stops_log.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);

	if (fd == -1) {
		remove_shared_memory();
		err(2, "Log file open problem!");
	}

	// Date and time
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	// Signal code
	char sig_code[10];

	if (sig == 2) {
		strncpy(sig_code, "SIGINT", 7);
		sig_code[6] = '\0';
	} else if (sig == 15) {
		strncpy(sig_code, "SIGTERM", 8);
	} else {
		strncpy(sig_code, "other", 6);
	}

	// Store info in file
	char str[] = "Server is stopped on: %d-%d-%d %d:%d:%d - by %s!\n";
	char str2[100];
	sprintf(str2,str,tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, sig_code);

	ssize_t write_size = write(fd, str2, strlen(str2));

	if (write_size <= 0) {
		const int saved_errno = errno;
    	close(fd);
		remove_shared_memory();
    	errno = saved_errno;
		err(3, "Write in log file problem!");
	}

    close(fd);

    remove_shared_memory();
}

// Get Account Balance by given index
uint32_t get_account_balance(short index) {
	// Open file with bank account
	int fd = open(storage_file, O_RDONLY, 0644);

	if (fd == -1) {
		remove_shared_memory();
		errx(1, "Something went wrong!");
	}

	uint32_t accounts[8];

	ssize_t read_size = read(fd, accounts, sizeof(accounts));

	if (read_size <= 0) {
		const int saved_errno = errno;
		close(fd);
		remove_shared_memory();
		errno = saved_errno;
		err(1, "Something went wrong!");
	}

	close(fd);

	return accounts[index];
}

// Change Account Balance by given index and value that will be added (+x or -x)
uint32_t change_account_balance(short index, int16_t change_balance) {
	// Open file with bank account
	int fd = open(storage_file, O_RDWR, 0644);

	if (fd == -1) {
		remove_shared_memory();
		errx(1, "Something went wrong 1!");
	}

	uint32_t accounts[8];

	ssize_t read_size = read(fd, accounts, sizeof(accounts));

	if (read_size <= 0) {
		const int saved_errno = errno;
		close(fd);
		remove_shared_memory();
		errno = saved_errno;
		errx(1, "Something went wrong 2!");
	}

	accounts[index] += change_balance;

	// Update bank
	lseek(fd, 0, SEEK_SET);

	ssize_t write_size = write(fd, &accounts, sizeof(accounts));

	if (write_size <= 0) {
		const int saved_errno = errno;
		close(fd);
		remove_shared_memory();
		errno = saved_errno;
		err(1, "Something went wrong 3!");
	}

	close(fd);

	return accounts[index];
}

int main(int argc, char * argv[]) {
	// Validation
	if (argc != 2) {
		errx(1, "Give 1 argument!");
	}

	// Check if file exists
	if( access( argv[1], F_OK ) == -1 ) {
	    // file doesn't exist => create it!

		uint32_t empty_accounts[8] = {0, 0, 0, 0, 0, 0, 0, 0};

	    int tfd = open(argv[1], O_CREAT | O_WRONLY, 0644);

	    if (tfd == -1) {
			errx(1, "File create problem!");
		}

	    ssize_t write_size = write(tfd, &empty_accounts, sizeof(empty_accounts));

	    if (write_size < 0)
	    {
	    	const int saved_errno = errno;
	    	close(tfd);
	    	errno = saved_errno;
	    	err(1, "Write in new file problem!");
	    }

	    close(tfd);
	}

	// Set bank account file
	storage_file = argv[1];

	// Signal Handler
	signal(SIGTERM, handle_sigterm); 
	signal(SIGINT, handle_sigterm);

	// ### Shared Memory
	ShmID = shmget(ShmKEY, sizeof(struct Memory), IPC_CREAT | 0600);
	if (ShmID < 0) {
		err(1, "*** shmget error (server) ***");
	}

	ShmPTR = (struct Memory *) shmat(ShmID, NULL, 0);
	if ( ShmPTR == NULL) {
		err(1, "*** shmat error (server) ***");
	}

	// Default memory values
	ShmPTR->has_client = 0;
	ShmPTR->account_id = 0;
	ShmPTR->account_balance = 0;
	ShmPTR->change_balance = 0;
	ShmPTR->status = 0;

	// ### Semaphore Initialization
	sem_init(&ShmPTR->sem_client, !0, 1);
	sem_init(&ShmPTR->sem_server, !0, 0);
	sem_init(&ShmPTR->t1, !0, 0);
	sem_init(&ShmPTR->t2, !0, 0);

	// ### Loop 
	short do_loop = 1;

	while (do_loop == 1) {
		// sleep(1);

		sem_wait(&ShmPTR->sem_server);

		if (ShmPTR->has_client == 1) {
			// Validate account id
			if (!(ShmPTR->account_id >= 0 && ShmPTR->account_id <= 7)) {
				sem_post(&ShmPTR->t1);
				ShmPTR->status = 2; // error with account id
			} else {

				ShmPTR->account_balance = get_account_balance(ShmPTR->account_id);

				sem_post(&ShmPTR->t1);
				sem_wait(&ShmPTR->t2);

				if (ShmPTR->status == 1) {
					// Check if change balance is valid
					if (ShmPTR->change_balance < 0) {
						if (((uint32_t) (-1)*ShmPTR->change_balance) > ShmPTR->account_balance) {
							sem_post(&ShmPTR->t1);
							ShmPTR->status = 3; // error with change balance
						} else {
							change_account_balance(ShmPTR->account_id, ShmPTR->change_balance);
							ShmPTR->status = 4; // success
							sem_post(&ShmPTR->t1);
						}
					} else {
						change_account_balance(ShmPTR->account_id, ShmPTR->change_balance);
						ShmPTR->status = 4; // success
						sem_post(&ShmPTR->t1);
					}
				}
			}

			
		}
		
		sem_post(&ShmPTR->sem_client);
	}

	// Actually, here never will be executed, because of infinite loop

	// Detach shared memory
    shmdt((void *) ShmPTR);

    // Remove shared memory
    remove_shared_memory();

	exit(0);
}