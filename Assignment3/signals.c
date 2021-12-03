#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

//loops child and parent process
bool running = true;

//signal handler for parent
void sigHandler(int number) {
	switch(number){
		case SIGUSR1:
			printf("received a SIGUSR1 signal\n");
			break;
		case SIGUSR2:
			printf("received a SIGUSR2 signal\n");
			break;
		case SIGINT:
			printf(" received.\n");
			running = false;
			break;
		default:
			break;
	
	}
	
}

//signal handler for child
void sigHandler2(int number) {
	switch(number){
		case SIGUSR1:
			sleep(1);
			running = false;
			break;
		default:
			break;
	
	}
	
}

int main() {
	
	pid_t who = fork();
	
	if (who == 0) {
		pid_t parent = getppid();
		srand(time(NULL)); //used to get random time for sleep and random signal
		signal(SIGUSR1, sigHandler2); //handle USR1 signal for stop request
		signal(SIGINT, SIG_IGN); //ignores ctrl-c in child
		while (running) {
			int randomSleep = (rand() %  5) + 1; //random sleep time
			int randomSignal = (rand() %  2) + 1; //random signal
			fflush(stdout);
			sleep(randomSleep);
			if(randomSignal == 1){ //sends USR1 or USR2 to parent
				kill(parent, SIGUSR1);
			}
			else{
				kill(parent, SIGUSR2);
			}
			
		}
	}
	else {
		int code;
		signal(SIGUSR1, sigHandler);
		signal(SIGUSR2, sigHandler);
		signal(SIGINT, sigHandler);
		while (running) {
			printf ("Waiting...    ");
			fflush(stdout); //prints waiting while the signal call still needs to be received
			pause(); //stops process until signal call is received
		}
		signal(SIGUSR1, SIG_IGN); //ignores any USR1 signals that were in process
		signal(SIGUSR2, SIG_IGN); //ignores any USR2 signals that were in process
		printf("That's it, I'm shutting you down...\n");
		kill(who, SIGUSR1); //sends USR1 to child so it can terminate
		wait(&code); //wait for child to terminate
		
	}
	return 0;
}
