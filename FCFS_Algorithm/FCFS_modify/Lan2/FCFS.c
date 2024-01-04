#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include"FCFS.h"

void main(void){
    float sumawait = 0;
    int num_process = 0, realt=0;
	Queue_Types* queue_p = (Queue_Types*)malloc(sizeof(Queue_Types));			//Cấp phát bộ nhớ cho hàng đợi 
    Process_t* process_p;
	Process_t* process_p_data;

    process_p = allocateDynamicQueue(&num_process, queue_p);					//Cấp phát bộ nhớ cho các tiến trình
	process_p_data = allocateDynamicData(&num_process);							//Cấp phát bộ nhớ cho mảng lưu đầu ra
    InitProcessData(process_p, process_p_data, &num_process);					//khởi tạo các tiến trình ban đầu
    ProcessingQueue(process_p, process_p_data, &num_process, &realt, queue_p);	//xử lý tiến trình bằng thuật toán
	DisplayResult(process_p_data, &num_process, &sumawait);						//hiển thị kết quả cuối thu được
    free(process_p_data);
	free(process_p);
	free(queue_p);
}

void pline(int x) {
	int i;
	for (i = 0; i < x; i++) 
	{
		printf("-");
	}
	printf("\n");
}

//Hàm cấp phát hàng đợi động 
Process_t* allocateDynamicQueue(int *num_process,  Queue_Types *Queue)
//chỉ làm cấp phát-> fix, có thể dùng lại hàm để cấp phát tiến trình
//
{
 	printf("Nhap so luong tien trinh: ");
	scanf("%d", num_process);
	//ProcessTemp
    Process_t* arr1 = (Process_t*)malloc((*num_process) * sizeof(Process_t));
	//Queue
    Process_t* arr3 = (Process_t*)malloc((*num_process) * sizeof(Process_t));
	//khởi tạo thông số cho Queue
    Queue->Front = 0;				//chỉ số đầu của queue
    Queue->Rear = -1;
    Queue->Size = *num_process;		//là kích cỡ có thể thay đổi 
    Queue->Capacity = 0;			//thường là dung lượng tối đa của queue
    Queue->Queue_Arr = arr3;
	return arr1;
}

// Hàm cấp phát dữ liệu động 
Process_t* allocateDynamicData(int *num_process)
{
	Process_t* temp = (Process_t*)malloc((*num_process) * sizeof(Process_t));
	return temp;
}

// Hàm khởi tạo dữ liệu tiến trình 
void InitProcessData(Process_t* process, Process_t* processdata, int* num_process)
{
    //@nhập thông tin thời gian đến và thời gian xử lý
	for (int i = 0; i < *num_process; i++) 
	{
		printf("Nhap thoi gian den va thoi gian xu ly Process %d: \n", i + 1);
        printf("Arrival: ");
        scanf("%d",&process[i].arrival);
        printf("Burst: ");
        scanf("%d",&process[i].burst);
		process[i].pid = i + 1;//cập nhật mã của tiến trình 
	}
    pline(100);
    pline(100);
    //@khởi tạo thông số khác cho các tiến trình được nhập
	for(int i = 0; i < *num_process; i++)
	{
		process[i].await = processdata[i].await = 0;
		process[i].startpro = processdata[i].startpro = 0;
		process[i].endpro = processdata[i].endpro = 0;
		process[i].flag_in = processdata[i].flag_in = 0;
		process[i].status = processdata[i].status = Trash;
		processdata[i].arrival = 0;
		processdata[i].burst = 0;
		processdata[i].pid = 0;

	}
}

// Hàm xử lí hàng đợi là FCFS
void ProcessingQueue(Process_t* process, Process_t *processdata, int* num_process, int* realt, Queue_Types* Queue)
{
	//SỬA PHẦN NÀY LÀ CHÍNH
	int check_first_in = 1;					//kiểm soát process vào đầu tiên
	int index_process_data_in = 0;			//lưu các process đã vào QUEUE
	int index_process_data_out = 0;			//lưu các process đã ra QUEUE
	while (1)
	{
		//kiểm tra ProcessData: process vào thì đẩy vào QUEUE và hiển thị thông báo mới
		for (int i = 0; i < *num_process; i++)
		{
			if((process+i)->arrival == *realt && (process+i)->status == Trash)
			{
				check_first_in = 0;
				Queue_PushData(Queue, process, index_process_data_in);
				printf("PROCESS %d DUOC THEM VAO QUEUE TAI GIAY THU' %d\n", index_process_data_in+1, *realt);
				index_process_data_in++;
				printf("TRANG THAI QUEUE:  ");
				for (int i = Queue->Rear; i >  Queue->Front; i--)
				{
					printf("Process %d -> ",(Queue->Queue_Arr+i)->pid);
				}
				printf("Process %d\n",(Queue->Queue_Arr[Queue->Front]).pid);
			}
		}
		//nếu queue ban đầu rỗng in ra thông số 0 - hàm này chỉ dùng đến khi có process vào đầu tiên
		if(Queue_IsEmpty(Queue) && check_first_in == 1)
		{
			pline(100);
			printf("PID\tArrival\tBurst\tStartpro\tEndpro\tAwait\tRealTime\tStatus\n");
			pline(100);	
			printf("0\t0\t0\t0\t\t0\t0\t%d\t\tNULL\n",*realt);
		}
		//xử lý process nằm tại vị trí đầu của QUEUE
		if(Queue->Queue_Arr->flag_in == 0 )
		{
			Queue->Queue_Arr->startpro =  *realt;
			Queue->Queue_Arr->flag_in = 1;
			Queue->Queue_Arr->status = Processing;
		}
		Queue->Queue_Arr->burst_remain--;

		//-----------------@hiển thị trạng thái các tiến trình đã và đang xử lý---------------------
		if (check_first_in == 0)
		{
			pline(100);
			printf("PID\tArrival\tBurst\tStartpro\tEndpro\tAwait\tRealTime\tStatus\n");
			pline(100);
		}
		
		//hiển thị process đã xử lý xong - lưu trong ProcessData
		for(int i = 0;i < *num_process ;i++){
			if((processdata+i)->status == Done)
			{
				printf("%d\t%d\t%d\t%d\t\t%d\t%d\t%d\t\tDone\n", processdata[i].pid, processdata[i].arrival, processdata[i].burst,processdata[i].startpro,processdata[i].endpro,processdata[i].await,*realt);
			}
		}
		//hiển thị process đang lưu tại QUEUE
		if(!Queue_IsEmpty(Queue))
		{
			printf("%d\t%d\t%d\t%d\t\t%d\t%d\t%d\t\tProcessing\n",(Queue->Queue_Arr)->pid, (Queue->Queue_Arr)->arrival, (Queue->Queue_Arr)->burst, (Queue->Queue_Arr)->startpro, (Queue->Queue_Arr)->endpro, (Queue->Queue_Arr)->await, *realt);
			for(int i = Queue->Front+1; i <= Queue->Rear; i++)
			{
				printf("%d\t%d\t%d\t%d\t\t%d\t%d\t%d\t\tWait\n",(Queue->Queue_Arr+i)->pid, (Queue->Queue_Arr+i)->arrival, (Queue->Queue_Arr+i)->burst, (Queue->Queue_Arr+i)->startpro, (Queue->Queue_Arr+i)->endpro, (Queue->Queue_Arr+i)->await, *realt);			
			}
		}
		//-------------------@cập nhật await cho các tiến trình đang đợi---------------------------
		for(int i = Queue->Front+1; i <= Queue->Rear; i++)
		{
			(Queue->Queue_Arr+i)->await++;
		}
		(*realt)++;
		mySleep(1);
		//--------------------@khi xử lý xong tiến trình hiện tại...--------------------------------
		if(Queue->Queue_Arr->burst_remain == 0)
		{
			//lưu process đã xử lý xong vào ProcessData
			(processdata+index_process_data_out)->pid = Queue->Queue_Arr->pid;
			(processdata+index_process_data_out)->arrival = Queue->Queue_Arr->arrival;
			(processdata+index_process_data_out)->burst = Queue->Queue_Arr->burst;
			(processdata+index_process_data_out)->startpro = Queue->Queue_Arr->startpro;
			(processdata+index_process_data_out)->endpro = *realt;
			(processdata+index_process_data_out)->await = Queue->Queue_Arr->await;
			(processdata+index_process_data_out)->status = Done;
			pline(100);
			printf("DA XU LY XONG PROCESS %d TAI GIAY THU' %d\n",Queue->Queue_Arr->pid, *realt);
			Queue_PopData(Queue);
			index_process_data_out++;
			printf("TRANG THAI QUEUE:  ");
			if(!Queue_IsEmpty(Queue))
			{
				for (int i = Queue->Rear; i >  Queue->Front; i--)
				{
					printf("Process %d -> ",(Queue->Queue_Arr+i)->pid);
				}
				printf("Process %d\n",(Queue->Queue_Arr[Queue->Front]).pid);
			}else
			{
				printf("Empty\n");
			}
		}
		if(index_process_data_out == *num_process) 
		{
			// printf("breakpoint\n");
			break;
		}		
	}
	pline(100);
	printf("\t\t\t\t\tRESULT\n");
	pline(100);
	printf("PID\tArrival\tBurst\tStartpro\tEndpro\tAwait\tRealTime\tStatus\n");
	pline(100);
	for(int i = 0;i < *num_process ;i++)
	{
		if ((processdata+i)->status = Done)
		{
			printf("%d\t%d\t%d\t%d\t\t%d\t%d\t%d\t\tDone\n", processdata[i].pid, processdata[i].arrival, processdata[i].burst,processdata[i].startpro,processdata[i].endpro,processdata[i].await,*realt);
		}
	}
}

void mySleep(unsigned int seconds) {
    clock_t start_time = clock(); 					// Lấy thời gian bắt đầu
    clock_t delay = seconds * CLOCKS_PER_SEC; 		// Chuyển giây thành đơn vị clock
    while (clock() < start_time + delay) {			// Đợi cho đến khi thời gian đã trôi qua đủ số giây
    }
}

void DisplayResult(Process_t* p, int* num_process, float* acg_await)
{
	for (int i = 0; i < *num_process; i++) 
	{
		*acg_await += (float)p[i].await;
	}
	printf(" \nTotal Await Time : %.3f", *acg_await);
	printf("\nAverage Await Time: %.3f", (*acg_await)/(*num_process));
}


/*------------------------------------------------@QUEUE------------------------------------------------*/
int Queue_IsEmpty(Queue_Types *Queue)
{
    int key = FALSE;
    if(Queue->Capacity == QUEUE_EMPTY)
    {
        key = TRUE;
    }
    return key;
}

int Queue_IsFull(Queue_Types *Queue)
{
    int key = FALSE;
    if(Queue->Capacity == Queue->Size)
    {
        key = TRUE; 
    }
    return key;
}

void Queue_PushData(Queue_Types *Queue,Process_t *Process, int index)
{
    if(!Queue_IsFull(Queue))
    {
        //int pid, arrival, burst, await, startpro,endpro; 
        //cập nhật giá trị process vào QUEUE
        Queue->Queue_Arr[Queue->Rear+1].pid = (Process+index)->pid;
        Queue->Queue_Arr[Queue->Rear+1].arrival = (Process+index)->arrival;
        Queue->Queue_Arr[Queue->Rear+1].burst = Queue->Queue_Arr[Queue->Rear+1].burst_remain = (Process+index)->burst;
        Queue->Queue_Arr[Queue->Rear+1].await = (Process+index)->await;
        Queue->Queue_Arr[Queue->Rear+1].startpro = (Process+index)->startpro;
        Queue->Queue_Arr[Queue->Rear+1].endpro = (Process+index)->endpro;
        Queue->Queue_Arr[Queue->Rear+1].flag_in = (Process+index)->flag_in;
        Queue->Queue_Arr[Queue->Rear+1].status = (Process+index)->status = Wait;

        Queue->Rear++;
        Queue->Capacity++;
    }
}

void Queue_PopData(Queue_Types *Queue)
{
    if(!Queue_IsEmpty(Queue))
    {
        Queue->Front++;
		Queue->Capacity--;//bỏ đi 1 process đã xử lý xong
	}
	if(!Queue_IsEmpty(Queue))
	{
        //đẩy lần lượt các process lên trước nếu hàm đợi còn process
        for (int i = Queue->Front; i <= Queue->Rear; i++)
        {
            ((Queue->Queue_Arr)+(i-Queue->Front))->pid = ((Queue->Queue_Arr)+(i))->pid;
            ((Queue->Queue_Arr)+(i-Queue->Front))->arrival = ((Queue->Queue_Arr)+(i))->arrival;
            ((Queue->Queue_Arr)+(i-Queue->Front))->burst = ((Queue->Queue_Arr)+(i))->burst;
            ((Queue->Queue_Arr)+(i-Queue->Front))->burst_remain = ((Queue->Queue_Arr)+(i))->burst_remain;
            ((Queue->Queue_Arr)+(i-Queue->Front))->await = ((Queue->Queue_Arr)+(i))->await;
            ((Queue->Queue_Arr)+(i-Queue->Front))->startpro = ((Queue->Queue_Arr)+(i))->startpro;
            ((Queue->Queue_Arr)+(i-Queue->Front))->endpro = ((Queue->Queue_Arr)+(i))->endpro;
            ((Queue->Queue_Arr)+(i-Queue->Front))->flag_in = ((Queue->Queue_Arr)+(i))->flag_in;
            ((Queue->Queue_Arr)+(i-Queue->Front))->status = ((Queue->Queue_Arr)+(i))->status;
        }
		Queue->Front = 0;
        Queue->Rear = Queue->Capacity-1;
        //reset dữ liệu cho các vị trí queue không dùng nữa --> có thể không cần thiết cho FCFS, thuật toán khác dùng
        for (int i = Queue->Capacity; i < Queue->Size; i++)
        {
            ((Queue->Queue_Arr)+(i))->pid = 0;
            ((Queue->Queue_Arr)+(i))->arrival = 0;
            ((Queue->Queue_Arr)+(i))->burst = 0;
            ((Queue->Queue_Arr)+(i))->burst_remain = 0;
            ((Queue->Queue_Arr)+(i))->await = 0;
            ((Queue->Queue_Arr)+(i))->startpro = 0;
            ((Queue->Queue_Arr)+(i))->endpro = 0;
            ((Queue->Queue_Arr)+(i))->flag_in = 0;
            ((Queue->Queue_Arr)+(i))->status = 0;
        }
    }else//nếu hàng đợi rỗng, reset Font vs Rear
	{
		Queue->Front = 0;
		Queue->Rear = -1;
	}
} 