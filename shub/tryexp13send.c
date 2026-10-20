#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>

struct Data{
	int pages[20];
	int capacity;
	int n;
};

int main(){
	int shmid = shmget(12345, sizeof(struct Data), 0666 | IPC_CREAT);
	struct Data *data;
	data = (struct Data *)shmat(shmid, NULL, 0);
	printf("Enter the value of N- ");
	scanf("%d", &data->n);
	printf("Enter the pages- ");
	for(int i=0; i<data->n; i++){
		scanf("%d", &data->pages[i]);
	}
	printf("Enter the value of capacity- ");
	scanf("%d", &data->capacity);
	printf("Message sent\n");
	shmdt(data);
}
