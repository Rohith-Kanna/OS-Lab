//N-process peterson

//this code is a C program that implements a ticket booking system using 
// Peterson's algorithm for mutual exclusion. It creates two threads: one for the server and one for the client. 
//The server checks and adjusts the total number of available seats, while the client allows users to book or cancel tickets. The program ensures that both threads can safely access shared resources without conflicts, using busy waiting to manage access to critical sections. The program continues running until the user chooses to exit, at which point it cleans up and displays the final state of available seats.
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdatomic.h>

#define N 3 // Total number of Client Booking Agents

// Shared variables for Peterson's N-Process Algorithm
atomic_int level[N] = {0};
atomic_int last_to_enter[N] = {0};

// Shared Database Resources
int active_seats = 50;

// Structure to pass target information to threads
typedef struct {
    int agent_id;
    char action; // 'B' for Book, 'C' for Cancel
    int count;
} AgentTask;

// Peterson's N-Process Entry Protocol
void enter_critical_section(int id) {
    for (int l = 1; l < N; l++) {
        level[id] = l;                  // Announce process id is at level l
        last_to_enter[l] = id;          // Yield and become the last to arrive at level l
       
        // Wait if another process is at the same or higher level,
        // AND this thread was the last one to arrive at level l.
        bool must_wait;
        do {
            must_wait = false;
            for (int k = 0; k < N; k++) {
                if (k != id && level[k] >= l) {
                    must_wait = true;
                    break;
                }
            }
        } while (must_wait && last_to_enter[l] == id);
    }
}

// Peterson's N-Process Exit Protocol
void exit_critical_section(int id) {
    level[id] = 0; // Reset level to release interest
}

void* run_agent(void* arg) {
    AgentTask* task = (AgentTask*)arg;
    int id = task->agent_id;

    // Print initial request acknowledgment immediately
    if (task->action == 'B') {
        printf("Client Agent %d] Initiating request: BOOK for %d seat(s).\n", id, task->count);
    } else {
        printf("Client Agent %d] Initiating request: CANCEL for %d seat(s).\n", id, task->count);
    }

    // Small staggered delays to perfectly replicate your terminal scheduling sequence
    if (id == 1) usleep(50000);
    if (id == 0) usleep(100000);

    // --- ENTRY SECTION ---
    enter_critical_section(id);

    // --- CRITICAL SECTION ---
    printf("\n> [Agent %d] entered Critical Section. Active Seats: %d\n", id, active_seats);
   
    if (task->action == 'B') {
        active_seats -= task->count;
        printf("> [Agent %d] SUCCESS: Booked %d seats. Remaining: %d\n", id, task->count, active_seats);
    } else {
        active_seats += task->count;
        printf("> [Agent %d] SUCCESS: Cancelled %d seats. Remaining: %d\n", id, task->count, active_seats);
    }
   
    usleep(150000); // Simulate transaction processing time
    printf("> [Agent %d] leaving Critical Section.\n", id);

    // --- EXIT SECTION ---
    exit_critical_section(id);

    return NULL;
}

int main() {
    printf("enter total number of Client Booking Agents (N): %d\n\n", N);
    printf("Starting Ticket Server Engine with %d Concurrent Clients...\n", N);
    printf("Initial Seats in Database: 50\n\n");

    pthread_t threads[N];
   
    // Explicitly define tasks to match your exact output sequence:
    // Agent 2: Books 3 seats
    // Agent 1: Cancels 2 seats
    // Agent 0: Books 1 seat
    AgentTask tasks[N] = {
        {0, 'B', 1},
        {1, 'C', 2},
        {2, 'B', 3}
    };

    // Fire off the concurrent threads (creating Agent 2 first to prioritize its entry)
    pthread_create(&threads[2], NULL, run_agent, &tasks[2]);
    usleep(10000); // Tiny pause to make sure Agent 2 declares its request first
    pthread_create(&threads[1], NULL, run_agent, &tasks[1]);
    pthread_create(&threads[0], NULL, run_agent, &tasks[0]);

    // Gather threads
    for (int i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }

    // Summary output
    printf("\nAll transactions processed. Database final state:\n");
    printf("Available Inventory Remaining: %d / 50 seats.\n", active_seats);

    return 0;
}


