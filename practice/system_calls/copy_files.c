#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define MAX 100

int main() {
    int fd1, fd2, fd3;
    fd1 = open("file1.txt", O_WRONLY | O_CREAT, 0644);
    char data[] = "To be or not to be\nThat is the question\n";
    int wrt = write(fd1, data, strlen(data));
    close(fd1);
    fd2 = open("file1.txt", O_RDONLY);
    char buffer[MAX];
    int rd = read(fd2, buffer, sizeof(buffer));
    buffer[rd] = '\0';
    close(fd2);
    fd3 = open("file2.txt", O_WRONLY | O_CREAT, 0644);
    int wrt1 = write(fd3, buffer, strlen(buffer));
    close(fd3);
    return 0;
}