#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

struct Process{
	int pid, at, bt, ct, wt, tat, rt;
};

struct message{
	long msgType;
	int n;
	struct Process data[10];
};


int main(){

	struct message m;
	m.msgType = 1;
	
	int msgid = msgget(1222, 0666 | IPC_CREAT);
	if(msgid == -1){
		printf("Error connecting");
		exit(1);
	}
	printf("Enter range- ");
	scanf("%d", &m.n);
	for(int i=0; i<m.n; i++){
		m.data[i].pid = i;
		printf("\nArrival time for P%d- ", i);
		scanf("%d", &m.data[i].at);
		printf("Burst time for P%d- ", i);
		scanf("%d", &m.data[i].bt);
	}
	
	if(msgsnd(msgid, &m, sizeof(struct message)-sizeof(long), 0) == -1){
		printf("Error sending");
		exit(1);
	}
	printf("Message sent\n");
}

