#include<stdio.h>
#include<unistd.h>
#include<semaphore.h>
#include<pthread.h>

#define N 5
sem_t mut;
sem_t forks[N];
void *philos(void *arg){
	int id=*(int *)arg;
	int left=id;
	int right=(id+1)%N;
	
	printf("P%d is thinking\n",id+1);
	sem_wait(&mut);
	sem_wait(&forks[left]);
	sem_wait(&forks[right]);
	sem_post(&mut);
	
	printf("P%d is eating \n",id+1);
	sleep(1);
	sem_post(&forks[left]);
	sem_post(&forks[right]);
	
	printf("P%d finished eating\n",id+1);
	return NULL;
}
int main(){
	pthread_t p[N];
	int pid[N];
	
	sem_init(&mut,0,1);
	for(int i=0;i<N;i++)
		sem_init(&forks[i],0,1);

	for(int i=0;i<N;i++){pid[i]=i;
		pthread_create(&p[i],NULL,philos,&pid[i]);}
		
	for(int i=0;i<N;i++){pthread_join(p[i],NULL);}
	
	sem_destroy(&mut);
	for(int i=0;i<N;i++)
		sem_destroy(&forks[i]);
	return 0;
}




