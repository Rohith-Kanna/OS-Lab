#include <stdio.h>

struct Process
{
    int pid, at, cpu;
    int rem;
    int ct, tat, wt, rt;
    int first;
};

/* ---------- ROUND ROBIN ---------- */

void rr(struct Process p[], int n, int q)
{
    int queue[100], front = 0, rear = 0;
    int time = 0, completed = 0;
    int current = -1, qtime = 0;
    int added[100] = {0};
    int i;

    for(i = 0; i < n; i++)
    {
        p[i].rem = p[i].cpu;
        p[i].first = -1;
    }

    printf("\nRR Gantt Chart:\n");

    while(completed < n)
    {
        /* Add arrived processes */
        for(i = 0; i < n; i++)
        {
            if(!added[i] && p[i].at <= time)
            {
                queue[rear++] = i;
                added[i] = 1;
            }
        }

        /* Take process from queue */
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

        /* Execute 1 unit */
        p[current].rem--;
        time++;
        qtime++;

        printf("P%d ", p[current].pid);

        /* Process completed */
        if(p[current].rem == 0)
        {
            p[current].ct = time;
            p[current].tat = p[current].ct - p[current].at;
            p[current].wt = p[current].tat - p[current].cpu;
            p[current].rt = p[current].first - p[current].at;

            completed++;
            current = -1;
        }

        /* Time quantum finished */
        else if(qtime == q)
        {
            queue[rear++] = current;
            current = -1;
            qtime = 0;
        }
    }

    printf("\n\nProcess\tAT\tCPU\tCT\tTAT\tWT\tRT\n");

    for(i = 0; i < n; i++)
    {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid, p[i].at, p[i].cpu,
               p[i].ct, p[i].tat, p[i].wt, p[i].rt);
    }
}


/* ---------- SRTF ---------- */

void srtf(struct Process p[], int n)
{
    int time = 0, completed = 0;
    int best, i;

    for(i = 0; i < n; i++)
    {
        p[i].rem = p[i].cpu;
        p[i].first = -1;
    }

    printf("\nSRTF Gantt Chart:\n");

    while(completed < n)
    {
        best = -1;

        /* Find shortest remaining time */
        for(i = 0; i < n; i++)
        {
            if(p[i].at <= time && p[i].rem > 0)
            {
                if(best == -1 ||
                   p[i].rem < p[best].rem)
                {
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

        if(p[best].first == -1)
            p[best].first = time;

        /* Execute 1 unit */
        p[best].rem--;
        time++;

        printf("P%d ", p[best].pid);

        /* Process completed */
        if(p[best].rem == 0)
        {
            p[best].ct = time;
            p[best].tat = p[best].ct - p[best].at;
            p[best].wt = p[best].tat - p[best].cpu;
            p[best].rt = p[best].first - p[best].at;

            completed++;
        }
    }

    printf("\n\nProcess\tAT\tCPU\tCT\tTAT\tWT\tRT\n");

    for(i = 0; i < n; i++)
    {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid, p[i].at, p[i].cpu,
               p[i].ct, p[i].tat, p[i].wt, p[i].rt);
    }
}


/* ---------- MAIN ---------- */

int main()
{
    struct Process p[100];
    int n, q, choice, i;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    for(i = 0; i < n; i++)
    {
        p[i].pid = i + 1;

        printf("\nP%d AT: ", i + 1);
        scanf("%d", &p[i].at);

        printf("P%d CPU: ", i + 1);
        scanf("%d", &p[i].cpu);
    }

    printf("\n1. Round Robin");
    printf("\n2. SRTF");
    printf("\nEnter choice: ");
    scanf("%d", &choice);

    if(choice == 1)
    {
        printf("Enter Time Quantum: ");
        scanf("%d", &q);

        rr(p, n, q);
    }
    else if(choice == 2)
    {
        srtf(p, n);
    }

    return 0;
}