#include <stdio.h>
#include <malloc.h>
#include <limits.h>

typedef struct
{
    int pid, arrival, burst, completion, response, turnaround, waiting;
    int remaining;
} Process;

void srtf(Process p[], int n) {
    int time = 0, done = 0;
    for (int i = 0; i < n; i++) p[i].remaining = p[i].burst;

    while (done < n) {
        int idx = -1, min_remaining = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (p[i].remaining > 0 && p[i].arrival <= time && p[i].remaining < min_remaining) {
                idx = i;
                min_remaining = p[i].remaining;
            }
        }
        if (idx == -1) {time++; continue;}
        if (p[idx].remaining == p[idx].burst) p[idx].response = time - p[idx].arrival;
        p[idx].remaining--; time++;
        if (p[idx].remaining == 0) {
            done++;
            p[idx].completion = time;
            p[idx].turnaround = p[idx].completion - p[idx].arrival;
            p[idx].waiting = p[idx].turnaround - p[idx].burst;
        }
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

    srtf(p, n);

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