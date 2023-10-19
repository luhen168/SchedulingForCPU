#include <stdio.h>
#include <stdlib.h>

struct Process {
    int id;
    int arrival_time;
    int burst_time;
    int waiting_time;
    int remaining_time;
    int completed;
};

void swap(struct Process *a, struct Process *b) {
    struct Process temp = *a;
    *a = *b;
    *b = temp;
}

int findShortestJob(struct Process processes[], int n, int current_time) {
    int min_burst = 9999;
    int shortest_job = -1;

    for (int i = 0; i < n; i++) {
        if (!processes[i].completed && processes[i].arrival_time <= current_time && processes[i].burst_time < min_burst) {
            min_burst = processes[i].burst_time;
            shortest_job = i;
        }
    }

    return shortest_job;
}

void sjf_preemptive(struct Process processes[], int n) {
    int total_waiting_time = 0;
    int completed = 0;
    int current_time = 0;

    while (completed < n) {
        int shortest_job = findShortestJob(processes, n, current_time);

        if (shortest_job == -1) {
            current_time++;
            continue;
        }

        processes[shortest_job].remaining_time--;

        if (processes[shortest_job].remaining_time == 0) {
            processes[shortest_job].completed = 1;
            completed++;
            processes[shortest_job].waiting_time = current_time + 1 - processes[shortest_job].arrival_time - processes[shortest_job].burst_time;
            total_waiting_time += processes[shortest_job].waiting_time;
        }

        current_time++;
    }

    printf("Process\tArrival Time\tBurst Time\tWaiting Time\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t\t%d\t\t%d\n", processes[i].id, processes[i].arrival_time, processes[i].burst_time, processes[i].waiting_time);
    }

    printf("Average Waiting Time: %.2f\n", (float)total_waiting_time / n);
}

int main() {
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    struct Process *processes = (struct Process *)malloc(n * sizeof(struct Process));

    printf("Enter arrival time and burst time for each process:\n");
    for (int i = 0; i < n; i++) {
        processes[i].id = i;
        processes[i].completed = 0;
        printf("P%d Arrival Time: ", i);
        scanf("%d", &processes[i].arrival_time);
        printf("P%d Burst Time: ", i);
        scanf("%d", &processes[i].burst_time);
        processes[i].remaining_time = processes[i].burst_time;
    }

    sjf_preemptive(processes, n);

    free(processes);
    return 0;
}
