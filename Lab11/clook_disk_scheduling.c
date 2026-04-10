#include <stdio.h>
#include <stdlib.h>

void sort(int arr[], int n) {
    int temp;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void CLOOK(int req[], int n, int head) {
    int total_movement = 0;
    int current_head = head;
    int arr[100];
    for(int i = 0; i < n; i++) arr[i] = req[i];
    
    // Append initial head only, then sort
    arr[n] = head;
    sort(arr, n + 1);
    
    // Find initial head position
    int pos;
    for (int i = 0; i < n + 1; i++) {
        if (arr[i] == head) { pos = i; break; }
    }
    
    printf("\n--- C-LOOK ---\n");
    printf("Path: %d", head);
    
    // Move Right (up to highest request)
    for (int i = pos + 1; i < n + 1; i++) {
        total_movement += abs(arr[i] - current_head);
        current_head = arr[i];
        printf(" -> %d", current_head);
    }
    
    // Jump directly to the lowest request
    for (int i = 0; i < pos; i++) {
        total_movement += abs(arr[i] - current_head);
        current_head = arr[i];
        printf(" -> %d", current_head);
    }
    printf("\nTotal Head Movement: %d\n", total_movement);
}

int main() {
    int n, head, disk_size, choice;
    int req[100];

    printf("Enter the number of requests: ");
    scanf("%d", &n);

    printf("Enter the request sequence: ");
    for (int i = 0; i < n; i++) {
        scanf("%d", &req[i]);
    }

    printf("Enter initial head position: ");
    scanf("%d", &head);

    CLOOK(req, n, head);

    return 0;
}