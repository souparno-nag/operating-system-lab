#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

typedef struct {
    int pid;
    int arrival;
    int burst;
    int remaining;
    int completion;
    int turnaround;
    int waiting;
    int response;
    bool started;
} Process;

void printGanttChart(int gantt[], int gantt_size) {
    printf("\nGantt Chart:\n ");
    
    // Compress consecutive same processes
    int compressed_gantt[1000];
    int compressed_time[1000];
    int compressed_size = 0;
    
    compressed_gantt[0] = gantt[0];
    compressed_time[0] = 0;
    compressed_size = 1;
    
    for (int i = 1; i < gantt_size; i++) {
        if (gantt[i] != gantt[i - 1]) {
            compressed_gantt[compressed_size] = gantt[i];
            compressed_time[compressed_size] = i;
            compressed_size++;
        }
    }
    compressed_time[compressed_size] = gantt_size;
    
    // Print compressed Gantt chart
    for (int i = 0; i < compressed_size; i++) {
        int len = compressed_time[i + 1] - compressed_time[i];
        for (int j = 0; j < len; j++) printf("--");
        printf(" ");
    }
    printf("\n|");
    
    for (int i = 0; i < compressed_size; i++) {
        int len = compressed_time[i + 1] - compressed_time[i];
        for (int j = 0; j < len - 1; j++) printf(" ");
        printf("P%d", compressed_gantt[i]);
        for (int j = 0; j < len - 1; j++) printf(" ");
        printf("|");
    }
    printf("\n ");
    
    for (int i = 0; i < compressed_size; i++) {
        int len = compressed_time[i + 1] - compressed_time[i];
        for (int j = 0; j < len; j++) printf("--");
        printf(" ");
    }
    printf("\n");
    
    printf("0");
    for (int i = 0; i < compressed_size; i++) {
        int len = compressed_time[i + 1] - compressed_time[i];
        for (int j = 0; j < len; j++) printf("  ");
        printf("%-2d", compressed_time[i + 1]);
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

void srtf(Process proc[], int n) {
    int current_time = 0;
    int completed = 0;
    int *gantt = (int *)malloc(1000 * sizeof(int));
    int gantt_size = 0;
    
    for (int i = 0; i < n; i++) {
        proc[i].remaining = proc[i].burst;
        proc[i].started = false;
    }
        
    while (completed < n) {
        int min_remaining = INT_MAX;
        int min_index = -1;
        
        // Find process with shortest remaining time
        for (int i = 0; i < n; i++) {
            if (proc[i].arrival <= current_time && proc[i].remaining > 0) {
                if (proc[i].remaining < min_remaining) {
                    min_remaining = proc[i].remaining;
                    min_index = i;
                } else if (proc[i].remaining == min_remaining && proc[i].arrival < proc[min_index].arrival) {
                    min_index = i;
                }
            }
        }
        
        if (min_index == -1) {
            current_time++;
            continue;
        }
        
        if (!proc[min_index].started) {
            proc[min_index].response = current_time - proc[min_index].arrival;
            proc[min_index].started = true;
        }
        
        gantt[gantt_size++] = proc[min_index].pid;
        proc[min_index].remaining--;
        current_time++;
        
        if (proc[min_index].remaining == 0) {
            proc[min_index].completion = current_time;
            proc[min_index].turnaround = proc[min_index].completion - proc[min_index].arrival;
            proc[min_index].waiting = proc[min_index].turnaround - proc[min_index].burst;
            completed++;
        }
    }
    printf("\n");
    
    printGanttChart(gantt, gantt_size);
    free(gantt);
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
    
    srtf(proc, n);
    printTable(proc, n);
    
    free(proc);
    return 0;
}