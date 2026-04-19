#include <stdio.h>
#include <stdbool.h>

int main() {
    int n, r;
    scanf("%d %d", &n, &r);

    int allocation[n][r], max[n][r], available[r], need[n][r];

    for (int i = 0; i < n; i++) for (int j = 0; j < r; j++) scanf("%d", &allocation[i][j]);
    for (int i = 0; i < n; i++) for (int j = 0; j < r; j++) scanf("%d", &max[i][j]);
    for (int i = 0; i < r; i++) scanf("%d", &available[i]);

    for (int i = 0; i < n; i++) for (int j = 0; j < r; j++) need[i][j] = max[i][j] - allocation[i][j];

    int choice;
    do
    {
        scanf("%d", &choice);

        if (choice == 1) {
            int temp_available[r], finish[n], safe_seq[n], idx = 0;
            for (int i = 0; i < n; i++) finish[i] = 0;
            for (int i = 0; i < r; i++) temp_available[i] = available[i];

            for (int k = 0; k < n; k++) {
                for (int i = 0; i < n; i++) if (!finish[i]) {
                    bool flag = true;
                    for (int j = 0; j < r; j++) if (need[i][j] > temp_available[j]) {flag = false; break;}
                    if (flag) {
                        for (int j = 0; j < r; j++) temp_available[j] += allocation[i][j];
                        finish[i] = 1;
                        safe_seq[idx++] = i;
                    }
                }
            }
            if (idx == n) {
                printf("Safe sequence: ");
                for (int i = 0; i < n; i++) printf("P%d, ", safe_seq[i]);
                printf("\n");
            } else {
                printf("No safe sequence exists\n");
            }
        } else if (choice == 2) {
            int pid, request[r];
            scanf("%d", &pid);
            for (int i = 0; i < r; i++) scanf("%d", &request[i]);

            bool valid = true;
            for (int i = 0; i < r; i++) if (request[i] > available[i]) {valid = false; break;}
            for (int i = 0; i < r; i++) if (request[i] > need[pid][i]) {valid = false; break;}

            if (!valid) continue;

            int ta[n][r], tn[n][r], tv[r];
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < r; j++) {
                    ta[i][j] = allocation[i][j];
                    tn[i][j] = need[i][j];
                }
            }
            for (int i = 0; i < r; i++) tv[i] = available[i];

            for (int i = 0; i < r; i++) {
                tv[i] -= request[i];
                ta[pid][i] += request[i];
                tn[pid][i] -= request[i];
            }

            int finish[n], safe_seq[n], idx = 0;
            for (int i = 0; i < n; i++) finish[i] = 0;
            for (int k = 0; k < n; k++) {
                for (int i = 0; i < n; i++) if (!finish[i]) {
                    bool flag = true;
                    for (int j = 0; j < r; j++) if (tn[i][j] > tv[j]) {flag = false; break;}
                    if (flag) {
                        finish[i] = 1;
                        safe_seq[idx++] = i;
                        for (int j = 0; j < r; j++) tv[j] += ta[i][j];
                    }
                }
            }
            if (idx == n) printf("Safe sequence exists for the given request\n");
        }
    } while (choice != 3);
    
    return 0;
}