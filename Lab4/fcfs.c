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

void printGanttChart(Process proc[], int n) {
    int i, j;
    printf("\nGantt Chart:\n");
    
    // Top border
    printf(" ");
    for (i = 0; i < n; i++) {
        for (j = 0; j < proc[i].burst; j++) printf("--");
        printf(" ");
    }
    printf("\n|");
    
    // Process names
    for (i = 0; i < n; i++) {
        for (j = 0; j < proc[i].burst - 1; j++) printf(" ");
        printf("P%d", proc[i].pid);
        for (j = 0; j < proc[i].burst - 1; j++) printf(" ");
        printf("|");
    }
    
    // Bottom border
    printf("\n ");
    for (i = 0; i < n; i++) {
        for (j = 0; j < proc[i].burst; j++) printf("--");
        printf(" ");
    }
    printf("\n");
    
    // Timeline
    printf("0");
    for (i = 0; i < n; i++) {
        for (j = 0; j < proc[i].burst; j++) printf("  ");
        if (proc[i].completion < 10) printf("%d", proc[i].completion);
        else printf("%d", proc[i].completion);
    }
    printf("\n");
}

void printTable(Process proc[], int n) {
    float avg_tat = 0, avg_wt = 0, avg_rt = 0;
    
    printf("\n+-----+------------+------------+------------+----------------+--------------+---------------+\n");
    printf("| PID | Arrival    | Burst      | Completion | Turnaround     | Waiting      | Response      |\n");
    printf("|     | Time       | Time       | Time       | Time           | Time         | Time          |\n");
    printf("+-----+------------+------------+------------+----------------+--------------+---------------+\n");
    
    for (int i = 0; i < n; i++) {
        printf("| P%-2d | %-10d | %-10d | %-10d | %-14d | %-12d | %-13d |\n",
               proc[i].pid, proc[i].arrival, proc[i].burst,
               proc[i].completion, proc[i].turnaround,
               proc[i].waiting, proc[i].response);
        
        avg_tat += proc[i].turnaround;
        avg_wt += proc[i].waiting;
        avg_rt += proc[i].response;
    }
    
    printf("+-----+------------+------------+------------+----------------+--------------+---------------+\n");
    printf("\nAverage Turnaround Time: %.2f\n", avg_tat / n);
    printf("Average Waiting Time: %.2f\n", avg_wt / n);
    printf("Average Response Time: %.2f\n", avg_rt / n);
}

void fcfs(Process proc[], int n) {
    int current_time = 0;
    
    // Sort by arrival time
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (proc[j].arrival > proc[j + 1].arrival) {
                Process temp = proc[j];
                proc[j] = proc[j + 1];
                proc[j + 1] = temp;
            }
        }
    }
    
    printf("\nReady Queue (Execution Order): ");
    for (int i = 0; i < n; i++) {
        printf("P%d ", proc[i].pid);
    }
    printf("\n");
    
    for (int i = 0; i < n; i++) {
        if (current_time < proc[i].arrival) {
            current_time = proc[i].arrival;
        }
        
        proc[i].response = current_time - proc[i].arrival;
        current_time += proc[i].burst;
        proc[i].completion = current_time;
        proc[i].turnaround = proc[i].completion - proc[i].arrival;
        proc[i].waiting = proc[i].turnaround - proc[i].burst;
    }
}

int main() {
    int n;
    
    printf("Enter number of processes: ");
    scanf("%d", &n);
    
    Process *proc = (Process *)malloc(n * sizeof(Process));
    
    printf("\nEnter Arrival Time and Burst Time for each process:\n");
    for (int i = 0; i < n; i++) {
        proc[i].pid = i + 1;
        printf("Process P%d:\n", i + 1);
        printf("  Arrival Time: ");
        scanf("%d", &proc[i].arrival);
        printf("  Burst Time: ");
        scanf("%d", &proc[i].burst);
    }
    
    fcfs(proc, n);
    printGanttChart(proc, n);
    printTable(proc, n);
    
    free(proc);
    return 0;
}