#include <stdio.h>
#include <limits.h>
#include <stdbool.h>

typedef struct {
    int pid, arrival, burst, completion, turnaround, waiting, response;
    int priority;
    bool completed;
} Process;

void priority(Process proc[], int n) {
    int time = 0, done = 0;

    while (done < n) {
        int idx = -1, min_priority = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (!proc[i].completed && proc[i].arrival <= time && proc[i].priority < min_priority) {
                idx = i;
                min_priority = proc[i].priority;
            }
        }
        if (idx == -1) {
            time++; continue;
        }
        proc[idx].response = time - proc[idx].arrival;
        time += proc[idx].burst;
        proc[idx].completion = time;
        proc[idx].turnaround = proc[idx].completion - proc[idx].arrival;
        proc[idx].waiting = proc[idx].turnaround - proc[idx].burst;
        proc[idx].completed = true; done++;
    }

    float avg_tat = 0, avg_wt = 0, avg_rt = 0;
    printf("PID\tAT\tBT\tCT\tTAT\tWT\tRT\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
        proc[i].pid, proc[i].arrival, proc[i].burst, proc[i].completion, proc[i].turnaround, proc[i].waiting, proc[i].response);
        avg_tat += proc[i].turnaround;
        avg_wt += proc[i].waiting;
        avg_rt += proc[i].response;
    }
    avg_tat /= n; avg_wt /= n; avg_rt /= n;
    printf("The average turnaround time is %.2f\n", avg_tat);
    printf("The average waiting time is %.2f\n", avg_wt);
    printf("The average response time is %.2f\n", avg_rt);
}

int main() {
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    Process proc[n];

    for (int i = 0; i < n; i++) {
        printf("Process P%d\n", i+1);
        printf("Arrival time: ");
        scanf("%d", &proc[i].arrival);
        printf("Burst time: ");
        scanf("%d", &proc[i].burst);
        printf("Priority: ");
        scanf("%d", &proc[i].priority);
    }

    priority(proc, n);
    return 0;
}