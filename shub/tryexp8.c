#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>

#define MAX 5
#define ITEM 10

sem_t empty;  // empty slots
sem_t full;   // filled slots
sem_t mutex;  // protects buffer access
int buffer[MAX];
int count = 0;

int in = 0;
int out = 0;

void *Producer(void *arg) {
    for (int i = 1; i <= ITEM; i++) {
        int item = i * 10;  // Each item is a number like 10, 20, 30...

        printf("Producer item generated\n");		
        sem_wait(&empty);  // Wait until there is an empty slot
        sem_wait(&mutex);  // Lock buffer before modifying it

        buffer[in] = item;                // Insert item at current write index
        printf("Producer item inserted- %d\n", item);
        in = (in + 1) % MAX;             // Move circular buffer write pointer
        count++;                         // Increase number of items in buffer

        sem_post(&mutex);  // Unlock buffer
        sem_post(&full);   // Notify that one item is available to consume
        sleep(1);          // Slow down so output is readable
    }
    return NULL;
}

void *Consumer(void *arg) {
    int item;

    for (int i = 1; i <= ITEM; i++) {
        sem_wait(&full);   // Wait until there is something to consume
        sem_wait(&mutex);  // Lock buffer before reading it

        item = buffer[out];              // Read item from current read index
        printf("Consumer had consumed- %d\n", item);
        out = (out + 1) % MAX;           // Move circular buffer read pointer
        count--;                         // Reduce number of items in buffer

        sem_post(&mutex);  // Unlock buffer
        sem_post(&empty); // Notify producer that one empty slot is available
        printf("Consumer item delivered\n");
        sleep(1); // Slow down so output is readable
    }
    return NULL;
}

int main() {
    pthread_t prod;
    pthread_t cons;

    sem_init(&empty, 0, MAX);  // 2nd arg = 0 means threads in same process share it
    sem_init(&full, 0, 0);
    sem_init(&mutex, 0, 1);

    pthread_create(&prod, NULL, Producer, NULL);
    pthread_create(&cons, NULL, Consumer, NULL);

    pthread_join(prod, NULL);  // Wait for producer thread to finish
    pthread_join(cons, NULL);  // Wait for consumer thread to finish

    printf("Orders Remaining to be delivered: %d\n", count);

    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&mutex);
}


