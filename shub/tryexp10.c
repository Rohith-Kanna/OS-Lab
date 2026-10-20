#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define N 5

sem_t mutex;
sem_t forks[N];

void *philos(void *arg){
	int id = *(int *)arg;
	int left = id;
	int right = (id+1)%N;
	
	sem_wait(&mutex);
	sem_wait(&forks[left]);
	sem_wait(&forks[right]);
	
	sem_post(&mutex);
	
	printf("Philosophers are eating!!!\n");
	sleep(1);
	sem_post(&forks[left]);
	sem_post(&forks[right]);
	
	
	printf("Philosphers finished eating!!!\n");
	return NULL;
}

int main(){
	pthread_t p[N];
	int pid[N];
	
	sem_init(&mutex, 0, 1);
	for(int i=0; i<N; i++){
		sem_init(&forks[i], 0, 1);
	}
	for(int i=0; i<N; i++){
		pid[i] = i;
		pthread_create(&p[i], NULL, philos, &pid[i]);
	}
	for(int i=0; i<N; i++){
		pthread_join(p[i], NULL);
	}
	sem_destroy(&mutex);
	for(int i=0; i<N; i++){
		sem_destroy(&forks[i]);
	}
	return 0;
}
