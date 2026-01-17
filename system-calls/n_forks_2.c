#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {
    pid_t pid;

    for (int i = 0; i < 5; i++) {
        // loop will run n times (n=5)
        pid = fork();
        if (pid < 0) {
            perror("fork");
            return 1;
        }
        if (pid == 0) {
            printf("[son] pid %d from [parent] pid %d\n", getpid(), getppid());
            exit(0);
        }
    }
    // loop will run n times (n=5)
    for (int i = 0; i < 5; i++)
        wait(NULL);
    return 0;
}