#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    pid_t p;
    printf("Before fork\n");
    p = fork();
    if (p < 0) {
        perror("fork failed");
        return 0;
    }
    if (p == 0) {
        printf("I am child having id %d\n", getpid());
        printf("My parent's id is %d\n", getppid());
    } else {
        printf("My child's id is %d\n", p);
        printf("I am parent having id %d\n", getpid());
    }
    printf("Common");
}