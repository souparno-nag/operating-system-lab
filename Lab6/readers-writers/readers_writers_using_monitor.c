#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_READERS 3
#define NUM_WRITERS 2
#define READER_ITERATIONS 5
#define WRITER_ITERATIONS 3

// Database monitor structure
typedef struct {
    int shared_data;
    int reader_count;
    int writer_waiting;  // For writer priority (Second Readers-Writers problem)
    
    // Synchronization primitives
    pthread_mutex_t mutex;
    pthread_cond_t readers_cond;
    pthread_cond_t writers_cond;
    
    // Flags for coordination
    int writer_active;
    int writer_count;  // Number of waiting writers
} DatabaseMonitor;

// Initialize monitor
void monitor_init(DatabaseMonitor* db) {
    db->shared_data = 0;
    db->reader_count = 0;
    db->writer_waiting = 0;
    db->writer_active = 0;
    db->writer_count = 0;
    
    pthread_mutex_init(&db->mutex, NULL);
    pthread_cond_init(&db->readers_cond, NULL);
    pthread_cond_init(&db->writers_cond, NULL);
}

// Monitor procedure: start_read
void monitor_start_read(DatabaseMonitor* db, int reader_id) {
    pthread_mutex_lock(&db->mutex);
    
    // Wait if there's an active writer or waiting writers (writer priority)
    while(db->writer_active > 0 || db->writer_count > 0) {
        printf("Reader %d: Waiting (writer active: %d, writers waiting: %d)\n", 
               reader_id, db->writer_active, db->writer_count);
        pthread_cond_wait(&db->readers_cond, &db->mutex);
    }
    
    db->reader_count++;
    printf("Reader %d: Started reading (active readers: %d)\n", 
           reader_id, db->reader_count);
    
    pthread_mutex_unlock(&db->mutex);
}

// Monitor procedure: end_read
void monitor_end_read(DatabaseMonitor* db, int reader_id) {
    pthread_mutex_lock(&db->mutex);
    
    db->reader_count--;
    printf("Reader %d: Finished reading (active readers: %d)\n", 
           reader_id, db->reader_count);
    
    // If this was the last reader and there are waiting writers
    if(db->reader_count == 0 && db->writer_count > 0) {
        pthread_cond_signal(&db->writers_cond);
    }
    
    pthread_mutex_unlock(&db->mutex);
}

// Monitor procedure: start_write
void monitor_start_write(DatabaseMonitor* db, int writer_id) {
    pthread_mutex_lock(&db->mutex);
    
    db->writer_count++;
    
    // Wait if there are active readers or another writer
    while(db->reader_count > 0 || db->writer_active > 0) {
        printf("Writer %d: Waiting (active readers: %d, active writer: %d)\n", 
               writer_id, db->reader_count, db->writer_active);
        pthread_cond_wait(&db->writers_cond, &db->mutex);
    }
    
    db->writer_count--;
    db->writer_active = 1;
    
    printf("Writer %d: Started writing\n", writer_id);
    
    pthread_mutex_unlock(&db->mutex);
}

// Monitor procedure: end_write
void monitor_end_write(DatabaseMonitor* db, int writer_id, int value) {
    pthread_mutex_lock(&db->mutex);
    
    db->shared_data = value;
    db->writer_active = 0;
    
    printf("Writer %d: Finished writing, shared_data = %d\n", 
           writer_id, db->shared_data);
    
    // Signal waiting writers first (writer priority)
    if(db->writer_count > 0) {
        pthread_cond_signal(&db->writers_cond);
    } else {
        // If no writers waiting, signal all readers
        pthread_cond_broadcast(&db->readers_cond);
    }
    
    pthread_mutex_unlock(&db->mutex);
}

// Monitor procedure: read
int monitor_read(DatabaseMonitor* db, int reader_id) {
    monitor_start_read(db, reader_id);
    
    // Reading critical section
    int value = db->shared_data;
    sleep(rand() % 2);  // Simulate reading time
    
    monitor_end_read(db, reader_id);
    return value;
}

// Monitor procedure: write
void monitor_write(DatabaseMonitor* db, int writer_id, int new_value) {
    monitor_start_write(db, writer_id);
    
    // Writing critical section
    sleep(rand() % 3);  // Simulate writing time
    monitor_end_write(db, writer_id, new_value);
}

// Clean up monitor
void monitor_destroy(DatabaseMonitor* db) {
    pthread_mutex_destroy(&db->mutex);
    pthread_cond_destroy(&db->readers_cond);
    pthread_cond_destroy(&db->writers_cond);
}

// Shared database monitor
DatabaseMonitor db_monitor;
pthread_mutex_t print_mutex;

void* reader_monitor(void* arg) {
    int id = *(int*)arg;
    
    for(int i = 0; i < READER_ITERATIONS; i++) {
        int value = monitor_read(&db_monitor, id);
        
        pthread_mutex_lock(&print_mutex);
        printf("Reader %d: Read value = %d\n", id, value);
        pthread_mutex_unlock(&print_mutex);
        
        sleep(rand() % 2);
    }
    return NULL;
}

void* writer_monitor(void* arg) {
    int id = *(int*)arg;
    static int write_counter = 100;  // For generating unique values
    
    for(int i = 0; i < WRITER_ITERATIONS; i++) {
        write_counter++;
        monitor_write(&db_monitor, id, write_counter);
        
        pthread_mutex_lock(&print_mutex);
        printf("Writer %d: Wrote value = %d\n", id, write_counter);
        pthread_mutex_unlock(&print_mutex);
        
        sleep(rand() % 3);
    }
    return NULL;
}

int main() {
    pthread_t readers[NUM_READERS], writers[NUM_WRITERS];
    int reader_id[NUM_READERS], writer_id[NUM_WRITERS];
    
    // Initialize monitor and print mutex
    monitor_init(&db_monitor);
    pthread_mutex_init(&print_mutex, NULL);
    
    printf("=== Readers-Writers Problem (Monitor Solution) ===\n");
    printf("Initial shared_data = %d\n", db_monitor.shared_data);
    printf("Writer Priority: Yes\n\n");
    
    // Create reader threads
    for(int i = 0; i < NUM_READERS; i++) {
        reader_id[i] = i + 1;
        pthread_create(&readers[i], NULL, reader_monitor, &reader_id[i]);
    }
    
    // Create writer threads
    for(int i = 0; i < NUM_WRITERS; i++) {
        writer_id[i] = i + 1;
        pthread_create(&writers[i], NULL, writer_monitor, &writer_id[i]);
    }
    
    // Wait for all threads to complete
    for(int i = 0; i < NUM_READERS; i++) {
        pthread_join(readers[i], NULL);
    }
    for(int i = 0; i < NUM_WRITERS; i++) {
        pthread_join(writers[i], NULL);
    }
    
    // Clean up
    monitor_destroy(&db_monitor);
    pthread_mutex_destroy(&print_mutex);
    
    printf("\nFinal shared_data = %d\n", db_monitor.shared_data);
    return 0;
}