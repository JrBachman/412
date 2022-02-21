//	This program will run for a long time, while writing to the standard output.
//
//	Jean-Yves Hervé, 	University of Rhode Island
//						Department of Computer Science and Statistics
//	2018-10-09

#include <stdio.h> 
#include <stdlib.h>
#include <sys/types.h> 
#include <unistd.h>
#include <time.h>
 
int main(int argc, char * argv[] )
{
	//-------------------------------------------------------
	//	Number and range of random elements to generate
	//-------------------------------------------------------
	const unsigned int NUM_VALUES = 7;
	const int 	MIN_VAL = -100,
				MAX_VAL = +100;
	
	//-------------------------------------------------------
	//	Min and max number of iterations of the main loop
	//-------------------------------------------------------
	const unsigned int	MIN_NUM_ITERATIONS = 20,
						MAX_NUM_ITERATIONS = 40;

	//-------------------------------------------------------
	//	Sleep time between iterations (in microseconds)
	//-------------------------------------------------------
	const useconds_t SLEEP_TIME = 2000000;

	//	who am I?
	const pid_t	pid = getpid();
	//	who is my parent process?	
	const pid_t ppid = getppid();

	//	Seed the random generator
	srand((unsigned int) time(NULL));
		
	//	How many times we will be going through the main loop
	const int numIterations = (unsigned int) (rand() % (MAX_NUM_ITERATIONS - MIN_NUM_ITERATIONS + 1) + 
												MIN_NUM_ITERATIONS);

	//	main loop of the program
	for (unsigned k=0; k<numIterations; k++)
	{
		//	Note that if I create many processes all trying to print to the standard
		//	output at the same time, this may result in mixed output.
		printf("Process %u (parent process %u) in iteration %u out of %u.\n\tSum of ",
				pid, ppid, k, numIterations);
		//	Compute the sum of NUM_VALUES random numbers between 0 and 100
		int sum = 0;
		for(unsigned int i=0; i<NUM_VALUES; i++)
		{
			int val = (int)(rand() % (MAX_VAL - MIN_VAL+1) + MIN_VAL);
			if (i != numIterations-1)
				printf("%d, ", val);
			else
				printf("%d", val);

			sum += val;
		}
		printf(" = %d\n", sum);
		
		//	Now sleep for the set amount.
		usleep(SLEEP_TIME);
	}
	
	//	I print out the result (for use in the terminal) *and* also return it (to be caught
	//	by a waitpid system call.
	printf("Process %u (parent process %u) terminating.\n", pid, ppid);
	 
	return 0;
}
