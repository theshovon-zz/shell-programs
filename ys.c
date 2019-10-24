#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <errno.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>
#define MAXCHAR 100 // max number of characters for a command
#define MAXARGS 10 // max number of commands
#define SMEM "/smfile" // PATH to named pipe
#define SIZE 4096	// size of shared memory object

// This escape sequence clears the screen
#define clear() printf("\033[H\033[J")

//shell tag displaying my student number
void printShellID() {
    printf("SHELL_215250343 -->  ");
}

void child(char *input, char *ptr, int shmfd) {
    /* 
    Execute command here** and write the result in PIPE
    popen executes shell command, works by forking a shell
    and creating a pipe to capture its output.
    execvp works too but it straight away executes the command 
    and gives us no opportunity to capture the output
    */
	FILE *fp = popen(input, "r");
        if (fp == NULL) {
            printf("...Sorry Failed to run command...\n");
            exit(1);
        }
	//*****************************//
	
	int c = getc(fp);
	while(c != EOF) {
		//putchar(c);
		int next = getc(fp);
            // not necessary but to be prcise with actual format
           	if (strcmp(input, "ls") == 0 || strcmp(input, "ls -a") == 0) {
               	if(c == '\n' && next != EOF) {
                   	c = ' ';
               	}
           	}
		char arr[] = {c};
		sprintf(ptr, "%s", arr);
		ptr += 1;
		c = next;
	}
	sprintf(ptr, "\0");
	ptr += 1;	
	
	// unmap and close childs sm access
	munmap (ptr, SIZE);
    close (shmfd);
	
	//****************************//
   	pclose(fp);
	exit(0);
}

void parent(pid_t pid, char *ptr, int shmfd) {
	// First diplays this message and then waits for the status of child
    // if child terminated successfully, reads output from PIPE if asked by user
	int status;
    printf("...working on request...\n");
    while(pid != wait(&status));
	//****************************//
	char c;
	if (status == 0) {
		printf("...output is ready. Display it [Y/N] >> ");
		c = getchar();
		if (c == 'Y' || c == 'y') {
			printf("%s", ptr);
		}
		else {
			printf("...ok no output shown...\n");
		}
	}
	else {
		printf("...sorry could not execute command. try again!...\n");
	}
	
	// unmap and close and delete the sm object
	munmap (ptr, SIZE);
	close (shmfd);
	shm_unlink (SMEM);
	//****************************//	
}

void execute(char * input) {
    int shmfd;
    char *shmptr;
    char *str;
    pid_t pid;
    int status;
    
    if((shmfd = shm_open (SMEM, O_CREAT | O_RDWR, 0666)) == -1) {
		printf("could not open sm\n");
		exit(1);
	}
	
    if ((ftruncate(shmfd, SIZE)) == -1) {
		printf("could not adjust size\n");
		exit(1);
    }
	
    if ((shmptr = (char *) mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0)) == -1) {
		printf("could not map\n");
		exit(1);
    }

    // fork into a child process
    pid = fork();

    if (pid < 0) {
        printf("Error : Could not create the child process\n");
        exit(1);
    } 
    else if (pid == 0) {
    	// is a child process
		child(input, shmptr, shmfd);
    }
    else {
    	// is a parent process
		parent(pid, shmptr, shmfd);
    }

}

int main() {
    char inputString[MAXCHAR], *command[MAXARGS];
    //clear();
	int c = 1;
    while(1) {
		if (c != 0) {
        	printShellID();
		}
        gets(inputString);
        // try again if no empty input given 
        if(strcmp(inputString, "\0") == 0) {
            //printf("...No command provided...\n");
			c = 0;
            continue;
        }
		c = 1;
        if (strcmp(inputString, "exit") == 0 || strcmp(inputString, "q") == 0) {
            printf("...exiting now...\n");
            break;
        }
	
        // execute the command
		execute(inputString);
    }
    return 0;
}
