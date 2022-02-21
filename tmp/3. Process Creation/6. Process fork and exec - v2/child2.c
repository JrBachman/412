//======================================================================================
//	This is the old code to be run by the first child process, but now written
//	as a separate program. 
//
//	Jean-Yves Hervé, 	University of Rhode Island
//						Department of Computer Science and Statistics
//	2018-02-15
//======================================================================================

#include <stdio.h> 
#include <stdlib.h>
#include <sys/types.h> 
#include <unistd.h> 

int main(int argc, char * argv[] )
{
	printf("In second child process\tPID %6d  PPID %6d  UID %5d  GID %6d\n",
			getpid(), getppid(), getuid(), getpgid(0));
		
	usleep(1000000);
	printf("Child 2 finishes\n");

	//	Terminates execution by returning a value 
	//	(should be the result of some computations) to the parent process
	exit(57);
}

