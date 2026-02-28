#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define MAX_ITEMS 10

// Buffer
int buffer[BUFFER_SIZE];
int in = 0, out = 0;

// Semaphores
sem_t empty;  // Counts empty slots
sem_t full;   // Counts full slots
pthread_mutex_t mutex;  // Binary semaphore for mutual exclusion

int item_count = 0;

void* producer(void* arg) {
    int id = *(int*)arg;
    
    for(int i = 0; i < MAX_ITEMS; i++) {
        // Produce an item
        int item = rand() % 100;
        
        // Wait for empty slot
        sem_wait(&empty);
        
        // Enter critical section
        pthread_mutex_lock(&mutex);
        
        // Add item to buffer
        buffer[in] = item;
        printf("Producer %d: Produced item %d at position %d\n", id, item, in);
        in = (in + 1) % BUFFER_SIZE;
        item_count++;
        
        // Exit critical section
        pthread_mutex_unlock(&mutex);
        
        // Signal that buffer has a new item
        sem_post(&full);
        
        sleep(rand() % 2);
    }
    return NULL;
}

void* consumer(void* arg) {
    int id = *(int*)arg;
    
    for(int i = 0; i < MAX_ITEMS; i++) {
        // Wait for full slot
        sem_wait(&full);
        
        // Enter critical section
        pthread_mutex_lock(&mutex);
        
        // Remove item from buffer
        int item = buffer[out];
        printf("Consumer %d: Consumed item %d from position %d\n", id, item, out);
        out = (out + 1) % BUFFER_SIZE;
        item_count--;
        
        // Exit critical section
        pthread_mutex_unlock(&mutex);
        
        // Signal that buffer has an empty slot
        sem_post(&empty);
        
        sleep(rand() % 3);
    }
    return NULL;
}

int main() {
    pthread_t prod[2], cons[2];
    int prod_id[2] = {1, 2};
    int cons_id[2] = {1, 2};
    
    // Initialize semaphores
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);
    
    // Create producer and consumer threads
    for(int i = 0; i < 2; i++) {
        pthread_create(&prod[i], NULL, producer, &prod_id[i]);
        pthread_create(&cons[i], NULL, consumer, &cons_id[i]);
    }
    
    // Wait for threads to complete
    for(int i = 0; i < 2; i++) {
        pthread_join(prod[i], NULL);
        pthread_join(cons[i], NULL);
    }
    
    // Clean up
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);
    
    return 0;
}