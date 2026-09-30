//this code is a C program that implements a ticket booking system using Peterson's algorithm for mutual exclusion. It creates two threads: one for the server and one for the client. The server checks and adjusts the total number of available seats, while the client allows users to book or cancel tickets. The program ensures that both threads can safely access shared resources without conflicts, using busy waiting to manage access to critical sections. The program continues running until the user chooses to exit, at which point it cleans up and displays the final state of available seats.

#include <stdio.h>
#include <stdlib.h>
#include<pthread.h>
#include <stdbool.h>
#include <unistd.h>

#define NUM_SEATS 100

//SharedVariables
int totalSeats=NUM_SEATS;
//Peterson'sSyncArrays:Index0=Server,Index1=Client 
int choosing[2] = {0, 0};
int turn=0;
bool keep_running=true;
void* server(void* arg) {
while(keep_running){
//ENTRYSECTION(Serverwantstoinspect/adjustdatabasesafely) 
choosing[0]= 1;
turn=1;//YieldturntoClient
while(choosing[1]&&turn==1);//Busy wait
//CRITICALSECTION
if(totalSeats<0){
printf("\n[ServerAlerts]:Systemerror!Seatsdroppedbelow0.Resetting inventory.\n");
totalSeats= 0;
}
//EXITSECTION
choosing[0]=0;//SleepbrieflytoavoidmonopolizingCPUcyclesandletClientprint
usleep(500000);
}
pthread_exit(NULL);
}
void *client(void*arg){
while (true) {
int choice;
printf("\n---TicketBookingDashboard(AvailableSeats:%d)---\n", totalSeats);
printf("1. Book Tickets\n");
printf("2.CancelTickets\n");
printf("0. Exit System\n");
printf("Enter selection: ");
if (scanf("%d", &choice) != 1) {
printf("Invalidinputstructure.\n"); break;
}
if (choice == 0) {
keep_running=false; break;
}
if(choice==1||choice==2){ int
count;
printf("Enternumberofseats:");
scanf("%d", &count);
if(count<=0){
printf("Error:Quantitymustbegreaterthanzero.\n"); continue;
}//ENTRYSECTION(Peterson'sLock)
choosing[1] = 1;
turn=0;//YieldturntoServer
while(choosing[0]&&turn==0);//Busy wait
//CRITICALSECTION(DatabaseModification) 
if (choice == 1) {
printf("\n[ClientCS]Processingbookingrequestfor%dseat(s)...\n",
count);
if (count<=totalSeats){ totalSeats
-= count;
printf("[ClientCS]Success!%dseatsbooked.\n",count);
}else{
printf("[ClientCS]Failed:Notenoughseatsavailable.\n");
}
}else{
printf("\n[ClientCS]Processingcancellationrequestfor%d seat(s)...\n", count);
if(totalSeats+count<=NUM_SEATS){ totalSeats+= count;
printf("[ClientCS]Success!%dseatsreturnedtopool.\n",count);
}else{
printf("[ClientCS]Failed:Cannotexceedbasecapacity(%d seats).\n",
NUM_SEATS);
}
}
//EXITSECTION
choosing[1]=0;
}else{
printf("Invalidchoice.Tryagain.\n");
}
}
pthread_exit(NULL);
}
int main()
{
pthread_t server_thread,client_thread;
printf("Initializing Ticket Engine System under Peterson'sProtocol...\n");
// Creating concurrent threads
pthread_create(&server_thread,NULL,server,NULL);
pthread_create(&client_thread, NULL, client, NULL);
// Waiting for completion
pthread_join(client_thread, NULL);
pthread_join(server_thread,NULL);
printf("\nProgram exiting cleanly.Final seatstate:%d\n",totalSeats);
return 0;
}

