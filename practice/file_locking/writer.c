#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main() {
    int fd;
    struct flock lock;

    fd = open("file.txt", O_WRONLY | O_CREAT, 0644);

    if (fd < 0) {
        perror("File cannot be created\n");
        exit(1);
    }

    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;

    printf("Waiting for lock...\n");
    fcntl(fd, F_SETLKW, &lock);
    printf("Exclusive lock has been set\n");

    char* data = "Hello, I am writing to a file\n";
    write(fd, data, strlen(data));

    sleep(30);

    printf("File has been written. Releasing the lock.\n");
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLKW, &lock);

    close(fd);

    return 0;
}
