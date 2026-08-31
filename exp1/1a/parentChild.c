#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    pid_t pid;

    pid = fork();

    if(pid == -1)
    {
        printf("Fork Failed\n");
    }
    else if(pid == 0)
    {
        printf("\n----- Child Process -----\n");
        printf("Child PID : %d\n", getpid());
        printf("Child's Parent PID : %d\n", getppid());
    }
    else
    {
        printf("\n----- Parent Process -----\n");
        printf("Parent PID : %d\n", getpid());
        printf("Parent's Parent PID : %d\n", getppid());

        wait(NULL);
    }

    return 0;
}
