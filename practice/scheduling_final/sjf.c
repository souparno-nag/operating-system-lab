#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>
#include <limits.h>

typedef struct
{
    int pid, arrival, burst, completion, response, turnaround, waiting;
    bool completed;
} Process;

void sjf(Process p[], int n) {
    int done = 0, time = 0;
    while (done < n) {
        int shortest = -1, min_duration = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (!p[i].completed && p[i].arrival <= time && min_duration > p[i].burst) {
                shortest = i;
                min_duration = p[i].burst;
            }
        }
        if (shortest == -1) {time++; continue;}
        p[shortest].completed = true;
        p[shortest].response = time - p[shortest].arrival;
        time += p[shortest].burst;
        p[shortest].completion = time;
        p[shortest].turnaround = p[shortest].completion - p[shortest].arrival;
        p[shortest].waiting = p[shortest].turnaround - p[shortest].burst;
        done++;
    }
} 

int main() {
    int n;
    scanf("%d", &n);
    
    Process *p = (Process *)malloc(n * sizeof(Process));
    for (int i = 0; i < n; i++) {
        p[i].pid = i+1;
        p[i].completed = false;
        scanf("%d %d", &p[i].arrival, &p[i].burst);
    }

    sjf(p, n);

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