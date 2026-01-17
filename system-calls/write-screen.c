#include <stdio.h>
#include <unistd.h>

int main() {
    ssize_t count;
    count = write(1, "hello\n", 6);
    printf("Total bytes written: %ld\n", count);
    return 0;
}