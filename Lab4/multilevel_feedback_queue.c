#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int pid;
    int arrival;
    int burst;
    int remaining;
    int completion;
    int turnaround;
    int waiting;
    int response;
    int current_queue;
    bool started;
} Process;

typedef struct {
    int items[100];
    int front;
    int rear;
} Queue;

void initQueue(Queue *q) {
    q->front = -1;
    q->rear = -1;
}

bool isEmpty(Queue *q) {
    return q->front == -1;
}

void enqueue(Queue *q, int value) {
    if (q->front == -1) q->front = 0;
    q->rear++;
    q->items[q->rear] = value;
}

int dequeue(Queue *q) {
    int item = q->items[q->front];
    if (q->front == q->rear) {
        q->front = -1;
        q->rear = -1;
    } else {
        q->front++;
    }
    return item;
}

void printGanttChart(int gantt[], int gantt_size) {
    printf("\nGantt Chart:\n ");
    
    // Compress consecutive same processes
    int compressed_gantt[1000];
    int compressed_time[1000];
    int compressed_size = 0;
    
    if (gantt_size == 0) return;
    
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

void multilevel_feedback_queue(Process proc[], int n, int num_queues, int quantum[]) {
    Queue *queues = (Queue *)malloc(num_queues * sizeof(Queue));
    for (int i = 0; i < num_queues; i++) {
        initQueue(&queues[i]);
    }
    
    int current_time = 0;
    int completed = 0;
    int *gantt = (int *)malloc(1000 * sizeof(int));
    int gantt_size = 0;
    bool *in_queue = (bool *)malloc(n * sizeof(bool));
    
    for (int i = 0; i < n; i++) {
        proc[i].remaining = proc[i].burst;
        proc[i].started = false;
        proc[i].current_queue = 0;  // All processes start in queue 0
        in_queue[i] = false;
    }
    
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
    
    printf("\nMultilevel Feedback Queue Configuration:\n");
    for (int i = 0; i < num_queues - 1; i++) {
        printf("Queue %d: Round Robin (Time Quantum = %d)\n", i, quantum[i]);
    }
    printf("Queue %d: FCFS\n", num_queues - 1);
    printf("\nProcesses start in Queue 0 and move down if they don't complete\n");
    
    printf("\nReady Queue (Execution Order):\n");
    
    int idx = 0;
    
    while (completed < n) {
        // Add newly arrived processes to queue 0
        while (idx < n && proc[idx].arrival <= current_time) {
            enqueue(&queues[0], idx);
            in_queue[idx] = true;
            proc[idx].current_queue = 0;
            idx++;
        }
        
        // Find highest priority non-empty queue
        int current_queue = -1;
        for (int i = 0; i < num_queues; i++) {
            if (!isEmpty(&queues[i])) {
                current_queue = i;
                break;
            }
        }
        
        if (current_queue == -1) {
            current_time++;
            continue;
        }
        
        int i = dequeue(&queues[current_queue]);
        in_queue[i] = false;
        
        if (!proc[i].started) {
            proc[i].response = current_time - proc[i].arrival;
            proc[i].started = true;
        }
        
        int exec_time;
        if (current_queue == num_queues - 1) {
            // FCFS for last queue
            exec_time = proc[i].remaining;
        } else {
            // Round Robin for other queues
            exec_time = (proc[i].remaining < quantum[current_queue]) ? 
                        proc[i].remaining : quantum[current_queue];
        }
        
        for (int t = 0; t < exec_time; t++) {
            gantt[gantt_size++] = proc[i].pid;
        }
        
        proc[i].remaining -= exec_time;
        current_time += exec_time;
        
        // Add newly arrived processes to queue 0
        while (idx < n && proc[idx].arrival <= current_time) {
            enqueue(&queues[0], idx);
            in_queue[idx] = true;
            proc[idx].current_queue = 0;
            idx++;
        }
        
        if (proc[i].remaining == 0) {
            proc[i].completion = current_time;
            proc[i].turnaround = proc[i].completion - proc[i].arrival;
            proc[i].waiting = proc[i].turnaround - proc[i].burst;
            completed++;
        } else {
            // Move to next lower priority queue if not in last queue
            if (current_queue < num_queues - 1) {
                proc[i].current_queue = current_queue + 1;
                enqueue(&queues[current_queue + 1], i);
            } else {
                enqueue(&queues[current_queue], i);
            }
            in_queue[i] = true;
        }
    }
    
    printGanttChart(gantt, gantt_size);
    free(gantt);
    free(in_queue);
    free(queues);
}

int main() {
    int n, num_queues;
    
    printf("Enter number of processes: ");
    scanf("%d", &n);
    
    printf("Enter number of queues (levels): ");
    scanf("%d", &num_queues);
    
    Process *proc = (Process *)malloc(n * sizeof(Process));
    int *quantum = (int *)malloc(num_queues * sizeof(int));
    
    printf("\nEnter Time Quantum for each queue:\n");
    printf("(Last queue will be FCFS automatically)\n");
    for (int i = 0; i < num_queues - 1; i++) {
        printf("Queue %d (Round Robin): ", i);
        scanf("%d", &quantum[i]);
    }
    quantum[num_queues - 1] = 0;  // FCFS
    
    printf("\nEnter Arrival Time and Burst Time for each process:\n");
    printf("(All processes start in Queue 0)\n");
    for (int i = 0; i < n; i++) {
        proc[i].pid = i + 1;
        printf("Process P%d:\n", i + 1);
        printf("  Arrival Time: ");
        scanf("%d", &proc[i].arrival);
        printf("  Burst Time: ");
        scanf("%d", &proc[i].burst);
    }
    
    multilevel_feedback_queue(proc, n, num_queues, quantum);
    printTable(proc, n);
    
    free(proc);
    free(quantum);
    return 0;
}