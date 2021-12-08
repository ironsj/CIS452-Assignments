#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#define MAX_BUF_LEN 1024

int main(int argc, char *argv[])
{

	DIR *dirPtr;
	struct dirent *entryPtr;
	struct stat statBuf;
	int iIn = 0;
	int nIn = 0;
	int sIn = 0;
	char *path = malloc(MAX_BUF_LEN * sizeof(char));
	
	for(int i = 0; i < argc; i++){
		if(strcmp(argv[i], "-i") == 0){
			iIn = 1;
		}
		else if(strcmp(argv[i], "-n") == 0){
			nIn = 1;
		}
		else if(strcmp(argv[i], "-s") == 0){
			sIn = 1;
		}
	}
	
	if(argc == 2){
		strcpy(path, argv[1]);
	}
	else if(argc == 3){
		strcpy(path, argv[2]);
	}
	else if(argc == 4){
		strcpy(path, argv[3]);
	}
	else if(argc == 5){
		strcpy(path, argv[4]);
	}
	
	if ((dirPtr = opendir (path)) == NULL) {
        	perror ("Cannot open .");
        	exit (1);
   	}
	
	if(sIn == 0 && nIn == 0 && iIn == 0){
		while ((entryPtr = readdir(dirPtr))){
			if(strcmp(entryPtr->d_name, ".") != 0 && strcmp(entryPtr->d_name, "..")){
				printf("%s\n", entryPtr->d_name);
				//stat(entryPtr->d_name, &statBuf);
				//printf("size in bytes is: %ld\n", statBuf.st_size);
				//printf("\n");
			}
		}
	
	}
	else if(sIn == 0 && nIn == 0 && iIn ==1){
		while ((entryPtr = readdir(dirPtr))){
			if(strcmp(entryPtr->d_name, ".") != 0 && strcmp(entryPtr->d_name, "..")){
				printf("%ld %s\n", entryPtr->d_ino, entryPtr->d_name);
			}
		}
	}
	else if(sIn == 1 && nIn == 0 && iIn ==0){
		int sum = 0;
		while ((entryPtr = readdir(dirPtr))){
			if(strcmp(entryPtr->d_name, ".") != 0 && strcmp(entryPtr->d_name, "..")){
				stat(entryPtr->d_name, &statBuf);
				sum += statBuf.st_blocks;
				printf("%-5ld %-5ld %s\n", statBuf.st_size, (statBuf.st_blocks / 2), entryPtr->d_name);
			}
		}
		printf("total %d\n", sum / 2);
	}
	else if(sIn == 0 && nIn == 1 && iIn ==0){
		while ((entryPtr = readdir(dirPtr))){
			if(strcmp(entryPtr->d_name, ".") != 0 && strcmp(entryPtr->d_name, "..")){
				stat(entryPtr->d_name, &statBuf);
				printf("%d %d %s\n", statBuf.st_uid, statBuf.st_gid,entryPtr->d_name);
			}
		}
	}
	else if(sIn == 1 && nIn == 1 && iIn ==0){
		int sum = 0;
		while ((entryPtr = readdir(dirPtr))){
			if(strcmp(entryPtr->d_name, ".") != 0 && strcmp(entryPtr->d_name, "..")){
				stat(entryPtr->d_name, &statBuf);
				sum += statBuf.st_blocks;
				printf("%-5ld %-5ld %d %d %s\n", statBuf.st_size, (statBuf.st_blocks / 2), statBuf.st_uid, statBuf.st_gid,entryPtr->d_name);
			}
		}
		printf("total %d\n", sum / 2);	
	}
	else if(sIn == 1 && nIn == 0 && iIn ==1){
		int sum = 0;
		while ((entryPtr = readdir(dirPtr))){
			if(strcmp(entryPtr->d_name, ".") != 0 && strcmp(entryPtr->d_name, "..")){
				stat(entryPtr->d_name, &statBuf);
				sum += statBuf.st_blocks;
				printf("%ld %-5ld %-5ld %s\n", entryPtr->d_ino, statBuf.st_size, (statBuf.st_blocks / 2), entryPtr->d_name);
			}
		}
		printf("total %d\n", sum / 2);		
	}
	else if(sIn == 0 && nIn == 1 && iIn ==1){
		while ((entryPtr = readdir(dirPtr))){
			if(strcmp(entryPtr->d_name, ".") != 0 && strcmp(entryPtr->d_name, "..")){
				stat(entryPtr->d_name, &statBuf);
				printf("%ld %d %d %s\n", entryPtr->d_ino, statBuf.st_uid, statBuf.st_gid,entryPtr->d_name);
			}
		}	
	}
	else if(sIn == 1 && nIn == 1 && iIn ==1){
		int sum = 0;
		while ((entryPtr = readdir(dirPtr))){
			if(strcmp(entryPtr->d_name, ".") != 0 && strcmp(entryPtr->d_name, "..")){
				stat(entryPtr->d_name, &statBuf);
				sum += statBuf.st_blocks;
				printf("%ld %-5ld %-5ld %d %d %s\n", entryPtr->d_ino, statBuf.st_size, (statBuf.st_blocks / 2), statBuf.st_uid, statBuf.st_gid, entryPtr->d_name);
			}
		}
		printf("total %d\n", sum / 2);	
	}
	printf("\n");
	
	

	
	closedir(dirPtr);
	free(path);
	return 0;
}
