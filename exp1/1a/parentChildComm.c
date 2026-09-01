#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

int main()
{
    int pipe_fd[2];
    char msg[100];

    pipe(pipe_fd);

    pid_t pid = fork();

    if(pid == 0) //child
    {
        close(pipe_fd[1]);        // Close write end

        read(pipe_fd[0], msg, sizeof(msg));

        printf("Child received : %s\n", msg);

        close(pipe_fd[0]);
    }
    else //parent
    {
        close(pipe_fd[0]);        // Close read end

        char text[] = "Hello from Parent";

        write(pipe_fd[1], text, sizeof(text));

        close(pipe_fd[1]);
    }

    return 0;
}
