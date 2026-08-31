#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    pid_t pid;

    pid = fork();

    if(pid > 0)
    {
        printf("Parent Process\n");
        printf("Parent PID : %d\n", getpid());

        sleep(10);     // Parent waits
    }
    else if(pid == 0)
    {
        printf("Child Process\n");
        printf("Child PID : %d\n", getpid());

        exit(0);       // Child exits immediately
    }

    return 0;
}
