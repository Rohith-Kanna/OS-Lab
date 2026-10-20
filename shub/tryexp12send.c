#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <string.h>

struct Data{
	int block[10];
	int m, n;
	int process[10];
};


int main(){
	int shmid;
	shmid = shmget(1721, sizeof(struct Data), 0666 | IPC_CREAT);
	struct Data *data;
	data = (struct Data *)shmat(shmid, NULL, 0);
	
	printf("Enter the size of Block- ");
	scanf("%d", &data->m);
	printf("Enter the elements of Block- ");
	for(int i=0; i<data->m; i++){
		scanf("%d", &data->block[i]);
	}
	
	printf("Enter the number of process- ");
	scanf("%d", &data->n);
	printf("Enter the process- ");
	for(int i=0; i<data->n; i++){
		scanf("%d", &data->process[i]);
	}
	
	printf("Message sent\n");
	shmdt(data);
}

