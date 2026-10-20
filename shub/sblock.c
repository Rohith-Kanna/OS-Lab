#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX 20

struct message{
	
	int item, in, out, max;
	int buffer[MAX];
	sem_t mutex, empty, full;
};

struct message *m;

void *Producer(void *arg){
	for(int i=1; i<=m->item; i++){
		sem_wait(&m->empty);
		sem_wait(&m->mutex);

		printf("Producer entered CS\n");
		int item = i*10;
		sleep(1);
		printf("Consumer consumed %d\n", item);

		m->buffer[m->in] = item;
		m->in = (m->in +1)%m->max;
		
		sem_post(&m->mutex);
		sem_post(&m->full);
		printf("Producer left CS\n");
	}
} 


int main(){

	int shmid = shmget(1213, sizeof(struct message), 0666 | IPC_CREAT);
	m = (struct message *)shmat(shmid, NULL, 0);
	
	printf("Enter number of items- ");
	scanf("%d", &m->item);
	printf("Enter size of buffer- ");
	scanf("%d", &m->max);
	
	
	m->in = 0;
	m->out =0;
	
	sem_init(&m->empty, 1, m->max); //val, pshared, value
	sem_init(&m->full, 1, 0); 
	sem_init(&m->mutex, 1, 1);
	
	pthread_t prod;
	
	pthread_create(&prod, NULL, Producer, NULL);
	
	pthread_join(prod, NULL);
	
	sem_destroy(&m->empty);
	sem_destroy(&m->full);
	sem_destroy(&m->mutex);
	
	
	printf("Message sent\n");
}
