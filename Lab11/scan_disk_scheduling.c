#include <stdio.h>
#include <stdlib.h>

void sort(int arr[], int n) {
    int i, j, temp;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void SCAN(int req[], int n, int head, int disk_size) {
    int total_movement = 0;
    int current_head = head;
    int arr[100];
    for(int i = 0; i < n; i++) arr[i] = req[i];
    
    // Append maximum boundary and initial head, then sort
    arr[n] = disk_size - 1;
    arr[n+1] = head;
    sort(arr, n + 2);
    
    // Find initial head position in sorted array
    int pos;
    for (int i = 0; i < n + 2; i++) {
        if (arr[i] == head) { pos = i; break; }
    }
    
    printf("\n--- SCAN ---\n");
    printf("Path: %d", head);
    
    // Move Right (towards boundary)
    for (int i = pos + 1; i < n + 2; i++) {
        total_movement += abs(arr[i] - current_head);
        current_head = arr[i];
        printf(" -> %d", current_head);
    }
    
    // Move Left (reverse direction)
    for (int i = pos - 1; i >= 0; i--) {
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

    printf("Enter total disk size (e.g., 200 for 0-199): ");
    scanf("%d", &disk_size);

    SCAN(req, n, head, disk_size);

    return 0;
}
