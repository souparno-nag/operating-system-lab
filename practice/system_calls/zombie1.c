#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    pid_t pid;
    pid = fork();

    if (pid < 0) {
        perror("Fork error\n");
        exit(0);
    } else if (pid == 0) {
        exit(0);
    } else {
        sleep(10);
        wait(NULL);
    }

    return 0;
}