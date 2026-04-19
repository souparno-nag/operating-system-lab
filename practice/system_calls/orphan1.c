#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

int main() {
    pid_t pid;
    pid = fork();

    if (pid < 0) {
        perror("Fork failed\n");
        exit(1);
    } else if (pid == 0) {
        printf("Child process id: %d; Parent process id: %d; (Before parent dies)\n", getpid(),getppid());
        sleep(10);
        printf("Child process id: %d; Parent process id: %d; (After parent dies)\n", getpid(),getppid());
    } else {
        printf("Original parent id: %d\n\n", getpid());
    }

    return 0;
}