#include <stdio.h>

#define MAX 20

void lru(int pages[], int pageCount, int frameCount) {
    int frames[MAX], lastUsed[MAX];
    for (int i = 0; i < frameCount; i++) {frames[i] = -1; lastUsed[i] = 0;}

    int count = 0, pageFaults = 0;

    for (int i = 0; i < pageCount; i++) {
        int found = -1;
        count++;
        for (int j = 0; j < frameCount; j++) {
            if (frames[j] == pages[i]) {found = j; break;}
        }
        if (found != -1) {
            lastUsed[found] = count;
            printf("PAGE HIT\n");
        } else {
            int slot = -1;
            for (int j = 0; j < frameCount; j++) {
                if (frames[j] == -1) {slot = j; break;}
            }
            if (slot == -1) {
                slot = 0;
                for (int j = 0; j < frameCount; j++) {
                    if (lastUsed[j] < lastUsed[slot]) {
                        slot = j;
                    }
                }
            }
            frames[slot] = pages[i];
            lastUsed[slot] = count;
            pageFaults++;
            printf("PAGE FAULT\n");
        }
    }
    printf("Total number of page faults is %d\n", pageFaults);
}

int main() {
    int pageCount, frameCount, pages[MAX];

    scanf("%d %d", &pageCount, &frameCount);
    for (int i = 0; i < pageCount; i++) scanf("%d", &pages[i]);

    lru(pages, pageCount, frameCount);
}