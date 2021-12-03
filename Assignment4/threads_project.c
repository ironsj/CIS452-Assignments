#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>

void *worker(void *arg);
bool running = true;
int serviceCount = 0;


void sigHandler(int number) {
	fclose(stdin);
	running = false;
}


int main(){
	srand(getpid());
	pthread_t thread1;
	int status;
	signal(SIGINT, sigHandler);
	
	while(running){
		char *file = (char*) malloc(50*sizeof(char));
		printf("Enter a file name: \n");
		
		if(scanf("%s", file) == EOF){
	    		pthread_kill(thread1, SIGINT);
	    		free(file);
		}
		if(running == true){
			if ((status = pthread_create(&thread1, NULL, worker, file)) != 0) {
		    		fprintf(stderr, "thread create error %d: %s\n", status, strerror(status));
		    		exit(1);
	    		}
	    		
	    		if((status = pthread_detach (thread1)) != 0){
	    			fprintf(stderr, "thread detach error %d: %s\n", status, strerror(status));
		    		exit(1);
	    		}
	    	}
    		
		
	}
	printf("\nThere were %d files received\n", serviceCount);
	sleep(10); 
	return 0;
}

void *worker(void *arg){
	char *acceptedFile = (char *) arg;
	long val = rand() % 10;
	long time = (rand() % 4) + 7;
	
	if(val >= 0 && val <= 7){
		sleep(1);
	}
	else{
		sleep(time);
	}
	if(running == true){
		printf("The file %s has been accessed.\n", acceptedFile);
		serviceCount++;
	}
	free(acceptedFile);
	pthread_exit(NULL);
}
