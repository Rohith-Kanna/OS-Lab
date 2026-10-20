#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define READER 5
#define WRITER 9

sem_t resource, rmutex, wmutex, readTry;

int readerCount = 0, writerCount = 0;
int shared_data = 0, mode;

void *Reader(void *arg)
{
    int id = *(int *)arg;

    if (mode == 1) //reader priority
    {
        sem_wait(&rmutex); // Lock the reader count mutex
        readerCount++;
        if (readerCount == 1)
            sem_wait(&resource); // If this is the first reader, lock the resource
        sem_post(&rmutex); // Unlock the reader count mutex
    } 
    else //writer priority
    {
        sem_wait(&readTry); //
        sem_wait(&rmutex);
        readerCount++;
        if (readerCount == 1)
            sem_wait(&resource);
        sem_post(&rmutex);
        sem_post(&readTry);
    }

    printf("Reader %d reading: %d\n", id, shared_data);
    sleep(1);

//reader exiting
    sem_wait(&rmutex);
    readerCount--;
    if (readerCount == 0)
        sem_post(&resource);
    sem_post(&rmutex);

    return NULL;
}

void *Writer(void *arg)
{
    int id = *(int *)arg;

    if (mode == 2) //writer priority
    {
        sem_wait(&wmutex);
        writerCount++;
        if (writerCount == 1)
            sem_wait(&readTry);
        sem_post(&wmutex);
    }

    sem_wait(&resource); // in both priorities, writer needs to lock the resource

    shared_data += 10;
    printf("Writer %d wrote: %d\n", id, shared_data);
    sleep(1);

    sem_post(&resource); //release the resource

    if (mode == 2)
    {
        sem_wait(&wmutex);
        writerCount--;
        if (writerCount == 0)
            sem_post(&readTry);
        sem_post(&wmutex);
    }

    return NULL;
}

int main()
{
    pthread_t r[READER], w[WRITER];
    int rid[READER], wid[WRITER];

    printf("1. Reader priority\n2. Writer priority\nChoice: ");
    scanf("%d", &mode); //  using & because scanf needs the address of the variable to store the input value

    if (mode != 1 && mode != 2)
        return 1;

    sem_init(&resource, 0, 1); //var, 0= same process threads, 1=initial value
    sem_init(&rmutex, 0, 1);
    sem_init(&wmutex, 0, 1);
    sem_init(&readTry, 0, 1);

    for (int i = 0; i < READER; i++)
    {
        rid[i] = i + 1;
        pthread_create(&r[i], NULL, Reader, &rid[i]); //thread, attr, function, arg
    }

    for (int i = 0; i < WRITER; i++)
    {
        wid[i] = i + 1;
        pthread_create(&w[i], NULL, Writer, &wid[i]);
    }

    for (int i = 0; i < READER; i++)
        pthread_join(r[i], NULL);//thread, return value

    for (int i = 0; i < WRITER; i++)
        pthread_join(w[i], NULL);

    printf("Final shared data = %d\n", shared_data);

    sem_destroy(&resource);
    sem_destroy(&rmutex);
    sem_destroy(&wmutex);
    sem_destroy(&readTry);

    return 0;
}