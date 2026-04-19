#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    printf("Parent Process ID: %d\n", getpid());
    printf("Creating child process...\n\n");
    pid = fork();

    if (pid < 1) {
        perror("Fork failed\n");
        exit(1);
    } else if (pid == 0) {
        printf("\nChild Process ID: %d\n", getpid());
        printf("Parent Process ID: %d\n", getppid());
        printf("Terminating child process\n\n");
        exit(0);
    } else {
        printf("Parent Process:\n");
        printf("Parent PID: %d\n", getpid());
        printf("Created Child PID: %d\n\n", pid);
        printf("Parent sleeping for 10 seconds... Child becomes zombie (terminated but not reaped)\n");
        sleep(10);
        printf("Parent will now reap the zombie child...\n");
        wait(NULL);
        printf("Zombie child reaped successfully. Parent process exiting...\n");
    }
}