// Communication between siblings using bidirectional pipes

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

int main()
{
    char s1[100], s2[100];
    pid_t id, pid;
    int id1[2], id2[2];

    if(pipe(id1)==-1)
    {
        printf("Pipe 1 failed...\n");
        return 0;
    }

    if(pipe(id2)==-1)
    {
        printf("Pipe 2 failed...\n");
        return 0;
    }

    id = fork();      // First Child

    if(id==-1)
    {
        printf("Child 1 not created...\n");
        return 0;
    }

    else if(id==0)
    {
        // Child 1

        close(id1[0]);      // Close read end of pipe1
        close(id2[1]);      // Close write end of pipe2

        char msg1[] = "This is Child 1...";

        write(id1[1], msg1, sizeof(msg1));

        read(id2[0], s1, sizeof(s1));

        printf("Child 1 reads : \"%s\" from Child 2\n", s1);

        close(id1[1]);
        close(id2[0]);

        exit(0);
    }

    else
    {
        pid = fork();       // Second Child

        if(pid==-1)
        {
            printf("Child 2 not created...\n");
            return 0;
        }

        else if(pid==0)
        {
            // Child 2

            close(id1[1]);      // Close write end of pipe1
            close(id2[0]);      // Close read end of pipe2

            char msg2[] = "This is Child 2...";

            read(id1[0], s2, sizeof(s2));

            printf("Child 2 reads : \"%s\" from Child 1\n", s2);

            write(id2[1], msg2, sizeof(msg2));

            close(id1[0]);
            close(id2[1]);

            exit(0);
        }

        else
        {
            // Parent

            close(id1[0]);
            close(id1[1]);
            close(id2[0]);
            close(id2[1]);

            wait(NULL);
            wait(NULL);

            printf("Final Parent Process...\n");
        }
    }

    return 0;
}
