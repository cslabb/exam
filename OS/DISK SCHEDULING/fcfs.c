#include <stdio.h>
struct process
{
int pid;
int bt;
int wt;
int tat;
} p[50];
int main()
{
int n, temp, i, j;
printf("Please Enter no of processes : ");
scanf("%d", &n);
printf("\nPlease Enter Burst Time of processes : \n");
for (i = 0; i < n; i++)
{
printf("P%d : ", i + 1);
p[i].pid = i + 1;
scanf("%d", &p[i].bt);
}
for (i = 0; i < n; i++)
{
if (i == 0)
{
p[i].wt = 0;
p[i].tat = p[i].wt + p[i].bt;
continue;
}
p[i].wt = p[i - 1].bt + p[i - 1].wt;
p[i].tat = p[i].wt + p[i].bt;
}
printf("\nPid\tBT\tWT\tTAT\n");
for (i = 0; i < n; i++)
{
printf("%d\t%d\t%d\t%d\n", p[i].pid, p[i].bt, p[i].wt, p[i].tat);
}
temp = 0;
for (i = 0; i < n; i++)
temp += p[i].wt;
printf("\ntotal waiting time\t\t:%d\n", temp);
printf("average waiting time\t\t:%f\n", (float)temp / n);
temp = 0;
for (i = 0; i < n; i++)
temp += p[i].tat;
printf("total turn around time\t\t:%d\n", temp);
printf("average turn around time\t:%f\n", (float)temp / n);
