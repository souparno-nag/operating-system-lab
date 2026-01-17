#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        return 0;
    }
    if (pid == 0) {
        printf("This is inside the child process.\n");
        printf("Process ID of child process (using getpid): %d\n", getpid());
        printf("Process ID of parent process (using getppid): %d\n", getppid());
    } else {
        printf("This is inside the parent process.\n");
        printf("Process ID of parent process (using getpid): %d\n", getpid());
        printf("Process ID of process called using getppid within parent process: %d\n", getppid());
    }
}