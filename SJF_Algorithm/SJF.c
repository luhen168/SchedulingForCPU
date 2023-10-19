#include <stdio.h>
#include <stdlib.h>

struct Process {
    int id;
    int burst_time;
    int waiting_time;
};

void swap(struct Process *a, struct Process *b) {
    struct Process temp = *a;
    *a = *b;
    *b = temp;
}

void sjf_non_preemptive(struct Process processes[], int n) {
    // Sắp xếp các tiến trình theo thời gian thực thi tăng dần
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (processes[i].burst_time > processes[j].burst_time) {
                swap(&processes[i], &processes[j]);
            }
        }
    }

    int total_waiting_time = 0;
    int waiting_time[n];
    waiting_time[0] = 0;

    for (int i = 1; i < n; i++) {
        waiting_time[i] = 0;
        for (int j = 0; j < i; j++) {
            waiting_time[i] += processes[j].burst_time;
        }
        total_waiting_time += waiting_time[i];
    }

    printf("Process Execution Order: ");
    for (int i = 0; i < n; i++) {
        printf("P%d ", processes[i].id);
    }

    printf("\nProcess\tBurst Time\tWaiting Time\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t\t%d\n", processes[i].id, processes[i].burst_time, waiting_time[i]);
    }
    printf("Average Waiting Time: %.2f\n", (float)total_waiting_time / n);
}

int main() {
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    struct Process *processes = (struct Process *)malloc(n * sizeof(struct Process));

    printf("Enter burst time for each process:\n");
    for (int i = 0; i < n; i++) {
        processes[i].id = i;
        printf("P%d: ", i);
        scanf("%d", &processes[i].burst_time);
        processes[i].waiting_time = 0;
    }

    sjf_non_preemptive(processes, n);

    free(processes);
    return 0;
}
