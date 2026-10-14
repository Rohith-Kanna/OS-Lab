
#include <stdio.h>
#include <stdbool.h>

bool isInMemory(int page, int frames[], int n) {
    for (int i = 0; i < n; i++)
        if (frames[i] == page)
            return true;
    return false;
}

int findOptimal(int pages[], int n, int frames[], int f, int start) {
    int index = -1, farthest = -1;

    for (int i = 0; i < f; i++) {
        int j;

        for (j = start; j < n; j++)
            if (frames[i] == pages[j])
                break;

        if (j == n)
            return i;

        if (j > farthest) {
            farthest = j;
            index = i;
        }
    }

    return index;
}

int simulateOptimal(int pages[], int n, int f) {
    int faults = 0, frames[f];

    for (int i = 0; i < f; i++)
        frames[i] = -1;

    printf("\nIncoming\t");
    for (int i = 0; i < f; i++)
        printf("Frame %d\t", i + 1);
    printf("\n");

    for (int i = 0; i < n; i++) {
        printf("%d\t\t", pages[i]);

        if (!isInMemory(pages[i], frames, f)) {
            faults++;
            int k = -1;

            for (int j = 0; j < f; j++)
                if (frames[j] == -1) {
                    k = j;
                    break;
                }

            if (k == -1)
                k = findOptimal(pages, n, frames, f, i + 1);

            frames[k] = pages[i];
        }

        for (int j = 0; j < f; j++) {
            if (frames[j] == -1)
                printf("-\t\t");
            else
                printf("%d\t\t", frames[j]);
        }
        printf("\n");
    }

    return faults;
}

int main() {
    int n, f;

    printf("Enter number of memory frames: ");
    scanf("%d", &f);
    printf("Enter number of page references: ");
    scanf("%d", &n);

    if (f <= 0 || n <= 0)
        return 1;

    int pages[n];

    printf("Enter page reference string:\n");
    for (int i = 0; i < n; i++)
        scanf("%d", &pages[i]);

    int faults = simulateOptimal(pages, n, f);
    int hits = n - faults;

    printf("\nTotal pages: %d\nTotal frames: %d\n", n, f);
    printf("Page faults (MISS) using Optimal: %d\n", faults);
    printf("Page hits using Optimal: %d\n", hits);
    printf("Miss percent: %.2f%%\n", (float)faults / n * 100);
    printf("Hit percent: %.2f%%\n", (float)hits / n * 100);
    printf("Hit ratio: %.4f\n", (float)hits / n);
    printf("Miss ratio: %.4f\n", (float)faults / n);

    return 0;
}

