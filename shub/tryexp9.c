#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#define reader 5
#define writer 9

sem_t mutex;
sem_t rw_mutex;
int shared_data = 0;
int readerCount;

void *Writer(void *arg){
	int id = *(int *)arg;
	sem_wait(&mutex);
	shared_data += 10;
	printf("[Writer %d] performing write. New shared data- %d\n", id, shared_data);
	sem_post(&mutex);
	printf("Writer finished writing\n");
	return NULL;
}

void *Reader(void *arg){

	int id = *(int *)arg;
	
	sem_wait(&rw_mutex);
	readerCount++;
	if(readerCount == 1){
		sem_wait(&mutex);
	}
	sem_post(&rw_mutex);
	
	printf("[Reader %d] performing read. Shared data- %d\n",id, shared_data);
	sleep(1);
	printf("Reader finished reading\n");
	
	sem_wait(&rw_mutex);
	readerCount--;
	if(readerCount == 0){
		sem_post(&mutex);
	}
	sem_post(&rw_mutex);
	return NULL;
}

int main(){
	pthread_t r[reader], w[writer];
	int rid[reader], wid[writer];
	sem_init(&mutex, 0, 1);
	sem_init(&rw_mutex, 0, 1);
	
	
	for(int i=0; i<reader; i++){
		rid[i] = i+1;
		pthread_create(&r[i], 0, Reader, &rid[i]);
	}
	for(int i=0; i<writer; i++){
		wid[i] = i+1;
		pthread_create(&w[i], 0, Writer, &wid[i]);
	}
	for(int i=0; i<reader; i++){
		pthread_join(r[i], NULL);
	}
	for(int i=0; i<writer; i++){
		pthread_join(w[i], NULL);
	}
	
	sem_destroy(&mutex);
	sem_destroy(&rw_mutex);
}
