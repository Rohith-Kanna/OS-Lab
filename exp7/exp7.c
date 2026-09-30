#include <stdio.h>
#include <pthread.h>

#define MAX 10

int n;
int flag[MAX];
int turn[MAX];
int shared = 0;

void lock(int i)
{
    int j;

    flag[i] = 1;
    turn[i] = 1;

    for(j = 0; j < n; j++)
    {
        if(j != i)
        {
            turn[j] = i;
            while(flag[j] && turn[j] == i);
        }
    }
}

void unlock(int i)
{
    flag[i] = 0;
}

void *process(void *arg)
{
    int i = *(int *)arg;
    int k;

    for(k = 0; k < 3; k++)
    {
        lock(i);

        printf("Process %d entered critical section\n", i);
        shared++;
        printf("Process %d: Shared value = %d\n", i, shared);
        printf("Process %d left critical section\n\n", i);

        unlock(i);
    }

    return NULL;
}

int main()
{
    pthread_t threads[MAX];
    int id[MAX];
    int i;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    for(i = 0; i < n; i++)
    {
        flag[i] = 0;
        turn[i] = 0;
        id[i] = i;

        pthread_create(&threads[i], NULL, process, &id[i]);
    }

    for(i = 0; i < n; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("Final shared value = %d\n", shared);

    return 0;
}