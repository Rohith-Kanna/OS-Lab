
#include <stdio.h>

#define B 5 //buffer blocks
#define P 4 //processes

struct block {
    int size, start, allocated, pid;
};

int mem[B] = {100, 500, 200, 300, 600};
int req[P] = {212, 417, 112, 426};

void init(struct block b[]) {
    int addr = 0;
    for (int i = 0; i < B; i++) {
        b[i].size = mem[i];
        b[i].start = addr;
        b[i].allocated = 0;
        b[i].pid = -1;
        addr += mem[i];
    }
}

int choose(struct block b[], int size, int mode) {
    int pos = -1;

    for (int i = 0; i < B; i++) {
        if (!b[i].allocated && b[i].size >= size) {
            if (mode == 1)
                return i;                 // First Fit

            if (pos == -1 ||
                (mode == 2 && b[i].size < b[pos].size) ||
                (mode == 3 && b[i].size > b[pos].size))
                pos = i;                  // Best / Worst Fit
        }
    }
    return pos;
}

void stats(struct block b[], int total, int request, int current) {
    int used = 0, freeMem = 0, largest = 0, internal = 0;

    for (int i = 0; i < B; i++) {
        if (b[i].allocated) {
            used += b[i].size;
            if (b[i].pid == current)
                internal = b[i].size - request;
        } else {
            freeMem += b[i].size;
            if (b[i].size > largest)
                largest = b[i].size;
        }
    }

    printf("Memory used: %d KB (%.2f%%)\n",
           used, used * 100.0 / total);
    printf("Internal fragmentation: %d KB\n", internal);
    printf("External fragmentation estimate: %d KB\n",
           freeMem - largest);
}

void simulate(int mode, char name[]) {
    struct block b[B];
    int total = 0, success = 0, failed = 0;

    init(b);
    for (int i = 0; i < B; i++)
        total += b[i].size;

    printf("\n--- %s ---\n", name);

    for (int p = 0; p < P; p++) {
        int k = choose(b, req[p], mode);

        printf("\nProcess P%d requests %d KB\n", p + 1, req[p]);

        if (k == -1) {
            printf("Allocation failed\n");
            failed++;
            stats(b, total, req[p], -1);
            continue;
        }

        b[k].allocated = 1;
        b[k].pid = p + 1;
        success++;

        printf("Allocated Block %d: start=%d, size=%d KB\n",
               k + 1, b[k].start, b[k].size);
        stats(b, total, req[p], p + 1);

        b[k].allocated = 0;
        b[k].pid = -1;
        printf("Process completed; memory deallocated\n");
        stats(b, total, req[p], -1);
    }

    printf("\nSuccessful allocations: %d\n", success);
    printf("Failed allocations: %d\n", failed);
}

int main(void) {
    simulate(1, "FIRST FIT");
    simulate(2, "BEST FIT");
    simulate(3, "WORST FIT");
    return 0;
}
