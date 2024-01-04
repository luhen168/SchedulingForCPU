#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include"FCFS.h"

void main(void){
    float sumawait = 0;
    int num_process = 0, realt=0;
    Process_t* process_p;
    process_p = allocateDynamicQueue(&num_process);		//Cấp phát bộ nhớ cho các tiến trình
    InitQueue(process_p, &num_process);					//khởi tạo các tiến trình ban đầu
    Processing(process_p, &num_process, &realt);		//xử lý tiến trình bằng thuật toán
	DisplayResult(process_p, &num_process, &sumawait);	//hiển thị kết quả cuối thu được
    free(process_p);
}

void pline(int x) {
	int i;
	for (i = 0; i < x; i++) {
		printf("-");
	}
	printf("\n");
}

Process_t* allocateDynamicQueue(int *num_process){
 	printf("Nhap so luong tien trinh: ");
	scanf("%d", num_process);
    Process_t* arr = (Process_t*)malloc((*num_process) * sizeof(Process_t));//cấp phát bộ nhớ động cho tiến trình
    return arr;
}

void InitQueue(Process_t* process, int* num_process){
    //@nhập thông tin thời gian đến và thời gian xử lý
	for (int i = 0; i < *num_process; i++) {
		printf("Nhap thoi gian den va thoi gian xu ly Process %d: \n", i + 1);
        printf("Arrival: ");
        scanf("%d",&process[i].arrival);
        printf("Burst: ");
        scanf("%d",&process[i].burst);
		process[i].pid = i + 1;//cập nhật mã của tiến trình 
	}
    pline(100);
    pline(100);
    //@sắp xếp thứ tự xử lý Process
	for (int i = 0; i < *num_process - 1; i++) {
		for (int j = 0; j < *num_process - i - 1; j++) {
			if (process[j].arrival > process[j + 1].arrival) {
				Process_t temp = process[j];
				process[j] = process[j + 1];
				process[j + 1] = temp;
			}
		}
	}

    //@khởi tạo thông số cho các tiến trình
	for(int i = 0; i < *num_process; i++){
		process[i].await=0;
		process[i].endpro =0;
		process[i].startpro=0;
	}
	//@hiển thị Queue đã sắp xếp
	printf("Starting...................\n");
	pline(100);
	pline(100);
	printf("Queue : ");
	for (int i = *num_process - 1; i >= 0; i--){
		if(i>0){
			printf("Process %d  ->  ", process[i].pid);
		}
		else{
			printf("Process %d\n", process[i].pid);
		}
	}
	pline(100);
	pline(100);
}

void Processing(Process_t* process, int* num_process, int* realt){
	int arrivaled = 0; //số tiến trình đã vào queue
	for(int i=0;i<*num_process;i++){
		int status_queue = 1;
		int x_now = i; //vị trí tiến trình đang xử lý hiện tại
		//cần fix từ đây
		while(1){
			if (x_now == 0 && (*realt < process[x_now].arrival) ){
				pline(100);
				printf("PID\tArrival\tBurst\tStartpro\tEndpro\tAwait\tRealTime\n");
				pline(100);	
				printf("0\t0\t0\t0\t\t0\t0\t%d\n",*realt);
				// riêng tiến trình đầu tiên luôn nhận startpro = arrival
				process[x_now].startpro = process[x_now].arrival;
				process[x_now].endpro = process[x_now].startpro + process[x_now].burst;
			}else{
				if(x_now == 0){
				process[x_now].startpro = process[x_now].arrival;
				process[x_now].endpro = process[x_now].startpro + process[x_now].burst;					
				}
				//@hiển thị queue
				if ((status_queue == 1 && *realt >= process[x_now].arrival) ||(status_queue == 1 && process[x_now].arrival == process[x_now-1].arrival))
				{
					pline(100);
					pline(100);
					printf("Process %d dang duoc CPU xu li\n", process[i].pid);
					pline(100);
					pline(100);
					printf("Queue : ");
					for (int j = (*num_process) - 1; j > x_now; j--){
						if(j>x_now+1){
							printf("Process %d -> ", process[j].pid);
						}
						else{
							printf("Process %d\n", process[j].pid);
						}
					}
					status_queue = 0;
				}
				//@cập nhật số tiến trình đã vào 
				for(int i = x_now; i < *num_process; i++){
					if(*realt == process[i].arrival){
						arrivaled++;
					}
				}
				pline(100);
				printf("PID\tArrival\tBurst\tStartpro\tEndpro\tAwait\tRealTime\n");
				pline(100);
				//@hiển thị trạng thái các tiến trình đã và đang xử lý
				for(int i = 0;i < arrivaled ;i++){
					printf("%d\t%d\t%d\t%d\t\t%d\t%d\t%d\n", process[i].pid, process[i].arrival, process[i].burst,process[i].startpro,process[i].endpro,process[i].await,*realt);
				}
				//cập nhật await cho các tiến trình đang đợi
				for (int i = x_now+1; i < arrivaled; i++)
				{
					process[i].await++;
				}
			}
			(*realt)++;
			mySleep(1);
			//@khi xử lý xong tiến trình hiện tại...
			if(*realt == process[x_now].startpro + process[x_now].burst){
				// cập nhật Startpro và Endpro cho tiến trình tiếp theo
				if(*realt >= process[x_now+1].arrival && x_now < *num_process){
				process[x_now+1].startpro = process[x_now].endpro;
				process[x_now+1].endpro = process[x_now+1].startpro + process[x_now+1].burst;						
				}else if (*realt < process[x_now+1].arrival && x_now < *num_process)
				{
				process[x_now+1].startpro = process[x_now+1].arrival;
				process[x_now+1].endpro = process[x_now+1].startpro + process[x_now+1].burst;
				}
				break;//thoát vòng while chuyển sang tiến trình tiếp theo
			}
		}
	}
	pline(100);
	printf("PID\tArrival\tBurst\tStartpro\tEndpro\tAwait\tRealTime\n");
	pline(100);
	for(int i = 0;i < *num_process ;i++){
		printf("%d\t%d\t%d\t%d\t\t%d\t%d\t%d\n", process[i].pid, process[i].arrival, process[i].burst,process[i].startpro,process[i].endpro,process[i].await,*realt);
	}	
}

void mySleep(unsigned int seconds) {
    clock_t start_time = clock(); 					// Lấy thời gian bắt đầu
    clock_t delay = seconds * CLOCKS_PER_SEC; 		// Chuyển giây thành đơn vị clock
    while (clock() < start_time + delay) {			// Đợi cho đến khi thời gian đã trôi qua đủ số giây
    }
}

void DisplayResult(Process_t* p, int* num_process, float* acg_await){
	pline(100);
	printf("\n\n\t\t\t\t\tRESULT\n");
	pline(100);
	printf("PID\tArrival\tBurst\tStartpro\tEndpro\t\tAwait\n");
	pline(100);
	for (int i = 0; i < *num_process; i++) {
		printf("%d\t%d\t%d\t%d\t\t%d\t\t%d\n", p[i].pid, p[i].arrival, p[i].burst,p[i].startpro,p[i].endpro,p[i].await);
		*acg_await += (float)p[i].await;
	}
	printf(" \nTotal Await Time : %.3f", *acg_await);
	printf("\nAverage Await Time: %.3f", (*acg_await)/(*num_process));
}