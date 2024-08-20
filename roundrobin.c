#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int id;
    int burstTime;
} Process;

void roundRobin(Process processes[], int numProcesses, int quantum);

int main() {
    int numProcesses, quantum;

    printf("Enter the number of processes: ");
    scanf("%d", &numProcesses);

    Process *processes = (Process*)malloc(numProcesses * sizeof(Process));

    for (int i = 0; i < numProcesses; i++) {
        printf("Enter burst time for process %d: ", i + 1);
        scanf("%d", &processes[i].burstTime);
        processes[i].id = i + 1;
    }

    printf("Enter the quantum time: ");
    scanf("%d", &quantum);

    roundRobin(processes, numProcesses, quantum);

    free(processes);

    return 0;
}

void roundRobin(Process processes[], int numProcesses, int quantum) {
    int *remainingBurstTimes = (int*)malloc(numProcesses * sizeof(int));
    for (int i = 0; i < numProcesses; i++) {
        remainingBurstTimes[i] = processes[i].burstTime;
    }

    int allDone = 0;
    while (!allDone) {
        allDone = 1;
        for (int i = 0; i < numProcesses; i++) {
            if (remainingBurstTimes[i] > 0) {
                allDone = 0;
                if (remainingBurstTimes[i] > quantum) {
                    printf("Process %d executed for %d units\n", processes[i].id, quantum);
                    remainingBurstTimes[i] -= quantum;
                } else {
                    printf("Process %d executed for %d units (finished)\n", processes[i].id, remainingBurstTimes[i]);
                    remainingBurstTimes[i] = 0;
                }
            }
        }
    }

    free(remainingBurstTimes);
}

