#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

bool flag[2] = {false, false};
int turn;
int shared=0;

void *petersons(void *arg){
	int id = *(int*)arg;
	int other = 1-id;
	for(int i=0; i<5; i++){
		flag[id] = true;
		turn = other;

		while(flag[other] && turn == other);//busy wait

		//critical section
		printf("%d has entered CS\n", id);
		int local = shared;
		shared = local + 10;
		sleep(1);
		printf("Updated shared value to- %d\n", shared);
		
		printf("%d has left CS\n", id);
		flag[id] = false;
		sleep(1);
	}
	return NULL;
}

int main(){
	pthread_t t1, t2;
	int id = 0;
	int other = 1;
	
	pthread_create(&t1, NULL, petersons, &id);
	pthread_create(&t2, NULL, petersons, &other);
	
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	
	printf("Final shared- %d\n", shared);
	
}
