#include <stdio.h> 
 
#define P 5  // Number of processes 
#define R 3  // Number of resource types 
 
// Global matrices and arrays 
int max[P][R], alloc[P][R], need[P][R], available[R]; 
 
// Function to calculate the Need matrix 
void calculateNeed() { 
    for (int i = 0; i < P; i++) { 
        for (int j = 0; j < R; j++) { 
            need[i][j] = max[i][j] - alloc[i][j]; 
        } 
    } 
} 
 
// Safety Algorithm to check if system is in a safe state 
int isSafe() { 
    int finish[P] = {0}; 
    int work[R]; 
 
    // Initialize work = available 
    for (int i = 0; i < R; i++) { 
        work[i] = available[i]; 
    } 
 
    int safeSequence[P]; 
    int count = 0; 
 
    while (count < P) { 
        int found = 0; 
        for (int i = 0; i < P; i++) { 
            if (!finish[i]) { 
                int canAllocate = 1; 
                for (int j = 0; j < R; j++) { 
                    if (need[i][j] > work[j]) { 
                        canAllocate = 0; 
                        break; 
                    } 
                } 
                if (canAllocate) { 
                    // Assume process i completes, release resources 
                    for (int j = 0; j < R; j++) { 
                        work[j] += alloc[i][j]; 
                    } 
                    finish[i] = 1; 
                    safeSequence[count] = i; 
                    count++; 
                    found = 1; 
                } 
            } 
        } 
        if (!found) { 
            // No process could be allocated resources in this pass 
            return 0; // Not safe 
        } 
    } 
 
    // If loop completes, system is in a safe state 
    printf("System is in a safe state.\nSafe sequence: "); 
    for (int i = 0; i < P; i++) { 
        printf("P%d ", safeSequence[i]); 
    } 
    printf("\n"); 
    return 1; 
} 
 
// Function to handle resource request from a process 
int requestResources(int process, int request[R]) { 
    // Check if request exceeds need 
    for (int j = 0; j < R; j++) { 
        if (request[j] > need[process][j]) { 
            printf("Error: Process P%d has exceeded its maximum 
claim.\n", process); 
            return 0; 
        } 
    } 
 
    // Check if resources are available 
    for (int j = 0; j < R; j++) { 
        if (request[j] > available[j]) { 
            printf("Resources unavailable for P%d. Process must wait.\n", 
process); 
            return 0; 
        } 
    } 
 
    // Try to allocate resources temporarily 
    for (int j = 0; j < R; j++) { 
        available[j] -= request[j]; 
        alloc[process][j] += request[j]; 
        need[process][j] -= request[j]; 
    } 
 
    // Check system safety after allocation 
    if (isSafe()) { 
        printf("Resources allocated to P%d.\n", process); 
        return 1; 
    } else { 
        // Rollback 
        for (int j = 0; j < R; j++) { 
            available[j] += request[j]; 
            alloc[process][j] -= request[j]; 
            need[process][j] += request[j]; 
        } 
        printf("Process P%d must wait, system not in safe state.\n", 
process); 
        return 0; 
    } 
} 
 
int main() { 
    // Example initialization 
    // Max matrix 
    int maxInput[P][R] = { 
        {7, 5, 3}, 
        {3, 2, 2}, 
        {9, 0, 2}, 
        {2, 2, 2}, 
        {4, 3, 3} 
    }; 
 
    // Allocation matrix 
    int allocInput[P][R] = { 
        {0, 1, 0}, 
        {2, 0, 0}, 
        {3, 0, 2}, 
        {2, 1, 1}, 
        {0, 0, 2} 
    }; 
 
    // Available resources 
    int availInput[R] = {3, 3, 2}; 
 
    // Assign to global variables 
    for (int i = 0; i < P; i++) { 
        for (int j = 0; j < R; j++) { 
            max[i][j] = maxInput[i][j]; 
            alloc[i][j] = allocInput[i][j]; 
        } 
    } 
    for (int j = 0; j < R; j++) { 
        available[j] = availInput[j]; 
    } 
 
    // Calculate Need matrix 
    calculateNeed(); 
 
    // Check initial system safety 
    isSafe(); 
 
    // Example resource requests 
    int request1[R] = {1, 0, 2}; // P1 requests 
    int request2[R] = {0, 2, 0}; // P0 requests 
 
    printf("\nProcess P1 requesting {1, 0, 2}\n"); 
    requestResources(1, request1); 
 
    printf("\nProcess P0 requesting {0, 2, 0}\n"); 
    requestResources(0, request2); 
 
    return 0; 
}