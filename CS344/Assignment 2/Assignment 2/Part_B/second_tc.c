#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main()
{
    int N = 10;
    int pids[N];
    int exitIdOrder[N];
    // To change the burst times change here
    int burstTimes[]={1,2,3,4,5,6,7,8,9,10};
    printf(1, "burst time of parent process = %d\n",get_burst_time());
    printf(1,"The CPU bound processes begin running\n");
    for (int i = 0; i < N; i++)
    {
        int pid_returned_by_fork = fork();
        if (pid_returned_by_fork == 0)
        {
            float x=1;
            //This is for delay
            while(x<10000000)x+=0.25;
            exit();
        }
        else
        {
            // printf(1,"%d %d\n",burstTimes[i],pid_returned_by_fork);
            set_burst_time(burstTimes[i],pid_returned_by_fork);
            pids[i] = pid_returned_by_fork;
        }
    }

    for (int i = 0; i < N; i++)
    {
        exitIdOrder[i] = wait();
    }

    printf(1, "\nChildren along with their pids and burstTimes\n");
    for (int i = 0; i < N; i++)
        printf(1, "Child %d.    pid %d      bursttime %d \n", i, pids[i],burstTimes[i]);

    printf(1, "\nThe Exit order of the children:- \n");
    for (int i = 0; i < N; i++)
        printf(1, "pid %d\n", exitIdOrder[i]);
    printf(1,"The CPU bound processes have ended running\n\n");
    printf(1,"The IO bound processes begin running\n");
    for (int i = 0; i < N; i++)
    {
        int pid_returned_by_fork = fork();
        if (pid_returned_by_fork == 0)
        {   
            int fd=open("testfile.txt",O_RDONLY);
            if(fd<0){
                printf(1,"Unable to open file\n");
                exit();
            }
            char *buffer=malloc(100);
            for(int j=0;j<21100;j++){
                read(fd,buffer,100);
            };
            exit();
        }
        else
        {
            set_burst_time(burstTimes[i],pid_returned_by_fork);
            pids[i] = pid_returned_by_fork;
        }
    }

    for (int i = 0; i < N; i++)
    {
        exitIdOrder[i] = wait();
    }

    printf(1, "\nChildren along with their pids and burstTimes\n");
    for (int i = 0; i < N; i++)
        printf(1, "Child %d.    pid %d      bursttime %d \n", i, pids[i],burstTimes[i]);

    printf(1, "\nThe Exit order of the children \n");
    for (int i = 0; i < N; i++)
        printf(1, "pid %d\n", exitIdOrder[i]);
    printf(1,"The IO bound processes have ended running\n");

    exit();
}
