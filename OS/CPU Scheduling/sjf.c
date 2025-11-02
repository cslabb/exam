/*
 * Fixed OS/CPU Scheduling/sjf.c
 * This is a complete rewrite for a correct non-preemptive SJF algorithm.
 */
#include<stdio.h>

struct Process {
    int pid;
    int arrivaltime;
    int bursttime;
    int completiontime;
    int waitingtime;
    int turnaroundtime;
    int isCompleted; // Flag to track completion
};

void calculateTimes(struct Process processes[], int n) {
    int currentTime = 0;
    int completed = 0;
    float total_wt = 0, total_tat = 0;

    for (int i = 0; i < n; i++) {
        processes[i].isCompleted = 0;
    }

    while (completed != n) {
        int shortest_job_index = -1;
        int min_burst = 999999;

        // Find the shortest job that has arrived and is not completed
        for (int i = 0; i < n; i++) {
            if (processes[i].arrivaltime <= currentTime && processes[i].isCompleted == 0) {
                if (processes[i].bursttime < min_burst) {
                    min_burst = processes[i].bursttime;
                    shortest_job_index = i;
                }
            }
        }

        if (shortest_job_index == -1) {
            // No process has arrived yet, increment time (IDLE)
            currentTime++;
        } else {
            // Process the shortest job
            struct Process *p = &processes[shortest_job_index];

            p->completiontime = currentTime + p->bursttime;
            p->turnaroundtime = p->completiontime - p->arrivaltime;
            p->waitingtime = p->turnaroundtime - p->bursttime;

            if (p->waitingtime < 0) {
                 p->waitingtime = 0;
            }

            total_wt += p->waitingtime;
            total_tat += p->turnaroundtime;

            p->isCompleted = 1;
            completed++;
            currentTime = p->completiontime;
        }
    }
}

void printResults(struct Process processes[], int n) {
    float avgwt = 0, avgtat = 0;
    
    printf("\nProcess Scheduling Table:\n");
    printf("--------------------------------------------------------------------------------\n");
    printf("PID\tArrival Time\tBurst Time\tCompletion Time\tWaiting Time\tTurnaround Time\n");
    printf("--------------------------------------------------------------------------------\n");
    
    for(int i = 0; i < n; i++) {
        printf("P%d\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n",
               processes[i].pid,
               processes[i].arrivaltime,
               processes[i].bursttime,
               processes[i].completiontime,
               processes[i].waitingtime,
               processes[i].turnaroundtime);
        
        avgwt += processes[i].waitingtime;
        avgtat += processes[i].turnaroundtime;
    }
    
    printf("--------------------------------------------------------------------------------\n");
    printf("Average Waiting Time: %.2f\n", avgwt/n);
    printf("Average Turnaround Time: %.2f\n", avgtat/n);
}

int main() {
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);
    
    struct Process processes[n];
    
    for(int i = 0; i < n; i++) {
        printf("Enter the arrival time and burst time for process P%d: ", i+1);
        scanf("%d %d", &processes[i].arrivaltime, &processes[i].bursttime);
        processes[i].pid = i+1;
    }
    
    // Calculate all times using correct SJF logic
    calculateTimes(processes, n);
    
    // Print results
    printResults(processes, n);
    
    return 0;
}
