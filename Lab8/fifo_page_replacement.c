#include <stdio.h>

int main() {
    int frames, n, pages[50], frame[10], front = 0;
    int pageFaults = 0;

    printf("Enter number of frames: ");
    scanf("%d", &frames);

    printf("Enter number of pages in reference string: ");
    scanf("%d", &n);

    printf("Enter the reference string:\n");
    for (int i = 0; i < n; i++)
        scanf("%d", &pages[i]);

    // Initialize frames to -1 (empty)
    for (int i = 0; i < frames; i++)
        frame[i] = -1;

    printf("\nPage\tFrames\t\t\tFault\n");
    printf("----\t");
    for (int i = 0; i < frames; i++) printf("----\t");
    printf("-----\n");

    for (int i = 0; i < n; i++) {
        int found = 0;

        // Check if page already in frame (no fault)
        for (int j = 0; j < frames; j++) {
            if (frame[j] == pages[i]) {
                found = 1;
                break;
            }
        }

        printf("%d\t", pages[i]);

        if (!found) {
            // Page fault: replace using FIFO (circular queue)
            frame[front] = pages[i];
            front = (front + 1) % frames;
            pageFaults++;

            for (int j = 0; j < frames; j++) {
                if (frame[j] == -1)
                    printf("-\t");
                else
                    printf("%d\t", frame[j]);
            }
            printf("YES\n");
        } else {
            for (int j = 0; j < frames; j++) {
                if (frame[j] == -1)
                    printf("-\t");
                else
                    printf("%d\t", frame[j]);
            }
            printf("NO\n");
        }
    }

    printf("\nTotal Page Faults (FIFO): %d\n", pageFaults);
    return 0;
}