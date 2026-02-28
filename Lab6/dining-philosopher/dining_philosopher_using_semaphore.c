#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define NUM_PHILOSOPHERS 5
#define THINKING 0
#define HUNGRY 1
#define EATING 2
#define LEFT (philosopher_id + 4) % NUM_PHILOSOPHERS
#define RIGHT (philosopher_id + 1) % NUM_PHILOSOPHERS

// Semaphores
sem_t chopsticks[NUM_PHILOSOPHERS];
pthread_mutex_t mutex;  // For protecting state changes

int state[NUM_PHILOSOPHERS];
int eat_count[NUM_PHILOSOPHERS] = {0};  // Track how many times each philosopher ate

void test(int philosopher_id) {
    // If philosopher is hungry and neighbors are not eating
    if (state[philosopher_id] == HUNGRY && 
        state[LEFT] != EATING && 
        state[RIGHT] != EATING) {
        
        state[philosopher_id] = EATING;
        printf("Philosopher %d starts EATING (meal #%d)\n", 
               philosopher_id + 1, ++eat_count[philosopher_id]);
        sem_post(&chopsticks[philosopher_id]);  // Grant chopsticks
    }
}

void pickup_chopsticks(int philosopher_id) {
    pthread_mutex_lock(&mutex);
    
    state[philosopher_id] = HUNGRY;
    printf("Philosopher %d is HUNGRY\n", philosopher_id + 1);
    
    // Try to acquire chopsticks
    test(philosopher_id);
    
    pthread_mutex_unlock(&mutex);
    
    // Wait if chopsticks not available
    sem_wait(&chopsticks[philosopher_id]);
}

void putdown_chopsticks(int philosopher_id) {
    pthread_mutex_lock(&mutex);
    
    state[philosopher_id] = THINKING;
    printf("Philosopher %d is THINKING\n", philosopher_id + 1);
    
    // Check if neighbors can now eat
    test(LEFT);
    test(RIGHT);
    
    pthread_mutex_unlock(&mutex);
}

void* philosopher(void* arg) {
    int philosopher_id = *(int*)arg;
    
    while(1) {
        // Thinking
        printf("Philosopher %d is THINKING\n", philosopher_id + 1);
        sleep(rand() % 3 + 1);  // Think for 1-3 seconds
        
        // Hungry - try to eat
        pickup_chopsticks(philosopher_id);
        
        // Eating
        sleep(rand() % 2 + 1);  // Eat for 1-2 seconds
        
        // Done eating
        putdown_chopsticks(philosopher_id);
    }
    
    return NULL;
}

int main() {
    pthread_t philosophers[NUM_PHILOSOPHERS];
    int philosopher_ids[NUM_PHILOSOPHERS];
    
    // Initialize random seed
    srand(time(NULL));
    
    // Initialize mutex
    pthread_mutex_init(&mutex, NULL);
    
    // Initialize semaphores
    for(int i = 0; i < NUM_PHILOSOPHERS; i++) {
        sem_init(&chopsticks[i], 0, 0);  // Initialize to 0 (not available)
        state[i] = THINKING;
    }
    
    printf("=== Dining Philosopher Problem (Semaphore Solution) ===\n");
    printf("Number of philosophers: %d\n\n", NUM_PHILOSOPHERS);
    
    // Create philosopher threads
    for(int i = 0; i < NUM_PHILOSOPHERS; i++) {
        philosopher_ids[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &philosopher_ids[i]);
    }
    
    // Let the simulation run for 30 seconds
    sleep(30);
    
    printf("\n=== Simulation Complete ===\n");
    printf("Eating summary:\n");
    for(int i = 0; i < NUM_PHILOSOPHERS; i++) {
        printf("Philosopher %d ate %d times\n", i + 1, eat_count[i]);
    }
    
    // Cancel threads
    for(int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_cancel(philosophers[i]);
        pthread_join(philosophers[i], NULL);
    }
    
    // Clean up
    pthread_mutex_destroy(&mutex);
    for(int i = 0; i < NUM_PHILOSOPHERS; i++) {
        sem_destroy(&chopsticks[i]);
    }
    
    return 0;
}