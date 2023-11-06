#include<stdio.h>

//Tạo một cấu trúc (PCB-khối điều khiển Process Control Block)
typedef struct Process {
	int pid;
    int arrival;
    int burst; 
    int turnaround; 
}process;

// ham in ra dòng " ---------------------- "
void pline(int x) {
	int i;
	for (i = 0; i < x; i++) {
		printf("-");
	}
	printf("\n");
}

//Ham nhap du lieu 
void InputData(process a[], int *num){
	printf("Enter the total number of Processes: ");
	scanf("%d", &(*num));
	for (int i = 0; i < *num; i++) {
		printf("Enter Arrival time and Burst time for Process %d: \n", i + 1);
		scanf("%d %d", &a[i].arrival, &a[i].burst);
		a[i].pid = i + 1;
	}
}

//Ham sap xep 
void SortSequence(process a[],int num){
	int temp;
	for (int i = 0; i < num - 1; i++) {
		for (int j = 0; j < num - i - 1; j++) {
			if (a[j].arrival > a[j + 1].arrival) {
				temp = a[j].arrival;
				a[j].arrival = a[j + 1].arrival;
				a[j + 1].arrival = temp;
			}
		}
	}
}

void CalculateTurnAround(process a[],int num){
	int wait;
	int sum;
	float avg;
	// Tinh Time Turnaround , timeturnaround = waitting + burst
	for (int i = 0; i < num; i++) {
		a[i].turnaround = a[i].burst + wait ; 
		wait = a[i].turnaround;
		sum += a[i].turnaround;
	}
	
	//In ra bang tien trinh
	pline(44);
	printf("PID\tArrival\tBurst\tTurnaround\n");
	pline(44);
	for (int i = 0; i < num; i++) {
		printf("%d\t%d\t%d\t%d\n", a[i].pid, a[i].arrival, a[i].burst, a[i].turnaround);
	}

	// tinh time average = total turnaround tinm / total process
	pline(44);
	avg = (float)(sum /num); 
	printf(" \nTotal Turnaround Time : %d", sum);
	printf("\nAverage Turnaround Time: %.3f", avg);
}

int main() {
	int num;
	float avg = 0.0;
	process p[10];

	//Goi ham nhap du lieu
	InputData(p, &num);
	//Goi ham sap xep lai cac tien trinh
	SortSequence(p,num);

	// Tinh Time Turnaround , timeturnaround = waitting + burst
	// for (int i = 0; i < num; i++) {
	// 	p[i].turnaround = p[i].burst + wait ; 
	// 	wait = p[i].turnaround;
	// 	sum += p[i].turnaround;
	// }
	
	// //In ra bang tien trinh
	// pline(44);
	// printf("PID\tArrival\tBurst\tTurnaround\n");
	// pline(44);
	// for (int i = 0; i < num; i++) {
	// 	printf("%d\t%d\t%d\t%d\n", p[i].pid, p[i].arrival, p[i].burst, p[i].turnaround);
	// }

	// // tinh time average = total turnaround tinm / total process
	// pline(44);
	// avg = (float)(sum /num); 
	// printf(" \nTotal Turnaround Time : %d", sum);
	// printf("\nAverage Turnaround Time: %.3f", avg);
	CalculateTurnAround(p, num);

	return 0;
}