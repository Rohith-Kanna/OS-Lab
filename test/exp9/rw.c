#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>
#define SHMSZ 1024
int main()
{
    key_t key1, key2;
    int shmid;
    int sec_id;
    char *shm, *k, *s; // Data
    char *ssm, *j, *t;
    key2 = 3400; // Pattern
    key1 = 3415; // Data
    if ((shmid = shmget(key1, SHMSZ, IPC_CREAT | 0666)) < 0)
    {
        perror("shmget");
        exit(1);
    }
    if ((shm = shmat(shmid, NULL, 0)) == (char *)-1)
    {
        perror("shmat");
        exit(1);
    }
    //***************************************************************
    if ((sec_id = shmget(key2, SHMSZ, IPC_CREAT | 0666)) < 0)
    {
        perror("shmget");
        exit(1);
    }
    if ((ssm = shmat(sec_id, NULL, 0)) == (char *)-1)
    {
        perror("shmat");
        exit(1);
    }
    t = ssm;
    s = shm;
    char pattern[10];
    printf("\nEnter the pattern in caps: ");
    fgets(pattern, 10, stdin); // Will accept till ENTER key is hit...
    int len = strlen(pattern); // Corrected length calculation
    for (int i = 0; i < len; i++)
    {
        *t = pattern[i];
        *s = pattern[i];
        t++;
        s++;
    }
    // Print the pattern written to shared memory
    printf("\nPattern written to shm mem of main : %s\nPattern written to wwp & rwp shm mem:", ssm);
    for (char *i = shm; *i != '\0'; i++)
        printf("%c", *i);
    putchar('\n');
    int choice;
    printf("1. READER PRIORITY\n2. WRITER PRIORITY\n3. BASED ON FIRST\n4. AS PER GIVEN\n");
    printf("Enter the priority (1/2/3/4): ");
    scanf("%d", &choice);
    if (choice == 1)
    {
        // Reader Priority
        char first = *ssm;
        printf("\nThe first character is: %c", first);
        int rcount = 0;
        int wcount = 0;
        for (k = ssm; *k != '\0'; k++)
        {
            if (*k == 'R')
                rcount++;
            else if (*k == 'W')
                wcount++;
        }
        printf("\nThe no. of readers is: %d", rcount);
        printf("\nThe no. of writers is: %d\n", wcount);
        for (int i = 0; i < rcount; i++)
        {
            system("gnome-terminal -- ./r.out");
            sleep(5);
        }
        for (int i = 0; i < wcount; i++)
        {
            system("gnome-terminal -- ./w.out");
            sleep(10);
        }
    }
    else if (choice == 2)
    {
        // Writer Priority
        char first = *ssm;
        printf("\nThe first character is: %c", first);
        int rcount = 0;
        int wcount = 0;
        for (k = ssm; *k != '\0'; k++)
        {
            if (*k == 'R')
                rcount++;
            else if (*k == 'W')
                wcount++;
        }
        printf("\nThe no. of readers is: %d", rcount);
        printf("\nThe no. of writers is: %d\n", wcount);
        // Open writer and reader processes in separate terminals
        for (int i = 0; i < wcount; i++)
        {
            system("gnome-terminal -- ./w.out");
            sleep(10);
        }
        for (int i = 0; i < rcount; i++)
        {
            system("gnome-terminal -- ./r.out");
            sleep(5);
        }
    }
    else if (choice == 3)
    {
        // based on first
        char first = *ssm;
        printf("\nThe first character is: %c", first);
        char *h;
        h = ssm;
        if (*h == 'R')
        {
            int rcount = 0;
            int wcount = 0;
            for (k = h; *k != '\0'; k++)
            {
                if (*k == 'R')
                    rcount++;
                else if (*k == 'W')
                    wcount++;
            }
            printf("\nThe no. of readers is: %d", rcount);
            printf("\nThe no. of writers is: %d\n", wcount);
            sleep(10);
            for (int i = 0; i < rcount; i++)
            {
                system("gnome-terminal -- ./r.out");
                sleep(5);
            }
            for (int i = 0; i < wcount; i++)
            {
                system("gnome-terminal -- ./w.out");
                sleep(10);
            }
        }
        else if (*h == 'W')
        {
            int rcount = 0;
            int wcount = 0;
            for (k = h; *k != '\0'; k++)
            {
                if (*k == 'R')
                    rcount++;
                else if (*k == 'W')
                    wcount++;
            }
            printf("\nThe no. of readers is: %d", rcount);
            printf("\nThe no. of writers is: %d\n", wcount);
            sleep(5);
            for (int i = 0; i < wcount; i++)
            {
                system("gnome-terminal -- ./w.out");
                sleep(10);
            }
            for (int i = 0; i < rcount; i++)
            {
                system("gnome-terminal -- ./r.out");
                sleep(5);
            }
        }
    }
    else if (choice == 4)
    {
        // as per given
        char f = *ssm;
        char *h = ssm;
        int rcount = 0;
        int wcount = 0;
        for (k = h; *k != '\0'; k++)
        {
            if (*k == 'R')
                rcount++;
            else if (*k == 'W')
                wcount++;
        }
        printf("\nThe first character is : %c", f);
        printf("\nThe no. of readers is: %d", rcount);
        printf("\nThe no. of writers is: %d\n", wcount);
        while (*h != '\0')
        {
            if (*h == 'R')
            {
                system("gnome-terminal -- ./r.out");
                sleep(5);
            }
            else if (*h == 'W')
            {
                system("gnome-terminal -- ./w.out");
                sleep(10);
            }
            h++;
        }
    }
    else
    {
        printf("Invalid choice.\n");
    }
    // Detach and remove shared memory segments
    shmdt(shm);
    shmdt(ssm);
    shmctl(shmid, IPC_RMID, NULL);
    shmctl(sec_id, IPC_RMID, NULL);
    return 0;
}
