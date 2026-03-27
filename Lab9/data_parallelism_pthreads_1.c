/*
 * Data Parallelism using Pthreads
 * OS Lab Program
 *
 * Concept: Divide a large array across N threads.
 *          Each thread computes the sum of its chunk in parallel.
 *          Main thread joins all and accumulates the total sum.
 *
 * Compile : gcc -o data_parallel data_parallelism_pthreads.c -lpthread
 * Run     : ./data_parallel
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define ARRAY_SIZE  16       /* Total number of elements in the array */
#define NUM_THREADS  4       /* Number of threads to spawn             */

/* ------------------------------------------------------------------ */
/* Shared data                                                          */
/* ------------------------------------------------------------------ */
int array[ARRAY_SIZE];       /* The array to process (shared, read-only) */
long partial_sum[NUM_THREADS]; /* Each thread writes its own partial sum  */

/* ------------------------------------------------------------------ */
/* Thread argument structure                                            */
/* ------------------------------------------------------------------ */
typedef struct {
    int thread_id;   /* Which thread is this?                  */
    int start_index; /* First index of this thread's chunk     */
    int end_index;   /* One-past-last index of the chunk       */
} ThreadArg;

/* ------------------------------------------------------------------ */
/* Thread function                                                      */
/* Each thread sums its assigned chunk and stores the result.          */
/* ------------------------------------------------------------------ */
void *compute_partial_sum(void *arg)
{
    ThreadArg *targ = (ThreadArg *)arg;
    long sum = 0;
    int i;

    printf("Thread %d: processing indices [%d, %d)\n",
           targ->thread_id, targ->start_index, targ->end_index);

    for (i = targ->start_index; i < targ->end_index; i++) {
        sum += array[i];
    }

    partial_sum[targ->thread_id] = sum;

    printf("Thread %d: partial sum = %ld\n", targ->thread_id, sum);

    pthread_exit(NULL);
}

/* ------------------------------------------------------------------ */
/* Main                                                                 */
/* ------------------------------------------------------------------ */
int main(void)
{
    pthread_t threads[NUM_THREADS];
    ThreadArg targs[NUM_THREADS];
    int chunk_size;
    int i;
    long total_sum   = 0;
    long serial_sum  = 0;

    /* ---- Initialise array with values 1..ARRAY_SIZE ---- */
    printf("Array: ");
    for (i = 0; i < ARRAY_SIZE; i++) {
        array[i] = i + 1;        /* 1, 2, 3, ..., 16 */
        printf("%d ", array[i]);
    }
    printf("\n\n");

    chunk_size = ARRAY_SIZE / NUM_THREADS;  /* Elements per thread */

    /* ---- Create threads ---- */
    for (i = 0; i < NUM_THREADS; i++) {
        targs[i].thread_id   = i;
        targs[i].start_index = i * chunk_size;
        targs[i].end_index   = (i == NUM_THREADS - 1)
                               ? ARRAY_SIZE              /* last thread takes remainder */
                               : (i + 1) * chunk_size;

        if (pthread_create(&threads[i], NULL, compute_partial_sum, &targs[i]) != 0) {
            perror("pthread_create failed");
            exit(EXIT_FAILURE);
        }
    }

    /* ---- Wait for all threads to finish ---- */
    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
        total_sum += partial_sum[i];   /* Accumulate partial sums */
    }

    /* ---- Serial sum for verification ---- */
    for (i = 0; i < ARRAY_SIZE; i++) {
        serial_sum += array[i];
    }

    /* ---- Results ---- */
    printf("\n--- Results ---\n");
    printf("Parallel total sum : %ld\n", total_sum);
    printf("Serial   total sum : %ld  (verification)\n", serial_sum);
    printf("Match              : %s\n", (total_sum == serial_sum) ? "YES" : "NO");

    return 0;
}