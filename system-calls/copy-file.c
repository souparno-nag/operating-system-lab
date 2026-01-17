#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
    char buff[64];
    ssize_t fd = open("foo.txt", O_WRONLY | O_CREAT);
    ssize_t c = write(fd, "hello\nworld", 11);
    close(fd);
    fd = open("foo.txt", O_RDONLY);
    ssize_t d = read(fd, buff, c);
    ssize_t fd1 = open("foo1.txt", O_WRONLY | O_CREAT);
    write(fd1, buff, c);
    close(fd1);
    return 0;
}