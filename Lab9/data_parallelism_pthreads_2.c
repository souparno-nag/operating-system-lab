/*
 * Data Parallelism using Pthreads — Performance Comparison
 * OS Lab Program
 *
 * Compares execution time of:
 *   (1) Serial summation of a large array
 *   (2) Parallel summation using Pthreads
 *
 * Compile : gcc -O0 -o perf_compare data_parallel_performance.c -lpthread
 * Run     : ./perf_compare
 *
 * Note: -O0 disables compiler optimisations so timing differences are visible.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define ARRAY_SIZE   100000000   /* 100 million elements */
#define NUM_THREADS  4

/* ------------------------------------------------------------------ */
/* Shared data                                                          */
/* ------------------------------------------------------------------ */
long long *array;                      /* Dynamically allocated large array */
long long  partial_sum[NUM_THREADS];   /* One slot per thread, no races     */

/* ------------------------------------------------------------------ */
/* Helper: elapsed time in seconds between two timespec values          */
/* ------------------------------------------------------------------ */
double elapsed_seconds(struct timespec start, struct timespec end)
{
    return (end.tv_sec - start.tv_sec)
         + (end.tv_nsec - start.tv_nsec) / 1e9;
}

/* ------------------------------------------------------------------ */
/* Thread argument structure                                            */
/* ------------------------------------------------------------------ */
typedef struct {
    int thread_id;
    int start_index;
    int end_index;
} ThreadArg;

/* ------------------------------------------------------------------ */
/* Thread function: sum assigned chunk, store in partial_sum[]          */
/* ------------------------------------------------------------------ */
void *parallel_sum(void *arg)
{
    ThreadArg  *targ = (ThreadArg *)arg;
    long long   sum  = 0;
    int i;

    for (i = targ->start_index; i < targ->end_index; i++) {
        sum += array[i];
    }

    partial_sum[targ->thread_id] = sum;
    pthread_exit(NULL);
}

/* ------------------------------------------------------------------ */
/* Serial computation                                                   */
/* ------------------------------------------------------------------ */
long long serial_computation(void)
{
    long long sum = 0;
    int i;
    for (i = 0; i < ARRAY_SIZE; i++) {
        sum += array[i];
    }
    return sum;
}

/* ------------------------------------------------------------------ */
/* Parallel computation                                                 */
/* ------------------------------------------------------------------ */
long long parallel_computation(void)
{
    pthread_t threads[NUM_THREADS];
    ThreadArg targs[NUM_THREADS];
    int chunk_size = ARRAY_SIZE / NUM_THREADS;
    long long total = 0;
    int i;

    /* Create threads */
    for (i = 0; i < NUM_THREADS; i++) {
        targs[i].thread_id   = i;
        targs[i].start_index = i * chunk_size;
        targs[i].end_index   = (i == NUM_THREADS - 1)
                               ? ARRAY_SIZE
                               : (i + 1) * chunk_size;

        if (pthread_create(&threads[i], NULL, parallel_sum, &targs[i]) != 0) {
            perror("pthread_create failed");
            exit(EXIT_FAILURE);
        }
    }

    /* Join threads and accumulate */
    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
        total += partial_sum[i];
    }

    return total;
}

/* ------------------------------------------------------------------ */
/* Main                                                                 */
/* ------------------------------------------------------------------ */
int main(void)
{
    struct timespec t_start, t_end;
    double serial_time, parallel_time;
    long long serial_result, parallel_result;
    int i;

    /* Allocate and initialise array */
    array = (long long *)malloc(ARRAY_SIZE * sizeof(long long));
    if (!array) {
        perror("malloc failed");
        return EXIT_FAILURE;
    }

    printf("Initialising array of %d elements...\n", ARRAY_SIZE);
    for (i = 0; i < ARRAY_SIZE; i++) {
        array[i] = 1;   /* Every element = 1, so expected sum = ARRAY_SIZE */
    }

    printf("Array size  : %d elements\n", ARRAY_SIZE);
    printf("Threads     : %d\n\n", NUM_THREADS);

    /* ---- Serial ---- */
    printf("Running serial computation...\n");
    clock_gettime(CLOCK_MONOTONIC, &t_start);
    serial_result = serial_computation();
    clock_gettime(CLOCK_MONOTONIC, &t_end);
    serial_time = elapsed_seconds(t_start, t_end);
    printf("  Result : %lld\n", serial_result);
    printf("  Time   : %.4f seconds\n\n", serial_time);

    /* ---- Parallel ---- */
    printf("Running parallel computation (%d threads)...\n", NUM_THREADS);
    clock_gettime(CLOCK_MONOTONIC, &t_start);
    parallel_result = parallel_computation();
    clock_gettime(CLOCK_MONOTONIC, &t_end);
    parallel_time = elapsed_seconds(t_start, t_end);
    printf("  Result : %lld\n", parallel_result);
    printf("  Time   : %.4f seconds\n\n", parallel_time);

    /* ---- Summary ---- */
    printf("======================================\n");
    printf("  Serial   time  : %.4f s\n", serial_time);
    printf("  Parallel time  : %.4f s\n", parallel_time);
    printf("  Speedup        : %.2fx\n",  serial_time / parallel_time);
    printf("  Results match  : %s\n",
           (serial_result == parallel_result) ? "YES" : "NO");
    printf("======================================\n");

    free(array);
    return 0;
}