#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<ctype.h>
#include<sys/shm.h>
#include<sys/ipc.h>

int main(){
	//key_t k=ftok("sfile",65);
	int len;
	char str1[100];
	key_t k=1234;
	int smid=shmget(k,1024,0666 | IPC_CREAT);
	char* str=shmat(smid,NULL,0);
	if(k==-1){
		printf("Unique key not created...\n");
		return 0;
	}
	if(smid==-1){
		printf("shm id not created...\n");
		return 0;
	}
	if(str==(char*)-1){
		printf("shm ptr not created...\n");
		return 0;
	}
	len=strlen(str);
	for(int i=0;i<len;i++){
		str1[i]=toupper(str[i]);
	}

	printf("The entered line in the server in Upper case :%s\n",str1);
	shmdt(str);
	return 0;
}