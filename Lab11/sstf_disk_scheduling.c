#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

void SSTF(int req[], int n, int head) {
    int total_movement = 0;
    int current_head = head;
    int visited[100] = {0}; // Keeps track of serviced requests
    int count = 0;
    
    printf("\n--- SSTF ---\n");
    printf("Path: %d", head);
    
    while (count < n) {
        int min_dist = INT_MAX;
        int index = -1;
        
        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                int dist = abs(req[i] - current_head);
                if (dist < min_dist) {
                    min_dist = dist;
                    index = i;
                }
            }
        }
        
        visited[index] = 1;
        total_movement += min_dist;
        current_head = req[index];
        printf(" -> %d", current_head);
        count++;
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

    SSTF(req, n, head);

    return 0;
}
