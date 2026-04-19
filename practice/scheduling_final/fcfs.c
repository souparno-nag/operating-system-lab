#include <stdio.h>
#include <malloc.h>

typedef struct
{
    int pid, arrival, burst, completion, response, turnaround, waiting;
} Process;

void fcfs(Process p[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (p[j].arrival > p[j + 1].arrival) {
                Process temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }
    int time = 0;
    for (int i = 0; i < n; i++) {
        if (time < p[i].arrival) time = p[i].arrival ;
        p[i].response = time - p[i].arrival;
        time += p[i].burst;
        p[i].completion = time;
        p[i].turnaround = p[i].completion - p[i].arrival;
        p[i].waiting = p[i].turnaround - p[i].burst;
    }
}

int main() {
    int n;
    scanf("%d", &n);
    
    Process *p = (Process *)malloc(n * sizeof(Process));
    for (int i = 0; i < n; i++) {
        p[i].pid = i+1;
        scanf("%d %d", &p[i].arrival, &p[i].burst);
    }

    fcfs(p, n);

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