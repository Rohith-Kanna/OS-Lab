#include <stdio.h>

// Process structure to store scheduling information
struct Process
{
    int pid;           // Process ID
    int at;            // Arrival Time
    int cb1;           // CPU Burst 1 (time units)
    int io;            // IO Burst time
    int cb2;           // CPU Burst 2 (time units)

    int rem1;          // Remaining CPU Burst 1
    int rem2;          // Remaining CPU Burst 2

    int io_done;       // When IO will be completed

    int ct;            // Completion Time
    int tat;           // Turnaround Time (ct - at)
    int wt;            // Waiting Time
    int rt;            // Response Time (first_start - at)

    int first_start;   // When process gets CPU for first time
    int completed;     // Flag: 1 if process is completed, 0 otherwise
};

// Print the scheduling results in a table format
void print_result(struct Process p[], int n, int total_time, float total_wt, float total_tat, float total_rt)
{
    int i;

    // Print header row
    printf("\nProcess\tAT\tCPU1\tIO\tCPU2\tCT\tTAT\tWT\tRT\n");

    // Print each process's scheduling metrics
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

    // Print average metrics
    printf("\nAverage Turnaround Time = %.2f", total_tat / n);
    printf("\nAverage Waiting Time = %.2f", total_wt / n);
    printf("\nAverage Response Time = %.2f", total_rt / n);
    printf("\nThroughput = %.2f processes/unit time\n",
           (float)n / total_time);
}

// SRTF (Shortest Remaining Time First) Scheduling Algorithm
// Preemptive: Always picks the process with shortest remaining time
void srtf(struct Process original[], int n)
{
    struct Process p[100];
    int time = 0;          // Current time unit
    int completed = 0;     // Number of completed processes
    int current = -1;      // Currently executing process
    int i, j;
    int best;              // Best process to execute next
    int total_time;        // Total time to complete all processes

    float total_wt = 0;    // Total waiting time
    float total_tat = 0;   // Total turnaround time
    float total_rt = 0;    // Total response time

    // Initialize process data
    for(i = 0; i < n; i++)
    {
        p[i] = original[i];

        p[i].rem1 = p[i].cb1;      // Copy CPU burst 1
        p[i].rem2 = p[i].cb2;      // Copy CPU burst 2
        p[i].io_done = 0;          // IO not yet started
        p[i].completed = 0;        // Not completed
        p[i].first_start = -1;     // Has not started
    }

    printf("\n========== SRTF ==========\n");
    printf("\nGantt Chart:\n");

    // Main scheduling loop - continue until all processes complete
    while(completed < n)
    {
        best = -1;  // Find the best process to execute

        // Select process with shortest remaining time that has arrived
        for(i = 0; i < n; i++)
        {
            // Check if process is not completed and has arrived
            if(p[i].completed == 0 && p[i].at <= time)
            {
                // Priority 1: Process still in CPU burst 1
                if(p[i].rem1 > 0)
                {
                    // Pick process with shortest remaining time in burst 1
                    if(best == -1 || p[i].rem1 < p[best].rem1)
                        best = i;
                }
                // Priority 2: Process in CPU burst 2 (after IO is done)
                else if(p[i].rem2 > 0 && time >= p[i].io_done)
                {
                    // Pick process with shortest remaining time in burst 2
                    if(best == -1 || p[i].rem2 < p[best].rem2)
                        best = i;
                }
            }
        }

        // No process available - CPU idle, skip to next time unit
        if(best == -1)
        {
            time++;
            continue;
        }

        current = best;  // Execute the selected process

        // Record first time this process gets CPU
        if(p[current].first_start == -1)
        {
            p[current].first_start = time;
        }

        printf("| P%d ", p[current].pid);  // Print Gantt chart

        // Execute one time unit
        if(p[current].rem1 > 0)
        {
            p[current].rem1--;  // Reduce CPU burst 1
        }
        else
        {
            p[current].rem2--;  // Reduce CPU burst 2
        }

        time++;

        // Check if CPU burst 1 is complete - start IO
        if(p[current].rem1 == 0 && p[current].io_done == 0)
        {
            p[current].io_done = time + p[current].io;  // Set when IO will complete
        }

        // Check if process is completely done (both bursts done + IO complete)
        if(p[current].rem1 == 0 &&
           p[current].rem2 == 0 &&
           p[current].io_done != 0 &&
           time >= p[current].io_done)
        {
            // Calculate metrics
            p[current].ct = time;  // Completion time
            p[current].tat = p[current].ct - p[current].at;  // Turnaround time

            // Waiting time = Total time - actual CPU and IO time
            p[current].wt =
                p[current].tat -
                p[current].cb1 -
                p[current].cb2 -
                p[current].io;

            // Response time = when first started - arrival time
            p[current].rt =
                p[current].first_start - p[current].at;

            p[current].completed = 1;
            completed++;

            // Add to totals
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

// Round Robin Scheduling Algorithm
// Non-preemptive within bursts: Each process gets a time quantum to execute
void round_robin(struct Process original[], int n, int quantum)
{
    struct Process p[100];

    int queue[1000];       // Ready queue of process indices
    int front = 0;         // Front of queue
    int rear = 0;          // Rear of queue

    int time = 0;          // Current time unit
    int completed = 0;     // Number of completed processes 
    int i;
    int current = -1;      // Currently executing process
    int qtime = 0;         // Time spent on current process (for quantum)

    int added[100] = {0};  // Track which processes have been added to queue

    float total_wt = 0;    // Total waiting time
    float total_tat = 0;   // Total turnaround time
    float total_rt = 0;    // Total response time

    // Initialize process data
    for(i = 0; i < n; i++)
    {
        p[i] = original[i];

        p[i].rem1 = p[i].cb1;      // Copy CPU burst 1
        p[i].rem2 = p[i].cb2;      // Copy CPU burst 2
        p[i].io_done = 0;          // IO not yet started
        p[i].completed = 0;        // Not completed
        p[i].first_start = -1;     // Has not started
    }

    printf("\n========== ROUND ROBIN ==========\n");
    printf("\nTime Quantum = %d\n", quantum);
    printf("\nGantt Chart:\n");

    // Main scheduling loop - continue until all processes complete
    while(completed < n)
    {
        // Add newly arrived processes to ready queue
        for(i = 0; i < n; i++)
        {
            if(added[i] == 0 && p[i].at <= time)
            {
                queue[rear++] = i;
                added[i] = 1;  // Mark as added
            }
        }

        // Add processes back from IO to ready queue
        for(i = 0; i < n; i++)
        {
            // Check if process completed burst 1, finished IO, and still has burst 2
            if(p[i].completed == 0 &&
               p[i].rem1 == 0 &&
               p[i].io_done != 0 &&
               p[i].io_done <= time)
            {
                if(p[i].rem2 > 0)
                {
                    queue[rear++] = i;  // Add back to queue for burst 2
                    p[i].io_done = -1;  // Mark IO as handled
                }
            }
        }

        // If no process is currently executing, pick next from queue
        if(current == -1)
        {
            if(front < rear)
            {
                current = queue[front++];  // Pick from front of queue
                qtime = 0;  // Reset quantum time

                // Record first time this process gets CPU
                if(p[current].first_start == -1)
                {
                    p[current].first_start = time;
                }
            }
            else
            {
                // Queue is empty - CPU idle
                time++;
                continue;
            }
        }

        // Execute one time unit
        if(p[current].rem1 > 0)
            p[current].rem1--;  // Execute CPU burst 1
        else
            p[current].rem2--;  // Execute CPU burst 2

        time++;
        qtime++;  // Track time spent on current quantum

        // Add any newly arrived processes
        for(i = 0; i < n; i++)
        {
            if(added[i] == 0 && p[i].at <= time)
            {
                queue[rear++] = i;
                added[i] = 1;
            }
        }

        // Check if burst 1 is complete - process goes to IO
        if(p[current].rem1 == 0 &&
           p[current].io_done == 0)
        {
            p[current].io_done = time + p[current].io;  // Set IO completion time
            current = -1;  // Remove from CPU
            qtime = 0;
        }
        // Check if burst 2 is complete - process is done
        else if(p[current].rem2 == 0)
        {
            // Calculate metrics
            p[current].ct = time;  // Completion time
            p[current].tat = p[current].ct - p[current].at;  // Turnaround time

            // Waiting time = Total time - actual CPU and IO time
            p[current].wt =
                p[current].tat -
                p[current].cb1 -
                p[current].cb2 -
                p[current].io;

            // Response time = when first started - arrival time
            p[current].rt =
                p[current].first_start - p[current].at;

            p[current].completed = 1;
            completed++;

            // Add to totals
            total_tat += p[current].tat;
            total_wt += p[current].wt;
            total_rt += p[current].rt;

            current = -1;  // Remove from CPU
            qtime = 0;
        }
        // Check if quantum time exceeded - send back to queue
        else if(qtime == quantum)
        {
            queue[rear++] = current;  // Add back to end of queue
            current = -1;  // Remove from CPU
            qtime = 0;     // Reset quantum time
        }

        printf("| P%d ", p[current == -1 ? queue[rear - 1] : current].pid);
    }

    printf("|\n");

    print_result(p, n, time,
                 total_wt, total_tat, total_rt);
}

// Main program to compare SRTF and Round Robin scheduling algorithms
int main()
{
    struct Process p[100];

    int n;          // Number of processes
    int i;
    int quantum;    // Time quantum for Round Robin

    // Get input from user
    printf("Enter number of processes: ");
    scanf("%d", &n);

    printf("\nEnter Arrival Time, CPU Burst 1, IO Burst and CPU Burst 2:\n");

    // Input process data
    for(i = 0; i < n; i++)
    {
        p[i].pid = i + 1;  // Assign process ID

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

    // Run Round Robin scheduling
    round_robin(p, n, quantum);

    // Run SRTF scheduling
    srtf(p, n);

    return 0;
}