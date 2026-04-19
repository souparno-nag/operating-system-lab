#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER 5

int buffer[BUFFER];
int in = 0, out = 0;
sem_t empty, full, mutex;

void* producer(void* arg) {
    for (int i = 0; i < 10; i++) {
        sem_wait(&empty);
        sem_wait(&mutex);

        buffer[in] = i;
        printf("Producer produced: %d\n", i);
        in = (in+1)%BUFFER;

        sem_post(&full);
        sem_post(&mutex);
        sleep(1);
    }
    return NULL;
}

void* consumer(void* arg) {
    int i;
    for (int j = 0; j < 10; j++) {
        sem_wait(&full);
        sem_wait(&mutex);

        i = buffer[out];
        printf("Consumer consumed: %d\n", i);
        out = (out+1)%BUFFER;

        sem_post(&empty);
        sem_post(&mutex);
        sleep(1);
    }
    return NULL;
}

int main() {
    pthread_t p, c;

    sem_init(&empty, 0, BUFFER);
    sem_init(&full, 0, 0);
    sem_init(&mutex, 0, 1);

    pthread_create(&p, NULL, producer, NULL);
    sleep(5);
    pthread_create(&c, NULL, consumer, NULL);

    pthread_join(p, NULL);
    pthread_join(c, NULL);

    sem_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}