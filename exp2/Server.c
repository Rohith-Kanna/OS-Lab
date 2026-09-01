#include <stdio.h>

#include <stdlib.h>
#include <string.h>

#include <unistd.h> // For fork() and other system calls
#include <sys/wait.h> // For wait() to wait for child process
#include <sys/shm.h> // For shared memory functions
#include <sys/ipc.h> // For IPC functions

#define SIZE 1024
 
int main()
{
    key_t key = 1234;
    int shmid;
    char *str;
    pid_t id;

    shmid = shmget(key, 1024, 0666 | IPC_CREAT);

    if(shmid == -1)
    {
        printf("Shared memory creation failed\n");
        return 0;
    }

    str = (char *)shmat(shmid, NULL, 0);

    if(str == (char *)-1)
    {
        printf("Shared memory attachment failed\n");
        return 0;
    }

    id = fork();

    if(id == -1)
    {
        printf("Child creation failed\n");
        return 0;
    }

    if(id == 0)
    {
        // Child
        char *cp = str;

        printf("Enter lines (type exit to stop):\n");

        while(fgets(cp, SIZE - (cp - str), stdin) != NULL)
        {
            if(strcmp(cp, "exit\n") == 0)
                break;

            cp += strlen(cp);
        }

        printf("\nSize of the given line: %ld\n", strlen(str));
    }
    else
    {
        // Parent
        wait(NULL);
        printf("\nParent after child completion...\n");
    }

    shmdt(str);

    return 0;
}