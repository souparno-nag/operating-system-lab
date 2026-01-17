#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    pid_t p1, p2;
    p1 = fork();
    p2 = fork();
    printf("Hello World\n");
    return 0;
}