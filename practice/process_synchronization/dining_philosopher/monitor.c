#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define N 5

pthread_mutex_t forks[N];

void* philosopher(void* arg) {
	int id = *(int*)arg;
	int left = id;
	int right = (id+1)%N;

	while (1) {
		printf("Philosopher %d is thinking\n", id);
		sleep(3);

		if (id == N-1) {
			pthread_mutex_lock(&forks[right]);
			pthread_mutex_lock(&forks[left]);
		} else {
			pthread_mutex_lock(&forks[left]);
			pthread_mutex_lock(&forks[right]);
		}

		printf("Philosopher %d is eating\n", id);
                sleep(2);

		printf("Philosopher %d finished eating\n", id);
		pthread_mutex_unlock(&forks[left]);
		pthread_mutex_unlock(&forks[right]);
	}
	return NULL;
}

int main() {
	pthread_t tid[N];
	int ids[N];

	for (int i = 0; i < N; i++) pthread_mutex_init(&forks[i], NULL);

	for (int i = 0; i < N; i++) {
		ids[i] = i;
		pthread_create(&tid[i], NULL, philosopher, &ids[i]);
	}

	for (int i = 0; i < N; i++) {
		pthread_join(tid[i], NULL);
	}

	return 0;
}
