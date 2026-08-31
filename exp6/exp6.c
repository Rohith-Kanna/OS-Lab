#include <stdio.h>

struct Process
{
    int pid;
    int at;
    int bt;
    int rem;
    int ct;
    int tat;
    int wt;
};

int main()
{
    struct Process p[20];
    int n;
    int q1, q2;
    int time = 0;
    int completed = 0;
    int i, j;
    int done;
    float avg_tat = 0, avg_wt = 0;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    printf("\nEnter Arrival Time and CPU Burst Time:\n");

    for(i = 0; i < n; i++)
    {
        p[i].pid = i + 1;

        printf("P%d Arrival Time: ", i + 1);
        scanf("%d", &p[i].at);

        printf("P%d Burst Time: ", i + 1);
        scanf("%d", &p[i].bt);

        p[i].rem = p[i].bt;
    }

    printf("\nEnter Time Quantum for Queue 1: ");
    scanf("%d", &q1);

    printf("Enter Time Quantum for Queue 2: ");
    scanf("%d", &q2);

    printf("\n========== MULTILEVEL FEEDBACK QUEUE ==========\n");

    printf("\nQueue 1: Round Robin, Time Quantum = %d", q1);
    printf("\nQueue 2: Round Robin, Time Quantum = %d", q2);
    printf("\nQueue 3: FCFS\n");

    printf("\nGantt Chart:\n");

    /* Queue 1 */

    for(i = 0; i < n; i++)
    {
        if(p[i].at <= time && p[i].rem > 0)
        {
            int run = q1;

            if(p[i].rem < run)
                run = p[i].rem;

            printf("| P%d ", p[i].pid);

            time = time + run;
            p[i].rem = p[i].rem - run;
        }
    }

    /* Queue 2 */

    for(i = 0; i < n; i++)
    {
        if(p[i].rem > 0 && p[i].at <= time)
        {
            int run = q2;

            if(p[i].rem < run)
                run = p[i].rem;

            printf("| P%d ", p[i].pid);

            time = time + run;
            p[i].rem = p[i].rem - run;
        }
    }

    /* Queue 3 - FCFS */

    for(i = 0; i < n; i++)
    {
        if(p[i].rem > 0)
        {
            printf("| P%d ", p[i].pid);

            time = time + p[i].rem;
            p[i].rem = 0;

            p[i].ct = time;
            p[i].tat = p[i].ct - p[i].at;
            p[i].wt = p[i].tat - p[i].bt;

            completed++;
        }
    }

    printf("|\n");

    printf("\nProcess\tAT\tBT\tCT\tTAT\tWT\n");

    for(i = 0; i < n; i++)
    {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid,
               p[i].at,
               p[i].bt,
               p[i].ct,
               p[i].tat,
               p[i].wt);

        avg_tat = avg_tat + p[i].tat;
        avg_wt = avg_wt + p[i].wt;
    }

    printf("\nAverage Turnaround Time = %.2f", avg_tat / n);
    printf("\nAverage Waiting Time = %.2f", avg_wt / n);

    printf("\nThroughput = %.2f processes/unit time\n",
           (float)n / time);

    return 0;
}