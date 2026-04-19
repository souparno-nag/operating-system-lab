#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t wrt, rdr;
int readCount = 0;

void* writer(void* arg) {
    int id = *(int*)arg;
    sem_wait(&wrt);

    printf("Writer %d is writing\n", id);
    sleep(3);

    sem_post(&wrt);
    sleep(3);

    return NULL;
}

void* reader(void* arg) {
    int id = *(int*)arg;

    sem_wait(&rdr);
    readCount++;
    if (readCount == 1) sem_wait(&wrt);
    sem_post(&rdr);

    printf("Reader %d is reading\n", id);
    sleep(3);

    sem_wait(&rdr);
    readCount--;
    if (readCount == 0) sem_post(&wrt);
    sem_post(&rdr);

    return NULL;
}

int main() {
    pthread_t wr_th[2], rd_th[5];
    int wr_id[2], rd_id[5];

    sem_init(&wrt, 0, 1);
    sem_init(&rdr, 0, 1);

    for (int i = 0; i < 5; i++) {
        rd_id[i] = i;
        pthread_create(&rd_th[i], NULL, reader, &rd_id[i]);
    }
    for (int i = 0; i < 2; i++) {
        wr_id[i] = i;
        pthread_create(&wr_th[i], NULL, writer, &wr_id[i]);
    }

    for (int i = 0; i < 5; i++) {
        pthread_join(rd_th[i], NULL);
    }
    for (int i = 0; i < 2; i++) {
        pthread_join(wr_th[i], NULL);
    }

    sem_destroy(&wrt);
    sem_destroy(&rdr);
}