#include "types.h"
#include "stat.h"
#include "user.h"
//A function to introduce delay in the program so as to increase the
//number of context switches
void delay(int cyles)
{
    int *data;
    data = (int *)malloc(sizeof(int) * 10000);
    for (int i = 0; i < cyles; i++){
        for (int k = 0; k < 10000; k++)
            for (int j = 0; j < 1024; j++)
                data[j]++;
    }
}
int main()
{
    int N=10;
    // To assign burst time change this table
    int toassign[]={10,9,8,7,6,5,4,3,2,1};
    int pids[N],exit_id_order[N];
    set_burst_time(0,getpid());
    for (int i = 0; i < N; i++)
    {
      int priority = toassign[i];

        int pid = fork(); // returns child's pid in parent process and 0 in child process
        if (pid == 0){
            set_burst_time(priority,getpid());
            //Delay to simulate actual burst time
            delay(21);
            exit();
        }
        else if (pid > 0){
            pids[i] = pid;
        }
    }
    printf(1, "Burst Time of parent process = %d\n", get_burst_time());
    for (int i = 0; i < N; i++){
        // delay(30);
        exit_id_order[i] = wait();
    }
    printf(1, "\nChildren Entry Order is:\n");
    for (int i = 0; i < N; i++)
        printf(1, "pid %d and Burst Time is %d\n", pids[i],toassign[i]);

    printf(1, "\nChildren Exit order is:\n");
    for (int i = 0; i < N; i++)
        printf(1, "pid %d\n", exit_id_order[i]);
    exit();
}
