#include <stdio.h>

struct Process
{
    int pid;
    int at;
    int cb1;
    int io;
    int cb2;

    int rem1;
    int rem2;

    int io_done;

    int ct;
    int tat;
    int wt;
    int rt;

    int first_start;
    int completed;
};

void print_result(struct Process p[], int n, int total_time, float total_wt, float total_tat, float total_rt)
{
    int i;

    printf("\nProcess\tAT\tCPU1\tIO\tCPU2\tCT\tTAT\tWT\tRT\n");

    for(i = 0; i < n; i++)
    {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid,
               p[i].at,
               p[i].cb1,
               p[i].io,
               p[i].cb2,
               p[i].ct,
               p[i].tat,
               p[i].wt,
               p[i].rt);
    }

    printf("\nAverage Turnaround Time = %.2f", total_tat / n);
    printf("\nAverage Waiting Time = %.2f", total_wt / n);
    printf("\nAverage Response Time = %.2f", total_rt / n);
    printf("\nThroughput = %.2f processes/unit time\n",
           (float)n / total_time);
}

void srtf(struct Process original[], int n)
{
    struct Process p[100];
    int time = 0;
    int completed = 0;
    int current = -1;
    int i, j;
    int best;
    int total_time;

    float total_wt = 0;
    float total_tat = 0;
    float total_rt = 0;

    for(i = 0; i < n; i++)
    {
        p[i] = original[i];

        p[i].rem1 = p[i].cb1;
        p[i].rem2 = p[i].cb2;
        p[i].io_done = 0;
        p[i].completed = 0;
        p[i].first_start = -1;
    }

    printf("\n========== SRTF ==========\n");
    printf("\nGantt Chart:\n");

    while(completed < n)
    {
        best = -1;

        for(i = 0; i < n; i++)
        {
            if(p[i].completed == 0 && p[i].at <= time)
            {
                if(p[i].rem1 > 0)
                {
                    if(best == -1 || p[i].rem1 < p[best].rem1)
                        best = i;
                }
                else if(p[i].rem2 > 0 && time >= p[i].io_done)
                {
                    if(best == -1 || p[i].rem2 < p[best].rem2)
                        best = i;
                }
            }
        }

        if(best == -1)
        {
            time++;
            continue;
        }

        current = best;

        if(p[current].first_start == -1)
        {
            p[current].first_start = time;
        }

        printf("| P%d ", p[current].pid);

        if(p[current].rem1 > 0)
        {
            p[current].rem1--;
        }
        else
        {
            p[current].rem2--;
        }

        time++;

        if(p[current].rem1 == 0 && p[current].io_done == 0)
        {
            p[current].io_done = time + p[current].io;
        }

        if(p[current].rem1 == 0 &&
           p[current].rem2 == 0 &&
           p[current].io_done != 0 &&
           time >= p[current].io_done)
        {
            p[current].ct = time;
            p[current].tat = p[current].ct - p[current].at;

            p[current].wt =
                p[current].tat -
                p[current].cb1 -
                p[current].cb2 -
                p[current].io;

            p[current].rt =
                p[current].first_start - p[current].at;

            p[current].completed = 1;
            completed++;

            total_tat += p[current].tat;
            total_wt += p[current].wt;
            total_rt += p[current].rt;
        }
    }

    printf("|\n");

    total_time = time;

    print_result(p, n, total_time,
                 total_wt, total_tat, total_rt);
}

void round_robin(struct Process original[], int n, int quantum)
{
    struct Process p[100];

    int queue[1000];
    int front = 0;
    int rear = 0;

    int time = 0;
    int completed = 0;
    int i;
    int current = -1;
    int qtime = 0;

    int added[100] = {0};

    float total_wt = 0;
    float total_tat = 0;
    float total_rt = 0;

    for(i = 0; i < n; i++)
    {
        p[i] = original[i];

        p[i].rem1 = p[i].cb1;
        p[i].rem2 = p[i].cb2;
        p[i].io_done = 0;
        p[i].completed = 0;
        p[i].first_start = -1;
    }

    printf("\n========== ROUND ROBIN ==========\n");
    printf("\nTime Quantum = %d\n", quantum);
    printf("\nGantt Chart:\n");

    while(completed < n)
    {
        for(i = 0; i < n; i++)
        {
            if(added[i] == 0 && p[i].at <= time)
            {
                queue[rear++] = i;
                added[i] = 1;
            }
        }

        for(i = 0; i < n; i++)
        {
            if(p[i].completed == 0 &&
               p[i].rem1 == 0 &&
               p[i].io_done != 0 &&
               p[i].io_done <= time)
            {
                if(p[i].rem2 > 0)
                {
                    queue[rear++] = i;
                    p[i].io_done = -1;
                }
            }
        }

        if(current == -1)
        {
            if(front < rear)
            {
                current = queue[front++];
                qtime = 0;

                if(p[current].first_start == -1)
                {
                    p[current].first_start = time;
                }
            }
            else
            {
                time++;
                continue;
            }
        }

        if(p[current].rem1 > 0)
            p[current].rem1--;
        else
            p[current].rem2--;

        time++;
        qtime++;

        for(i = 0; i < n; i++)
        {
            if(added[i] == 0 && p[i].at <= time)
            {
                queue[rear++] = i;
                added[i] = 1;
            }
        }

        if(p[current].rem1 == 0 &&
           p[current].io_done == 0)
        {
            p[current].io_done = time + p[current].io;
            current = -1;
            qtime = 0;
        }
        else if(p[current].rem2 == 0)
        {
            p[current].ct = time;
            p[current].tat = p[current].ct - p[current].at;

            p[current].wt =
                p[current].tat -
                p[current].cb1 -
                p[current].cb2 -
                p[current].io;

            p[current].rt =
                p[current].first_start - p[current].at;

            p[current].completed = 1;
            completed++;

            total_tat += p[current].tat;
            total_wt += p[current].wt;
            total_rt += p[current].rt;

            current = -1;
            qtime = 0;
        }
        else if(qtime == quantum)
        {
            queue[rear++] = current;
            current = -1;
            qtime = 0;
        }

        printf("| P%d ", p[current == -1 ? queue[rear - 1] : current].pid);
    }

    printf("|\n");

    print_result(p, n, time,
                 total_wt, total_tat, total_rt);
}

int main()
{
    struct Process p[100];

    int n;
    int i;
    int quantum;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    printf("\nEnter Arrival Time, CPU Burst 1, IO Burst and CPU Burst 2:\n");

    for(i = 0; i < n; i++)
    {
        p[i].pid = i + 1;

        printf("\nP%d Arrival Time: ", i + 1);
        scanf("%d", &p[i].at);

        printf("P%d CPU Burst 1: ", i + 1);
        scanf("%d", &p[i].cb1);

        printf("P%d IO Burst: ", i + 1);
        scanf("%d", &p[i].io);

        printf("P%d CPU Burst 2: ", i + 1);
        scanf("%d", &p[i].cb2);
    }

    printf("\nEnter Time Quantum for Round Robin: ");
    scanf("%d", &quantum);

    round_robin(p, n, quantum);

    srtf(p, n);

    return 0;
}