#include <stdio.h>
#include <unistd.h>

int main() {
    ssize_t count_read, count_write;
    char buff[20];
    count_read = read(0, buff, 10);
    count_write = write(1, buff, 10);
    return 0;
}