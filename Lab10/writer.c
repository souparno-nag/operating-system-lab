#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main() {
    int fd;
    struct flock lock;
    char *filename = "shared_file.txt";

    // Open file for writing (create if it doesn't exist)
    fd = open(filename, O_WRONLY | O_CREAT, 0666);
    if (fd == -1) {
        perror("Writer: Error opening file");
        exit(1);
    }

    printf("Writer (PID %d): Requesting WRITE lock...\n", getpid());

    // Setup an Exclusive Write Lock
    memset(&lock, 0, sizeof(lock));
    lock.l_type = F_WRLCK;    // Write lock
    lock.l_whence = SEEK_SET; 
    lock.l_start = 0;         
    lock.l_len = 0;           

    // Block until lock is acquired
    if (fcntl(fd, F_SETLKW, &lock) == -1) {
        perror("Writer: Error acquiring lock");
        close(fd);
        exit(1);
    }

    printf("Writer (PID %d): WRITE lock acquired! Writing to file...\n", getpid());

    // Write to the file
    char buffer[100];
    sprintf(buffer, "Hello from Writer PID %d!\n", getpid());
    write(fd, buffer, strlen(buffer));

    // Hold the lock for 10 seconds to simulate a long write process
    printf("Writer (PID %d): Holding lock for 10 seconds...\n", getpid());
    sleep(10);

    // Release the lock
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    
    printf("Writer (PID %d): WRITE lock released. Exiting.\n", getpid());

    close(fd);
    return 0;
}