#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main() {
    int fd;
    struct flock lock;
    char *filename = "shared_file.txt";

    // Open file for reading only
    fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("Reader: Error opening file (Run writer first!)");
        exit(1);
    }

    printf("Reader (PID %d): Requesting READ lock...\n", getpid());

    // Setup a Shared Read Lock
    memset(&lock, 0, sizeof(lock));
    lock.l_type = F_RDLCK;    // Read lock
    lock.l_whence = SEEK_SET; 
    lock.l_start = 0;         
    lock.l_len = 0;           

    // Block until lock is acquired
    if (fcntl(fd, F_SETLKW, &lock) == -1) {
        perror("Reader: Error acquiring lock");
        close(fd);
        exit(1);
    }

    printf("Reader (PID %d): READ lock acquired! Reading file:\n", getpid());
    printf("-----------------------------------\n");

    // Read the file contents
    char buffer[100];
    int bytes_read;
    // Move to the beginning of the file just in case
    lseek(fd, 0, SEEK_SET); 
    while ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes_read] = '\0'; // Null-terminate the string
        printf("%s", buffer);
    }
    printf("-----------------------------------\n");

    // Hold the lock for 5 seconds to simulate reading time
    printf("Reader (PID %d): Holding lock for 5 seconds...\n", getpid());
    sleep(5);

    // Release the lock
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    
    printf("Reader (PID %d): READ lock released. Exiting.\n", getpid());

    close(fd);
    return 0;
}