#include<stdio.h>
#include<unistd.h>
#include<semaphore.h>
#include<sys/shm.h>
#include<sys/types.h>
#include<pthread.h>
#define MAX 20

struct msg{
	int item,in,out,max;
	int buff[MAX];
	sem_t mutex,empty,full;
};
struct msg *m;

void *Consumer(void *arg){
	for(int i=1;i<=m->item;i++){
		int item;
		sem_wait(&m->full);
		sem_wait(&m->mutex);
		
		printf("cons entered CS\n");
		item=m->buff[m->out];
		sleep(1);
		printf("Cons consumed %d\n",item);
		
		
		m->out=(m->out +1)%m->max;
		
		sem_post(&m->mutex);
		sem_post(&m->empty);
		printf("cons left CS\n");
		
	}
	
}
int main(){
	int shmid;
	shmid=shmget(1234,sizeof(struct msg),0666 | IPC_CREAT);
	m=(struct msg *)shmat(shmid,NULL,0);
	//m->in=0;m->out=0;
	
	/*printf("no of items: ");
	scanf("%d",&m->item);
	printf("size of buff: ");
	scanf("%d",&m->max);
	
	
	
	sem_init(&m->empty,1,m->max);
	sem_init(&m->full,1,0);
	sem_init(&m->mutex,1,1);*/
	
	pthread_t cons;
	
	pthread_create(&cons,NULL,Consumer,NULL);
	pthread_join(cons,NULL);
	
	sem_destroy(&m->empty);
	sem_destroy(&m->full);
	sem_destroy(&m->mutex);
	
	printf("recvd\n");return 0;

}
