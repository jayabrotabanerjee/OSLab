#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_PHILOSOPHERS 10

pthread_mutex_t forks[MAX_PHILOSOPHERS];
int num_philosophers;

void *philosopher(void *num) {
    int id = *(int *)num;
    
    while (1) {
        printf("Philosopher %d is thinking.\n", id);
        sleep(rand() % 3);  // Thinking time

        // Pick up forks
        pthread_mutex_lock(&forks[id]);
        pthread_mutex_lock(&forks[(id + 1) % num_philosophers]);
        
        // Eating
        printf("Philosopher %d is eating.\n", id);
        sleep(rand() % 3);  // Eating time
        
        // Put down forks
        pthread_mutex_unlock(&forks[id]);
        pthread_mutex_unlock(&forks[(id + 1) % num_philosophers]);
    }

    return NULL;
}

int main() {
    printf("Enter the number of philosophers (1 to %d): ", MAX_PHILOSOPHERS);
    scanf("%d", &num_philosophers);
    
    if (num_philosophers < 1 || num_philosophers > MAX_PHILOSOPHERS) {
        printf("Invalid number of philosophers. Exiting.\n");
        return 1;
    }

    pthread_t philosophers[num_philosophers];
    int ids[num_philosophers];

    // Initialize mutexes for forks
    for (int i = 0; i < num_philosophers; i++) {
        pthread_mutex_init(&forks[i], NULL);
    }

    // Create philosopher threads
    for (int i = 0; i < num_philosophers; i++) {
        ids[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &ids[i]);
    }

    // Join philosopher threads (this will run indefinitely)
    for (int i = 0; i < num_philosophers; i++) {
        pthread_join(philosophers[i], NULL);
    }

    // Cleanup mutexes (this code will never be reached in this example)
    for (int i = 0; i < num_philosophers; i++) {
        pthread_mutex_destroy(&forks[i]);
    }

    return 0;
}

