#ifndef _FCFS_
#define _FCFS_

typedef struct Process {
	int pid, arrival, burst, await, startpro,endpro; 
    // pid:             mã của tiến trình
    // arrival:         thời gian vào của tiến trình
    // burst:           thời gian xử lý của tiến trình
    // await:           thời gian chờ đợi của tiến trình
    // startpro:        thời gian bắt đầu của tiến trình
    // endpro:          thời gian kết thúc của tiến trình
}Process_t;
void pline(int x);                                                      // hàmvẽ đường kẻ
void mySleep(unsigned int seconds);                                     // hàm dừng mục đích mô phỏng thời gian xử lý tiến trình
Process_t* allocateDynamicQueue(int *num_process);                      // cấp phát bộ nhớ động cho QUEUE
void InitQueue(Process_t* process, int* num_process);                   // hàm khởi tạo tiến trình
void Processing(Process_t* process, int* num_process, int* realt);      // hàm xử lý tiến trình
void DisplayResult(Process_t* p, int* num_process, float* acg_await);   // hàm hiển thị cuối dùng
#endif //__FCFS_H__
