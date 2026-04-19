#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main() {
    int fd;
    struct flock lock;

    fd = open("file.txt", O_RDONLY);

    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;

    printf("Establishing shared lock...\n");
    fcntl(fd, F_SETLKW, &lock);

    char buffer[100];
    int n = read(fd, buffer, sizeof(buffer)-1);
    buffer[n] = '\0';

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLKW, &lock);

    printf("Contents of the file are:\n");
    printf("%s\n", buffer);

    close(fd);
    return 0;
}
