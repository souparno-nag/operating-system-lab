#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid;

    printf("Parent Process ID: %d\n", getpid());
    printf("Creating child process...\n\n");

    pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } else if (pid == 0) {
        printf("Child process:\n");
        printf("Child PID: %d\n", getpid());
        printf("Parent PID: %d\n", getppid());

        printf("\nChild sleeping for 3 seconds...\n");
        printf("Parent will terminate during this time...\n\n");
        sleep(3);

        printf("Child Process (After becoming orphan):\n");
        printf("Child PID: %d\n", getpid());
        printf("New Parent PID: %d\n", getppid());

        printf("\nChild is now an ORPHAN process\n");
        printf("Child continues running for 5 more seconds...\n");
        sleep(5);        
        printf("Orphan child terminating...\n");
        exit(0);
    } else {
        printf("Parent Process:\n");
        printf("Parent PID: %d\n", getpid());
        printf("Created Child PID: %d\n\n", pid);

        printf("Parent sleeping for 2 seconds, then exiting...\n");
        sleep(2);        
        printf("Parent terminating NOW (child becomes orphan)...\n\n");
        exit(0);
    }
}