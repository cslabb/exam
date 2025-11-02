/*
 * Fixed OS/CPU Scheduling/fcfs.c
 */
#include <stdio.h>

typedef struct {
    int name;
    int arrival;
    int burst;
    int completion;
    int waiting;
    int turnaround;
} Process;

// --- BUG FIX: Added sort function ---
void sortByArrival(Process processes[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (processes[j].arrival > processes[j + 1].arrival) {
                Process temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }
}
// --- END BUG FIX ---

int main() {
    int n;

    printf("enter the number of processes: ");
    scanf("%d",&n);

    Process processes[n];
    for(int i=0;i<n;i++){
        printf("Enter arrival time for P%d: ",i+1);
        processes[i].name = i+1;
        scanf("%d",&processes[i].arrival);
        printf("Enter burst time for P%d: ",i+1);
        scanf("%d",&processes[i].burst);
    }
    
    // --- BUG FIX: Call the sort function ---
    sortByArrival(processes, n);
    // --- END BUG FIX ---
    
    int currenttime = 0;
    float avgwt = 0.0, avgtat = 0.0;
    
    printf("\n--- Gantt Chart (Order of Execution) ---\n");

    for(int i=0;i<n;i++){
        // Handle idle time
        if(currenttime < processes[i].arrival){
            printf("Time %d-%d: IDLE\n", currenttime, processes[i].arrival);
            currenttime = processes[i].arrival;
        }

        printf("Time %d-%d: P%d\n", currenttime, currenttime + processes[i].burst, processes[i].name);
        
        currenttime += processes[i].burst;

        processes[i].completion = currenttime;
        processes[i].turnaround = processes[i].completion - processes[i].arrival;
        processes[i].waiting = processes[i].turnaround - processes[i].burst;
        
        avgwt += processes[i].waiting;
        avgtat += processes[i].turnaround;
    }


    printf("\nProcess | Arrival | Burst | Completion | Waiting | Turnaround\n");
    for(int i=0;i<n;i++){
        printf("P%d%10d%10d%15d%15d%15d\n",processes[i].name,processes[i].arrival,processes[i].burst,processes[i].completion,processes[i].waiting,processes[i].turnaround);
    }
    
    avgwt /= n;
    avgtat /= n;

    printf("\nAverage waiting time: %.2f",avgwt);
    printf("\nAverage turnaround time: %.2f",avgtat);

    return 0;
}
