#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>

#define R 5
#define W 9

sem_t rmut,wmut,res,rTry;
int mode,shared=0,rC=0,wC=0;

void *Reader(void *arg){
	int id=*(int *)arg;
	if(mode==1){
		//R prior
		sem_wait(&rmut);
		rC++;
		if(rC==1)sem_wait(&res);
		sem_post(&rmut);
	}else{
		//w prior
		sem_wait(&rTry);
		sem_wait(&rmut);
		rC++;
		if(rC==1)sem_wait(&res);
		sem_post(&rmut);
		sem_post(&rTry);}
	
	printf("Reader %d reading = %d\n",id,shared);
	sleep(1);
	//r exiting
	sem_wait(&rmut);
	rC--;
	if(rC==0)sem_post(&res);
	sem_post(&rmut);
	return NULL;
}

void *Writer(void *arg){
	int id=*(int *)arg;
	if(mode==2){
		sem_wait(&wmut);
		wC++;
		if(wC==1)sem_wait(&rTry);
		sem_post(&wmut);
	}
	sem_wait(&res);
	shared+=10;
	printf("Writer%d wrote %d\n",id,shared);
	sem_post(&res);
	if(mode==2){
		sem_wait(&wmut);
		wC--;
		if(wC==0)sem_post(&rTry);
		sem_post(&wmut);
	}
	return NULL;
	
}
int main(){
	pthread_t r[R],w[W];
	int rid[R],wid[W];
	
	printf("Mode :");
	scanf("%d",&mode);
	
	sem_init(&rmut,0,1);
	sem_init(&wmut,0,1);
	sem_init(&rTry,0,1);
	sem_init(&res,0,1);
	
	for(int i=0;i<R;i++){
		rid[i]=i+1;
		pthread_create(&r[i],NULL,Reader,&rid[i]);}
	
	for(int i=0;i<W;i++){
		wid[i]=i+1;
		pthread_create(&w[i],NULL,Writer,&wid[i]);}
		
	for(int i=0;i<R;i++){
		pthread_join(r[i],NULL);}
	
	for(int i=0;i<W;i++){
		pthread_join(w[i],NULL);}
	
	sem_destroy(&rmut);
	sem_destroy(&wmut);
	sem_destroy(&rTry);
	sem_destroy(&res);
	
	printf("Final shared value = %d\n",shared);
	return 0;
}












