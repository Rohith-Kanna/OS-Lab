#include <stdio.h>

struct Process
{
    int pid, at, cpu1, io, cpu2;
    int rem1, rem2;
    int io_done;
    int ct, tat, wt, rt;
    int first;
    int done;
};

void rr(struct Process p[], int n, int q)
{
    int queue[100], front = 0, rear = 0;
    int time = 0, completed = 0;
    int current = -1, qtime = 0; //qtime is the time spent on the current process in the current quantum
    int added[100] = {0};
    int i;

    for(i = 0; i < n; i++)
    {
        p[i].rem1 = p[i].cpu1;
        p[i].rem2 = p[i].cpu2;
        p[i].io_done = 0;
        p[i].done = 0;
        p[i].first = -1;
    }

    printf("\nGantt Chart:\n");

    while(completed < n)
    {
        /* Add newly arrived processes */
        for(i = 0; i < n; i++)
        {
            if(!added[i] && p[i].at <= time)
            {
                queue[rear++] = i;
                added[i] = 1; //prevent adding the same process again
            }
        }

        /* Add processes whose I/O is finished */
        for(i = 0; i < n; i++)
        {
            if(!p[i].done && p[i].rem1 == 0 &&
               p[i].io_done > 0 && p[i].io_done <= time)
            {
                if(p[i].rem2 > 0)
                {
                    queue[rear++] = i;
                    p[i].io_done = -1;
                }
            }
        }

        /* Get next process */
        if(current == -1)
        {
            if(front < rear)
            {
                current = queue[front++];
                qtime = 0;

                if(p[current].first == -1)
                    p[current].first = time;
            }
            else
            {
                time++;
                continue;
            }
        }

        /* Execute for 1 unit */
        if(p[current].rem1 > 0)
            p[current].rem1--;
        else
            p[current].rem2--;

        time++;
        qtime++;

        /* CPU1 finished -> I/O */
        if(p[current].rem1 == 0 && p[current].io_done == 0)
        {
            p[current].io_done = time + p[current].io;
            current = -1;
            qtime = 0;
        }

        /* CPU2 finished -> process complete */
        else if(p[current].rem2 == 0)
        {
            p[current].ct = time;
            p[current].tat = p[current].ct - p[current].at;

            p[current].wt = p[current].tat
                          - p[current].cpu1
                          - p[current].io
                          - p[current].cpu2;

            p[current].rt = p[current].first - p[current].at;

            p[current].done = 1;
            completed++;

            current = -1;
            qtime = 0;
        }

        /* Quantum finished */
        else if(qtime == q)
        {
            queue[rear++] = current;
            current = -1;
            qtime = 0;
        }
    }

    printf("\n\nProcess\tAT\tCPU1\tIO\tCPU2\tCT\tTAT\tWT\tRT\n");

    for(i = 0; i < n; i++)
    {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
            p[i].pid, p[i].at, p[i].cpu1, p[i].io,
            p[i].cpu2, p[i].ct, p[i].tat,
            p[i].wt, p[i].rt);
    }
}

int main()
{
    struct Process p[100];
    int n, q, i;

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

    printf("\nEnter Time Quantum: ");
    scanf("%d", &q);

    rr(p, n, q);

    return 0;
}