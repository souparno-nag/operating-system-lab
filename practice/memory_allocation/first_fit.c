#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int size, allocation, id;
} MemoryBlock;

typedef struct {
    int size, id;
} Process;

#define MAX 5

void first_fit (MemoryBlock blocks[], int blockCount, Process processes[], int processCount) {
    for (int i = 0; i < processCount; i++) {
        bool found = false;
        for (int j = 0; j < blockCount; j++) {
            if (blocks[j].allocation == -1 && processes[i].size <= blocks[j].size) {
                blocks[j].allocation = processes[i].id;
                printf("Process %d is allocated to block %d.\n", processes[i].id, blocks[j].id);
                found = true;
                break;
            }
        }
        if (!found) printf("Process %d could not be allocated.\n", processes[i].id);
    }
}

int main() {
    MemoryBlock blocks[MAX] = {{100,-1,1},{500,-1,2},{200,-1,3},{300,-1,4},{600,-1,5}};
    Process processes[MAX] = {{212,1},{417,2},{112,3},{426,4}};
    first_fit(blocks, 5, processes, 4);
}