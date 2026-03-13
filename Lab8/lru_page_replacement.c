#include <stdio.h>

int main() {
    int frames, n, pages[50], frame[10], lastUsed[10];
    int pageFaults = 0, count = 0;

    printf("Enter number of frames: ");
    scanf("%d", &frames);

    printf("Enter number of pages in reference string: ");
    scanf("%d", &n);

    printf("Enter the reference string:\n");
    for (int i = 0; i < n; i++)
        scanf("%d", &pages[i]);

    // Initialize frames and lastUsed to -1 (empty)
    for (int i = 0; i < frames; i++) {
        frame[i] = -1;
        lastUsed[i] = -1;
    }

    printf("\nPage\tFrames\t\t\tFault\n");
    printf("----\t");
    for (int i = 0; i < frames; i++) printf("----\t");
    printf("-----\n");

    for (int i = 0; i < n; i++) {
        int found = -1;
        count++;

        // Check if page is already in a frame
        for (int j = 0; j < frames; j++) {
            if (frame[j] == pages[i]) {
                found = j;
                break;
            }
        }

        printf("%d\t", pages[i]);

        if (found != -1) {
            // Page hit: update last used time
            lastUsed[found] = count;
            for (int j = 0; j < frames; j++) {
                if (frame[j] == -1) printf("-\t");
                else printf("%d\t", frame[j]);
            }
            printf("NO\n");
        } else {
            pageFaults++;

            // Check if there's an empty frame
            int emptySlot = -1;
            for (int j = 0; j < frames; j++) {
                if (frame[j] == -1) {
                    emptySlot = j;
                    break;
                }
            }

            if (emptySlot != -1) {
                frame[emptySlot] = pages[i];
                lastUsed[emptySlot] = count;
            } else {
                // Find LRU: frame with smallest lastUsed value
                int lruIndex = 0;
                for (int j = 1; j < frames; j++) {
                    if (lastUsed[j] < lastUsed[lruIndex])
                        lruIndex = j;
                }
                frame[lruIndex] = pages[i];
                lastUsed[lruIndex] = count;
            }

            for (int j = 0; j < frames; j++) {
                if (frame[j] == -1) printf("-\t");
                else printf("%d\t", frame[j]);
            }
            printf("YES\n");
        }
    }

    printf("\nTotal Page Faults (LRU): %d\n", pageFaults);
    return 0;
}