#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

#define MAX 20
struct Data{
	int request[20];
	int n;
	int head;
	int diskSize;
	int direction;
	
};

void FCFS(int request[], int n, int head){
	int totalSeek = 0;
	int curr = head;
	
	for(int i=0; i<n; i++){
		totalSeek += abs(request[i] - curr);
		curr = request[i];
		printf("%d ->", curr);
	}	
	printf("\nTotal seek time = %d\n", totalSeek);
	printf("\nAverage seek time = %.2f\n", (float)totalSeek/n);
	
}

int SSTF(int request[], int n, int head){
	int totalSeek = 0;
	int curr = head;
	int done = 0;
	int visited[n];
	for(int i=0; i<n; i++){
		visited[i] = 0;
	}
	while(done < n){
		int minDist = 1e9;
		int ind = -1;
		for(int i=0; i<n; i++){
			if(!visited[i] && abs(request[i] - curr) < minDist){
				minDist = abs(request[i] - curr);
				ind = i;
			}
		}
		visited[ind] = 1;
		totalSeek += minDist;
		done++;
		curr = request[ind];
		printf("%d ->", curr);
	}
	printf("\nTotal seek time = %d\n", totalSeek);
	printf("\nAverage seek time = %.2f\n", (float)totalSeek/n);
}

void SCAN(int request[], int n, int head, int diskSize, int direction){
	int totalSeek =0;
	int left[MAX];
	int right[MAX];
	int l=0, r=0;
		
	if(direction == 1){
		right[r++] = diskSize-1;
	}
	else{
		left[l++] = 0;
	}
	
	for(int i=0; i<n; i++){
		if(request[i] > head){
			right[r++] = request[i];
		}
		else{
			left[l++] = request[i];
		}
	}
	
	for(int i=0; i<l-1; i++){
		for(int j=0; j<l-i-1; j++){
			if(left[j] > left[j+1]){
				int temp = left[j];
				left[j] = left[j+1];
				left[j+1] = temp;
			}
		}
	}
	for(int i=0; i<r-1; i++){
		for(int j=0; j<r-i-1; j++){
			if(right[j] > right[j+1]){
				int temp = right[j];
				right[j] = right[j+1];
				right[j+1] = temp;
			}
		}
	}
	int curr = head;
	if(direction == 1){
		for(int i=0; i<r; i++){
			totalSeek += abs(right[i] - curr);
			curr = right[i];
			printf("%d ->", curr);
		}
		for(int i=l-1; i>=0; i--){
			totalSeek += abs(left[i] - curr);
			curr = left[i];
			printf("%d ->", curr);
		}
		
	}
	else{
		for(int i=l-1; i>=0; i--){
			totalSeek += abs(left[i] - curr);
			curr = left[i];
			printf("%d ->", curr);
		}
		for(int i=0; i<r; i++){
			totalSeek += abs(right[i] - curr);
			curr = right[i];
			printf("%d ->", curr);
		}
		
	}
	printf("\nTotal seek time = %d\n", totalSeek);
	printf("\nAverage seek time = %.2f\n", (float)totalSeek/n);
	

}



void CSCAN(int request[], int n, int head, int diskSize, int direction){
	int totalSeek =0;
	int left[MAX];
	int right[MAX];
	int l=0, r=0;
		
	right[r++] = diskSize-1;
	left[l++] = 0;
	
	
	for(int i=0; i<n; i++){
		if(request[i] > head){
			right[r++] = request[i];
		}
		else{
			left[l++] = request[i];
		}
	}
	
	for(int i=0; i<l-1; i++){
		for(int j=0; j<l-i-1; j++){
			if(left[j] > left[j+1]){
				int temp = left[j];
				left[j] = left[j+1];
				left[j+1] = temp;
			}
		}
	}
	for(int i=0; i<r-1; i++){
		for(int j=0; j<r-i-1; j++){
			if(right[j] > right[j+1]){
				int temp = right[j];
				right[j] = right[j+1];
				right[j+1] = temp;
			}
		}
	}
	int curr = head;
		for(int i=0; i<r; i++){
			totalSeek += abs(right[i] - curr);
			curr = right[i];
			printf("%d ->", curr);
		}
		for(int i=l-1; i>=0; i--){
			totalSeek += abs(left[i] - curr);
			curr = left[i];
			printf("%d ->", curr);
		}
	printf("\nTotal seek time = %d\n", totalSeek);
	printf("\nAverage seek time = %.2f\n", (float)totalSeek/n);
	

}


void LOOK(int request[], int n, int head, int diskSize, int direction){
	int totalSeek =0;
	int left[MAX];
	int right[MAX];
	int l=0, r=0;
		
	/*if(direction == 1){
		right[r++] = diskSize-1;
	}
	else{
		left[l++] = 0;
	}*/
	
	for(int i=0; i<n; i++){
		if(request[i] > head){
			right[r++] = request[i];
		}
		else{
			left[l++] = request[i];
		}
	}
	
	for(int i=0; i<l-1; i++){
		for(int j=0; j<l-i-1; j++){
			if(left[j] > left[j+1]){
				int temp = left[j];
				left[j] = left[j+1];
				left[j+1] = temp;
			}
		}
	}
	for(int i=0; i<r-1; i++){
		for(int j=0; j<r-i-1; j++){
			if(right[j] > right[j+1]){
				int temp = right[j];
				right[j] = right[j+1];
				right[j+1] = temp;
			}
		}
	}
	int curr = head;
	if(direction == 1){
		for(int i=0; i<r; i++){
			totalSeek += abs(right[i] - curr);
			curr = right[i];
			printf("%d ->", curr);
		}
		for(int i=l-1; i>=0; i--){
			totalSeek += abs(left[i] - curr);
			curr = left[i];
			printf("%d ->", curr);
		}
		
	}
	else{
	for(int i=l-1; i>=0; i--){
			totalSeek += abs(left[i] - curr);
			curr = left[i];
			printf("%d ->", curr);
		}
		for(int i=0; i<r; i++){
			totalSeek += abs(right[i] - curr);
			curr = right[i];
			printf("%d ->", curr);
		}
		
	}
	printf("\nTotal seek time = %d\n", totalSeek);
	printf("\nAverage seek time = %.2f\n", (float)totalSeek/n);
	

}

void CLOOK(int request[], int n, int head, int diskSize, int direction){
	int totalSeek =0;
	int left[n];
	int right[n];
	int l=0, r=0;
		
	//right[r++] = diskSize-1;
	//left[l++] = 0;
	
	
	for(int i=0; i<n; i++){
		if(request[i] > head){
			right[r++] = request[i];
		}
		else{
			left[l++] = request[i];
		}
	}
	
	for(int i=0; i<l-1; i++){
		for(int j=0; j<l-i-1; j++){
			if(left[j] > left[j+1]){
				int temp = left[j];
				left[j] = left[j+1];
				left[j+1] = temp;
			}
		}
	}
	for(int i=0; i<r-1; i++){
		for(int j=0; j<r-i-1; j++){
			if(right[j] > right[j+1]){
				int temp = right[j];
				right[j] = right[j+1];
				right[j+1] = temp;
			}
		}
	}
	int curr = head;
		for(int i=0; i<r; i++){
			totalSeek += abs(right[i] - curr);
			curr = right[i];
			printf("%d ->", curr);
		}
		for(int i=l-1; i>=0; i--){
			totalSeek += abs(left[i] - curr);
			curr = left[i];
			printf("%d ->", curr);
		}
	printf("\nTotal seek time = %d\n", totalSeek);
	printf("\nAverage seek time = %.2f\n", (float)totalSeek/n);
	

}

int main(){
	
	int shmid = shmget(1234, sizeof(struct Data), 0666 | IPC_CREAT);
	struct Data *data;
	data = (struct Data *)shmat(shmid, NULL, 0);
	
	printf("number of request- %d\n", data->n);
	
	for(int i=0; i<data->n; i++){
		printf("%d ", data->request[i]);
	}
	printf("\nhead- %d\n", data->head);
	printf("diskSize- %d\n", data->diskSize);
	
	printf("direction- %d\n", data->direction);
	printf("\n---FCFS---\n");
	FCFS(data->request, data->n, data->head);
	printf("\n---SSTF---\n");
	SSTF(data->request, data->n, data->head);
	printf("\n---SCAN---\n");
	SCAN(data->request, data->n, data->head, data->diskSize, data->direction);
	printf("\n---CSCAN---\n");
	CSCAN(data->request, data->n, data->head, data->diskSize, data->direction);
	printf("\n---LOOK---\n");
	LOOK(data->request, data->n, data->head, data->diskSize, data->direction);
	printf("\n---CLOOK---\n");
	CLOOK(data->request, data->n, data->head, data->diskSize, data->direction);
	
	printf("Message received!\n");
	shmdt(data);
	
}
