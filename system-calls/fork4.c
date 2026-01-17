#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    pid_t p; int a = 5;
    p = fork();
    if (p == -1) {
        printf("There is an error while calling fork()\n");
        return 0;
    }
    if (p == 0) // child process
    {
        a += 5;
    } else // parent process
    {
        a += 2;
    }
    printf("%d\n", a);
    return 0;
}