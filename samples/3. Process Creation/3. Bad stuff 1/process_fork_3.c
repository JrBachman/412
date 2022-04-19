//=======================================================================================
//	Processes created with a loop.  This shows that if you are not careful, your process
//	is going to end up with a complex family of processes, with children, grandchildren,
//	grand-grandchildrend, etc.  

//	Look at the value of PPID reported by some of the children, grandchildren, etc.
//	Then change the SLEEP_VERSION setting below and compare.
//	What do you think is going on?
//
//	Jean-Yves Hervé, 	University of Rhode Island
//						Department of Computer Science and Statistics
//	2017-10-10, revised 2018-02-15, 2021-10-10
//=======================================================================================


#include <stdio.h> 
#include <sys/types.h>  
#include <unistd.h>
#include <stdlib.h>

#define		SLEEP_VERSION_ONE	1
#define		SLEEP_VERSION_TWO	2
//
//	pick one of the two constants defined above
#define		SLEEP_VERSION	SLEEP_VERSION_ONE

int main(int argc, char * argv[] )
{
	//	We are going to increment this counter after each fork() in the child process 
	//	branch of the code, to make sure that the child process is one generation more than
	//	its parent.
	int generation = 0;
	
	//	Same as we did in process_0.c, you will probably need to add usleep() calls
	//	to this code for it to report the proper process family.
	
	printf("\n\ninitial process PID: %6d  PPID: %6d  UID: %5d  GID: %6d\n\n",
			getpid(), getppid(), getuid(), getpgid(0)); 
	
	//	Buggy code that was meant to create only 4 child processes
	for (unsigned int i=0; i<4; i++)
	{
		int p = fork( );
		if (p < 0)
		{	
			for (int k=0; k<generation; k++)
				printf("    ");
			printf("Gen %d process  PID: %6d  PPID: %6d  UID: %5d  GID: %6d  failed creating child process\n",
					generation, getpid(), getppid(), getuid(), getpgid(0));
		}	
		else if (p != 0) 
		{
			for (int k=0; k<generation; k++)
				printf("    ");
			printf("Gen %d process  PID: %6d  PPID: %6d  UID: %5d  GID: %6d  created child process %d\n",
					generation, getpid(), getppid(), getuid(), getpgid(0), p);
		}
		else // p == 0
		{
			generation += 1;
			for (int k=0; k<generation; k++)
				printf("    ");
			printf("In Gen %d process  PID: %6d  PPID: %6d  UID: %5d  GID: %6d\n",
					generation, getpid(), getppid(), getuid(), getpgid(0));
		}
	}

	#if	SLEEP_VERSION == SLEEP_VERSION_ONE

		//	sleep for a while before terminating
		usleep(generation*400000);

	#elif SLEEP_VERSION == SLEEP_VERSION_TWO

		//	sleep for a while before terminating
		usleep((5 - generation)*1000000);

	#else

		#error Invalid choice of SLEEP_VERSION

	#endif

		
	for (int k=0; k<generation; k++)
		printf("    ");
	printf("Gen %d process  PID %6d terminates\n",
			generation, getpid());
	return 0;
}
