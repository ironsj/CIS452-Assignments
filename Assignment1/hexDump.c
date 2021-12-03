#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#define OFFSET 16

/* 
HOW TO RUN: COMPILE SOURCE CODE AS YOU
NORMALLY WOULD. THEN TO RUN PROGRAM 
TYPE IN: 
	./a.out [flag] [name-of-file-to-be-hexdumped]
AN EXAMPLE WOULD BE:
	./a.out -C test.txt
IF NO FLAG IS INCLUDED IT WILL RUN LIKE HEXDUMP WITHOUT A FLAG.
IF THE '-b' FLAG IS USED IT WILL GIVE YOU THE ONE BYTE OCTAL DUMP.
IF THE '-c' FLAG IS USED IT WILL GIVE YOU THE HEX+ASCII
*/

void hexDump(char *fileName){
	int file;
	int address = 0;
	int bytesRead = 0;
	char buf[OFFSET] = "";
	
	file = open(fileName,O_RDONLY); //opening file to be read
	if(file == -1){ //exit program if file does not exist
		perror("This file does not exist. Try again with existing file");
		exit(0);
	}
      	
      	int finalBytes = 0;
      	while((bytesRead = read(file, buf, OFFSET)) > 0){
      		finalBytes = 0;
      		printf("%07x ", address); //printing byte offset in hex
      		address += OFFSET;
      		for (int i = 0; i < OFFSET; i = i + 2){ //printing hex values
      			if(i >= bytesRead){
      				printf(" ");
      			}
      			else if(i + 1 >= bytesRead){ //handles when there is odd number of bytes
      				printf("00");
      				printf("%02x", (unsigned char)buf[i]);
      				finalBytes += 1;
      			}
      			else{
                		printf("%02x", (unsigned char)buf[i+1]);
                		printf("%02x", (unsigned char)buf[i]);
                		printf(" ");
                		finalBytes += 2;
                	}
                }
                printf("\n");
      	}
      	printf("%07x \n", (address - 16 + finalBytes)); //prints total bytes
      	close(file); //closes file
}

void hexDumpC(char *fileName){
	int file;
	int address = 0;
	int bytesRead = 0;
	char buf[OFFSET] = "";
	
	file = open(fileName,O_RDONLY);
	if(file == -1){
		perror("This file does not exist. Try again with existing file");
		exit(0);
	}
      	
      	int finalBytes = 0;
      	int count = 0;
      	while((bytesRead = read(file, buf, OFFSET)) > 0){
      		finalBytes = 0;
      		printf("%08x ", address);
      		printf(" ");
      		address += OFFSET;
      		for (int i = 0; i < OFFSET; i++){
      			if(i >= bytesRead){
      				printf("   ");
      				count += 1;
      			}
      			else{
      				printf("%02x", (unsigned char)buf[i]);
      				printf(" ");
      				finalBytes += 1;
      				count += 1;
      			}
      			if(count == 8){ //adds extra space between first 8 and last 8 bytes
      				printf(" ");
      				count = 0;
      			}
                }
                printf("|");
                for (int j = 0; j < OFFSET; j++){ //prints characters after single byte hex
                	if(j >= bytesRead){
                		break;
                	}
                	if(isprint(buf[j]) != 0){
                		printf("%c", buf[j]);
                	}
                	else{
                		printf(".");
                	}
                }
                printf("|");
                
                printf("\n");
      	}
      	printf("%08x \n", (address - 16 + finalBytes));
      	close(file);
}

void hexDumpB(char *fileName){
	int file;
	int address = 0;
	int bytesRead = 0;
	char buf[OFFSET] = "";
	
	file = open(fileName,O_RDONLY);
	if(file == -1){
		perror("This file does not exist. Try again with existing file");
		exit(0);
	}
      	
      	int finalBytes = 0;
      	while((bytesRead = read(file, buf, OFFSET)) > 0){
      		finalBytes = 0;
      		printf("%07x ", address);
      		address += OFFSET;
      		for (int i = 0; i < OFFSET; i++){ //print single byte octal
      			if(i >= bytesRead){
      				printf(" ");
      			}
      			else{
      				printf("%03o", (unsigned char)buf[i]); 
      				printf(" ");
      				finalBytes += 1;
      			}
                }
                printf("\n");
      	}
      	printf("%07x \n", (address - 16 + finalBytes));
      	close(file);
}



int main(int argc, char **argv){
	if(argc == 2){
		hexDump(argv[1]);
	}
	
	if(argc == 3){
		if(strcmp(argv[1], "-b") == 0){
			hexDumpB(argv[2]);
		}
		else if(strcmp(argv[1], "-C") == 0){
			hexDumpC(argv[2]);
		}
	}
	return 0;
}
