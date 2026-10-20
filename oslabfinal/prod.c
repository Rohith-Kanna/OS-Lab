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

void *Producer(void *arg){
	for(int i=1;i<=m->item;i++){
		sem_wait(&m->empty);
		sem_wait(&m->mutex);
		
		printf("Prod entered CS\n");
		int item=i*10;
		sleep(1);
		
		
		m->buff[m->in]=item;
		printf("prod produced %d\n",item);
		m->in=(m->in +1)%m->max;
		
		sem_post(&m->mutex);
		sem_post(&m->full);
		printf("Prod left CS\n");
		
	}
	
}
int main(){
	int shmid;
	shmid=shmget(1234,sizeof(struct msg),0666 | IPC_CREAT);
	m=(struct msg *)shmat(shmid,NULL,0);
	
	printf("no of items: ");
	scanf("%d",&m->item);
	printf("size of buff: ");
	scanf("%d",&m->max);
	
	m->in=0;m->out=0;
	
	sem_init(&m->empty,1,m->max);
	sem_init(&m->full,1,0);
	sem_init(&m->mutex,1,1);
	
	pthread_t prod;
	
	pthread_create(&prod,NULL,Producer,NULL);
	pthread_join(prod,NULL);
	
	/*sem_destroy(&m->empty);
	sem_destroy(&m->full);
	sem_destroy(&m->mutex);*/
	
	printf("done\n");return 0;

}






















