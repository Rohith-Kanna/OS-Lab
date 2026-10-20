#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

struct Data{
	int request[20];
	int n;
	int head;
	int diskSize;
	Sint direction;
};



int main(){
	
	int shmid = shmget(1234, sizeof(struct Data), 0666 | IPC_CREAT);
	struct Data *data;
	data = (struct Data *)shmat(shmid, NULL, 0);
	
	printf("Enter the number of request- ");
	scanf("%d", &data->n);
	for(int i=0; i<data->n; i++){
		scanf("%d", &data->request[i]);
	}
	printf("Enter the head- ");
	scanf("%d", &data->head);
	printf("Enter the diskSize- ");
	scanf("%d", &data->diskSize);
	printf("Enter the direction [0/1]- ");
	scanf("%d", &data->direction);
	printf("Message sent!");
	shmdt(data);
	
}
