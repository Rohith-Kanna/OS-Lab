// client 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct M2
{
    long no;
    float amnt;
} m;
int main()
{
    key_t k;
    int mqid;
    long uk;
    char s1[2], s2[3] = "n";
    // key = ftok("progfile", 65);

    k = 126014035;
    mqid = msgget(k, 0666 | IPC_CREAT);
    do
    {
        printf("Enter the msg type : ");
        scanf("%ld", &uk);
        if (msgrcv(mqid, &m, sizeof(m), uk, 0) == -1)
        {
            printf("\nNo data present for the entered msgid %ld\n", uk);
        }
        else
        {
            printf("\nThe client of no : %ld \nShould pay : %f\n", m.no, m.amnt);
        }
        printf("One more needed (y/n) : ");
        scanf("%s", s1);
    } while (strcmp(s1, s2) != 0);

    msgctl(mqid, IPC_RMID, 0);

    return 0;
}
