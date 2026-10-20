#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define KEY 1214
#define R 5
#define W 9

struct Shared
{
    int shared, rc, wc, mode;
    sem_t res, rmut, wmut, rTry, start, readerDone;
};

struct Shared *m;

void *Writer(void *arg)
{
    int id = *(int *)arg;

    if (m->mode == 2)
    {
        sem_wait(&m->wmut);
        m->wc++;

        if (m->wc == 1)
            sem_wait(&m->rTry);

        sem_post(&m->wmut);
    }

    sem_wait(&m->res);

    m->shared += 10;
    printf("Writer %d wrote %d\n", id, m->shared);

    sem_post(&m->res);

    if (m->mode == 2)
    {
        sem_wait(&m->wmut);
        m->wc--;

        if (m->wc == 0)
            sem_post(&m->rTry);

        sem_post(&m->wmut);
    }

    return NULL;
}

int main()
{
    int shmid;
    pthread_t w[W];
    int wid[W];

    shmid = shmget(KEY, sizeof(struct Shared),
                   IPC_CREAT | IPC_EXCL | 0666);

    if (shmid == -1)
    {
        perror("shmget");
        return 1;
    }

    m = shmat(shmid, NULL, 0);

    if (m == (void *)-1)
    {
        perror("shmat");
        shmctl(shmid, IPC_RMID, NULL);
        return 1;
    }

    printf("1. Reader priority\n2. Writer priority\nChoice: ");
    if (scanf("%d", &m->mode) != 1 ||
        (m->mode != 1 && m->mode != 2))
    {
        shmdt(m);
        shmctl(shmid, IPC_RMID, NULL);
        return 1;
    }

    m->shared = 0;
    m->rc = 0;
    m->wc = 0;

    sem_init(&m->res, 1, 1);
    sem_init(&m->rmut, 1, 1);
    sem_init(&m->wmut, 1, 1);
    sem_init(&m->rTry, 1, 1);
    sem_init(&m->start, 1, 0);
    sem_init(&m->readerDone, 1, 0);

    printf("Shared memory ready. Run reader in Terminal 2.\n");
    fflush(stdout);

    sem_wait(&m->start);

    for (int i = 0; i < W; i++)
    {
        wid[i] = i + 1;
        pthread_create(&w[i], NULL, Writer, &wid[i]);
    }

    for (int i = 0; i < W; i++)
        pthread_join(w[i], NULL);

    sem_wait(&m->readerDone);

    printf("Final shared value = %d\n", m->shared);

    sem_destroy(&m->res);
    sem_destroy(&m->rmut);
    sem_destroy(&m->wmut);
    sem_destroy(&m->rTry);
    sem_destroy(&m->start);
    sem_destroy(&m->readerDone);

    shmdt(m);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}