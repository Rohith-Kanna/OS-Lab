// server1
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct M1
{
    long no;
    float amnt;
} m;
int main()
{
    key_t k;
    int mqid;
    char s1[5], s2[5] = "n";
    // k = ftok("progfile", 65);

    k = 126014035;
    mqid = msgget(k, 0666 | IPC_CREAT);
    if (k == -1)
    {
        printf("unique key not created...\n");
        return 0;
    }
    if (mqid == -1)
    {
        printf("msg id not created...\n");
        return 0;
    }
    do
    {
        printf("Enter no & amnt 4 msg Q : ");
        scanf("%ld%f", &m.no, &m.amnt);
        msgsnd(mqid, &m, sizeof(m), 0 /*this is flag*/);
        printf("One more need (y/n) : ");
        scanf("%s", s1);
    } while (strcmp(s1, s2) != 0); // 0 = same

    // msgctl(mqid,IPC_RMID,0);

    return 0;
}
