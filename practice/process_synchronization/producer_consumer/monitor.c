#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER 5

int buffer[BUFFER];
int count = 0, in = 0, out = 0;
pthread_mutex_t mutex;
pthread_cond_t not_full, not_empty;

void* producer(void* args) {
    int item;
    for (item = 0; item < 10; item++) {
        pthread_mutex_lock(&mutex);
        while (count == BUFFER) pthread_cond_wait(&not_full, &mutex);

        buffer[in] = item;
        printf("Item produced: %d\n", item);
        in = (in+1)%BUFFER;
        count++;

        pthread_cond_signal(&not_empty);
        pthread_mutex_unlock(&mutex);
    }
}

void* consumer(void* args) {
    int i;
    for (int item = 0; item < 10; item++) {
        pthread_mutex_lock(&mutex);
        while (count == 0) pthread_cond_wait(&not_empty, &mutex);

        i = buffer[out];
        printf("Item consumed: %d\n", i);
        out = (out+1)%BUFFER;
        count--;

        pthread_cond_signal(&not_full);
        pthread_mutex_unlock(&mutex);
    }
}

int main() {
    pthread_t p, c;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&not_empty, NULL);
    pthread_cond_init(&not_full, NULL);

    pthread_create(&p, NULL, producer, NULL);
    pthread_create(&c, NULL, consumer, NULL);

    pthread_join(p, NULL);
    pthread_join(c, NULL);

    pthread_cond_destroy(&not_empty);
    pthread_cond_destroy(&not_full);
    pthread_mutex_destroy(&mutex);

    return 0;
}