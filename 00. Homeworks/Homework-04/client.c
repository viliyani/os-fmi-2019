// Common File
#include "comm.h"

int main(int argc, char * argv[]) {

	// Validation
	if (argc != 2) {
		errx(1, "Please, give account - from A to H");
	}

	if (strlen(argv[1]) != 1) {
		errx(1, "Error! Account need to be only 1 letter! You give an account that is not only 1 letter!");
	}

	// Check account 
	int accIdx = -1;

	switch (argv[1][0]) {
		case 'A':
			accIdx = 0;
			break;
		case 'B':
			accIdx = 1;
			break;
		case 'C':
			accIdx = 2;
			break;
		case 'D':
			accIdx = 3;
			break;
		case 'E':
			accIdx = 4;
			break;
		case 'F':
			accIdx = 5;
			break;
		case 'G':
			accIdx = 6;
			break;
		case 'H':
			accIdx = 7;
			break;
		default:
			errx(2, "The given account (%s) is not valid! It can be from A-H!", argv[1]);
			break;
	}

	// Open file with bank account
	// int fd = open(storage_file, O_RDONLY, 0644);

	// if (fd == -1) {
	// 	err(1, "Something went wrong!");
	// }

	// uint32_t accounts[8];

	// ssize_t read_size = read(fd, accounts, sizeof(accounts));

	// if (read_size <= 0) {
	// 	err(1, "Something went wrong!");
	// }

	//for (int i = 0; i < 8; ++i)	{
		// printf("Account [%d] --> Balance: %d \n", (accIdx), accounts[accIdx]);
	//}

	// printf("Hello from: %s\n", "Client!");

	// printf("Acc index is: %d --> %s\n", accIdx, argv[1]);

	// close(fd);

	// Shared Memory
	ShmID = shmget(ShmKEY, sizeof(struct Memory), 0666);
    if (ShmID < 0) {
          printf("*** shmget error (client) ***\n");
        exit(1);
    }
    // printf("Client has received a shared memory...\n");

    ShmPTR = (struct Memory *) shmat(ShmID, NULL, 0);
    if (ShmPTR == NULL) {
        printf("*** shmat error (client) ***\n");
        exit(1);
    }
    // printf("Client has attached the shared memory to it's virtual memory space...\n");


    // Semaphore actions
    // sem_wait(&ShmPTR->sem_client);

    // sleep(2);
    	sem_wait(&ShmPTR->sem_client);
    
    ShmPTR->has_client = 1;
    ShmPTR->account_id = accIdx;

	sem_post(&ShmPTR->sem_server);




	sem_wait(&ShmPTR->t1);

	// If there is error with invalid account id, so exit the program
	if (ShmPTR->status == 2) {
		// Detach shared memory
    	shmdt((void *) ShmPTR);

		// Error -> Invalid account id
    	errx(2, "Invalid accound id! It can be only A-H.");
	}

	// printf("Account Balance is: %d\n", ShmPTR->account_balance);
	printf("%d\n", ShmPTR->account_balance);

	ShmPTR->status = 4;

	// printf("- Change Balance (enter amount +X or -X)\n");
	int16_t temp_change_balance;
	// int16_t check_new_balance;
	int sres = scanf("%hd", &temp_change_balance);
	if (sres == -1) {
		errx(1, "Scanf problem!");
	}

	// check_new_balance = ShmPTR->account_balance + temp_change_balance;
	// printf("New balance: %d\n", check_new_balance);


	// if (check_new_balance < 0) {
	// 	ShmPTR->status = 0;
	// 	printf("You enter imposible amount!\n");
	// } else {
		ShmPTR->change_balance = temp_change_balance;
		ShmPTR->status = 1;
	// }
		// printf("--hey 1\n");

	sem_post(&ShmPTR->t2);

	sem_wait(&ShmPTR->t1);
		// printf("--hey 2\n");

	// If there is error with change balance, so exit the program
	if (ShmPTR->status == 3) {
		// Detach shared memory
    	shmdt((void *) ShmPTR);

		// Error -> Invalid change balance
    	errx(3, "Invalid change balance! You try to get more that it has.");
	}

	// If success
	if (ShmPTR->status == 4) {
		// printf("***** SUCCESS ***** Good job!\n");
	}

	// printf("Result status: %d\n", ShmPTR->status);

	ShmPTR->has_client = 0;


	// Detach shared memory
    shmdt((void *) ShmPTR);
    exit(0);
}