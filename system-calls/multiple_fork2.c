#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    pid_t p1, p2; int i = 0;
    p1 = fork();
    p2 = fork();
    printf("%d ", (i + 1));
    return 0;
}