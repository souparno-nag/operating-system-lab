#include <stdio.h>
#include <limits.h>

typedef struct {
    int id, size, allocation;
} MemoryBlock;

typedef struct {
    int id, size;
} Process;

#define MAX 10

void worst_fit(MemoryBlock blocks[], int blockCount, Process processes[], int processCount) {
    for (int i = 0; i < processCount; i++) {
        int worst = -1, maxDiff = INT_MIN;
        for (int j = 0; j < blockCount; j++) {
            int diff = blocks[j].size - processes[i].size;
            if (blocks[j].allocation == -1 && diff >= 0 && diff > maxDiff) {
                worst = j;
                maxDiff = diff;
            }
        }
        if (worst != -1) {
            blocks[worst].allocation = processes[i].id;
            printf("Process %d is allocated to block %d.\n", processes[i].id, blocks[worst].id);
        } else {
            printf("Process %d could not be allocated.\n", processes[i].id);
        }
    }
}

int main() {
    MemoryBlock blocks[MAX] = {{1,100,-1},{2,500,-1},{3,500,-1},{4,300,-1},{5,600,-1}};
    Process processes[MAX] = {{1,212},{2,417},{3,112},{4,426}};
    worst_fit(blocks, 5, processes, 4);
}