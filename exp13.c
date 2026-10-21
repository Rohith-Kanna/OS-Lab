//program to simulate FIFO, Optimal and LRU page replacement algorithms

#include <stdio.h>
#include <stdbool.h>

bool isInMemory(int page, int frames[], int n) {
    for (int i = 0; i < n; i++)
        if (frames[i] == page)
            return true;
    return false;
}

void printFrames(int frames[], int f) {
    for (int i = 0; i < f; i++) {
        if (frames[i] == -1)
            printf("-\t\t");
        else
            printf("%d\t\t", frames[i]);
    }
    printf("\n");
}

void printStats(int faults, int n) {
    int hits = n - faults;
    printf("Page faults: %d\n", faults);
    printf("Page hits: %d\n", hits);
    printf("Miss percent: %.2f%%\n", (float)faults / n * 100);
    printf("Hit percent: %.2f%%\n", (float)hits / n * 100);
    printf("Miss ratio: %.4f\n", (float)faults / n);
    printf("Hit ratio: %.4f\n", (float)hits / n);
}

int simulateFIFO(int pages[], int n, int f) {
    int faults = 0, frames[f], ptr = 0;

    for (int i = 0; i < f; i++)
        frames[i] = -1;

    printf("\nFIFO\nIncoming\t");
    for (int i = 0; i < f; i++)
        printf("Frame %d\t", i + 1);
    printf("\n");

    for (int i = 0; i < n; i++) {
        printf("%d\t\t", pages[i]);

        if (!isInMemory(pages[i], frames, f)) { //page fault
            frames[ptr] = pages[i];
            ptr = (ptr + 1) % f;
            faults++;
            printFrames(frames, f);
            // Status is printed separately below in the compact version
        } else {
            printFrames(frames, f);
        }
    }

    return faults;
}

int findOptimal(int pages[], int n, int frames[], int f, int start) {
    int index = 0, farthest = -1;

    for (int i = 0; i < f; i++) {//finding farthest page to be used in future
        int j;

        //find next use of frames[i]
        for (j = start; j < n; j++)
            if (frames[i] == pages[j])
                break;

        if (j == n)//page never used again, return i meaning replace this page
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

    printf("\nOPTIMAL\nIncoming\t");
    for (int i = 0; i < f; i++)
        printf("Frame %d\t", i + 1);
    printf("\n");

    for (int i = 0; i < n; i++) {
        printf("%d\t\t", pages[i]);

        if (!isInMemory(pages[i], frames, f)) {
            faults++;
            int k = 0; //find first empty frame

            while (k < f && frames[k] != -1)
                k++;

            if (k == f) //no empty frame, find optimal page to replace
                k = findOptimal(pages, n, frames, f, i + 1); //i+1 because we want to find the next use of the pages after the current page

            frames[k] = pages[i];
        }

        printFrames(frames, f);
    }

    return faults;
}

int simulateLRU(int pages[], int n, int f) {
    int faults = 0, frames[f], last_used[f];

    for (int i = 0; i < f; i++) {
        frames[i] = -1;
        last_used[i] = -1;
    }

    printf("\nLRU\nIncoming\t");
    for (int i = 0; i < f; i++)
        printf("Frame %d\t", i + 1);
    printf("\n");

    for (int i = 0; i < n; i++) {
        printf("%d\t\t", pages[i]);

        int j;
        if (isInMemory(pages[i], frames, f)) { //page hit, update last used index
            for (j = 0; j < f; j++)
                if (frames[j] == pages[i]) {
                    last_used[j] = i;
                    break;
                }
        } else { //page fault, find least recently used page to replace
            faults++;
            int k = -1;

            for (j = 0; j < f; j++) //find first empty frame
                if (frames[j] == -1) {
                    k = j;
                    break;
                }

            if (k == -1) { //no empty frame, find least recently used page to replace
                k = 0;
                for (j = 1; j < f; j++)//j=1 because we are comparing with the first frame
                    if (last_used[j] < last_used[k])
                        k = j;
            }

            frames[k] = pages[i];
            last_used[k] = i;
        }

        printFrames(frames, f);
    }

    return faults;
}

int main() {
    int n, f;

    printf("Enter number of memory frames: ");
    scanf("%d", &f);

    printf("Enter number of page references: ");
    scanf("%d", &n);

    // if (f <= 0 || n <= 0)
    //     return 1;

    int pages[n];

    printf("Enter page reference string:\n");
    for (int i = 0; i < n; i++)
        scanf("%d", &pages[i]);

    int faults;

    faults = simulateFIFO(pages, n, f);
    printStats(faults, n);

    faults = simulateOptimal(pages, n, f);
    printStats(faults, n);

    faults = simulateLRU(pages, n, f);
    printStats(faults, n);

    return 0;
}
