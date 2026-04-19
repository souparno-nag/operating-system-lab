#include <stdio.h>
#include <stdbool.h>

#define MAX 50

int fifo(int pages[], int pageCount, int frameCount) {
    int frames[MAX];
    for (int i = 0; i < frameCount; i++) frames[i] = -1;
    int front = 0, pageFaultCount = 0;

    for (int i = 0; i < pageCount; i++) {
        bool found = false;
        for (int j = 0; j < frameCount; j++) {
            if (frames[j] == pages[i]) {
                found = true;
                printf("PAGE HIT\n");
                break;
            }
        }
        if (!found) {
            frames[front] = pages[i];
            front = (front + 1)%frameCount;
            pageFaultCount++;
            printf("PAGE FAULT\n");
        }
    }
    return pageFaultCount;
}

int main() {
    int pageCount, frameCount, pages[MAX];

    scanf("%d %d", &pageCount, &frameCount);
    for (int i = 0; i < pageCount; i++) scanf("%d", &pages[i]);

    int pageFaultCount = fifo(pages, pageCount, frameCount);
    printf("Number of page faults is %d\n", pageFaultCount);

    return 0;
}