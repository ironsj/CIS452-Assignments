#define _GNU_SOURCE
#include <sched.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

#define CHILDREN 10
#define LOOP 500000000

int main(){
	pid_t pids[CHILDREN];
	for (int i = 0; i < CHILDREN; ++i) {
  		if ((pids[i] = fork()) < 0) {
    			perror("fork");
    			abort();
  		} 
  		else if (pids[i] == 0) {
  			long sum = 0;
    			for (long j = 0; j < LOOP * 2; j++) {
        			sum += j;
    			}
    			printf("done\n");
    			return 0;
  		}
  	}
  	
  	struct sched_param p;
	p.sched_priority = 0;
	int change = sched_setscheduler(getpid(), SCHED_IDLE, &p);
	if (change == -1) {
  		perror("sched_setscheduler failed\n");
  		return 1;
	} 
  	
  	long sum = 0;
	for (long j = 0; j < LOOP; j++) {
		sum += j;
	}
	printf("parent done\n");
	
	/*int policy = sched_getscheduler(getpid());
	switch(policy){
    		case SCHED_OTHER: 
    			printf("SCHED_OTHER\n"); 
    			break;
    		case SCHED_IDLE:   
    			printf("SCHED_IDLE\n"); 
    			break;
    		default:   
    			printf("Something else...\n");
  	}*/
	return 0;
}
