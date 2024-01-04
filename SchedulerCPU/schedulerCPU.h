#ifndef _FCFS_
#define _FCFS_
//chú thích, in ra khi cần thiết thay đổi tiến trình
//cập nhật thời gian thực

#define TRUE            1
#define FALSE           0
#define QUEUE_EMPTY     0
#define TRASH_VALUE     0

typedef enum{Trash, Wait, Processing, Done}statusProcess;

typedef struct Process {
	int pid, arrival, burst, burst_remain, await, startpro ,endpro; 
    int flag_in;//tiến trình đã vào
    statusProcess status;
    // pid:             mã của tiến trình
    // arrival:         thời gian vào của tiến trình
    // burst:           thời gian xử lý của tiến trình
    // await:           thời gian chờ đợi của tiến trình
    // startpro:        thời gian bắt đầu của tiến trình
    // endpro:          thời gian kết thúc của tiến trình
}Process_t;

typedef struct 
{
    int Front, Rear, Size;
    int Capacity;
    Process_t* Queue_Arr;
}Queue_Types;

//Func common
void selectAlgorithm(int *select);
void pline(int x);                                                                                                          // hàmvẽ đường kẻ
void mySleep(unsigned int seconds);                                                                                         // hàm dừng nhằm mô phỏng thời gian xử lý tiến trình
Process_t* allocateDynamicQueue(int *num_process,  Queue_Types *Queue, const char *filename);                               // cấp phát bộ nhớ động cho QUEUE
Process_t* allocateDynamicData(int *num_process);
void InitProcessData(Process_t* process, Process_t* processdata, int* num_process, const char *filename);                   // hàm khởi tạo tiến trình
void DisplayResult(Process_t* p, int* num_process, float* acg_await);                                                       // hàm hiển thị cuối dùng

//Func algorithm
void ProcessingQueue(Process_t* process, Process_t *processdata, int* num_process, int* realt, Queue_Types* Queue, int select); // hàm xử lý tiến trình

//@QUEUE Func
int Queue_IsEmpty(Queue_Types *Queue);
int Queue_IsFull(Queue_Types *Queue);
void Queue_PushDataFCFS(Queue_Types *Queue, Process_t *Process, int index);
void Queue_PushDataSJF(Queue_Types *Queue,Process_t *Process, int index);
void Queue_PushDataSRTF(Queue_Types *Queue,Process_t *Process, int index);
void Queue_PopData(Queue_Types *Queue);

#endif //__FCFS_H__