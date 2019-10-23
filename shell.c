#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>
#define MAX_LINE		80 /* 80 chars per line, per command */
#define SH_SIZE 		1024 /*Shared memory size*/
void init_shell(int init){

	 if (init == 0)
    {
        system("clear");
        init = 1;
    }
	printf("--SHELL--215246473>> ");
}

void exec(char * myfifo){
	char cnfrm;
	int fd;
	fd = open(myfifo, O_RDWR);
	printf("....working on request...\n");
		wait(NULL);
		
		printf("....output is ready. Display it now [Y/N]>> ");
		scanf("%c", &cnfrm);

		if (cnfrm == 'y' || cnfrm == 'Y')
		{
			
			char str[200];
			read(fd, str, 200);
			printf("%s\n", str);
			
		}

		close(fd);
}

int file_exists(const char * input){
	
	struct stat buffer;
	int exist = stat(input, &buffer);
	if(exist){
		printf(" Yes : 1");
		return 1;
		}
	else{	
		//printf("Not exist : 0");
		return 0;
	}
}
int main()
{
	pid_t pid;
	char input[80] ="";
	char command[]="/bin/";
	int  size;
   	char * myfifo = "/tmp/myfifo";
	remove(myfifo);
	mkfifo(myfifo, 0666);
	int init = 0;
	init_shell(init);
	//fflush(stdout);
    fgets(input,100,stdin);
	size = strlen(input);
	input[size-1]=' ';
	int shm_id;
	key_t k;
	char *shm;
    char *shmptr;
    char *str;
    k = ftok("shmfile", 65);

    shm_id = shmget(k, SH_SIZE, IPC_CREAT | 0666);
    if(shm_id < 0 ){
        perror("shmget");
        exit(1);
    }
        shm = shmat(shm_id, NULL, 0);
         if(shm_id == -1){
             perror("shmat");
            exit(1);
         }
	    //save in named pipe
		strncat(command, input, 20);
		char * cmd = command;
		//int status = file_exists(command);
		//printf("stat: %d", status);
		strncat(command, "> ", 2);
		strncat(command, myfifo, 12);
		
		pid = fork();
		if (pid < 0) { //error occurred 
			fprintf(stderr, "Fork Failed");
			return 1;
		}
		else if (pid == 0) { // child process 
            // FILE *fp = popen(cmd, "r");
            // if (fp == NULL) {
            // printf("...Sorry Failed to run command...\n");
            // exit(1);
            // }
			// shm=shmat(shm_id,NULL,0);
			// printf( "what : %s", cmd);
			// system("ls");
			// shmdt(shm);
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
		}
		else { // parent process 
         FILE *fp = popen(cmd, "r");
            if (fp == NULL) {
                 printf("...Sorry Failed to run command...\n");
            exit(1);
            }

			exec(myfifo);
			
		}


return 0;
}