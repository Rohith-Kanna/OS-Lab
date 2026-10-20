#include<sys/shm.h>
#include<sys/wait.h>
#include<sys/ipc.h>
#include<sys/types.h>
#include<stdio.h>

#define MAX 20

struct Process{
	int pid,at,bt,ct,tat,wt;
};

struct Data{
	struct Process p[MAX];
	int n;
};

int main(){
	int shmid;
	struct Data *data;
	shmid=shmget(1234,sizeof(struct Data),0666 | IPC_CREAT);
	data=(struct Data *)shmat(shmid,NULL,0);
	
	printf("N= ");
	scanf("%d",&data->n);
	printf("at and bt: \n");
	for(int i=0;i<data->n;i++){
		data->p[i].pid=i+1;
		scanf("%d%d",&data->p[i].at,&data->p[i].bt);
	}
	shmdt(data);
	return 0;
	
}
