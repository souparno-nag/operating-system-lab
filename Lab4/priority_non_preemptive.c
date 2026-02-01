#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

typedef struct {
    int pid;
    int arrival;
    int burst;
    int priority;
    int completion;
    int turnaround;
    int waiting;
    int response;
    bool completed;
} Process;

void printGanttChart(int gantt[], int gantt_time[], int gantt_size) {
    printf("\nGantt Chart:\n");
    
    // Print top border
    printf(" ");
    for (int i = 0; i < gantt_size; i++) {
        int duration = gantt_time[i + 1] - gantt_time[i];
        for (int j = 0; j < duration * 2; j++) {
            printf("-");
        }
        printf(" ");
    }
    printf("\n");
    
    // Print process IDs
    printf("|");
    for (int i = 0; i < gantt_size; i++) {
        int duration = gantt_time[i + 1] - gantt_time[i];
        int spaces = duration * 2 - 1;
        int left_space = spaces / 2;
        int right_space = spaces - left_space;
        
        for (int j = 0; j < left_space; j++) printf(" ");
        printf("P%d", gantt[i]);
        for (int j = 0; j < right_space; j++) printf(" ");
        printf("|");
    }
    printf("\n");
    
    // Print bottom border
    printf(" ");
    for (int i = 0; i < gantt_size; i++) {
        int duration = gantt_time[i + 1] - gantt_time[i];
        for (int j = 0; j < duration * 2; j++) {
            printf("-");
        }
        printf(" ");
    }
    printf("\n");
    
    // Print timeline
    printf("%-2d", gantt_time[0]);
    for (int i = 0; i < gantt_size; i++) {
        int duration = gantt_time[i + 1] - gantt_time[i];
        for (int j = 0; j < duration * 2 - 1; j++) {
            printf(" ");
        }
        printf("%-2d", gantt_time[i + 1]);
    }
    printf("\n");
}

void printTable(Process proc[], int n) {
    float avg_tat = 0, avg_wt = 0, avg_rt = 0;
    
    printf("\n+-----+------------+------------+----------+------------+----------------+--------------+---------------+\n");
    printf("| PID | Arrival    | Burst      | Priority | Completion | Turnaround     | Waiting      | Response      |\n");
    printf("|     | Time       | Time       |          | Time       | Time           | Time         | Time          |\n");
    printf("+-----+------------+------------+----------+------------+----------------+--------------+---------------+\n");
    
    for (int i = 0; i < n; i++) {
        printf("| P%-2d | %-10d | %-10d | %-8d | %-10d | %-14d | %-12d | %-13d |\n",
               proc[i].pid, proc[i].arrival, proc[i].burst, proc[i].priority,
               proc[i].completion, proc[i].turnaround,
               proc[i].waiting, proc[i].response);
        
        avg_tat += proc[i].turnaround;
        avg_wt += proc[i].waiting;
        avg_rt += proc[i].response;
    }
    
    printf("+-----+------------+------------+----------+------------+----------------+--------------+---------------+\n");
    printf("\nAverage Turnaround Time: %.2f\n", avg_tat / n);
    printf("Average Waiting Time: %.2f\n", avg_wt / n);
    printf("Average Response Time: %.2f\n", avg_rt / n);
}

void priority_non_preemptive(Process proc[], int n) {
    int current_time = 0;
    int completed = 0;
    int *gantt = (int *)malloc(n * sizeof(int));
    int *gantt_time = (int *)malloc((n + 1) * sizeof(int));
    int gantt_size = 0;
    
    for (int i = 0; i < n; i++) {
        proc[i].completed = false;
    }
    
    printf("\nReady Queue (Execution Order): ");
    gantt_time[0] = 0;
    
    while (completed < n) {
        int highest_priority = INT_MAX;
        int min_index = -1;
        
        // Find process with highest priority (lower number = higher priority)
        for (int i = 0; i < n; i++) {
            if (!proc[i].completed && proc[i].arrival <= current_time) {
                if (proc[i].priority < highest_priority) {
                    highest_priority = proc[i].priority;
                    min_index = i;
                } else if (proc[i].priority == highest_priority && proc[i].arrival < proc[min_index].arrival) {
                    min_index = i;
                }
            }
        }
        
        if (min_index == -1) {
            current_time++;
            continue;
        }
        
        printf("P%d ", proc[min_index].pid);
        gantt[gantt_size] = proc[min_index].pid;
        
        proc[min_index].response = current_time - proc[min_index].arrival;
        current_time += proc[min_index].burst;
        proc[min_index].completion = current_time;
        proc[min_index].turnaround = proc[min_index].completion - proc[min_index].arrival;
        proc[min_index].waiting = proc[min_index].turnaround - proc[min_index].burst;
        proc[min_index].completed = true;
        
        gantt_time[gantt_size + 1] = current_time;
        gantt_size++;
        completed++;
    }
    printf("\n");
    
    printGanttChart(gantt, gantt_time, gantt_size);
    free(gantt);
    free(gantt_time);
}

int main() {
    int n;
    
    printf("Enter number of processes: ");
    scanf("%d", &n);
    
    Process *proc = (Process *)malloc(n * sizeof(Process));
    
    printf("\nEnter Arrival Time, Burst Time, and Priority for each process:\n");
    printf("(Lower priority number = Higher priority)\n");
    for (int i = 0; i < n; i++) {
        proc[i].pid = i + 1;
        printf("Process P%d:\n", i + 1);
        printf("  Arrival Time: ");
        scanf("%d", &proc[i].arrival);
        printf("  Burst Time: ");
        scanf("%d", &proc[i].burst);
        printf("  Priority: ");
        scanf("%d", &proc[i].priority);
    }
    
    priority_non_preemptive(proc, n);
    printTable(proc, n);
    
    free(proc);
    return 0;
}