#include <stdio.h>

struct Process
{
    int pid;
    int at;
    int bt;
    int ct;
    int tat;
    int wt;
};

void fcfs(struct Process p[], int n)
{
    int i;
    int time = 0;
    float avg_tat = 0, avg_wt = 0;

    printf("\n========== FCFS ==========\n");

    for (i = 0; i < n; i++)
    {
        if (time < p[i].at)
        {
            time = p[i].at;
        }

        time = time + p[i].bt;
        p[i].ct = time;
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;

        avg_tat = avg_tat + p[i].tat; // calc sum here
        avg_wt = avg_wt + p[i].wt;    // sum
    }

    printf("\nGantt Chart:\n");

    time = 0;
    printf("0");

    for (i = 0; i < n; i++)
    {
        if (time < p[i].at)
            time = p[i].at;

        time += p[i].bt;
        printf(" -- P%d -- %d", p[i].pid, time);
    }

    printf("\n\n");

    printf("Process\tAT\tBT\tCT\tTAT\tWT\n");

    for (i = 0; i < n; i++)
    {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid,
               p[i].at,
               p[i].bt,
               p[i].ct,
               p[i].tat,
               p[i].wt);
    }

    printf("\nAverage Turnaround Time = %.2f", avg_tat / n);
    printf("\nAverage Waiting Time = %.2f\n", avg_wt / n);
}

// sjf - shortest job first
void sjf(struct Process p[], int n)
{
    int completed[100] = {0};
    int completed_count = 0;
    int time = 0;
    int i, j;
    int index;
    float avg_tat = 0, avg_wt = 0;

    printf("\n========== SJF ==========\n");

    printf("\nGantt Chart:\n");

    printf("0");

    while (completed_count < n)
    {
        index = -1; // index of the process with the shortest burst time

        for (i = 0; i < n; i++)
        {
            if (completed[i] == 0 && p[i].at <= time)
            {
                if (index == -1 || p[i].bt < p[index].bt)
                {
                    index = i;
                }
            }
        }

        if (index == -1)
        {
            time++;

            printf("    | Idle |    %d", time);
        }
        else
        {
            time = time + p[index].bt;

            p[index].ct = time;
            p[index].tat = p[index].ct - p[index].at;
            p[index].wt = p[index].tat - p[index].bt;

            completed[index] = 1;
            completed_count++;

            printf("    | P%d |    %d", p[index].pid, time);

            avg_tat = avg_tat + p[index].tat;
            avg_wt = avg_wt + p[index].wt;
        }
    }

    printf("\n\n");

    printf("Process\tAT\tBT\tCT\tTAT\tWT\n");

    for (i = 0; i < n; i++)
    {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid,
               p[i].at,
               p[i].bt,
               p[i].ct,
               p[i].tat,
               p[i].wt);
    }

    printf("\nAverage Turnaround Time = %.2f", avg_tat / n);
    printf("\nAverage Waiting Time = %.2f\n", avg_wt / n);
}

int main()
{
    struct Process p[100];
    struct Process fcfs_p[100];
    struct Process sjf_p[100];

    int n;
    int i;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    printf("\nEnter Arrival Time and CPU Burst Time:\n");

    for (i = 0; i < n; i++)
    {
        p[i].pid = i + 1;

        printf("P%d Arrival Time: ", i + 1);
        scanf("%d", &p[i].at);

        printf("P%d Burst Time: ", i + 1);
        scanf("%d", &p[i].bt);

        fcfs_p[i] = p[i];
        sjf_p[i] = p[i];
    }

    fcfs(fcfs_p, n);

    sjf(sjf_p, n);

    return 0;
}