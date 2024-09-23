#include <stdio.h>
#include <limits.h>

struct Process {
    int pid;        // Process ID
    int arrival;    // Arrival Time
    int burst;      // Burst Time
    int remaining;  // Remaining Burst Time
    int completion; // Completion Time
    int turnaround; // Turnaround Time
    int waiting;    // Waiting Time
};

void calculateTimes(struct Process proc[], int n, int gantt[], int *time_size) {
    int completed = 0, current_time = 0, min_idx;
    int min_remaining_time;
    int is_completed[n];

    for (int i = 0; i < n; i++) {
        is_completed[i] = 0;
        proc[i].remaining = proc[i].burst;
    }

    int g_index = 0; // Gantt chart time index

    while (completed != n) {
        min_idx = -1;
        min_remaining_time = INT_MAX;

        // Find the process with the smallest remaining burst time
        for (int i = 0; i < n; i++) {
            if (proc[i].arrival <= current_time && !is_completed[i] && proc[i].remaining < min_remaining_time) {
                min_idx = i;
                min_remaining_time = proc[i].remaining;
            }
        }

        if (min_idx == -1) {  // No process is currently available
            gantt[g_index++] = -1;  // Idle time
            current_time++;
            continue;
        }

        gantt[g_index++] = proc[min_idx].pid;  // Add process ID to the Gantt chart
        proc[min_idx].remaining--;
        current_time++;

        // If the process is completed
        if (proc[min_idx].remaining == 0) {
            completed++;
            is_completed[min_idx] = 1;
            proc[min_idx].completion = current_time;
            proc[min_idx].turnaround = proc[min_idx].completion - proc[min_idx].arrival;
            proc[min_idx].waiting = proc[min_idx].turnaround - proc[min_idx].burst;
        }
    }

    *time_size = g_index; // Total time slots used in the Gantt chart
}

void printProcesses(struct Process proc[], int n) {
    printf("\nPID\tArrival\tBurst\tCompletion\tTurnaround\tWaiting\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t\t%d\t\t%d\n", proc[i].pid, proc[i].arrival, proc[i].burst, 
               proc[i].completion, proc[i].turnaround, proc[i].waiting);
    }
}

void printGanttChart(int gantt[], int time_size) {
    printf("\nGantt Chart:\n");

    // Print top bar
    printf(" ");
    for (int i = 0; i < time_size; i++) {
        if (gantt[i] == -1) {
            printf("----");
        } else {
            printf("----");
        }
    }
    printf("-\n");

    // Print process execution in time slots
    printf("|");
    for (int i = 0; i < time_size; i++) {
        if (gantt[i] == -1) {
            printf(" IDL|"); // Idle time
        } else {
            printf(" P%d |", gantt[i]);
        }
    }
    printf("\n");

    // Print bottom bar
    printf(" ");
    for (int i = 0; i < time_size; i++) {
        if (gantt[i] == -1) {
            printf("----");
        } else {
            printf("----");
        }
    }
    printf("-\n");

    // Print timeline
    printf("0");
    for (int i = 1; i <= time_size; i++) {
        printf("%4d", i);
    }
    printf("\n");
}

int main() {
    int n;

    // User-driven input
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    struct Process proc[n];

    for (int i = 0; i < n; i++) {
        printf("Enter arrival time and burst time for process %d: ", i + 1);
        proc[i].pid = i + 1;
        scanf("%d %d", &proc[i].arrival, &proc[i].burst);
    }

    int gantt[100], time_size;

    calculateTimes(proc, n, gantt, &time_size);
    printProcesses(proc, n);
    printGanttChart(gantt, time_size);

    return 0;
}

