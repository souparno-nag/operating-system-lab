#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define MAX 100

void SSTF(int req[], int head, int n) {
    int total = 0, curr = head, count = 0;
    int visited[n];
    for (int i = 0; i < n; i++) visited[i] = 0;
    printf("%d", head);
    while (count < n) {
        int idx = -1, min_dist = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (!visited[i] && abs(curr - req[i]) < min_dist) {
                idx = i;
                min_dist = abs(curr - req[i]);
            }
        }
        visited[idx] = 1;
        total += min_dist;
        curr = req[idx];
        printf(" -> %d", curr);
        count++;
    }
    printf("\nTotal head movement: %d\n", total);
}

int main() {
    int n, head, req[MAX];
    scanf("%d", &n);
    for (int i = 0; i < n; i++) scanf("%d", &req[i]);
    scanf("%d", &head);
    SSTF(req, head, n);
    return 0;
}