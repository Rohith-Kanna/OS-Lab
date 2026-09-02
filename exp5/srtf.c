#include <stdio.h>

struct Process
{
    int pid, at, cpu1, io, cpu2;
    int rem1, rem2;
    int io_done;
    int ct, tat, wt, rt;
    int first, done;
};

void srtf(struct Process p[], int n)
{
    int time = 0, completed = 0;
    int current, best;
    int i;

    for(i = 0; i < n; i++)
    {
        p[i].rem1 = p[i].cpu1;
        p[i].rem2 = p[i].cpu2;
        p[i].io_done = 0;
        p[i].first = -1;
        p[i].done = 0;
    }

    printf("\nGantt Chart:\n");

    while(completed < n)
    {
        best = -1;

        /* Find process with shortest remaining time */
        for(i = 0; i < n; i++)
        {
            if(!p[i].done && p[i].at <= time)
            {
                if(p[i].rem1 > 0)
                {
                    if(best == -1 ||
                       p[i].rem1 < p[best].rem1)
                        best = i;
                }
                else if(p[i].rem2 > 0 &&
                        p[i].io_done <= time)
                {
                    if(best == -1 ||
                       p[i].rem2 < p[best].rem2)
                        best = i;
                }
            }
        }

        /* No process available */
        if(best == -1)
        {
            time++;
            continue;
        }

        current = best;

        /* First CPU response */
        if(p[current].first == -1)
            p[current].first = time;

        /* Execute 1 unit */
        if(p[current].rem1 > 0)
            p[current].rem1--;
        else
            p[current].rem2--;

        printf("P%d ", p[current].pid);

        time++;

        /* CPU1 finished → I/O */
        if(p[current].rem1 == 0 &&
           p[current].io_done == 0)
        {
            p[current].io_done =
                time + p[current].io;
        }

        /* CPU2 finished → DONE */
        if(p[current].rem1 == 0 &&
           p[current].rem2 == 0 &&
           time >= p[current].io_done)
        {
            p[current].ct = time;

            p[current].tat =
                p[current].ct - p[current].at;

            p[current].wt =
                p[current].tat
                - p[current].cpu1
                - p[current].io
                - p[current].cpu2;

            p[current].rt =
                p[current].first - p[current].at;

            p[current].done = 1;
            completed++;
        }
    }

    printf("\n\nProcess\tAT\tCPU1\tIO\tCPU2\tCT\tTAT\tWT\tRT\n");

    for(i = 0; i < n; i++)
    {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid,
               p[i].at,
               p[i].cpu1,
               p[i].io,
               p[i].cpu2,
               p[i].ct,
               p[i].tat,
               p[i].wt,
               p[i].rt);
    }
}

int main()
{
    struct Process p[100];
    int n, i;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    for(i = 0; i < n; i++)
    {
        p[i].pid = i + 1;

        printf("\nP%d AT: ", i + 1);
        scanf("%d", &p[i].at);

        printf("P%d CPU1: ", i + 1);
        scanf("%d", &p[i].cpu1);

        printf("P%d IO: ", i + 1);
        scanf("%d", &p[i].io);

        printf("P%d CPU2: ", i + 1);
        scanf("%d", &p[i].cpu2);
    }

    srtf(p, n);

    return 0;
}