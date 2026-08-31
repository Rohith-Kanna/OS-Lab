#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#define smsize 1024

int main(){
	int n;
	pid_t id;
	//key_t k=ftok("sfile",65);
	key_t k=128003213;
	int smid=shmget(k,smsize,0666 | IPC_CREAT);
	char* str=shmat(smid,NULL,0);

	if (k == -1){
		printf("Unique key not created ...\n");
		return 0;
	}
	if(smid==-1){
		printf("shm id not created...\n");
		return 0;
	}
	if(str==(char*)-1){
		printf("shm adress ptr not created...\n");
		return 0;
	}

	id=fork();
	if(id==-1){
		printf("Child not created...\n");
		return 0;
	}
	else if(id==0){
		//child process
		/*printf("Enter a line to read 4 server : ");// to read one line
		from user
		scanf("%[^\n]",str);*/
		char* cp=str;
		printf("Enter many lines & type \"exit\" to end the process :");

		while (fgets(cp, smsize - (cp - str), stdin) != NULL){
			cp += strlen(cp); // Move pointer to the end
			if (strcmp(cp-strlen("exit\n"), "exit\n") == 0){
				break; // Exit the loop if the user types 'exit'
			}
		}
		printf("\nSize of \"%s\" is : %ld",str,strlen(str));
	}
	else{
		//parent process

		wait(NULL);
		printf("\nThe parent after completion of child...\n");
	}
	shmdt(str);
	return 0;
}
