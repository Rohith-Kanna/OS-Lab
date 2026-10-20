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

void *Consumer(void *arg){
	for(int i=1; i<=m->item; i++){
		int item;
		sem_wait(&m->full);
		sem_wait(&m->mutex);

		printf("Consumer entered CS\n");
		item = m->buffer[m->out];
		sleep(1);
		printf("Consumer consumed %d\n", item);
		
		m->out = (m->out +1)%m->max;
		sem_post(&m->mutex);
		sem_post(&m->empty);
		printf("Consumer left CS\n");
	}
} 


int main(){
	int shmid = shmget(1213, sizeof(struct message), 0666 | IPC_CREAT);
	m = (struct message *)shmat(shmid, NULL, 0);
	
	m->in = 0;
	m->out =0;
	
	/*
	sem_init(&m->empty, 1, m->max);
	sem_init(&m->full, 1, 0);
	sem_init(&m->mutex, 1, 1);
	*/
	
	pthread_t cons;
	
	pthread_create(&cons, NULL, Consumer, NULL);
	
	pthread_join(cons, NULL);
	
	sem_destroy(&m->empty);
	sem_destroy(&m->full);
	sem_destroy(&m->mutex);
	
	
	printf("Message Received\n");
}
