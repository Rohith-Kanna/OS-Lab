#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>

#define BUFFER_SIZE 5



   int main() {
       key_t k1 = ftok(".", 65);
       key_t k3 = ftok(".", 67);

       int shmid_mutex = shmget(k1, sizeof(int), 0666 | IPC_CREAT);
       int shmid_index = shmget(k3, sizeof(int), 0666 | IPC_CREAT);

       int *mutex = (int*)shmat(shmid_mutex, NULL, 0);
       int *index = (int*)shmat(shmid_index, NULL, 0);

       if (mutex == (void*)-1 || index == (void*)-1) {
           perror("shmat");
           exit(1);
       }
       *mutex = 0;
       *index = 0;

       printf("Shared memory initialized.\n");

       shmdt(mutex);
       shmdt(index);

       return 0;
   }
