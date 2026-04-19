#include <stdio.h>

int main() {
    // Input the number of processes and resources
    int n, r;
    printf("Enter the number of processes: ");
    scanf("%d", &n);
    printf("Enter the number of resources: ");
    scanf("%d", &r);

    // Input the allocation matrix
    int allocation[n][r];
    printf("Enter the allocation matrix\n");
    for(int i = 0; i < n; i++) {
        printf("Process %d: ", i);
        fflush(stdout);
        for (int j = 0; j < r; j++) {
            scanf("%d", &allocation[i][j]);
        }
    }

    // Input the max matrix
    int max[n][r];
    printf("Enter the max matrix\n");
    for(int i = 0; i < n; i++) {
        printf("Process %d: ", i);
        fflush(stdout);
        for (int j = 0; j < r; j++) {
            scanf("%d", &max[i][j]);
        }
    }

    // Input initial available resources
    int available[r];
    printf("Enter the initial available matrix\n");
    for (int i = 0; i < r; i++) {
        scanf("%d", &available[i]);
    }

    // Calculate the need matrix
    int need[n][r];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < r; j++) {
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }

    int choice;
    do {
        printf("========== MENU ==========\n");
        printf("Enter 1 to check safe sequence\n");
        printf("Enter 2 to process resource request\n");
        printf("Enter 3 to exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1){
            // Create temporary available array
            int temp_avail[r];
            for (int i = 0; i < r; i++) {
                temp_avail[i] = available[i];
            }
            // Initialize finish array
            int finish[n];
            for (int i = 0; i < n; i++) {
                finish[i] = 0;
            }
            // Initialize safe sequence
            int safe_seq[n], idx = 0;

            // calculate safe sequence
            for (int k = 0; k < n; k++) {
                for (int i = 0; i < n; i++) {
                    if (finish[i] == 0) {
                        int flag = 0;
                        for (int j = 0; j < r; j++) {
                            if (need[i][j] > temp_avail[j]) {
                                flag = 1;
                                break;
                            }
                        }
                        if (flag == 0) {
                            safe_seq[idx++] = i;
                            for (int j = 0; j < r; j++) {
                                temp_avail[j] += allocation[i][j];
                            }
                            finish[i] = 1;
                        }
                    }
                }
            }

            // print safe sequence if it exists
            if (idx == n) {
                printf("\nSystem is in a safe state.\n");
                printf("The safe sequence is:\n");
                printf("P%d ", safe_seq[0]);
                for (int i = 1; i < n; i++) {
                    printf(" -> P%d ", safe_seq[i]);
                }
                printf("\n");
            } else {
                printf("\nSystem is NOT in a safe state.\n");
            }
        } else if (choice == 2) {
            // Input process number
            int pid;
            printf("Enter process number: ");
            scanf("%d", &pid);
            // Input request
            int request[r];
            printf("Enter resource request for process:\n");
            for (int i = 0; i < r; i++) {
                scanf("%d", &request[i]);
            }
            // Check if request <= need
            int valid = 1;
            for (int i = 0; i < r; i++) {
                if (request[i] > need[pid][i]) {
                    printf("Request exceeds maximum need\n");
                    valid = 0;
                    break;
                }
            }
            if (!valid) continue;
            // Check if request <= available
            valid = 1;
            for (int i = 0; i < r; i++) {
                if (request[i] > available[i]) {
                    printf("Error: Resources not available. Process must wait.\n");
                    valid = 0;
                    break;
                }
            }
            if (!valid) {
                continue;
            }
            // Pretend to allocate resources
            // Create temporary copies
            int temp_alloc[n][r];
            int temp_need[n][r];
            int temp_avail[r];
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < r; j++) {
                    temp_alloc[i][j] = allocation[i][j];
                    temp_need[i][j] = need[i][j];
                }
            }
            for (int i = 0; i < r; i++) temp_avail[i] = available[i];
            // Pretend allocation
            for (int i = 0; i < r; i++) {
                temp_avail[i] -= request[i];
                temp_alloc[pid][i] += request[i];
                temp_need[pid][i] -= request[i];
            }
            // Check safety
            int finish[n], safe_seq[n], idx = 0;
            for (int i = 0; i < n; i++) finish[i] = 0;
            for (int k = 0; k < n; k++) {
                for (int i = 0; i < n; i++) {
                    if (!finish[i]) {
                        int flag = 0;
                        for (int j = 0; j < r; j++) {
                            if (temp_need[i][j] > temp_avail[j]) {
                                flag = 1;
                                break;
                            }
                        }
                        if (!flag) {
                            finish[i] = 1;
                            safe_seq[idx++] = i;
                            for (int j = 0; j < r; j++) {
                                temp_avail[j] += temp_alloc[i][j];
                            }
                        }
                    }
                }
            }
            if (idx == n) {
                printf("\nRequest can be granted safely\n");
                printf("Safe sequence after allocation:\n");
                printf("P%d ", safe_seq[0]);
                for (int i = 1; i < n; i++) {
                    printf(" -> P%d ", safe_seq[i]);
                }
                printf("\nResources allocated to process P%d\n", pid);
            } else {
                printf("\nResources cannot be granted - would lead to unsafe state\n");
            }
        } else if (choice == 3) {
            printf("Exiting...\n");
            break;
        }
    } while (choice != 3);
    return 0;
}