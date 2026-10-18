#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// Structure to represent a memory block as required by lab specs
typedef struct {
    int id;
    int total_size;
    int remaining_size;
    int start_address;
    bool is_allocated; 
} MemoryBlock;

// Structure to represent a process
typedef struct {
    int id;
    int size;
    int allocated_block_id; // -1 if not allocated
    int internal_frag;
} Process;

void simulate_memory_strategy(MemoryBlock originalBlocks[], int m, Process originalProcesses[], int n, const char* method) {
    MemoryBlock blocks[m];
    Process processes[n];
    
    // Deep copy structures to isolate each run
    for (int i = 0; i < m; i++) blocks[i] = originalBlocks[i];
    for (int i = 0; i < n; i++) {
        processes[i] = originalProcesses[i];
        processes[i].allocated_block_id = -1;
        processes[i].internal_frag = 0;
    }

    int successful_allocations = 0;
    int failed_allocations = 0;

    // Simulate Allocation Process
    for (int i = 0; i < n; i++) {
        int chosenIdx = -1;

        if (strcmp(method, "First Fit") == 0) {
            for (int j = 0; j < m; j++) {
                if (!blocks[j].is_allocated && blocks[j].remaining_size >= processes[i].size) {
                    chosenIdx = j;
                    break;
                }
            }
        } 
        else if (strcmp(method, "Best Fit") == 0) {
            for (int j = 0; j < m; j++) {
                if (!blocks[j].is_allocated && blocks[j].remaining_size >= processes[i].size) {
                    if (chosenIdx == -1 || blocks[j].remaining_size < blocks[chosenIdx].remaining_size) {
                        chosenIdx = j;
                    }
                }
            }
        } 
        else if (strcmp(method, "Worst Fit") == 0) {
            for (int j = 0; j < m; j++) {
                if (!blocks[j].is_allocated && blocks[j].remaining_size >= processes[i].size) {
                    if (chosenIdx == -1 || blocks[j].remaining_size > blocks[chosenIdx].remaining_size) {
                        chosenIdx = j;
                    }
                }
            }
        }

        // Handle allocation execution
        if (chosenIdx != -1) {
            processes[i].allocated_block_id = blocks[chosenIdx].id;
            processes[i].internal_frag = blocks[chosenIdx].remaining_size - processes[i].size;
            
            blocks[chosenIdx].remaining_size = 0; // Contiguous block assigned to process
            blocks[chosenIdx].is_allocated = true;
            successful_allocations++;
        } else {
            failed_allocations++;
        }
    }

    // Calculate External Fragmentation
    int total_external_frag = 0;
    for (int j = 0; j < m; j++) {
        if (!blocks[j].is_allocated) {
            total_external_frag += blocks[j].remaining_size;
        }
    }

    // Print Lab Simulation Summary
    printf("\n=======================================================\n");
    printf("           %s STRATEGY ALLOCATION PATTERN             \n", method);
    printf("=======================================================\n");
    printf("Process No\tProcess Size\tBlock No\tInternal Frag\n");
    printf("-------------------------------------------------------\n");
    
    int total_internal_frag = 0;
    for (int i = 0; i < n; i++) {
        printf("%d\t\t%d\t\t", processes[i].id, processes[i].size);
        if (processes[i].allocated_block_id != -1) {
            printf("%d\t\t%d\n", processes[i].allocated_block_id, processes[i].internal_frag);
            total_internal_frag += processes[i].internal_frag;
        } else {
            printf("Not Allocated\t0\n");
        }
    }
    
    printf("-------------------------------------------------------\n");
    printf(">> STATISTICS SUMMARY:\n");
    printf("Total Successful Allocations : %d\n", successful_allocations);
    printf("Total Allocation Failures    : %d\n", failed_allocations);
    printf("Total Internal Fragmentation : %d KB\n", total_internal_frag);
    printf("Total External Fragmentation : %d KB\n", total_external_frag);
    printf("=======================================================\n");
}

int main() {
    int m, n;
    int current_address = 1000; // Base starting address simulation

    printf("Enter number of memory blocks: ");
    if (scanf("%d", &m) != 1) return 1;
    
    MemoryBlock blocks[m];
    for (int i = 0; i < m; i++) {
        blocks[i].id = i + 1;
        printf("Size of block %d: ", i + 1);
        if (scanf("%d", &blocks[i].total_size) != 1) return 1;
        blocks[i].remaining_size = blocks[i].total_size;
        blocks[i].start_address = current_address;
        blocks[i].is_allocated = false;
        current_address += blocks[i].total_size; // Track structural addresses
    }

    printf("\nEnter number of processes: ");
    if (scanf("%d", &n) != 1) return 1;
    
    Process processes[n];
    for (int i = 0; i < n; i++) {
        processes[i].id = i + 1;
        printf("Size of process %d: ", i + 1);
        if (scanf("%d", &processes[i].size) != 1) return 1;
    }

    // Run strategy engine simulation 
    simulate_memory_strategy(blocks, m, processes, n, "First Fit");
    simulate_memory_strategy(blocks, m, processes, n, "Best Fit");
    simulate_memory_strategy(blocks, m, processes, n, "Worst Fit");

    return 0;
}