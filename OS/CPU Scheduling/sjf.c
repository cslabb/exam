#include<stdio.h>

struct Process {
    int pid;
    int arrivaltime;
    int bursttime;
    int completiontime;
    int waitingtime;
    int turnaroundtime;
};

// Sort processes based on burst time (SJF)
void sortProcesses(struct Process processes[], int n) {
    for(int i = 0; i < n-1; i++) {
        for(int j = 0; j < n-i-1; j++) {
            if(processes[j].bursttime > processes[j+1].bursttime) {
                struct Process temp = processes[j];
                processes[j] = processes[j+1];
                processes[j+1] = temp;
            }
        }
    }
}

void calculateTimes(struct Process processes[], int n) {
    // Calculate completion time, waiting time, and turnaround time
    processes[0].completiontime = processes[0].arrivaltime + processes[0].bursttime;
    processes[0].turnaroundtime = processes[0].completiontime - processes[0].arrivaltime;
    processes[0].waitingtime = processes[0].turnaroundtime - processes[0].bursttime;

    for(int i = 1; i < n; i++) {
        // If process arrives after previous process completion
        if(processes[i].arrivaltime > processes[i-1].completiontime) {
            processes[i].completiontime = processes[i].arrivaltime + processes[i].bursttime;
        } else {
            processes[i].completiontime = processes[i-1].completiontime + processes[i].bursttime;
        }
        
        processes[i].turnaroundtime = processes[i].completiontime - processes[i].arrivaltime;
        processes[i].waitingtime = processes[i].turnaroundtime - processes[i].bursttime;
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
    
    // Input process details
    for(int i = 0; i < n; i++) {
        printf("Enter the arrival time and burst time for process P%d: ", i+1);
        scanf("%d %d", &processes[i].arrivaltime, &processes[i].bursttime);
        processes[i].pid = i+1;
    }
    
    // Sort processes by burst time (SJF)
    sortProcesses(processes, n);
    
    // Calculate all times
    calculateTimes(processes, n);
    
    // Print results
    printResults(processes, n);
    
    return 0;
}
