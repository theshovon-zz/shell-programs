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
#define SH_SIZE 		4096 /*Shared memory size*/
void init_shell(int init){

	 if (init == 0)
    {
        system("clear");
        init = 1;
    }
	printf("--SHELL--215246473>> ");
}
void exect(FILE *fp, char* shmem,  int shm_id){
	char cnfrm;
	printf("....working on request...\n");
	wait(NULL);
		
	printf("....output is ready. Display it now [Y/N]>> ");
	scanf("%c", &cnfrm);

	if (cnfrm == 'y' || cnfrm == 'Y')
	{		
  	/* Reattach the shared memory segment, at a different address.  */ 
  		shmem = (char*) shmat (shm_id, (void*) 0x5000000, 0); 
 		 /* Print out the string from shared memory.  */ 
 		 printf ("%s\n", shmem); 
  		/* Detach the shared memory segment.  */ 
 			 shmdt (shmem); 

 			 /* Deallocate the shared memory segment.  */ 
  			shmctl (shm_id, IPC_RMID, 0); 	
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
  	char* shmem; 
 	 struct shmid_ds shmbuffer; 
  	int segment_size; 
  	const int shared_segment_size = 0x6400; 
  	shm_id = shmget (IPC_PRIVATE, shared_segment_size, 
                 IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR); 
  	shmem = (char*) shmat (shm_id, 0, 0); 
  	shmctl (shm_id, IPC_STAT, &shmbuffer); 
  	segment_size  =  shmbuffer.shm_segsz; 
  
	strncat(command, input, 20);
	strncat(command," > ",20);
	
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
		char str[999];
		char tmp[999];
		int c;
   		while ((c = getc(fp)) != EOF){
		 sprintf (shmem,"%c",c);
		 shmem++;
		}
  			shmdt (shmem); 
 			fclose(fp);
		}
		else { // parent process 

			exect(fp, shmem, shm_id );	
		}


return 0;
}
