#include<sys/shm.h>
#include<sys/wait.h>
#include<sys/ipc.h>
#include<unistd.h>
#include<stdio.h>
#define MAX 20
struct Process{
int pid,at,bt,ct,tat,wt;};

struct Data{
struct Process p[MAX];int n;
};

void fcfs(struct Process p[],int n){
	int time=0;
	for(int i=0;i<n;i++){
		if(time<p[i].at)time=p[i].at;
		time=time+p[i].bt;
		p[i].ct=time;
		p[i].tat=p[i].ct-p[i].at;
		p[i].wt=p[i].tat-p[i].bt;
	}
	
	printf("Process\tAT\tBT\tCT\tTAT\tWT\n");
	for(int i=0;i<n;i++){
		printf("%d\t%d\t%d\t%d\t%d\t%d\n",
		p[i].pid,p[i].at,p[i].bt,p[i].ct,p[i].tat,p[i].wt);
	}
}

void sjf(struct Process p[],int  n){
	int time=0;
	int completed[100]={0};
	int cc=0;
	int i;
	while(cc<n){
		int mInd=-1;
		for(i=0;i<n;i++){
			if(completed[i]==0 && p[i].at<=time){
				if(mInd=-1 || p[mInd].bt>p[i].bt)mInd=i;
			}
		}
		if(mInd==-1)time++;
		else{
			time=time+p[mInd].bt;
			p[mInd].ct=time;
			p[mInd].tat=p[mInd].ct-p[mInd].at;
			p[mInd].wt=p[mInd].tat-p[mInd].bt;
			
			completed[mInd]=1;
			cc++;
		}
	}
	printf("Process\tAT\tBT\tCT\tTAT\tWT\n");
	for(int i=0;i<n;i++){
		printf("%d\t%d\t%d\t%d\t%d\t%d\n",
		p[i].pid,p[i].at,p[i].bt,p[i].ct,p[i].tat,p[i].wt);
	}
}
int main(){
	int shmid;
	struct Data *data;
	struct Process p[MAX],fcfs_p[MAX],sjf_p[MAX];
	shmid=shmget(1234,sizeof(struct Data),0666 | IPC_CREAT);
	data=(struct Data *)shmat(shmid,NULL,0);
	int n;
	n=data->n;
	for(int i=0;i<n;i++){
		p[i]=data->p[i];
		fcfs_p[i]=data->p[i];
		sjf_p[i]=data->p[i];
	}
	fcfs(fcfs_p,n);
	sjf(sjf_p,n);
	shmdt(data);
	return 0;
}
