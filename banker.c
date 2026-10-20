#include <stdio.h>

#define P 5
#define R 3

int max[P][R], alloc[P][R], need[P][R], available[R];

void calculateNeed()
{
    for (int i = 0; i < P; i++)
        for (int j = 0; j < R; j++)
            need[i][j] = max[i][j] - alloc[i][j];
}

int isSafe()
{
    int finish[P] = {0};
    int work[R], seq[P], count = 0;

    for (int j = 0; j < R; j++)
        work[j] = available[j];

    while (count < P)
    {
        int found = 0;

        for (int i = 0; i < P; i++)
        {
            if (!finish[i])
            {
                int ok = 1;

                for (int j = 0; j < R; j++)
                    if (need[i][j] > work[j])
                        ok = 0;

                if (ok)
                {
                    for (int j = 0; j < R; j++)
                        work[j] += alloc[i][j];

                    seq[count++] = i;
                    finish[i] = 1;
                    found = 1;
                }
            }
        }

        if (!found)
        {
            printf("System is NOT in a safe state.\n");
            return 0;
        }
    }

    printf("System is in a safe state.\nSafe sequence: ");
    for (int i = 0; i < P; i++)
        printf("P%d ", seq[i]);

    printf("\n");
    return 1;
}

void requestResources(int p, int req[R])
{
    for (int j = 0; j < R; j++)
    {
        if (req[j] > need[p][j])
        {
            printf("Request exceeds process P%d's need.\n", p);
            return;
        }
    }

    for (int j = 0; j < R; j++)
    {
        if (req[j] > available[j])
        {
            printf("Resources unavailable. Process must wait.\n");
            return;
        }
    }

    for (int j = 0; j < R; j++)
    {
        available[j] -= req[j];
        alloc[p][j] += req[j];
        need[p][j] -= req[j];
    }

    if (isSafe())
        printf("Request granted to P%d.\n", p);
    else
    {
        for (int j = 0; j < R; j++)
        {
            available[j] += req[j];
            alloc[p][j] -= req[j];
            need[p][j] += req[j];
        }

        printf("Request denied. Allocation rolled back.\n");
    }
}

int main()
{
    int req[R], p;

    printf("Enter Max matrix (%d x %d):\n", P, R);
    for (int i = 0; i < P; i++)
        for (int j = 0; j < R; j++)
            scanf("%d", &max[i][j]);

    printf("Enter Allocation matrix (%d x %d):\n", P, R);
    for (int i = 0; i < P; i++)
        for (int j = 0; j < R; j++)
            scanf("%d", &alloc[i][j]);

    printf("Enter Available resources (%d values):\n", R);
    for (int j = 0; j < R; j++)
        scanf("%d", &available[j]);

    calculateNeed();

    printf("\nNeed matrix:\n");
    for (int i = 0; i < P; i++)
    {
        for (int j = 0; j < R; j++)
            printf("%d ", need[i][j]);
        printf("\n");
    }

    printf("\nInitial safety check:\n");
    isSafe();

    printf("\nEnter requesting process number (0-4): ");
    scanf("%d", &p);

    if (p < 0 || p >= P)
    {
        printf("Invalid process number.\n");
        return 1;
    }

    printf("Enter request (%d resource values):\n", R);
    for (int j = 0; j < R; j++)
        scanf("%d", &req[j]);

    requestResources(p, req);

    return 0;
}