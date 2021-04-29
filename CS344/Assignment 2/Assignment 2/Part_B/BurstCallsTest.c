#include "types.h"
#include "stat.h"
#include "user.h"

int main()
{
        printf(1,"Setting the burst time of current process to 15\n");
        set_burst_time(15,getpid());
        printf(1,"Fetched Burst Time is");
        printf(1, " %d\n", get_burst_time());
	exit();
}
