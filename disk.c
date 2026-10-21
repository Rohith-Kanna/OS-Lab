
#include <stdio.h>
#include <stdlib.h>

#define MAX 20

int a[MAX], n, head, diskSize, direction; //n- number of requests

void sort(int x[], int n)
{
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (x[j] > x[j + 1]) {
                int t = x[j];
                x[j] = x[j + 1];
                x[j + 1] = t;
            }
}

void show(int x[], int n, int start) //used to display the sequence of head movements and calculate total and average seek distance
{
    int curr = start, total = 0;
    printf("%d", start);
    for (int i = 0; i < n; i++) {
        total += abs(x[i] - curr);
        curr = x[i];
        printf(" -> %d", curr);
    }

    printf("\nTotal seek distance = %d\n", total);
    printf("Average seek distance = %.2f\n", (float)total / n);
}

void FCFS()
{
    printf("\n--- FCFS ---\n");
    show(a, n, head);
}

void SSTF()
{
    int used[MAX] = {0}, b[MAX], curr = head;

    for (int i = 0; i < n; i++) {
        int best = -1, dist = 1000000;

        for (int j = 0; j < n; j++) //find the closest request to the current head position
            if (!used[j] && abs(a[j] - curr) < dist) {
                dist = abs(a[j] - curr);
                best = j;
            }

        b[i] = a[best];
        used[best] = 1;
        curr = b[i];
    }

    printf("\n--- SSTF ---\n");
    show(b, n, head);
}

void SCAN()
{
    int left[MAX + 1], right[MAX + 1], l = 0, r = 0;

    for (int i = 0; i < n; i++){
        if (a[i] < head) left[l++] = a[i];
        else if (a[i] > head) right[r++] = a[i];
    }
    //adding only one boundary
    if (direction == 1) right[r++] = diskSize - 1; // add last (199)
    else left[l++] = 0;

    sort(left, l);
    sort(right, r);

    int b[MAX + 2], k = 0;

    if (direction == 1) {
        for (int i = 0; i < r; i++) b[k++] = right[i];
        for (int i = l - 1; i >= 0; i--) b[k++] = left[i];
    } else {
        for (int i = l - 1; i >= 0; i--) b[k++] = left[i];
        for (int i = 0; i < r; i++) b[k++] = right[i];
    }

    printf("\n--- SCAN ---\n");
    show(b, k, head);
}

void CSCAN()
{
    int left[MAX + 1], right[MAX + 1], l = 0, r = 0;

    for (int i = 0; i < n; i++)
        if (a[i] < head) left[l++] = a[i];
        else if (a[i] > head) right[r++] = a[i];

    //add both boundaries
    left[l++] = 0;
    right[r++] = diskSize - 1;

    sort(left, l);
    sort(right, r);

    int b[MAX + 2], k = 0;

    if (direction == 1) { 
        for (int i = 0; i < r; i++) b[k++] = right[i];
        for (int i = 0; i < l; i++) b[k++] = left[i];
    } else {
        for (int i = l - 1; i >= 0; i--) b[k++] = left[i];
        for (int i = r - 1; i >= 0; i--) b[k++] = right[i];
    }

    printf("\n--- C-SCAN ---\n");
    show(b, k, head);
}

void LOOK()
{
    int left[MAX], right[MAX], l = 0, r = 0;

    for (int i = 0; i < n; i++)
        if (a[i] < head) left[l++] = a[i];
        else if (a[i] > head) right[r++] = a[i];

    //no boundaries
    sort(left, l);
    sort(right, r);

    int b[MAX], k = 0;

    if (direction == 1) {// same as SCAN but without boundaries
        for (int i = 0; i < r; i++) b[k++] = right[i];
        for (int i = l - 1; i >= 0; i--) b[k++] = left[i];
    } else {
        for (int i = l - 1; i >= 0; i--) b[k++] = left[i];
        for (int i = 0; i < r; i++) b[k++] = right[i];
    }

    printf("\n--- LOOK ---\n");
    show(b, k, head);
}

void CLOOK()
{
    int left[MAX], right[MAX], l = 0, r = 0;

    for (int i = 0; i < n; i++)
        if (a[i] < head) left[l++] = a[i];
        else if (a[i] > head) right[r++] = a[i];

    //no boundaries, just sort and traverse
    sort(left, l);
    sort(right, r);

    int b[MAX], k = 0;

    if (direction == 1) { //same as C-SCAN but without boundaries
        for (int i = 0; i < r; i++) b[k++] = right[i];
        for (int i = 0; i < l; i++) b[k++] = left[i];
    } else {
        for (int i = l - 1; i >= 0; i--) b[k++] = left[i];
        for (int i = r - 1; i >= 0; i--) b[k++] = right[i];
    }

    printf("\n--- C-LOOK ---\n");
    show(b, k, head);
}

int main()
{
    printf("Enter number of requests (max %d): ", MAX);
    scanf("%d", &n);

    printf("Enter disk size (number of tracks): ");
    scanf("%d", &diskSize);

    // if (n < 1 || n > MAX || diskSize < 2) {
    //     printf("Invalid input\n");
    //     return 1;
    // }

    printf("Enter request queue: ");
    for (int i = 0; i < n; i++) {
        scanf("%d", &a[i]);
        if (a[i] < 0 || a[i] >= diskSize) {
            printf("Invalid track number\n");
            return 1;
        }
    }

    printf("Enter initial head position: ");
    scanf("%d", &head);

    // if (head < 0 || head >= diskSize) {
    //     printf("Invalid head position\n");
    //     return 1;
    // }

    printf("Direction (1 = up, 0 = down): ");
    scanf("%d", &direction);

    // if (direction != 0 && direction != 1) {
    //     printf("Invalid direction\n");
    //     return 1;
    // }

    FCFS();
    SSTF();
    SCAN();
    CSCAN();
    LOOK();
    CLOOK();

    return 0;
}
