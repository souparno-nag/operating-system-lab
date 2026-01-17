#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    printf("Parent PID: %d\n", getpid());

    for (int i = 0; i < 3; i++) {
        pid = fork();
        if (pid < 0) {
            perror("fork");
            return 1;
        }
        if (pid == 0) {
            // child process
            printf("Child %d | PID: %d | Parent PID: %d\n",
                i+1, getpid(), getppid());
            return 0; // child exists, but does not fork again
        }
    }
    // parent waits for all children
    for (int i = 0; i < 3; i++) {
        wait(NULL);
    }
    printf("Parent exiting\n");
    return 0;
}