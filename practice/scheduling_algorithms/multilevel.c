#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int pid, at, bt, q, rt, ct, tat, wt, rt_time;
} Process;

typedef struct { int items[100], f, r; } Queue;

void q_init(Queue *q) { q->f = q->r = -1; }
int q_empty(Queue *q) { return q->f == -1; }
void q_push(Queue *q, int v) {
    if (q->f == -1) q->f = 0;
    q->items[++q->r] = v;
}
int q_pop(Queue *q) {
    int v = q->items[q->f];
    if (q->f == q->r) q->f = q->r = -1;
    else q->f++;
    return v;
}

void mlfq(Process p[], int n, int qs[], int m) {
    Queue *queues = malloc(m * sizeof(Queue));
    for (int i = 0; i < m; i++) q_init(&queues[i]);
    
    for (int i = 0; i < n; i++) p[i].rt = p[i].bt;
    
    // Sort by arrival
    for (int i = 0; i < n-1; i++)
        for (int j = 0; j < n-i-1; j++)
            if (p[j].at > p[j+1].at) {
                Process t = p[j]; p[j] = p[j+1]; p[j+1] = t;
            }
    
    int time = 0, done = 0, idx = 0, in_queue[100] = {0};
    
    while (done < n) {
        // Add arrived processes
        while (idx < n && p[idx].at <= time) 
            q_push(&queues[p[idx].q], idx), in_queue[idx++] = 1;
        
        int cur = -1;
        for (int i = 0; i < m; i++) if (!q_empty(&queues[i])) { cur = i; break; }
        if (cur == -1) { time++; continue; }
        
        int i = q_pop(&queues[cur]);
        in_queue[i] = 0;
        
        if (p[i].rt == p[i].bt) p[i].rt_time = time - p[i].at;
        
        int exec = (qs[cur] == 0) ? p[i].rt : (p[i].rt < qs[cur] ? p[i].rt : qs[cur]);
        time += exec;
        p[i].rt -= exec;
        
        while (idx < n && p[idx].at <= time) 
            q_push(&queues[p[idx].q], idx), in_queue[idx++] = 1;
        
        if (p[i].rt == 0) {
            p[i].ct = time;
            p[i].tat = p[i].ct - p[i].at;
            p[i].wt = p[i].tat - p[i].bt;
            done++;
        } else q_push(&queues[cur], i), in_queue[i] = 1;
    }
    
    free(queues);
    
    printf("\nPID\tAT\tBT\tQ\tCT\tTAT\tWT\tRT\n");
    for (int i = 0; i < n; i++)
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", 
               p[i].pid, p[i].at, p[i].bt, p[i].q, p[i].ct, p[i].tat, p[i].wt, p[i].rt_time);
}

int main() {
    int n, m, q[10];
    printf("Processes: "); scanf("%d", &n);
    printf("Queues: "); scanf("%d", &m);
    
    for (int i = 0; i < m; i++) {
        printf("Q%d quantum (0=FCFS): ", i);
        scanf("%d", &q[i]);
    }
    
    Process *p = malloc(n * sizeof(Process));
    for (int i = 0; i < n; i++) {
        p[i].pid = i+1;
        printf("P%d AT BT Q: ", i+1);
        scanf("%d %d %d", &p[i].at, &p[i].bt, &p[i].q);
    }
    
    mlfq(p, n, q, m);
    free(p);
    return 0;
}