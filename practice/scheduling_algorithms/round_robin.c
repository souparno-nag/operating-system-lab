#include <stdio.h>
#include <limits.h>

typedef struct
{
    int pid, arrival, burst, completion, turnaround, waiting, response;
    int remaining;
} Process;

void round_robin(Process proc[], int n, int tq) {
    int time = 0, done = 0;
    int queue[100], front = 0, rear = 0, in_queue[n];

    for (int i = 0; i < n; i++) {
        proc[i].remaining = proc[i].burst;
        in_queue[i] = 0;
    }
    // Add initial resources
    int idx = 0;
    for (int i = 0; i < n; i++) {
        if (proc[i].arrival <= time) {
            queue[rear++] = i;
            in_queue[i] = 1;
            idx = i+1;
        }
    }
    while (done < n) {
        if (front == rear) {
            time++;
            while (idx < n && proc[idx].arrival <= time) {
                queue[rear++] = idx;
                in_queue[idx++] = 1;
            }
            continue;
        }
        int i = queue[front++];
        in_queue[i] = 0;
        if (proc[i].remaining == proc[i].burst) proc[i].response = time - proc[i].arrival;
        int exec = (proc[i].remaining < tq) ? proc[i].remaining : tq;
        proc[i].remaining -= exec;
        time += exec;
        while (idx < n && proc[idx].arrival <= time) {
            queue[rear++] = idx;
            in_queue[idx++] = 1;
        }
        if (proc[i].remaining == 0) {
            proc[i].completion = time;
            proc[i].turnaround = proc[i].completion - proc[i].arrival;
            proc[i].waiting = proc[i].turnaround - proc[i].burst;
            done++;
        } else {
            queue[rear++] = i;
            in_queue[i] = 1;
        }
    }
    float avg_tat = 0, avg_wt = 0, avg_rt = 0;
    printf("PID\tAT\tBT\tCT\tTAT\tWT\tRT\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t\n",
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
    int n, tq;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    Process proc[n];

    printf("Enter AT and BT for each process\n");
    for (int i = 0; i < n; i++) {
        printf("Process P%d:\n", i+1);
        proc[i].pid = i+1;
        printf("Arrival time: ");
        scanf("%d", &proc[i].arrival);
        printf("Burst time: ");
        scanf("%d", &proc[i].burst);
    }

    printf("Enter time quantum: ");
    scanf("%d", &tq);

    round_robin(proc, n, tq);    
    return 0;
}