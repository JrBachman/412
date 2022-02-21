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
#include <unistd.h>

int main(int argc, char * argv[] )
{
	printf("Sum program running with \"name\": %s and pid: %d\n", argv[0], getpid());
	
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
	
	//	I print out the result (for use in the terminal) *and* also return it (to be caught
	//	by a waitpid system call.
	printf("Sum = %d\n", sum);
	return sum;
}
