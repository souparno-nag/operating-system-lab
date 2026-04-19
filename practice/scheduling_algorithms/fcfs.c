#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int pid;
    int arrival;
    int burst;
    int completion;
    int turnaround;
    int waiting;
    int response;
} Process;

void fcfs(Process proc[], int n) {
    // Sort by arrival time;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (proc[i].arrival > proc[j].arrival) {
                Process temp = proc[i];
                proc[i] = proc[j];
                proc[j] = temp;
            }
        }
    }
    // Print Ready Queue
    printf("Ready Queue:\n");
    for (int i = 0; i < n-1; i++) {
        printf("P%d, ", proc[i].pid);
    }
    printf("P%d\n", proc[n-1].pid);
    // Calculate CT, TAT, WT and RT for each process
    int current_time = 0;
    for (int i = 0; i < n; i++) {
        // Handle idle CPU
        if (current_time < proc[i].arrival) {
            current_time = proc[i].arrival;
        }
        proc[i].response = current_time - proc[i].arrival;
        current_time += proc[i].burst;
        proc[i].completion = current_time;
        proc[i].turnaround = proc[i].completion - proc[i].arrival;
        proc[i].waiting = proc[i].turnaround - proc[i].burst;
    }
    return;
}

void printGanttChart (Process proc[], int n) {
    printf("\nGantt Chart:\n");
    // Top Border
    printf(" ");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < proc[i].burst; j++) printf("--");
        printf("|");
    }
    printf("\n|");
    // Process Names
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < proc[i].burst-1; j++) printf(" ");
        printf("P%d", proc[i].pid);
        for (int j = 0; j < proc[i].burst-1; j++) printf(" ");
        printf("|");
    }
    // Bottom Border
    printf("\n ");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < proc[i].burst; j++) printf("--");
        printf("|");
    }
    printf("\n");
    // Timeline
    printf("0");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < proc[i].burst; j++) printf("  ");
        printf("%d", proc[i].completion);
    }
    printf("\n");
}

void printTable (Process proc[], int n) {
    float avg_tat = 0, avg_wt = 0, avg_rt = 0;
    printf("| PID | Arrival    | Burst      | Completion | Turnaround     | Waiting      | Response      |\n");
    for (int i = 0; i < n; i++) {
        printf("| P%-2d | %-10d | %-10d | %-10d | %-14d | %-12d | %-13d |\n",
               proc[i].pid, proc[i].arrival, proc[i].burst,
               proc[i].completion, proc[i].turnaround,
               proc[i].waiting, proc[i].response);
        
        avg_tat += proc[i].turnaround;
        avg_wt += proc[i].waiting;
        avg_rt += proc[i].response;
    }
    printf("\nAverage Turnaround Time: %.2f\n", avg_tat / n);
    printf("Average Waiting Time: %.2f\n", avg_wt / n);
    printf("Average Response Time: %.2f\n", avg_rt / n);
    return;
}

int main() {
    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);
    Process *proc = (Process *)malloc(n * sizeof(Process));
    printf("Enter arrival and burst times for each process:\n");
    for (int i = 0; i < n; i++) {
        printf("Process P%d:\n", i+1);
        proc[i].pid = i+1;
        printf("Arrival Time: ");
        scanf("%d", &proc[i].arrival);
        printf("Burst Time: ");
        scanf("%d", &proc[i].burst);
    }
    fcfs(proc, n);
    printGanttChart(proc, n);
    printTable(proc, n);
    free(proc);
    return 0;
}