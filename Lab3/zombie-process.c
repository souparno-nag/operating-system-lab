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

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } else if(pid == 0) {
        printf("Child process:\n");
        printf("Child PID: %d\n", getpid());
        printf("Parent PID: %d\n", getppid());
        printf("Child will terminate now\n\n");
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
