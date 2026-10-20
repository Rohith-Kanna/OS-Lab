#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <string.h>

struct Data{
	int block[10];
	int m, n;
	int process[10];
};



void firstFit(int block[], int m, int process[], int n){
	int allocation[n];
	for(int i=0; i<n; i++){
		allocation[i] = -1;
	}
	for(int i=0; i<n; i++){
		for(int j=0; j<m; j++){
			if(process[i] <= block[j]){
				allocation[i] = j;
				block[j] -= process[i];
				break;
			}
		}
	}
	printf("\nFirst fit\n");
	printf("Process No\tProcess Size\tBlock No\n");
	for(int i=0; i<n; i++){
		printf("%d\t%d\t", i+1, process[i]);
		if(allocation[i] == -1){
			printf("\tNot Alloc\n");
		}
		else printf("%d\n", allocation[i]+1);
	}
}


void bestFit(int block[], int m, int process[], int n){
	int allocation[n];
	for(int i=0; i<n; i++){
		allocation[i] = -1;
	}
	for(int i=0; i<n; i++){
		int idx = -1;
		for(int j=0; j<m; j++){
			if(block[j] > process[i]){
				if(idx == -1 || (block[j] < block[idx])){
					idx = j;
				}
			}
		}
		if(idx != -1){
			allocation[i] = idx;
			block[idx] -= process[i];
		}
	}
	printf("\nBest fit\n");
	printf("Process No\tProcess Size\tBlock No\n");
	for(int i=0; i<n; i++){
		printf("%d\t%d\t", i+1, process[i]);
		if(allocation[i] == -1){
			printf("\tNot Alloc\n");
		}
		else printf("%d\n", allocation[i]+1);
	}
}


void worstFit(int block[], int m, int process[], int n){
	int allocation[10];
	for(int i=0; i<n; i++){
		allocation[i] = -1;
	}
	for(int i=0; i<n; i++){
		int idx = -1;
		for(int j=0; j<m; j++){
			if(block[j] > process[i]){
				if(idx == -1 || (block[j] > block[idx])){
					idx = j;
				}
			}
		}
		if(idx != -1){
			allocation[i] = idx;
			block[idx] -= process[i];
		}
	}
	printf("\nWorst fit\n");
	printf("Process No\tProcess Size\tBlock No\n");
	for(int i=0; i<n; i++){
		printf("%d\t%d\t", i+1, process[i]);
		if(allocation[i] == -1){
			printf("\tNot Alloc\n");
		}
		else printf("%d\n", allocation[i]+1);
	}
}


int main(){
	int shmid;
	shmid = shmget(1721, sizeof(struct Data), 0666 | IPC_CREAT);
	struct Data *data;
	data = (struct Data *)shmat(shmid, NULL, 0);
	
	printf("size of Block- %d\n", data->m);
	
	
	for(int i=0; i<data->m; i++){
		printf("%d, ", data->block[i]);
	}
	printf("size of process- %d\n", data->n);
	
	
	for(int i=0; i<data->n; i++){
		printf("%d, ", data->process[i]);
	}
	
	printf("Message Recieved\n");
	int b1[10], b2[10], b3[10];
	for(int i=0; i<data->m; i++){
		b1[i] = b2[i] = b3[i] = data->block[i];
	}
	firstFit(b1, data->m, data->process, data->n);
	bestFit(b2, data->m, data->process, data->n);
	worstFit(b3, data->m, data->process, data->n);
	
	shmdt(data);
	shmctl(shmid, IPC_RMID, NULL);
	printf("test\n");
	
}
