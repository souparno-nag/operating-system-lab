#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid;

    printf("Parent Process ID: %d\n", getpid());
    printf("Creating child process...\n");

    pid = fork();

    if (pid < 0) {
        perror("Fork failed\n");
        exit(1);
    } else if (pid == 0) {
        printf("\nEntering child process\n");
        printf("Child Process ID: %d\n", getpid());
        printf("Parent Process ID: %d\n", getppid());

        printf("\nChild process sleeping for 4 seconds...\n");
        printf("Parent process is terminating in this time...\n");
        sleep(4);

        printf("\nAfter becoming orphan...\n");
        printf("Child Process ID: %d\n", getpid());
        printf("Parent Process ID: %d\n", getppid());

        printf("\nChild is now an ORPHAN process\n");
        printf("Child continues running for 5 more seconds...\n");
        sleep(5);        
        printf("Orphan child terminating...\n");
        exit(0);
    } else {
        printf("\nEntering parent state\n");
        printf("Parent Process ID: %d\n", getpid());
        
        printf("Parent sleeps for 2 seconds...\n");
        sleep(2);        
        printf("Parent terminating...\n");
        exit(0);
    }
}