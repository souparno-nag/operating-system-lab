#include <stdio.h>
#include <stdbool.h>

#define MAX 20

int findOptimal (int frames[], int frameCount, int pages[], int pageCount, int current) {
    int farthest = -1, replacementIndex = -1;
    for (int i = 0; i < frameCount; i++) {
        int j;
        for (j = current+1; j < pageCount; j++) {
            if (frames[i] == pages[j]) {
                if (j > farthest) {
                    replacementIndex = i;
                    farthest = j;
                    break;
                }
            }
        }
        if (j == pageCount) return i;
    }
    return replacementIndex;
}

int main() {
    int pageCount, frameCount, pages[MAX], frames[MAX], pageFaults = 0;

    scanf("%d %d", &pageCount, &frameCount);
    for (int i = 0; i < pageCount; i++) scanf("%d", &pages[i]);
    for (int i = 0; i < frameCount; i++) frames[i] = -1;

    for (int i = 0; i < pageCount; i++) {
        bool found = false;
        for (int j = 0; j < frameCount; j++) {
            if (frames[j] == pages[i]) {found = true; break;}
        }
        if (!found) {
            int slot = -1;
            for (int j = 0; j < frameCount; j++) {
                if (frames[j] == -1) {slot = j; break;}
            }
            frames[(slot == -1) ? findOptimal(frames, frameCount, pages, pageCount, i) : slot] = pages[i];
            pageFaults++;
            printf("PAGE FAULT\n");
        } else {
            printf("PAGE HIT\n");
        }
    }
    printf("Total number of page faults is %d\n", pageFaults);
}