#include <stdio.h>
#include <limits.h>

typedef struct {
    int id, size, allocation;
} MemoryBlock;

typedef struct {
    int id, size;
} Process;

#define MAX 10

void best_fit(MemoryBlock blocks[], int blockCount, Process processes[], int processCount) {
    for (int i = 0; i < processCount; i++) {
        int best = -1, minWaste = INT_MAX;
        for (int j = 0; j < blockCount; j++) {
            int waste = blocks[j].size - processes[i].size;
            if (blocks[j].allocation == -1 && waste >= 0 && waste < minWaste) {
                best = j;
                minWaste = waste;
            }
        }
        if (best != -1) {
            blocks[best].allocation = processes[i].id;
            printf("Process %d is allocated to block %d.\n", processes[i].id, blocks[best].id);
        } else {
            printf("Process %d could not be allocated.\n", processes[i].id);
        }
    }
}

int main() {
    MemoryBlock blocks[MAX] = {{1,100,-1},{2,500,-1},{3,500,-1},{4,300,-1},{5,600,-1}};
    Process processes[MAX] = {{1,212},{2,417},{3,112},{4,426}};
    best_fit(blocks, 5, processes, 4);
}