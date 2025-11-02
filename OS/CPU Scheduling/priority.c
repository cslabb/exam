#include <stdio.h>

struct Process {
    int processNo;
    int arrivalTime;
    int burstTime;
    int priority;
    int completionTime;
    int waitingTime;
    int turnaroundTime;
    int isCompleted;
};

// Function to find highest priority process available at currentTime
int findHighestPriority(struct Process proc[], int n, int currentTime) {
    int highestPriority = -1;
    int minPriority = 999999;

    for (int i = 0; i < n; i++) {
        if (!proc[i].isCompleted && proc[i].arrivalTime <= currentTime) {
            if (proc[i].priority < minPriority) {
                minPriority = proc[i].priority;
                highestPriority = i;
            }
        }
    }
    return highestPriority;
}

void calculateTimes(struct Process proc[], int n) {
    int currentTime = 0;
    int completed = 0;

    // Initialize completion flags
    for (int i = 0; i < n; i++) {
        proc[i].isCompleted = 0;
    }

    while (completed != n) {
        int currentProcess = findHighestPriority(proc, n, currentTime);

        if (currentProcess == -1) {
            // No process has arrived yet, CPU idle
            currentTime++;
            continue;
        }

        // Execute the selected process completely (non-preemptive)
        currentTime += proc[currentProcess].burstTime;
        proc[currentProcess].completionTime = currentTime;
        proc[currentProcess].turnaroundTime = proc[currentProcess].completionTime - proc[currentProcess].arrivalTime;
        proc[currentProcess].waitingTime = proc[currentProcess].turnaroundTime - proc[currentProcess].burstTime;
        proc[currentProcess].isCompleted = 1;
        completed++;
    }
}

int main() {
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    struct Process proc[n];

    printf("Enter process details (arrival time, burst time, priority):\n");
    for (int i = 0; i < n; i++) {
        proc[i].processNo = i + 1;
        printf("P%d: ", i + 1);
        scanf("%d %d %d", &proc[i].arrivalTime, &proc[i].burstTime, &proc[i].priority);
    }

    calculateTimes(proc, n);

    float total_waiting = 0, total_turnaround = 0;
    for (int i = 0; i < n; i++) {
        total_waiting += proc[i].waitingTime;
        total_turnaround += proc[i].turnaroundTime;
    }

    float avg_waiting = total_waiting / n;
    float avg_turnaround = total_turnaround / n;

    printf("\nProcess\tArrival Time\tBurst Time\tPriority\tWaiting Time\tTurnaround Time\tCompletion Time\n");
    printf("------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n",
               proc[i].processNo,
               proc[i].arrivalTime,
               proc[i].burstTime,
               proc[i].priority,
               proc[i].waitingTime,
               proc[i].turnaroundTime,
               proc[i].completionTime);
    }
    printf("------------------------------------------------------------------------------------------------\n");
    printf("Average Waiting Time: %.2f\n", avg_waiting);
    printf("Average Turnaround Time: %.2f\n", avg_turnaround);

    return 0;
}
