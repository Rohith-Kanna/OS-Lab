
#include <stdio.h>
#include <stdbool.h>

bool isInMemory(int page, int frames[], int n) {
    for (int i = 0; i < n; i++)
        if (frames[i] == page)
            return true;
    return false;
}

int simulateLRU(int pages[], int n, int f) {
    int faults = 0, frames[f], last_used[f];

    for (int i = 0; i < f; i++) {
        frames[i] = -1;
        last_used[i] = -1;
    }

    printf("\nIncoming\t");
    for (int i = 0; i < f; i++)
        printf("Frame %d\t", i + 1);
    printf("\n");

    for (int i = 0; i < n; i++) {
        printf("%d\t\t", pages[i]);

        int j;
        if (isInMemory(pages[i], frames, f)) {
            for (j = 0; j < f; j++)
                if (frames[j] == pages[i]) {
                    last_used[j] = i;
                    break;
                }
        } else {
            faults++;
            int k = -1;

            for (j = 0; j < f; j++)
                if (frames[j] == -1) {
                    k = j;
                    break;
                }

            if (k == -1) {
                k = 0;
                for (j = 1; j < f; j++)
                    if (last_used[j] < last_used[k])
                        k = j;
            }

            frames[k] = pages[i];
            last_used[k] = i;
        }

        for (j = 0; j < f; j++) {
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

    int faults = simulateLRU(pages, n, f);
    int hits = n - faults;

    printf("\nTotal pages: %d\nTotal frames: %d\n", n, f);
    printf("Page faults (MISS) using LRU: %d\n", faults);
    printf("Page hits using LRU: %d\n", hits);
    printf("Miss percent: %.2f%%\n", (float)faults / n * 100);
    printf("Hit percent: %.2f%%\n", (float)hits / n * 100);
    printf("Hit ratio: %.4f\n", (float)hits / n);
    printf("Miss ratio: %.4f\n", (float)faults / n);

    return 0;
}

