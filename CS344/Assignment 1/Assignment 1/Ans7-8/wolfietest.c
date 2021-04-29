#include "types.h"
#include "stat.h"
#include "user.h"

int main()
{
	//sets buffer to the address of the newly alloted memory
	char *buffer = malloc(10000);
	//call to wolfie
	if (wolfie(buffer,10000)!=-1)
		//prints the buffer 
		printf(1,"%s\n",buffer);
	else printf(1,"Unable to print");
	exit();
}
