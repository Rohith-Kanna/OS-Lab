#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>

#define BUFFER_SIZE 5

int main() {
    key_t k1 = ftok(".", 65);
    key_t k2 = ftok(".", 66);
    key_t k3 = ftok(".", 67);

    int shmid_mutex = shmget(k1, sizeof(int), 0666 | IPC_CREAT);
    int shmid_buffer = shmget(k2, BUFFER_SIZE * sizeof(int), 0666 | IPC_CREAT);
    int shmid_index = shmget(k3, sizeof(int), 0666 | IPC_CREAT);

    int *mutex = (int*)shmat(shmid_mutex, NULL, 0);
    int *buffer = (int*)shmat(shmid_buffer, NULL, 0);
    int *index = (int*)shmat(shmid_index, NULL, 0);

    if (mutex == (void*)-1 || buffer == (void*)-1 || index == (void*)-1) {
        perror("shmat");
        exit(1);
    }

    // Initialize shared variables if first time
    // (Optional: check if first time, or just set to 0)
    *mutex = 0;
    *index = 0;

    int choice, value;

    printf("\n--- PRODUCER ---\n");
    do {
        if (*mutex == 0) {
            *mutex = 1;  // Lock
            if (*index < BUFFER_SIZE) {
                printf("Enter value: ");
                scanf("%d", &value);
                buffer[*index] = value;
                (*index)++;
                printf("Produced. Buffer size: %d\n", *index);
            } else {
                printf("Buffer FULL!\n");
            }
            *mutex = 0; // Unlock
        } else {
            printf("Consumer is working... Try later\n");
        }

        printf("Continue? (1/0): ");
        scanf("%d", &choice);
    } while (choice != 0);

    shmdt(mutex);
    shmdt(buffer);
    shmdt(index);
    return 0;
}
