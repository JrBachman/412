//	This little program computes and prints out the sum of its arguments,
//	which are expected to be integers (no testing in this sample)
//	
//	The code of the proc_exec.c program is hard-coded to expect this
//	little program to be built with the executable name sum
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
	printf("In sum process PID %6d  PPID %6d  UID %5d  GID %6d\n",
			getpid(), getppid(), getuid(), getpgid(0));

	//	compute the sum
	int sum = 0;
	for (int k=1; k<argc; k++)
	{
		int val;
		//	convert the argument to an int.  sscanf returns the number
		//	of values that were successfully read.  Here we only try to read
		//	one, so value == 1 --> success,  0 --> failure (not an int)
		if (sscanf(argv[k], "%d", &val) == 1)
		{
			sum += val;
		}
		else
		{
			//	error handling
		}
	}

	//	slow down a bit for things to look nice in the terminal
	usleep(4000000);

	//	I print out the result (for use in the terminal) *and* also return it (to be caught
	//	by a waitpid system call.
	printf("sum process PID %6d finishes\n", getpid());

	return sum;
}
