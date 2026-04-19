#include <stdio.h>
#include <malloc.h>
#include <limits.h>

typedef struct
{
    int pid, arrival, burst, completion, response, turnaround, waiting;
    int remaining;
} Process;

void round_robin(Process p[], int n, int tq) {
    int time = 0, done = 0;
    int queue[100], front = 0, rear = 0, in_queue[n];

    for (int i = 0; i < n; i++) {
        in_queue[i] = 0;
        p[i].remaining = p[i].burst;
    }

    for (int j = 0; j < n; j++) {
        if (p[j].arrival <= time && p[j].remaining > 0 && !in_queue[j]) {
            queue[rear++] = j;
            in_queue[j] = 1;
        }
    }

    while (done < n) {
        if (front == rear) {
            time++;
            for (int j = 0; j < n; j++) {
                if (p[j].arrival <= time && p[j].remaining > 0 && !in_queue[j]) {
                    queue[rear++] = j;
                    in_queue[j] = 1;
                }
            }
        }
        int i = queue[front++]; in_queue[i] = 0;
        int exec = (p[i].remaining <= tq) ? p[i].remaining : tq;
        if (p[i].remaining == p[i].burst) p[i].response = time - p[i].arrival;
        p[i].remaining -= exec; time += exec;
        for (int j = 0; j < n; j++) {
            if (j == i) continue;
            if (p[j].arrival <= time && p[j].remaining > 0 && !in_queue[j]) {
                queue[rear++] = j;
                in_queue[j] = 1;
            }
        }
        if (p[i].remaining == 0) {
            done++;
            p[i].completion = time;
            p[i].turnaround = p[i].completion - p[i].arrival;
            p[i].waiting = p[i].turnaround - p[i].burst;
        } else {
            queue[rear++] = i; in_queue[i] = 1;
        }
    }
} 

int main() {
    int n, tq;
    scanf("%d", &n);
    
    Process *p = (Process *)malloc(n * sizeof(Process));
    for (int i = 0; i < n; i++) {
        p[i].pid = i+1;
        scanf("%d %d", &p[i].arrival, &p[i].burst);
    }
    scanf("%d", &tq);

    round_robin(p, n, tq);

    float avg_t = 0, avg_w = 0, avg_r = 0;
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\n", p[i].pid, p[i].arrival, p[i].burst, p[i].completion, p[i].turnaround, p[i].waiting, p[i].response);
        avg_t += p[i].turnaround;
        avg_w += p[i].waiting;
        avg_r += p[i].response;
    }

    free(p);

    printf("Avg turnaround: %.2f\nAvg waiting: %.2f\nAvg response: %.2f\n", avg_t/n, avg_w/n, avg_r/n);
    return 0;
}