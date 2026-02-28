#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define MAX_ITEMS 10

// Monitor structure
typedef struct {
    int buffer[BUFFER_SIZE];
    int count;
    int in;
    int out;
    
    // Monitor locks and condition variables
    pthread_mutex_t mutex;
    pthread_cond_t not_full;
    pthread_cond_t not_empty;
} Monitor;

// Initialize monitor
void monitor_init(Monitor* mon) {
    mon->count = 0;
    mon->in = 0;
    mon->out = 0;
    pthread_mutex_init(&mon->mutex, NULL);
    pthread_cond_init(&mon->not_full, NULL);
    pthread_cond_init(&mon->not_empty, NULL);
}

// Monitor procedure: insert
void monitor_insert(Monitor* mon, int item, int producer_id) {
    pthread_mutex_lock(&mon->mutex);
    
    // Wait if buffer is full
    while(mon->count == BUFFER_SIZE) {
        printf("Producer %d: Buffer full, waiting...\n", producer_id);
        pthread_cond_wait(&mon->not_full, &mon->mutex);
    }
    
    // Insert item
    mon->buffer[mon->in] = item;
    printf("Producer %d: Produced item %d at position %d (count: %d)\n", 
           producer_id, item, mon->in, mon->count + 1);
    mon->in = (mon->in + 1) % BUFFER_SIZE;
    mon->count++;
    
    // Signal that buffer is not empty
    pthread_cond_signal(&mon->not_empty);
    
    pthread_mutex_unlock(&mon->mutex);
}

// Monitor procedure: remove
int monitor_remove(Monitor* mon, int consumer_id) {
    pthread_mutex_lock(&mon->mutex);
    
    // Wait if buffer is empty
    while(mon->count == 0) {
        printf("Consumer %d: Buffer empty, waiting...\n", consumer_id);
        pthread_cond_wait(&mon->not_empty, &mon->mutex);
    }
    
    // Remove item
    int item = mon->buffer[mon->out];
    printf("Consumer %d: Consumed item %d from position %d (count: %d)\n", 
           consumer_id, item, mon->out, mon->count - 1);
    mon->out = (mon->out + 1) % BUFFER_SIZE;
    mon->count--;
    
    // Signal that buffer is not full
    pthread_cond_signal(&mon->not_full);
    
    pthread_mutex_unlock(&mon->mutex);
    return item;
}

// Clean up monitor
void monitor_destroy(Monitor* mon) {
    pthread_mutex_destroy(&mon->mutex);
    pthread_cond_destroy(&mon->not_full);
    pthread_cond_destroy(&mon->not_empty);
}

// Shared monitor
Monitor monitor;

void* producer_monitor(void* arg) {
    int id = *(int*)arg;
    
    for(int i = 0; i < MAX_ITEMS; i++) {
        int item = rand() % 100;
        monitor_insert(&monitor, item, id);
        sleep(rand() % 2);
    }
    return NULL;
}

void* consumer_monitor(void* arg) {
    int id = *(int*)arg;
    
    for(int i = 0; i < MAX_ITEMS; i++) {
        monitor_remove(&monitor, id);
        sleep(rand() % 3);
    }
    return NULL;
}

int main() {
    pthread_t prod[2], cons[2];
    int prod_id[2] = {1, 2};
    int cons_id[2] = {1, 2};
    
    // Initialize monitor
    monitor_init(&monitor);
    
    printf("=== Producer-Consumer Problem using Monitor ===\n");
    printf("Buffer size: %d, Items per thread: %d\n\n", BUFFER_SIZE, MAX_ITEMS);
    
    // Create threads
    for(int i = 0; i < 2; i++) {
        pthread_create(&prod[i], NULL, producer_monitor, &prod_id[i]);
        pthread_create(&cons[i], NULL, consumer_monitor, &cons_id[i]);
    }
    
    // Wait for threads
    for(int i = 0; i < 2; i++) {
        pthread_join(prod[i], NULL);
        pthread_join(cons[i], NULL);
    }
    
    // Clean up
    monitor_destroy(&monitor);
    
    return 0;
}