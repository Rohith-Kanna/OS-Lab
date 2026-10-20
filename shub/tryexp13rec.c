#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>

struct Data{
	int pages[20];
	int capacity;
	int n;
};

void displayFrame(int frames[], int capacity){
	for(int i=0; i<capacity; i++){
		printf("%d ", frames[i]);
	}
	printf("\n");
}
void FCFS(int pages[], int n, int capacity){
	int frames[capacity];
	int pageHit=0;
	int pageFault = 0;
	int front = 0;
	
	for(int i=0; i<capacity; i++){
		frames[i] = -1;
	}
	
	for(int i=0; i<n; i++){
		bool found = false;
		for(int j=0; j<capacity; j++){
			if(frames[j] == pages[i]){
				pageHit++;
				front = (front+1)%capacity;
				//frames[j] = pages[i];
				found=true;
				break;
			}
		}
		if(!found){
			pageFault++;
			frames[front] = pages[i];
			//found = true;
			front = (front+1)%capacity;
		}
		printf("Page %d ->", pages[i]);
		displayFrame(frames, capacity);
	}
	printf("Number of Page fault- %d\n", pageFault);
	printf("Number of Page Hit- %d\n", pageHit);
	printf("Number of Page fault %%- %.2f%%\n", ((float)pageFault/n)*100);
	printf("Number of Page Hit %%- %.2f%%\n", ((float)pageHit/n)*100);
}

void LRU(int pages[], int n, int capacity){
	int frames[capacity];
	int lastUsed[capacity];
	int pageHit=0;
	int pageFault = 0;
	
	
	for(int i=0; i<capacity; i++){
		frames[i] = -1;
		lastUsed[i] = 0;
	}
	for(int i=0; i<n; i++){
		bool found = false;
		for(int j=0; j<capacity; j++){
			if(frames[j] == pages[i]){
				pageHit++;
				lastUsed[j] = i;
				//frames[j] = pages[i];
				found=true;
				break;
			}
		}
		if(!found){
			int pos = -1;
			
			for(int i=0; i<capacity; i++){
				if(frames[i] == -1){
					pos = i;
				}
			}
			if(pos == -1){
				pos = 0;
				for(int j=1; j<capacity; j++){
					if(frames[j] < frames[pos]){
						pos = j;
					}
				}
			}		
			pageFault++;
			lastUsed[pos] = i;
			frames[pos] = pages[i];
		}
		printf("Page %d ->", pages[i]);
		displayFrame(frames, capacity);
	}
	
	printf("Number of Page fault- %d\n", pageFault);
	printf("Number of Page Hit- %d\n", pageHit);
	printf("Number of Page fault %%- %.2f%%\n", ((float)pageFault/n)*100);
	printf("Number of Page Hit %%- %.2f%%\n", ((float)pageHit/n)*100);
}

void Optimal(int pages[], int n, int capacity){
	int frames[capacity];
	int pageHit=0;
	int pageFault = 0;
	int front = 0;
	for(int i=0; i<capacity; i++){
		frames[i] = -1;
	}
	for(int i=0; i<n; i++){
		bool found = false;
		for(int j=0; j<capacity; j++){
			if(frames[j] == pages[i]){
				pageHit++;
				found=true;
				break;
			}
		}
		if(!found){
			int pos = -1;
			for(int j=0; j<capacity; j++){
				if(frames[j] == -1){
					pos = j;
				}
			}
			if(pos == -1){
				int farthest=-1;
				for(int j=0; j<capacity; j++){
					int nextUse = -1;
					for(int k=i+1; k<n; k++){
						if(pages[k] == frames[j]){
							nextUse = k;
							break;
						} 
					}
					if(nextUse == -1){
						pos = j;
						break;
					}	
					if(farthest < nextUse){
						farthest = nextUse;
						pos = j;
					}
				}				
			}
			pageFault++;
			frames[pos] = pages[i];
			
			
		}
		printf("Page %d ->", pages[i]);
		displayFrame(frames, capacity);
	}
	
	printf("Number of Page fault- %d\n", pageFault);
	printf("Number of Page Hit- %d\n", pageHit);
	printf("Number of Page fault %%- %.2f%%\n", ((float)pageFault/n)*100);
	printf("Number of Page Hit %%- %.2f%%\n", ((float)pageHit/n)*100);
}




int main(){
	int shmid = shmget(12345, sizeof(struct Data), 0666 | IPC_CREAT);
	struct Data *data;
	data = (struct Data *)shmat(shmid, NULL, 0);
	int n = data->n;
	int capacity = data->capacity;
	int pages[n];
	for(int i=0; i<n; i++){
		pages[i] = data->pages[i];
	}
	
	printf("N- %d\n", n);
	printf("Capacity- %d\n", capacity);
	printf("Message Received\n");
	
	FCFS(data->pages, data->n, data->capacity);
	LRU(data->pages, data->n, data->capacity);
	Optimal(data->pages, data->n, data->capacity);
	
	shmdt(data);
	shmctl(shmid, IPC_RMID, NULL);
}
