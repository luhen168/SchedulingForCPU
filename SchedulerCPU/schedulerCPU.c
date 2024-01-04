#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include"schedulerCPU.h"

void main(void){
    float sumawait = 0;
    int num_process = 0, realt=0, select; //select là biến lựa chọn thuật toán
    selectAlgorithm(&select);
	Queue_Types* queue_p = (Queue_Types*)malloc(sizeof(Queue_Types));
    Process_t* process_p;
	Process_t* process_p_data;
    process_p = allocateDynamicQueue(&num_process, queue_p,"data.txt");					//Cấp phát bộ nhớ cho các tiến trình
	process_p_data = allocateDynamicData(&num_process);									//Cấp phát bộ nhớ cho mảng lưu đầu ra
    InitProcessData(process_p, process_p_data, &num_process,"data.txt" );				//khởi tạo các tiến trình ban đầu
    ProcessingQueue(process_p, process_p_data, &num_process, &realt, queue_p,select);	//xử lý tiến trình bằng thuật toán  
	DisplayResult(process_p_data, &num_process, &sumawait);								//hiển thị kết quả cuối thu được
    free(process_p_data);
	free(process_p);
	free(queue_p->Queue_Arr);
	free(queue_p);
}

/* Function select Algorithm*/
void selectAlgorithm(int *select) {
    int isValidInput = 0;
    do {
        printf("1. FCFS\n");
        printf("2. SJF\n"); 
        printf("3. SRTF\n"); 
        printf("4. RR\n");
        printf("Vui long chon thuat toan theo so thu tu:\n");

        if (scanf("%d", select) == 1) {
            if (*select >= 1 && *select <= 4) {
                isValidInput = 1;
            } else {
                printf("Nhap sai. Vui long nhap lai.\n");
            }
        } else {
            printf("Nhap sai dinh dang. Vui long nhap lai.\n");
            // Xóa bộ đệm đầu vào nếu có lỗi
            while (getchar() != '\n');
        }
    } while (!isValidInput);
}

/* Function printf line ------*/
void pline(int x) {
	int i;
	for (i = 0; i < x; i++) 
	{
		printf("-");
	} 
	printf("\n");
}

/**
 * @brief Funtion pointer structure to allocate dynamic queue
 * 
 * @param num_process 
 * @param Queue 
 * @param filename 
 * @return * Function* 
 */
Process_t* allocateDynamicQueue(int *num_process,  Queue_Types *Queue, const char *filename)
{
 	// printf("Nhap so luong tien trinh: ");
	// scanf("%d", num_process);
    // Đọc số lượng tiến trình từ file
	FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Khong the mo file.\n");
        exit(EXIT_FAILURE);
    }
    if (fscanf(file, "%d", num_process) != 1)
    {
        fprintf(stderr, "Khong the doc so luong tien trinh tu file.\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }
	//ProcessTemp
    Process_t* arr1 = (Process_t*)malloc((*num_process) * sizeof(Process_t));    // ???? 
	//Queue
    Process_t* arr3 = (Process_t*)malloc((*num_process) * sizeof(Process_t));	 // ????
	//khởi tạo thông số cho Queue
    Queue->Front = 0;
    Queue->Rear = -1;
    Queue->Size = *num_process;//sai ý nghĩa tên biến
    Queue->Capacity = 0;//sai tên biến
    Queue->Queue_Arr = arr3;
	return arr1;
}

/**
 * @brief Funtion pointer structure to allocate dynamic data
 * 
 * @param num_process 
 * @return Process_t* 
 */
Process_t* allocateDynamicData(int *num_process)
{
	Process_t* temp = (Process_t*)malloc((*num_process) * sizeof(Process_t));
	return temp;
}

/**
 * @brief Function to init process data, read data from file and save each process into struct
 * 
 * @param process      //  ????
 * @param processdata  //  ????
 * @param num_process 
 * @param filename 
 */
void InitProcessData(Process_t* process, Process_t* processdata, int* num_process, const char *filename)
{
    //@nhập thông tin thời gian đến và thời gian xử lý
	FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("khong the mo tep");
    }
    // Bỏ qua dòng đầu tiên
    char firstLine[256];
    fgets(firstLine, sizeof(firstLine), file);
	// đọc từ dòng thứ 2
    int temp1, temp2, pos;
	pos = 0;
	printf("breakpoint\n");
    while (fscanf(file, "%d %d", &temp1, &temp2) == 2) {
		process[pos].arrival = temp1;
		process[pos].burst = temp2;
		process[pos].pid = pos+1;
		printf("Process %d - Arrival: %d - Burst: %d\n", process[pos].pid, process[pos].arrival,process[pos].burst);
		pos++;
    }
    fclose(file);
    
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

void mySleep(unsigned int seconds) {
    clock_t start_time = clock(); 					// Lấy thời gian bắt đầu
    clock_t delay = seconds * CLOCKS_PER_SEC; 		// Chuyển giây thành đơn vị clock
    while (clock() < start_time + delay) {			// Đợi cho đến khi thời gian đã trôi qua đủ số giây
    }
}

/**
 * @brief Function to display result 
 * 
 * @param p 			// ???? not use in function
 * @param num_process 
 * @param acg_await 
 */
void DisplayResult(Process_t* p, int* num_process, float* acg_await)
{
	for (int i = 0; i < *num_process; i++) 
	{
		*acg_await += (float)p[i].await;
	}
	printf("\nTotal Await Time : %.3f", *acg_await);
	printf("\nAverage Await Time: %.3f", (*acg_await)/(*num_process));
}


/*------------------------------------------------@QUEUE------------------------------------------------*/
/**
 * @brief Function check Queue isEmpty
 * 
 * @param Queue 
 * @return int 
 */
int Queue_IsEmpty(Queue_Types *Queue)  //size?????
{
    int key = FALSE;
    if(Queue->Capacity == QUEUE_EMPTY)
    {
        key = TRUE;
    }
    return key;
}

/**
 * @brief Function check Queue isFull
 * 
 * @param Queue 
 * @return int 
 */
int Queue_IsFull(Queue_Types *Queue)  //size == capacity
{
    int key = FALSE;
    if(Queue->Capacity == Queue->Size)
    {
        key = TRUE; 
    }
    return key;
}

/**
 * @brief Function Queue for FCFS
 * 
 * @param Queue 
 * @param Process 
 * @param index 
 */
void Queue_PushDataFCFS(Queue_Types *Queue,Process_t *Process, int index)
{
    if(!Queue_IsFull(Queue))
    {
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

/**
 * @brief Function Queue for SJF Non-Preemptive
 * 
 * @param Queue 
 * @param Process 
 * @param index 
 */
void Queue_PushDataSJF(Queue_Types *Queue,Process_t *Process, int index)
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
		//sắp xếp lại hàng đợi theo thời gian thực thi
		for (int i = Queue->Front + 1; i <= Queue->Rear; i++) {
			for (int j = i + 1; j <= Queue->Rear; j++) {
				if (Queue->Queue_Arr[i].burst > Queue->Queue_Arr[j].burst) 
				{
					// Hoán đổi vị trí nếu burst time của tiến trình i lớn hơn tiến trình j
					Process_t temp = Queue->Queue_Arr[i];
					Queue->Queue_Arr[i] = Queue->Queue_Arr[j];
					Queue->Queue_Arr[j] = temp;
				}
			}
		}
		printf("da sap xep xong queue\n");
    }
}

/**
 * @brief Function Queue for SRTF
 * 
 * @param Queue 
 * @param Process 
 * @param index 
 */
void Queue_PushDataSRTF(Queue_Types *Queue,Process_t *Process, int index)
{
    if(!Queue_IsFull(Queue))
    {
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
		//sắp xếp lại thứ tự xử lý theo thời gian thực thi còn lại
        int current_index = Queue->Rear;
        while (current_index > Queue->Front && 
               Queue->Queue_Arr[current_index].burst_remain < Queue->Queue_Arr[current_index - 1].burst_remain)
        {
            // Swap
            Process_t temp = Queue->Queue_Arr[current_index];
            Queue->Queue_Arr[current_index] = Queue->Queue_Arr[current_index - 1];
            Queue->Queue_Arr[current_index - 1] = temp;
            Queue->Queue_Arr[current_index].status = Wait;
			Queue->Queue_Arr[Queue->Front].status = Processing;
            current_index--;
        }
		printf("da sap xep xong queue\n");
    }
}

/**
 * @brief Function Queue popdata
 * 
 * @param Queue 
 */
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
    }else//nếu hàng đợi rỗng, reset Font vs Rear
	{
		Queue->Front = 0;
		Queue->Rear = -1;
	}
} 

/**
 * @brief 
 * 
 * @param Queue 
 */
void Queue_MoveToFront(Queue_Types *Queue)
{
    if (Queue_IsEmpty(Queue) || Queue->Front == Queue->Rear)
    {
        // Hàng đợi trống hoặc chỉ có một tiến trình
        return;
    }

    // Lưu trữ thông tin của tiến trình đầu tiên
    Process_t first_process = Queue->Queue_Arr[Queue->Front];

    // Di chuyển lần lượt các tiến trình từ phần tử thứ 2 đến cuối lên trên
    for (int i = Queue->Front + 1; i <= Queue->Rear; i++)
    {
        Queue->Queue_Arr[i - 1] = Queue->Queue_Arr[i];
    }

    // Đặt tiến trình đầu tiên vào cuối hàng đợi
	Queue->Queue_Arr[Queue->Front].status = Processing;
    Queue->Queue_Arr[Queue->Rear] = first_process;
	Queue->Queue_Arr[Queue->Rear].status = Wait;
}

/**
 * @brief Function to show process queue when select algorithm 
 * 
 * @param process 
 * @param processdata 
 * @param num_process 
 * @param realt 
 * @param Queue 
 * @param select 
 */
void ProcessingQueue(Process_t* process, Process_t *processdata, int* num_process, int* realt, Queue_Types* Queue, int select)
{
    //dành riêng cho thuat toan RR
    int T_quantum, dive_time_quantum;
	dive_time_quantum = *realt;
    if(select == 4){
		printf("Nhap T_quantum: \n");
		scanf("%d",&T_quantum);
    }
	//SỬA PHẦN NÀY LÀ CHÍNH
	int check_first_in = 1;					//kiểm soát process vào đầu tiên
	int index_process_data_in = 0;			//lưu các process đã vào QUEUE - biến này vô nghĩa không dùng -))
	int index_process_data_out = 0;			//lưu các process đã ra QUEUE
	while (1)
	{
		if((dive_time_quantum)%T_quantum==0 && dive_time_quantum != 0 && select == 4){
			Queue_MoveToFront(Queue);
		}        
		//kiểm tra ProcessData: process vào thì đẩy vào QUEUE và hiển thị thông báo mới
		for (int i = 0; i < *num_process; i++)
		{
			if((process+i)->arrival == *realt && ((process+i)->status != Processing) && ((process+i)->status != Done)) 
			{

				check_first_in = 0;
                switch (select)
                {
                case 1:
				    Queue_PushDataFCFS(Queue, process, i);
                    break;
                case 2:
                    Queue_PushDataSJF(Queue, process, i);
                    break;
                case 3:
                    Queue_PushDataSRTF(Queue, process, i);
                    break;
				case 4:
				    Queue_PushDataFCFS(Queue, process, i);
					break;
                default:
                    break;
                }
				printf("PROCESS %d DUOC THEM VAO QUEUE TAI GIAY THU' %d\n", (process+i)->pid, *realt);
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
        if(select == 4){dive_time_quantum++;} //dành riêng cho RR
		// mySleep(1);
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
            if(select == 4){dive_time_quantum = 0;}
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
		if(index_process_data_out == *num_process )
		{
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