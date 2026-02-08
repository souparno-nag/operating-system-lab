#include <stdio.h>

int main() {
    int n, r;
    printf("Enter the number of processes: ");
    scanf("%d", &n);
    printf("Enter the number of resources: ");
    scanf("%d", &r);
    int alloc[n][r];
    printf("Enter the allocation matrix:\n");
    for (int i = 0; i < n; i++) {
        printf("Process %d: ", i);
        fflush(stdout);
        for (int j = 0; j < r; j++){
            scanf("%d", &alloc[i][j]);
        }
    }
    int max[n][r];
    printf("Enter the MAX matrix:\n");
    for (int i = 0; i < n; i++) {
        printf("Process %d: ", i);
        fflush(stdout);
        for (int j = 0; j < r; j++){
            scanf("%d", &max[i][j]);
        }
    }
    int avail[r];
    printf("Enter the initial available resources:\n");
    for (int i = 0; i < r; i++) {
        scanf("%d", &avail[i]);
    }
    int need[n][r];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < r; j++){
            need[i][j] = max[i][j] - alloc[i][j];
        }
    }
    
    int choice;
    do {
        printf("\n========== MENU ==========\n");
        printf("1. Check Safe State\n");
        printf("2. Process Resource Request\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        if (choice == 1) {
            // Original safe state checking code
            int finish[n], safeseq[n], idx = 0;
            int temp_avail[r];
            // Make a copy of available resources
            for (int i = 0; i < r; i++) {
                temp_avail[i] = avail[i];
            }
            for (int i = 0; i < n; i++) {
                finish[i] = 0;
            }
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
                            safeseq[idx++] = i;
                            for (int y = 0; y < r; y++) {
                                temp_avail[y] += alloc[i][y];
                            }
                            finish[i] = 1;
                        }
                    }
                }
            }
            // Check if system is in safe state
            if (idx == n) {
                printf("System is in safe state.\n");
                printf("The safe sequence is: ");
                for (int i = 0; i < n-1; i++) {
                    printf("P%d -> ", safeseq[i]);
                }
                printf("P%d\n", safeseq[n-1]);
            } else {
                printf("System is NOT in a safe state!\n");
            }
        }
        else if (choice == 2) {
            // New: Handle resource request
            int pid;
            printf("Enter process number (0 to %d): ", n-1);
            scanf("%d", &pid);
            
            int request[r];
            printf("Enter request for process P%d: ", pid);
            for (int i = 0; i < r; i++) {
                scanf("%d", &request[i]);
            }
            
            // Step 1: Check if request <= need
            int valid = 1;
            for (int i = 0; i < r; i++) {
                if (request[i] > need[pid][i]) {
                    printf("Error: Request exceeds maximum need.\n");
                    valid = 0;
                    break;
                }
            }
            
            if (!valid) continue;
            
            // Step 2: Check if request <= available
            for (int i = 0; i < r; i++) {
                if (request[i] > avail[i]) {
                    printf("Error: Resources not available. Process must wait.\n");
                    valid = 0;
                    break;
                }
            }
            
            if (!valid) continue;
            
            // Step 3: Pretend to allocate resources
            // Create temporary copies
            int temp_alloc[n][r];
            int temp_need[n][r];
            int temp_avail[r];
            
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < r; j++) {
                    temp_alloc[i][j] = alloc[i][j];
                    temp_need[i][j] = need[i][j];
                }
            }
            for (int i = 0; i < r; i++) {
                temp_avail[i] = avail[i];
            }
            
            // Pretend allocation
            for (int i = 0; i < r; i++) {
                temp_avail[i] -= request[i];
                temp_alloc[pid][i] += request[i];
                temp_need[pid][i] -= request[i];
            }
            
            // Step 4: Check safety with pretend allocation
            int finish[n], safeseq[n], idx = 0;
            for (int i = 0; i < n; i++) {
                finish[i] = 0;
            }
            
            for (int k = 0; k < n; k++) {
                for (int i = 0; i < n; i++) {
                    if (finish[i] == 0) {
                        int flag = 0;
                        for (int j = 0; j < r; j++) {
                            if (temp_need[i][j] > temp_avail[j]) {
                                flag = 1;
                                break;
                            }
                        }
                        if (flag == 0) {
                            safeseq[idx++] = i;
                            for (int y = 0; y < r; y++) {
                                temp_avail[y] += temp_alloc[i][y];
                            }
                            finish[i] = 1;
                        }
                    }
                }
            }
            
            // Step 5: Decision
            if (idx == n) {
                printf("Request can be granted safely.\n");
                printf("Safe sequence after allocation: ");
                for (int i = 0; i < n-1; i++) {
                    printf("P%d -> ", safeseq[i]);
                }
                printf("P%d\n", safeseq[n-1]);
                
                // Actually allocate resources
                for (int i = 0; i < r; i++) {
                    avail[i] -= request[i];
                    alloc[pid][i] += request[i];
                    need[pid][i] -= request[i];
                }
                printf("Resources allocated to process P%d\n", pid);
            } else {
                printf("Request cannot be granted - would lead to unsafe state.\n");
            }
        }
        else if (choice == 3) {
            printf("Exiting...\n");
            break;
        }
        else {
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 3);
    
    return 0;
}