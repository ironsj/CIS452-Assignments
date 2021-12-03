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

//holds shared data in memory
struct data {
  	int user[2];
  	char input[FOO];
};

int main(int argc, char *argv[]){
	int shmId;
	key_t key = ftok("writer", 'J');
	struct data *shared;
	
	if (key == -1) {
		perror("couldn't get key");
		exit(1);
	}
	
	if ((shmId = shmget (key, 0, 0)) < 0) {
        	perror ("could not find shared memory\n");
        	exit (1);
    	}
    	
    	if ((shared = shmat (shmId, NULL, 0)) == (void *) -1) {
        	perror ("can't find address\n");
        	exit (1);
    	}
    	
    	//allows the writer to access shared memory first
    	shared -> user[READER] = 0;
    	
    	while(1){
    		//blocks readers from accessing shared memory 
    		while(shared->user[WRITER] > 0);
    		
    		//usleeps fix synchonization combo
    		usleep(100);
    		shared->user[READER]++;
    		
    		//can not continue until both readers have incremented
    		while(shared->user[READER] != 2);
    		
    		//detaches from shared memory if writer puts quit in shared memory
    		if(strcmp(shared->input, "quit") == 0){
    			if (shmdt (shared) < 0) {
        			perror ("just can't let go\n");
        			exit (1);
    			}
    			printf("Reader #%d is shutting down\n", atoi(argv[1]));
    			break;
    		}
    		
    		//prints what the writer put into shared memory
    		printf("I, reader #%d, have read '%s' from the shared memory\n", atoi(argv[1]), shared->input);
    		
    		//printf("Reader:%i  Writer:%i\n", shared->user[READER], shared->user[WRITER]);
    		
    		usleep(100);
    		//decrement 
    		shared->user[READER]--;
    		
    		//blocks reader from pre-emptively allowing the writer to access shared memory before others finish
    		while(shared->user[READER] != 0);
    		shared->user[WRITER] = 1;
    		usleep(100);
    		
    	}
    	
    	return 0;
}
