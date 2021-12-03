#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define FOO 4096
#define WRITER 1
#define READER 0

//holds the data in the shared memory
struct data {
  		int user[2];
  		char input[FOO];
};

int main(){
	int shmId;
	key_t key = ftok("writer", 'J');
	//pointer to the shared memory
	struct data *shared;
	
	
	
	if (key == -1) {
		perror("couldn't create key");
		exit(1);
	}
	
	if ((shmId = shmget (key, sizeof(struct data), IPC_CREAT | S_IRUSR | S_IWUSR)) < 0) {
        	perror ("shared memory not created\n");
        	exit (1);
    	}
    	
    	if ((shared = shmat (shmId, NULL, 0)) == (void *) -1) {
        	perror ("can't attach\n");
        	exit (1);
    	}
    	
    	//makes it so the writer writes first
    	shared -> user[WRITER] = 1;
    	
    	while(1){
    		//writer can not write when the readers are accessing the shared memory
    		while((shared->user[READER] > 0) && (shared->user[WRITER] == 0));
    		
    		printf("Input a string\n");
    		//puts user input into shared memory
    		scanf("%s", shared->input);
    		
    		
    		//printf("Writer:%i  Reader:%i\n", shared->user[WRITER], shared->user[READER]);
    		
    		//signals to the readers that the writer is done accessing shared memory
    		shared -> user[WRITER] = 0;
    		//if user inputs quit detaches process from shared memory
    		if(strcmp(shared->input, "quit\0") == 0){
    			if (shmdt (shared) < 0) {
        			perror ("just can't let go\n");
        			exit (1);
    			}
    			printf("Writer is shutting down\n");
    			sleep(1);
    			//deallocates shared memory
    			if (shmctl (shmId, IPC_RMID, 0) < 0) {
        			perror ("can't deallocate\n");
        			exit (1);
    			}
    			break;
    		}
    		
    	
    	}
	return 0;
}
