#include <stdio.h>

typedef struct {
    int name;
    int arrival;
    int burst;
    int completion;
    int waiting;
    int turnaround;
} Process;

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
    int currenttime = 0;
    printf("\nTime  | Process\n");
    printf("------------+-----------\n");

    for(int i=0;i<n;i++){
        if(currenttime < processes[i].arrival){
            currenttime = processes[i].arrival;
    }
    int executiontime = processes[i].burst;
    currenttime += executiontime;

    printf("%-5d |  P%d\n",currenttime,processes[i].name);

    processes[i].completion = currenttime;
    processes[i].turnaround = processes[i].completion - processes[i].arrival;
    processes[i].waiting = processes[i].turnaround - processes[i].burst;
}
float avgwt = 0.0, avgtat = 0.0;

printf("\nProcess | Arrival | Burst | Completion | Waiting | Turnaround\n");
for(int i=0;i<n;i++){
    printf("P%d%10d%10d%15d%15d%15d\n",processes[i].name,processes[i].arrival,processes[i].burst,processes[i].completion,processes[i].waiting,processes[i].turnaround);
    avgwt+= processes[i].waiting;
    avgtat+= processes[i].turnaround;
}
avgwt/=n;
avgtat/=n;

printf("\nAverage waiting time: %.2f",avgwt);
printf("\nAverage turnaround time: %.2f",avgtat);

return 0;
}
