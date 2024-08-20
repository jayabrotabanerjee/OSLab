#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int id;
    int priority;
    int burstTime;
} Process;

typedef struct {
    Process *items;
    int capacity;
    int size;
} PriorityQueue;

PriorityQueue* createPriorityQueue(int capacity);
void insert(PriorityQueue *pq, int id, int priority, int burstTime);
Process extractMin(PriorityQueue *pq);
void minHeapify(PriorityQueue *pq, int index);
void swap(Process *a, Process *b);
void printPriorityQueue(PriorityQueue *pq);

int main() {
    int capacity;
    printf("Enter the number of processes: ");
    scanf("%d", &capacity);

    PriorityQueue *pq = createPriorityQueue(capacity);

    for (int i = 0; i < capacity; i++) {
        int priority, burstTime;
        printf("Enter priority and burst time for process %d: ", i + 1);
        scanf("%d %d", &priority, &burstTime);
        insert(pq, i + 1, priority, burstTime); // Automatically assign IDs starting from 1
    }

    printf("Priority Queue:\n");
    printPriorityQueue(pq);

    printf("Extracting processes in priority order:\n");
    while (pq->size > 0) {
        Process proc = extractMin(pq);
        printf("Process ID: %d, Priority: %d, Burst Time: %d\n", proc.id, proc.priority, proc.burstTime);
    }

    free(pq->items);
    free(pq);

    return 0;
}

PriorityQueue* createPriorityQueue(int capacity) {
    PriorityQueue *pq = (PriorityQueue*)malloc(sizeof(PriorityQueue));
    pq->capacity = capacity;
    pq->size = 0;
    pq->items = (Process*)malloc(capacity * sizeof(Process));
    return pq;
}

void swap(Process *a, Process *b) {
    Process temp = *a;
    *a = *b;
    *b = temp;
}

void insert(PriorityQueue *pq, int id, int priority, int burstTime) {
    if (pq->size == pq->capacity) {
        printf("Priority Queue is full\n");
        return;
    }

    int index = pq->size++;
    pq->items[index].id = id;
    pq->items[index].priority = priority;
    pq->items[index].burstTime = burstTime;

    while (index != 0 && pq->items[index].priority < pq->items[(index - 1) / 2].priority) {
        swap(&pq->items[index], &pq->items[(index - 1) / 2]);
        index = (index - 1) / 2;
    }
}

Process extractMin(PriorityQueue *pq) {
    if (pq->size <= 0) {
        printf("Priority Queue is empty\n");
        Process emptyProc = {-1, -1, -1};
        return emptyProc;
    }

    if (pq->size == 1) {
        pq->size--;
        return pq->items[0];
    }

    Process root = pq->items[0];
    pq->items[0] = pq->items[--pq->size];
    minHeapify(pq, 0);

    return root;
}

void minHeapify(PriorityQueue *pq, int index) {
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < pq->size && pq->items[left].priority < pq->items[smallest].priority) {
        smallest = left;
    }

    if (right < pq->size && pq->items[right].priority < pq->items[smallest].priority) {
        smallest = right;
    }

    if (smallest != index) {
        swap(&pq->items[index], &pq->items[smallest]);
        minHeapify(pq, smallest);
    }
}

void printPriorityQueue(PriorityQueue *pq) {
    for (int i = 0; i < pq->size; i++) {
        printf("Process ID: %d, Priority: %d, Burst Time: %d\n", pq->items[i].id, pq->items[i].priority, pq->items[i].burstTime);
    }
    printf("\n");
}

