#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>

int main(){
	//key_t k=ftok("sfile",65);
	key_t k=1234;
	int smid=shmget(k,1024,0666 | IPC_CREAT);

	char* str=shmat(smid,NULL,0);
	if(k==-1){
		printf("Unique key not created...\n");
		return 0;
	}
	if(smid==-1){
		printf("Shm id not created...\n");
		return 0;
	}
	if(str==(char*)-1){
		printf("Shm ptr not created...\n");
		return 0;
	}	
	printf("Line entered in server is : %s\n",str);
	printf("The size of the given line : %ld\n",strlen(str));
	shmdt(str);
	return 0;
}