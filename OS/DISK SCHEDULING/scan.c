/*
 * Fixed OS/DISK SCHEDULING/scan.c
 * The original was flawed. This is a correct LOOK algorithm (an optimization of SCAN).
 */
#include <stdio.h>
#include <stdlib.h>

// Sort function (e.g., bubble sort)
void sort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int main() {
    int queue[100], n, head_pos, i, j;
    int direction; // 1 for high (up), 0 for low (down)
    int total_head_movement = 0;

    printf("Enter the number of queue requests: ");
    scanf("%d", &n);

    printf("Enter the disk positions to be read(queue): ");
    for (i = 0; i < n; i++) {
        scanf("%d", &queue[i]);
    }

    printf("Enter the initial head position: ");
    scanf("%d", &head_pos);

    printf("Enter the initial direction (1 for high, 0 for low): ");
    scanf("%d", &direction);

    // Sort the requests
    sort(queue, n);

    printf("\n--- LOOK Disk Scheduling Path ---\n");
    printf("Head starts at %d\n", head_pos);

    if (direction == 1) { // Moving high
        // 1. Move high, servicing requests
        for (i = 0; i < n; i++) {
            if (queue[i] >= head_pos) {
                int movement = abs(queue[i] - head_pos);
                total_head_movement += movement;
                printf("Move from %d to %d (Movement: %d)\n", head_pos, queue[i], movement);
                head_pos = queue[i];
            }
        }
        
        // 2. Move low (reverse), servicing remaining requests
        for (i = n - 1; i >= 0; i--) {
            if (queue[i] < head_pos) {
                int movement = abs(queue[i] - head_pos);
                total_head_movement += movement;
                printf("Move from %d to %d (Movement: %d)\n", head_pos, queue[i], movement);
                head_pos = queue[i];
            }
        }
    } 
    else { // Moving low
        // 1. Move low, servicing requests
        for (i = n - 1; i >= 0; i--) {
            if (queue[i] <= head_pos) {
                int movement = abs(queue[i] - head_pos);
                total_head_movement += movement;
                printf("Move from %d to %d (Movement: %d)\n", head_pos, queue[i], movement);
                head_pos = queue[i];
            }
        }
        
        // 2. Move high (reverse), servicing remaining requests
        for (i = 0; i < n; i++) {
            if (queue[i] > head_pos) {
                int movement = abs(queue[i] - head_pos);
                total_head_movement += movement;
                printf("Move from %d to %d (Movement: %d)\n", head_pos, queue[i], movement);
                head_pos = queue[i];
            }
        }
    }

    printf("\nTotal Head Movement: %d\n", total_head_movement);
    printf("Average Head Movement: %.2f\n", (float)total_head_movement / n);

    return 0;
}
