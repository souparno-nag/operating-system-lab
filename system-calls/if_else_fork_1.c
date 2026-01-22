#include <stdio.h>
#include <unistd.h>

int main() {
    if (fork() || !fork()) {
        fprintf(stdout, "AA\n");
    } else if (!fork() && fork()) {
        fprintf(stdout, "BB\n");
    } else if (fork()) {
        fprintf(stdout, "CC\n");
    } else {
        fprintf(stdout, "DD\n");
    }
}