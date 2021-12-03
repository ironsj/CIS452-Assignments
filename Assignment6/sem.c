#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <unistd.h>

sem_t mtx; //only one thread can access buffer
sem_t fillBin; //tells consumer when bin has been filled
sem_t emptyBin; //tells producer when a bin has been emptied
void* producer(void*); 
void* consumer(void*);
bool running = true; //true while running
char ch;
int sleepTimePro; //sleep time of producer
int sleepTimeCon; //sleep time of consumer
int *bin; //holds the integers
int binSize; //size of the bin
bool done = false;


int main(int argc, char* argv[]) {
	
	pthread_t pro, con; //producer and consumer threads
	sem_init(&mtx, 0, 1); //creates semaphore to access bin
	sem_init(&emptyBin, 0, atoi(argv[1])); //creates semaphore indicating when a bin is empty
	sem_init(&fillBin, 0, 0); //creates sempaphore indicating when a bin has been filled
	binSize = atoi(argv[1]); //sets bin size to user specification
	bin = malloc( binSize * sizeof(int)); //allocate memory for bin
	
	sleepTimePro = atoi(argv[2]) * 1000; //sets producer sleep time to specification
	sleepTimeCon = atoi(argv[3]) * 1000; //sets consumer sleep time to specification
	pthread_create(&pro, NULL, producer, &sleepTimePro); //creates producer thread
	pthread_create(&con, NULL, consumer, &sleepTimeCon); //creates consumer thread
	
	while(running){ //takes user input in order to change sleep times and quit when needed
		scanf("%c", &ch);
		if(ch == 'q'){
			printf("Preparing to quit\n");
			running = false;
		}
		else if(ch == 'a'){
			sleepTimePro += 250000;
		}
		else if(ch == 'z'){
			sleepTimePro -= 250000;
		}
		else if(ch == 's'){
			sleepTimeCon += 250000;
		}
		else if(ch == 'x'){
			sleepTimeCon -= 250000;
		}
	}
	
	int conJoin, proJoin; 
    	pthread_join(con, (void **) &conJoin); //waits for consumer thread to terminate
    	pthread_join(pro, (void **) &proJoin); //waits for producer thread to terminate
    	free(bin); //frees memory allocated to bin
    	sem_destroy(&mtx); //destroys semaphore
    	sem_destroy(&fillBin); //destroys semaphore
    	sem_destroy(&emptyBin); //destroys semaphore
	printf("End of main\n");
	return 0;
}

void* producer(void *arg) {
    	int *num = (int *) arg; //points to producer sleep time
    	srand(getpid());
    	int random; //hold random integer 
    	int in = 0; //bin number
	while (running) {
		random = (rand() % 8001) + 1000; //random int 1000- 9000
		sem_wait(&emptyBin); //waits for bin to be empty
		sem_wait(&mtx); //waits until able to access bin
		bin[in] = random; //fills bin
		printf("Put %d into bin %d\n", random, in);
		in++; //increment bin
		in %= binSize; //sets bin number to 0 when goes over bin size
		//fflush(stdout);
		sem_post(&mtx); //makes bin accessable again
		sem_post(&fillBin); //tells consumer a bin has been filled
		usleep(*num);
	}
	sem_post(&mtx); //increment semaphore in case consumer is waiting on exit
	sem_post(&fillBin); //increment semaphore in case consumer is waiting on exit
    	printf("End of producer\n");
    	pthread_exit(NULL);
}

void* consumer(void *arg) {
	int *num = (int *) arg; //points to consumer sleep time
	int out = 0; //bin number
	int result; //set to whatever was in the bin
	while (1) {
		if(!running && bin[out] == 0){ //breaks from loop in the case the consumer was still consuming after quit was called
			break;
		}
		sem_wait(&fillBin); //waits for a bin to be filled
		sem_wait(&mtx); //waits until it is able to access the bin
		if(!running && bin[out] == 0){ //breaks from the loop in the case the consumer was waiting when quit was called
			break;
		}
		result = bin[out]; //result is whatever is in the bin
		bin[out] = 0; //empty the bin
		printf("\tGet %d from bin %d\n", result, out);
		out++;
		out %= binSize;
		sem_post(&mtx); //allow access to bins again
		sem_post(&emptyBin); //indicate a bin has been emptied
		
		//fflush(stdout);
		usleep(*num);
	}
    	printf("\tEnd of consumer\n");
	pthread_exit(NULL);
}


