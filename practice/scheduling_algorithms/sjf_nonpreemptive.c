#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

typedef struct {
    int pid;
    int arrival;
    int burst;
    int completion;
    int turnaround;
    int waiting;
    int response;
    bool completed;
} Process;

void sjf(Process proc[], int n) {
    int time = 0, done = 0;
    int gantt[100], gantt_time[101], gantt_idx = 0;
    gantt_time[0] = 0;

    while (done < n) {
        int shortest = -1, min_burst = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (!proc[i].completed && proc[i].arrival <= time && proc[i].burst <= min_burst) {
                shortest = i;
                min_burst = proc[i].burst;
            }
        }
        if (shortest == -1) {
            int next = INT_MAX;
            for (int i = 0; i < n; i++) {
                if (!proc[i].completed && proc[i].arrival < next) {
                    next = time;
                }
            }
            time = next;
            continue;
        }
        proc[shortest].response = time - proc[shortest].arrival;
        time += proc[shortest].burst;
        proc[shortest].completion = time;
        proc[shortest].turnaround = proc[shortest].completion - proc[shortest].arrival;
        proc[shortest].waiting = proc[shortest].turnaround - proc[shortest].burst;
        proc[shortest].completed = true;
        done++;
    }
    float avg_tat = 0, avg_wt = 0, avg_rt = 0;
    printf("Pid\tAT\tBT\tCT\tTAT\tWT\tRT\n");
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
    printf("Enter number of processes: ");
    scanf("%d", &n);

    Process *proc = (Process *)malloc(n * sizeof(Process));

    printf("Enter AT and BT for each process\n");
    for (int i = 0; i < n; i++) {
        printf("Process P%d:\n", i+1);
        proc[i].pid = i+1;
        proc[i].completed = 0;
        printf("Arrival time: ");
        scanf("%d", &proc[i].arrival);
        printf("Burst time: ");
        scanf("%d", &proc[i].burst);
    }
    sjf(proc, n);
    free(proc);
    return 0;
}