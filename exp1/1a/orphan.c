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

        exit(0);      // Parent exits immediately, child becomes orphan
    }
    else if(pid == 0)
    {
        sleep(5);     // Child waits

        printf("\nChild Process\n");
        printf("Child PID : %d\n", getpid());
        printf("New Parent PID : %d\n", getppid());
    }

    return 0;
}
