//	This little program computes and prints out the max of its arguments,
//	which are expected to be integers (no testing in this sample)
//	
//	The code of the proc_exec.c program is hard-coded to expect this
//	little program to be built with the executable name max
//
//	Jean-Yves Hervé, 	University of Rhode Island
//						Department of Computer Science and Statistics
//	2018-02-15

#include <stdio.h>  
#include <stdlib.h>
#include <sys/types.h> 
#include <unistd.h>

int main(int argc, char * argv[] )
{
	printf("In max process PID %6d  PPID %6d  UID %5d  GID %6d\n",
			getpid(), getppid(), getuid(), getpgid(0));

	//	compute the sum
	int max;
	int val;
	if (sscanf(argv[1], "%d", &val) == 1)
	{
		max = val;
	}
	else
	{
		//	error handling
	}
	
	for (int k=2; k<argc; k++)
	{
		int val;
		//	convert the argument to an int.  sscanf returns the number
		//	of values that were successfully read.  Here we only try to read
		//	one, so value == 1 --> success,  0 --> failure (not an int)
		if (sscanf(argv[k], "%d", &val) == 1)
		{
			if (val > max)
				max = val;
		}
		else
		{
			//	error handling
		}
	}

	//	slow down a bit for things to look nice in the terminal
	usleep(2500000);

	//	I print out the result (for use in the terminal) *and* also return it (to be caught
	//	by a waitpid system call.
	printf("max process PID %6d finishes\n", getpid());

	return max;
}
