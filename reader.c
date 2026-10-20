#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define KEY 1214
#define R 5

struct Shared
{
    int shared, rc, wc, mode;
    sem_t res, rmut, wmut, rTry, start, readerDone;
};

struct Shared *m;

void *Reader(void *arg)
{
    int id = *(int *)arg;

    if (m->mode == 2)
        sem_wait(&m->rTry);

    sem_wait(&m->rmut);
    m->rc++;

    if (m->rc == 1)
        sem_wait(&m->res);

    sem_post(&m->rmut);

    if (m->mode == 2)
        sem_post(&m->rTry);

    printf("Reader %d reading %d\n", id, m->shared);
    sleep(1);

    sem_wait(&m->rmut);
    m->rc--;

    if (m->rc == 0)
        sem_post(&m->res);

    sem_post(&m->rmut);

    return NULL;
}

int main()
{
    int shmid;
    pthread_t r[R];
    int rid[R];

    shmid = shmget(KEY, sizeof(struct Shared), 0666);

    if (shmid == -1)
    {
        perror("Run writer first");
        return 1;
    }

    m = shmat(shmid, NULL, 0);

    if (m == (void *)-1)
    {
        perror("shmat");
        return 1;
    }

    for (int i = 0; i < R; i++)
    {
        rid[i] = i + 1;
        pthread_create(&r[i], NULL, Reader, &rid[i]);
    }

    sem_post(&m->start);

    for (int i = 0; i < R; i++)
        pthread_join(r[i], NULL);

    printf("Readers finished\n");

    sem_post(&m->readerDone);
    shmdt(m);

    return 0;
}