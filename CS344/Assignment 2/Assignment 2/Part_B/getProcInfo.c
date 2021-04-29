#include "types.h"
#include "stat.h"
#include "user.h"
//A function to introduce delay in the program so as to increase the
//number of context switches
void delay(int count)
{
    int *data;
    data = (int *)malloc(sizeof(int) * 1024 * 10);
    for (int i = 0; i < count; i++){
        for (int k = 0; k < 10000; k++)
            for (int j = 0; j < 1024; j++)
                data[j]++;
    }
}
int main()
{
    struct processInfo* temp[10];
    int pids[10];
    for(int i=0;i<10;i++){
        temp[i]=malloc(100);
        int pid_of_child=fork();
        if(pid_of_child==0){
            set_burst_time(i,getpid());
            delay(20);
            exit();
        }
        else pids[i]=pid_of_child;
    }
    for(int i=0;i<10;i++){
        getProcInfo(pids[i],temp[i]);
        int x=wait();
	    printf(1,"PPID : %d, NoCS : %d, size : %d, pid : %x\n",(temp[i]->ppid),temp[i]->numberContextSwitches,temp[i]->psize,x);
    }
    // To test if the function returns -1 on an incorrect pid
    int todemonstrateerror=getProcInfo(-5,temp[0]);
    if(todemonstrateerror==-1){
        printf(1,"The Pid -5 is not present in table");
    }
    exit();
}
