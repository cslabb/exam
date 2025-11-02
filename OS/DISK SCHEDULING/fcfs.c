/*
 * Fixed OS/DISK SCHEDULING/fcfs.c
 * The original file was a CPU scheduler. This is a correct DISK FCFS.
 */
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int queue[100], n, head_pos, i;
    int total_head_movement = 0;

    printf("Enter the number of disk requests: ");
    scanf("%d", &n);

    printf("Enter the requests in order: \n");
    for (i = 0; i < n; i++) {
        scanf("%d", &queue[i]);
    }

    printf("Enter the initial head position: ");
    scanf("%d", &head_pos);

    printf("\n--- FCFS Disk Scheduling Path ---\n");
    printf("Head starts at %d\n", head_pos);

    // Calculate total head movement
    for (i = 0; i < n; i++) {
        int movement = abs(queue[i] - head_pos);
        total_head_movement += movement;
        printf("Move from %d to %d (Movement: %d)\n", head_pos, queue[i], movement);
        head_pos = queue[i]; // Update head position
    }

    printf("\nTotal Head Movement: %d\n", total_head_movement);

    float avg_head_movement = (float)total_head_movement / n;
    printf("Average Head Movement: %.2f\n", avg_head_movement);

    return 0;
}
