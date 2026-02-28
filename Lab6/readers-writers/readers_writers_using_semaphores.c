#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_READERS 3
#define NUM_WRITERS 2
#define READER_ITERATIONS 5
#define WRITER_ITERATIONS 3

// Shared data
int shared_data = 0;
int reader_count = 0;

// Semaphores
sem_t mutex;      // Protects reader_count
sem_t wrt;        // For writer exclusive access
pthread_mutex_t print_mutex; // For clean console output

void* reader(void* arg) {
    int id = *(int*)arg;
    
    for(int i = 0; i < READER_ITERATIONS; i++) {
        // Entry section
        sem_wait(&mutex);
        reader_count++;
        if(reader_count == 1) {
            sem_wait(&wrt);  // First reader locks writers out
        }
        sem_post(&mutex);
        
        // Critical section (Reading)
        pthread_mutex_lock(&print_mutex);
        printf("Reader %d: reading shared_data = %d (active readers: %d)\n", 
               id, shared_data, reader_count);
        pthread_mutex_unlock(&print_mutex);
        sleep(rand() % 2);
        
        // Exit section
        sem_wait(&mutex);
        reader_count--;
        if(reader_count == 0) {
            sem_post(&wrt);  // Last reader allows writers
        }
        sem_post(&mutex);
        
        sleep(rand() % 2);
    }
    return NULL;
}

void* writer(void* arg) {
    int id = *(int*)arg;
    
    for(int i = 0; i < WRITER_ITERATIONS; i++) {
        // Entry section
        sem_wait(&wrt);
        
        // Critical section (Writing)
        shared_data++;
        pthread_mutex_lock(&print_mutex);
        printf("Writer %d: writing shared_data = %d\n", id, shared_data);
        pthread_mutex_unlock(&print_mutex);
        sleep(rand() % 3);
        
        // Exit section
        sem_post(&wrt);
        
        sleep(rand() % 2);
    }
    return NULL;
}

int main() {
    pthread_t readers[NUM_READERS], writers[NUM_WRITERS];
    int reader_id[NUM_READERS], writer_id[NUM_WRITERS];
    
    // Initialize semaphores
    sem_init(&mutex, 0, 1);
    sem_init(&wrt, 0, 1);
    pthread_mutex_init(&print_mutex, NULL);
    
    printf("=== Readers-Writers Problem (Semaphore Solution) ===\n");
    printf("Initial shared_data = %d\n\n", shared_data);
    
    // Create reader threads
    for(int i = 0; i < NUM_READERS; i++) {
        reader_id[i] = i + 1;
        pthread_create(&readers[i], NULL, reader, &reader_id[i]);
    }
    
    // Create writer threads
    for(int i = 0; i < NUM_WRITERS; i++) {
        writer_id[i] = i + 1;
        pthread_create(&writers[i], NULL, writer, &writer_id[i]);
    }
    
    // Wait for all threads to complete
    for(int i = 0; i < NUM_READERS; i++) {
        pthread_join(readers[i], NULL);
    }
    for(int i = 0; i < NUM_WRITERS; i++) {
        pthread_join(writers[i], NULL);
    }
    
    // Clean up
    sem_destroy(&mutex);
    sem_destroy(&wrt);
    pthread_mutex_destroy(&print_mutex);
    
    printf("\nFinal shared_data = %d\n", shared_data);
    return 0;
}