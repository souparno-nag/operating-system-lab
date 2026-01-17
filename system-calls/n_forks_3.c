#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t p2, p3, p4, p5;
    printf("P1 (Parent) PID: %d\n", getpid());

    // create P2
    p2 = fork();
    if (p2 == 0) {
        // P2 process
        printf("P2 PID: %d | Parent PID: %d\n", getpid(), getppid());
        return 0;
    }

    // Only P1 reaches here
    // create P3
    p3 = fork();
    if (p3 == 0) {
        // P3 process
        printf("P3 PID: %d | Parent PID: %d\n", getpid(), getppid());

        // P3 creates P4
        p4 = fork();
        if (p4 == 0) {
            printf("P4 PID: %d | Parent PID: %d\n", getpid(), getppid());
            return 0;
        }

        // P3 creates P5
        p5 = fork();
        if (p5 == 0) {
            printf("P5 PID: %d | Parent PID: %d\n", getpid(), getppid());
            return 0;
        }

        // P3 waits for P4 and P5
        sleep(10);
        wait(NULL);
        wait(NULL);
        return 0;
    }

    // P1 waits for P2 and P3
    wait(NULL);
    wait(NULL);
    return 0;
}

// run after execution: pstree -p <P1_PID>