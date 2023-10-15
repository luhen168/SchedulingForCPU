#include <stdio.h>
#include <stdlib.h>

struct Process {
    int id;
    int burst_time;
};

void swap(struct Process *a, struct Process *b) {
    struct Process temp = *a;
    *a = *b;
    *b = temp;
}

void sjf(struct Process processes[], int n) {
    // Sắp xếp các tiến trình theo thời gian thực thi tăng dần
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (processes[j].burst_time > processes[j + 1].burst_time) {
                swap(&processes[j], &processes[j + 1]);
            }
        }
    }

    int waiting_time = 0;
    printf("Process Execution Order: ");
    for (int i = 0; i < n; i++) {
        printf("P%d ", processes[i].id);
        waiting_time += waiting_time + processes[i].burst_time;
    }

    printf("\nAverage Waiting Time: %.2f\n", (float)waiting_time / n);
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
    }

    sjf(processes, n);

    free(processes);
    return 0;
}
