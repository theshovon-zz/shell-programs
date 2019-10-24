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
#define SH_SIZE 		1024 /*Shared memory size*/
void init_shell(int init){

	 if (init == 0)
    {
        system("clear");
        init = 1;
    }
	printf("--SHELL--215246473>> ");
}

void exect(FILE *fp, char *shmptr ){
	char cnfrm;
	printf("....working on request...\n");
	wait(NULL);
		
	printf("....output is ready. Display it now [Y/N]>> ");
	scanf("%c", &cnfrm);

	if (cnfrm == 'y' || cnfrm == 'Y')
	{		
  		printf("%s", shmptr);		
	}
}

int file_exists(const char * input){
	
	struct stat buffer;
	int exist = stat(input, &buffer);
	if(exist){
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
	int init = 0;
	init_shell(init);
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
	strncat(command, input, 20);
	strncat(command," > ",20);
	//char * cmd = command;
		FILE *fp;
		fp=fopen("file.txt","w");
		strncat(command,"file.txt",20);

	int status = file_exists(command);		

		pid = fork();
		if (pid < 0) { //error occurred 
			fprintf(stderr, "Fork Failed");
			return 1;
		}
		else if (pid == 0) { // child process 
          		system(command);
		fp = fopen("file.txt", "r"); // read mode
   		if (fp == NULL)
   		{
     		 perror("Error while opening the file.\n");
      		exit(EXIT_FAILURE);
   		}
char ch;
   		while((ch = fgetc(fp)) != EOF)
		sprintf(shmptr,"%c", ch);
		shmptr += 1;
 			fclose(fp);
		}
		else { // parent process 

			exect(fp, shmptr);
			munmap (shmptr, SH_SIZE);

			
		}


return 0;
}
