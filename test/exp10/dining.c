// to excute and run use:
//  gcc dining.c -o dining -pthread
//  ./dining

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_PHILOSOPHERS 5
#define RUN_TIME 5 // Simulation run time in seconds

sem_t forks[NUM_PHILOSOPHERS];

void *philosopher(void *num)
{
    int id = *(int *)num;

    // Run the simulation for a limited time
    int cycles = 2;
    while (cycles > 0)
    {
        printf("Philosopher %d is thinking.\n", id);
        sleep(1);

        printf("Philosopher %d is hungry and wants to take forks.\n", id);

        // Deadlock prevention logic (Asymmetric approach)
        if (id % 2 == 0)
        {
            // Even philosophers pick up Left then Right
            sem_wait(&forks[id]);
            sem_wait(&forks[(id + 1) % NUM_PHILOSOPHERS]);
        }
        else
        {
            // Odd philosophers pick up Right then Left
            sem_wait(&forks[(id + 1) % NUM_PHILOSOPHERS]);
            sem_wait(&forks[id]);
        }

        printf("Philosopher %d took both forks and is eating.\n", id);
        sleep(2); // Simulating eating

        // Put down forks
        sem_post(&forks[id]);
        sem_post(&forks[(id + 1) % NUM_PHILOSOPHERS]);

        printf("Philosopher %d released both forks.\n\n", id);
        cycles--;
    }

    printf("Philosopher %d has finished their meals.\n", id);
    return NULL;
}

int main()
{
    pthread_t thread_id[NUM_PHILOSOPHERS];
    int phil_ids[NUM_PHILOSOPHERS];

    // Initialize the semaphores for forks
    for (int i = 0; i < NUM_PHILOSOPHERS; i++)
    {
        sem_init(&forks[i], 0, 1);
    }

    // Create philosopher threads
    for (int i = 0; i < NUM_PHILOSOPHERS; i++)
    {
        phil_ids[i] = i;
        pthread_create(&thread_id[i], NULL, philosopher, &phil_ids[i]);
    }

    // Wait for all philosophers to finish
    for (int i = 0; i < NUM_PHILOSOPHERS; i++)
    {
        pthread_join(thread_id[i], NULL);
    }

    // Destroy semaphores
    for (int i = 0; i < NUM_PHILOSOPHERS; i++)
    {
        sem_destroy(&forks[i]);
    }

    printf("\nSimulation complete. All philosophers ate safely without deadlock!\n");
    return 0;
}
