#include <stdio.h>

// Returns the index of the frame holding the page used farthest in future
// Returns -1 if a page is never used again (should be replaced first)
int findOptimal(int frame[], int frames, int pages[], int n, int current) {
    int farthest = -1, replaceIndex = -1;

    for (int i = 0; i < frames; i++) {
        int j;
        for (j = current + 1; j < n; j++) {
            if (frame[i] == pages[j]) {
                if (j > farthest) {
                    farthest = j;
                    replaceIndex = i;
                }
                break;
            }
        }
        // If page in frame is never used again, replace it immediately
        if (j == n)
            return i;
    }

    // If all pages will be used again, replace the one used farthest
    return replaceIndex;
}

int main() {
    int frames, n, pages[50], frame[10];
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

        // Check if page is already in a frame
        for (int j = 0; j < frames; j++) {
            if (frame[j] == pages[i]) {
                found = 1;
                break;
            }
        }

        printf("%d\t", pages[i]);

        if (!found) {
            pageFaults++;

            // Check for empty frame first
            int emptySlot = -1;
            for (int j = 0; j < frames; j++) {
                if (frame[j] == -1) {
                    emptySlot = j;
                    break;
                }
            }

            if (emptySlot != -1) {
                frame[emptySlot] = pages[i];
            } else {
                // Use optimal replacement
                int replaceIndex = findOptimal(frame, frames, pages, n, i);
                frame[replaceIndex] = pages[i];
            }

            for (int j = 0; j < frames; j++) {
                if (frame[j] == -1) printf("-\t");
                else printf("%d\t", frame[j]);
            }
            printf("YES\n");
        } else {
            for (int j = 0; j < frames; j++) {
                if (frame[j] == -1) printf("-\t");
                else printf("%d\t", frame[j]);
            }
            printf("NO\n");
        }
    }

    printf("\nTotal Page Faults (OPTIMAL): %d\n", pageFaults);
    return 0;
}