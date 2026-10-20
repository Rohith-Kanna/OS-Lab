#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <stdbool.h>
struct Process{
	int pid, at, bt, ct, wt, tat, rt;
};

struct message{
	long msgType;
	int n;
	struct Process data[10];
};

void sortByArrival(struct Process process[], int n){
	for(int i=0; i<n-1; i++){
		for(int j=0; j<n-i-1; j++){
			if(process[j].at > process[j+1].at || (process[j].at == process[j+1].at && process[j].pid > process[j+1].pid)){
				struct Process temp = process[j];
				process[j] = process[j+1];
				process[j+1] = temp;
			}
		}
	}
}

void printGantt(int order[], int start[], int end[], int count){
	for(int i=0; i<count; i++){
		printf("|   P%d   ", order[i]);
	}	
	printf("|\n");
	printf("%d       ", start[0]);
	for(int i=0; i<count; i++){
		printf("%d       ", end[i]);
	}
	printf("\n");
}

void calcValues(struct Process p[], int n){
	for(int i=0; i<n; i++){
		p[i].tat = p[i].ct - p[i].at;
		p[i].wt = p[i].tat - p[i].bt;
		p[i].rt = p[i].wt;
	}
	
	int avgTAT=0, avgWT=0, avgRT =0;
	int maxCT=0;
	printf("PID\tAT\tBT\tCT\tTAT\tWT\tRT\n");
	for(int i=0; i<n; i++){
		
		printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\n", p[i].pid,p[i].at,p[i].bt,p[i].ct,p[i].tat,p[i].wt,p[i].rt);
		
		avgTAT += p[i].tat;
		avgWT += p[i].wt;
		avgRT += p[i].rt;
		
		if(maxCT < p[i].ct){
			maxCT = p[i].ct;
		}
		
	}
	double throughput = (double)n/maxCT;

    printf("\nAverage Turnaround Time : %.2f\n", (double)avgTAT/n);
    printf("Average Waiting Time    : %.2f\n", (double)avgWT/n);
    printf("Average Response Time   : %.2f\n", (double)avgRT/n);
    printf("Throughput              : %.2f processes/unit time\n",
           throughput);
}



void FCFS(struct Process process[], int n){
	struct Process temp[n];
	for(int i=0; i<n; i++){
		temp[i] = process[i];
	}
	sortByArrival(temp, n);
	
	int order[n], start[n], end[n];
	int count=0;
	int currentTime=0;
	for(int i=0; i<n; i++){
		if(currentTime < temp[i].at){
			currentTime = temp[i].at;
		}
		order[count] = temp[i].pid;
		start[count] = currentTime;
		currentTime += temp[i].bt;
		end[count] = currentTime;
		temp[i].ct = currentTime;
		count++;
	}
	
	calcValues(temp, n);
	printGantt(order, start, end, count);
	
}

void SJF(struct Process p[], int n){
	struct Process temp[n];
	for(int i=0; i<n; i++){
		temp[i] = p[i];
	}
	sortByArrival(temp, n);
	
	int order[n], start[n], end[n];
	int count=0;
	int currentTime=0;
	int completed=0;
	bool isCompleted[n];
	for (int i = 0; i < n; i++) {
    	isCompleted[i] = false;
	}
	
	while(completed<n){
		int idx = -1;
		for(int i=0; i<n; i++){
			if(temp[i].at <= currentTime && !isCompleted[i]){
				if(idx == -1 || temp[i].bt < temp[idx].bt || (temp[i].bt == temp[idx].bt && temp[i].pid < temp[idx].pid)){
					idx = i;
				}
			}
		}
		if(idx == -1){
			currentTime++;
			continue;
		}
		order[count] = temp[idx].pid;
		start[count] = currentTime;
		currentTime += temp[idx].bt;
		end[count] = currentTime;
		temp[idx].ct = currentTime;
		completed++, count++;
		isCompleted[idx]=1;
			
	}
	calcValues(temp, n);
	printGantt(order, start, end, count);
	
}

int main(){

	struct message m;
	m.msgType = 1;
	
	int msgid = msgget(1222, 0666 | IPC_CREAT);
	if(msgid == -1){
		printf("Error connecting");
		exit(1);
	}
	if(msgrcv(msgid, &m, sizeof(struct message)-sizeof(long), 1, 0) == -1){
		printf("Error receiving");
		exit(1);
	}
	printf("range- %d", m.n);
	for(int i=0; i<m.n; i++){
		printf("PID for P%d- %d\n",i, m.data[i].pid);
		printf("\nArrival time for P%d- %d\n",i, m.data[i].at);
		printf("Burst time for P%d- %d\n", i, m.data[i].bt);
	}
	printf("Message received\n\n");
	printf("\n--FCFS--\n");
	FCFS(m.data, m.n);
	printf("\n--SJF--\n");
	SJF(m.data, m.n);
	
	/*if(msgctl(msgid, IPC_RMID, NULL) == -1){
		printf("Message not deleted");
		exit(1);
	}
	printf("Message Deleted");*/
}

