#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>



int main() {
	int cnt = 0; //counts if it is the first command given
	struct rusage usage = {}; //used to find the cpu time and involuntary context switches
    	struct rusage last_usage = {};
    	struct timeval time = {}; 
    	struct timeval last_time = {};
	
	while (1) {
		char command[50]; //used to take the command
		char* user_arg[sizeof(command)]; //arguments to be passed to execvp()
		char* tok;
		char* cmd; //PATH environment variable
		int i = 0;
		pid_t pid, child;
		int status;
		
    		printf("Please enter a command to execute\n");
    		fgets(command, sizeof(command), stdin); //takes user input
    		if(strcmp(command, "quit\n") == 0){ //ends when user enters quit
    			break;
    		}
    		
    		tok = strtok(command, " \n"); //tokenizing string
    		cmd = tok; //PATH is first token
    		while (tok != NULL) {
    			printf ("[%s]\n", tok);
    			user_arg[i++] = tok;  //adding rest of arguments to array
    			tok =strtok(NULL, " \n");
  		}
  		
  		user_arg[i] = NULL;
    		
    		if ((pid = fork()) < 0) { //fork parent process
        		perror("Fork failure");
        		exit(1);
    		}
    		else if(pid == 0){
    			if (execvp(cmd, user_arg) < 0) { //run execvp and give error if doesn't execute
        			perror("Execution failed");
        			exit(1);
    			}
    		}
    		else{
    			child = wait(&status); //waiting for child to execute and get status
    			printf("\nChild PID %ld terminated with return status %d\n", (long) child, WEXITSTATUS(status));
    			
    			last_usage = usage; //records previous resource usage statistics
    			last_time = time;
    			getrusage(RUSAGE_CHILDREN, &usage); //used to get resource usage
    			time = usage.ru_utime; //user CPU time usage
    			if(cnt == 0){ //prints user CPU time and involuntary context switches
    				printf("User CPU Time Used (Microseconds): %ld\n",  time.tv_usec);
    				printf("Involuntary Context Switches: %ld\n",  usage.ru_nivcsw);
    				cnt++;
    			}
    			else{ //subtracts the previous resource usage stats from the current to get them for child process
    				printf("User CPU Time Used (Microseconds): %ld\n",  time.tv_usec - last_time.tv_usec);
    				printf("Involuntary Context Switches: %ld\n",  usage.ru_nivcsw - last_usage.ru_nivcsw);
    			}
    			printf("\n\n\n");
    		}
    		
	}
  return 0;
}



