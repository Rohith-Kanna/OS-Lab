#include <stdio.h>
#include <stdbool.h>

#define maxP 10
#define maxR 10

int P, R;
int allocation[maxP][maxR], need[maxP][maxR], max[maxP][maxR];
int avail[maxR];


void calcNeed(){
	for(int i=0; i<P; i++){
		for(int j=0; j<R; j++){
			need[i][j] = max[i][j] - allocation[i][j];
		}
	}
}

bool safeState(){
	int work[maxR];
	bool Finish[maxP] = {false};
	int count =0;
	int safeSeq[maxP];
	
	for(int i=0; i<R; i++){
		work[i] = avail[i];
	}
	
	while(count < P){
		bool found = false;
		for(int i=0; i<P; i++){
			if(!Finish[i]){
				bool canAllocate=true;
				for(int j=0; j<R; j++){
					if(need[i][j] > work[j]){
						canAllocate = false;
						break;
					}
				}
				if(canAllocate){
					for(int j=0; j<R; j++){
						work[j] += allocation[i][j];
					}
					safeSeq[count] = i;
					count++;
					Finish[i] = true;
					found = true;
				}
			}
		}
		if(!found){
			printf("System is in unsafe state\n");
			return false;
		}
	}
	printf("System is in safe state\n");
	for(int i=0; i<P; i++){
		printf("%d -> ", safeSeq[i]);
	}
	return true;
}

bool requestAlgo(int process, int request[]){
	
	for(int i=0; i<R; i++){
		if(request[i] > need[process][i]){
			printf("Error, Exceeding need\n");
			return false;
		}
	}
	for(int i=0; i<R; i++){
		if(request[i] > avail[i]){
			printf("Error, Exceeding need\n");
			return false;
		}
	}
	
	for(int j=0; j<R; j++){
		avail[j] -= request[j];
		allocation[process][j] += request[j];
		need[process][j] -= request[j];
	}
	if(safeState()){
		printf("Resources granted\n");
		return true;
	}
	printf("Request Denied\n");
	for(int j=0; j<R; j++){
		avail[j] += request[j];
		allocation[process][j] -= request[j];
		need[process][j] += request[j];
	}
	return false;
	
}

int main(){
	
	int process, request[maxR];
	
	printf("Enter number of process- ");
	scanf("%d", &P);
	printf("Enter the number of resources- ");
	scanf("%d", &R);
	
	for(int i=0; i<P; i++){
		printf("Enter alloc matrix for P%d",i);
		for(int j=0; j<R; j++){
			scanf("%d", &allocation[i][j]);
		}
	}
	for(int i=0; i<P; i++){
		printf("Enter Max matrix for P%d",i);
		for(int j=0; j<R; j++){
			scanf("%d", &max[i][j]);
		}
	}
	printf("Enter Available matrix");
	for(int j=0; j<R; j++){
		scanf("%d", &avail[j]);
	}
	calcNeed();
	printf("Initial safe check\n");
	safeState();
	char choice;
	do{
			
			printf("Enter your choice: (y/n)");
			scanf(" %c", &choice);
			if(choice == 'y'){
				printf("Enter process number- ");
				scanf("%d", &process);
				printf("Enter request resources- ");
				for(int j=0; j<R; j++){
					scanf("%d", &request[j]);
				}
				requestAlgo(process, request);
				for(int i=0; i<R; i++){
					printf("%d ", avail[i]);
				}
				for(int i=0; i<P; i++){
					for(int j=0; j<R; j++){
						printf("%d", allocation[i][j]);
					}
					printf("\n");
				}
				for(int i=0; i<P; i++){
					for(int j=0; j<R; j++){
						printf("%d", need[i][j]);
					}
					printf("\n");
				}
			}
	}while(choice=='y');
	return 0;
	
}


