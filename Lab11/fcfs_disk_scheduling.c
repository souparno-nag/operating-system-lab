#include <stdio.h>
#include <stdlib.h>

void FCFS(int req[], int n, int head) {
    int total_movement = 0;
    int current_head = head;
    
    printf("\n--- FCFS ---\n");
    printf("Path: %d", head);
    
    for (int i = 0; i < n; i++) {
        total_movement += abs(req[i] - current_head);
        current_head = req[i];
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

    FCFS(req, n, head);

    return 0;
}
