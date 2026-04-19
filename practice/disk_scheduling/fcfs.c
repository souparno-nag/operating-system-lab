#include <stdio.h>
#include <stdlib.h>
#define MAX 100

void FCFS(int req[], int head, int n) {
    int total = 0, curr = head;
    printf("%d", head);
    for (int i = 0; i < n; i++) {
        total += abs(curr - req[i]);
        curr = req[i];
        printf(" -> %d", curr);
    }
    printf("\nTotal head movement: %d\n", total);
}

int main() {
    int n, head, req[MAX];
    scanf("%d", &n);
    for (int i = 0; i < n; i++) scanf("%d", &req[i]);
    scanf("%d", &head);
    FCFS(req, head, n);
    return 0;
}