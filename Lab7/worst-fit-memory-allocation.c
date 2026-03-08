#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_BLOCKS 10
#define MAX_PROCESSES 10

typedef struct {
    int size;
    int allocated;  // Process ID that occupies this block, -1 if free
    int id;         // Block ID
} MemoryBlock;

typedef struct {
    int size;
    int id;         // Process ID
    int allocated;  // Block ID where allocated, -1 if not allocated
} Process;

void worstFit(MemoryBlock blocks[], int blockCount, Process processes[], int processCount) {
    printf("\n=== WORST-FIT ALLOCATION ===\n");
    
    // Make a copy of blocks to work with
    MemoryBlock workingBlocks[MAX_BLOCKS];
    for (int i = 0; i < blockCount; i++) {
        workingBlocks[i] = blocks[i];
        workingBlocks[i].allocated = -1;
    }
    
    // Allocate each process
    for (int i = 0; i < processCount; i++) {
        int worstBlockIndex = -1;
        int maxSize = -1;
        
        // Find largest available block
        for (int j = 0; j < blockCount; j++) {
            if (workingBlocks[j].allocated == -1 && workingBlocks[j].size >= processes[i].size) {
                if (workingBlocks[j].size > maxSize) {
                    maxSize = workingBlocks[j].size;
                    worstBlockIndex = j;
                }
            }
        }
        
        if (worstBlockIndex != -1) {
            workingBlocks[worstBlockIndex].allocated = processes[i].id;
            printf("Process P%d (size %d) allocated to Block B%d (size %d)\n", 
                   processes[i].id, processes[i].size, 
                   workingBlocks[worstBlockIndex].id, workingBlocks[worstBlockIndex].size);
        } else {
            printf("Process P%d (size %d) could NOT be allocated\n", 
                   processes[i].id, processes[i].size);
        }
    }
    
    // Show final memory state
    printf("\nFinal Memory State:\n");
    printf("Block\tSize\tStatus\n");
    for (int i = 0; i < blockCount; i++) {
        printf("B%d\t%d\t", workingBlocks[i].id, workingBlocks[i].size);
        if (workingBlocks[i].allocated == -1)
            printf("Free\n");
        else
            printf("Allocated to P%d\n", workingBlocks[i].allocated);
    }
}

int main() {
    // SAMPLE INPUT - Same for all three algorithms
    MemoryBlock blocks[MAX_BLOCKS] = {
        {100, -1, 1},   // Block 1: size 100, free
        {500, -1, 2},   // Block 2: size 500, free
        {200, -1, 3},   // Block 3: size 200, free
        {300, -1, 4},   // Block 4: size 300, free
        {600, -1, 5}    // Block 5: size 600, free
    };
    int blockCount = 5;
    
    Process processes[MAX_PROCESSES] = {
        {212, 1, -1},   // Process 1: size 212
        {417, 2, -1},   // Process 2: size 417
        {112, 3, -1},   // Process 3: size 112
        {426, 4, -1}    // Process 4: size 426
    };
    int processCount = 4;
    
    printf("Memory Blocks:\n");
    for (int i = 0; i < blockCount; i++) {
        printf("Block B%d: %d KB\n", blocks[i].id, blocks[i].size);
    }
    
    printf("\nProcesses to allocate:\n");
    for (int i = 0; i < processCount; i++) {
        printf("Process P%d: %d KB\n", processes[i].id, processes[i].size);
    }
    
    // Run all three algorithms
    worstFit(blocks, blockCount, processes, processCount);
    
    return 0;
}