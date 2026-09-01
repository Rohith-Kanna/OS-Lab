#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
    int fd1[2]; // Parent -> Child
    int fd2[2]; // Child -> Parent

    char fixed_str[] = "Happy ";
    char input_str[100];

    if (pipe(fd1) == -1)
    {
        fprintf(stderr, "Pipe Failed");
        return 1;
    }

    if (pipe(fd2) == -1)
    {
        fprintf(stderr, "Pipe Failed");
        return 1;
    }

    printf("Enter a word : ");
    scanf("%s", input_str);

    pid_t p = fork();

    if (p < 0)
    {
        fprintf(stderr, "Fork Failed");
        return 1;
    }

    // Parent Process
    else if (p > 0)
    {
        char concat_str[100];

        close(fd1[0]); // Close reading end of first pipe
        write(fd1[1], input_str, strlen(input_str) + 1); // +1 because

        close(fd1[1]);

        wait(NULL); //wait till child process completes

        close(fd2[1]);
        read(fd2[0], concat_str, 100);
        printf("Concatenated String : %s\n", concat_str);
        close(fd2[0]);
    }

    // Child Process
    else
    {
        close(fd1[1]);// Close writing end of first pipe

        char concat_str[100];

        read(fd1[0], concat_str, 100);

        int k = strlen(concat_str);

        for (int i = 0; i < strlen(fixed_str); i++)
            concat_str[k++] = fixed_str[i];

        concat_str[k] = '\0';

        close(fd1[0]);

        close(fd2[0]);

        write(fd2[1], concat_str, strlen(concat_str) + 1);

        close(fd2[1]);
    }

    return 0;
}
