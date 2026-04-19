#include <stdio.h>
#include <stdlib.h>
#define MAX 100

void sort(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (arr[j] > arr[j+1]) {int t = arr[j]; arr[j] = arr[j+1]; arr[j+1] = t;}
        }
    }
}

void CLOOK(int req[], int head, int n) {
    int total = 0, curr = head, arr[MAX];
    for (int i = 0; i < n; i++) arr[i] = req[i];
    arr[n] = head;
    sort(arr, n+1);
    int pos;
    for (pos = 0; pos < n+1; pos++) if (arr[pos] == head) break;
    printf("%d", head);
    for (int i = pos+1; i < n+1; i++) {
        total += abs(curr - arr[i]);
        curr = arr[i];
        printf(" -> %d", curr);
    }
    for (int i = 0; i < pos; i++) {
        total += abs(curr - arr[i]);
        curr = arr[i];
        printf(" -> %d", curr);
    }
    printf("\nTotal head movement: %d\n", total);
}

int main() {
    int n, head, req[MAX];
    scanf("%d", &n);
    for (int i = 0; i < n; i++) scanf("%d", &req[i]);
    scanf("%d", &head);
    CLOOK(req, head, n);
    return 0;
}