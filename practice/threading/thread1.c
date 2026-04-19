#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 100
#define THREADS 4

int arr[SIZE];
int partial_sum[THREADS];

void* add_sum(void* arg) {
    int thread_id = *(int*)arg;
    int start = (thread_id) * (SIZE/THREADS);
    int stop = start + (SIZE/THREADS);

    partial_sum[thread_id] = 0;

    for (int i = start; i < stop; i++) {
        partial_sum[thread_id] += arr[i];
    }

    pthread_exit(0);
}

int main() {
    pthread_t threads[THREADS];
    int thread_id[THREADS];

    for (int i = 0; i < SIZE; i++) arr[i] = i+1;

    for (int i = 0; i < THREADS; i++) {
        thread_id[i] = i;
        pthread_create(&threads[i], NULL, add_sum, &thread_id[i]);
    }

    for (int i = 0; i < THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    int total = 0;
    for (int i = 0; i < THREADS; i++) {
        total += partial_sum[i];
    }

    printf("The total sum is %d\n", total);
}