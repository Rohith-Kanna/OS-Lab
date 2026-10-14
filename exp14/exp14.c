
#include <stdio.h>
#include <stdlib.h>

#define N 8
#define MAX 199

int a[N] = {98, 183, 37, 122, 14, 124, 65, 67};

void sort(int b[]) {
    for (int i = 0; i < N - 1; i++)
        for (int j = 0; j < N - i - 1; j++)
            if (b[j] > b[j + 1]) {
                int t = b[j];
                b[j] = b[j + 1];
                b[j + 1] = t;
            }
}

void result(char name[], int seq[], int n, int head) {
    int total = 0, cur = head;

    printf("\n%s\nSequence: %d", name, head);

    for (int i = 0; i < n; i++) {
        total += abs(cur - seq[i]);
        cur = seq[i];
        printf(" -> %d", cur);
    }

    printf("\nTotal head movement: %d", total);
    printf("\nAverage head movement: %.2f\n", (float)total / N);
}

int main() {
    int head = 53, b[N], seq[2 * N + 2], n, i, k;
    int done[N] = {0}, cur, best, dist, total;

    printf("Disk tracks: 0-%d\n", MAX);
    printf("Request queue: ");
    for (i = 0; i < N; i++)
        printf("%d ", a[i]);

    printf("\nInitial head position (assumed): %d\n", head);

    // FCFS
    result("FCFS", a, N, head);

    // SSTF
    for (i = 0; i < N; i++) done[i] = 0;
    cur = head;
    total = 0;
    printf("\nSSTF\nSequence: %d", head);

    for (i = 0; i < N; i++) {
        best = -1;
        dist = 999999;

        for (int j = 0; j < N; j++)
            if (!done[j] && abs(a[j] - cur) < dist) {
                dist = abs(a[j] - cur);
                best = j;
            }

        done[best] = 1;
        total += dist;
        cur = a[best];
        printf(" -> %d", cur);
    }

    printf("\nTotal head movement: %d", total);
    printf("\nAverage head movement: %.2f\n", (float)total / N);

    // Sort requests
    for (i = 0; i < N; i++) b[i] = a[i];
    sort(b);

    // SCAN: move toward 0 first
    n = 0;
    for (i = N - 1; i >= 0; i--)
        if (b[i] < head) seq[n++] = b[i];
    seq[n++] = 0;
    for (i = 0; i < N; i++)
        if (b[i] >= head) seq[n++] = b[i];
    result("SCAN (toward 0 first)", seq, n, head);

    // C-SCAN: move upward first, wrap from MAX to 0
    n = 0;
    for (i = 0; i < N; i++)
        if (b[i] >= head) seq[n++] = b[i];
    seq[n++] = MAX;
    seq[n++] = 0;
    for (i = 0; i < N; i++)
        if (b[i] < head) seq[n++] = b[i];
    result("C-SCAN (upward first)", seq, n, head);

    // LOOK: move upward first, then reverse
    n = 0;
    for (i = 0; i < N; i++)
        if (b[i] >= head) seq[n++] = b[i];
    for (i = N - 1; i >= 0; i--)
        if (b[i] < head) seq[n++] = b[i];
    result("LOOK (upward first)", seq, n, head);

    // C-LOOK: move upward, jump to lowest pending request
    n = 0;
    for (i = 0; i < N; i++)
        if (b[i] >= head) seq[n++] = b[i];
    for (i = 0; i < N; i++)
        if (b[i] < head) seq[n++] = b[i];
    result("C-LOOK (upward first)", seq, n, head);

    return 0;
}
