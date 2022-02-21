//======================================================================================
//	This is a revised version of process_fork_exec_3.c in this series.  It demonstrates 
//	a very common mistake that students make every semester in fork-exec assignments,
//	and again later in the semester in threading assignments.
//
//	Look first at the code and see if you can figure out what is wrong and why.
//	Then build the code and run it to verify.
//	Really, make the effort to look at the code first, because when you run it the
//	problem is obvious and you can kid yourself that "oh sure, I would have seen it 
//	if I had bothered to look at the code first."
//
//	As before, first build the executable for the 2 child programs
//		gcc -Wall sum.c -o sum
//		gcc -Wall max.c -o max 
//	and then build and execute this program.
//
//	Jean-Yves Hervé, 	University of Rhode Island
//						Department of Computer Science and Statistics
//
//	2017-10-10, revised 2018-02-15, 2018-10-09
//======================================================================================

#include <stdio.h> 
#include <stdlib.h>
#include <sys/types.h> 
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/wait.h> 

//	Prototype for a placeholder function
void do_stuff(void);


int main(int argc, char * argv[] )
{
	//	Store names of the child executables
	char* EXECUTABLE_STR[2] = {	"./sum", "./max" };
	
	printf("\n\ninitial process \tPID %6d\tPPID %6d\tUID %d \tGID %6d\n\n",
			getpid(), getppid(), getuid(), getpgid(0)); 
	
	//	I am going to create a (small) bunch of child processes and store their pids
	//	in an array
	#define	NB_CHILDREN		10
	unsigned char executable[NB_CHILDREN];
	
	//	Seed the random generator
	//-----------------------------
	//	I learned something today.  In retrospect it sort of makes sense.  When I 
	//	made the call the rand() inside the "if (p == 0)" block, I found that all my
	//	children processes where were running the same executable with the same list of 
	//	arguments.  This is because they were all working with a random generator
	//	that had been seeded at the same time, and therefore, they were pulling out
	//	identical lists of pseudo-random values.  The lesson of this is:  I need
	//	to generate my random list of arguments here, in the parent process, so that
	//	the children end up with different lists.
	srand((unsigned int) time(NULL));
	

	//-------------------------------------------------------------------------
	//	Part I: Create the child processes and assign them randomly the 
	//	executable they are going to run and their list of arguments
	//-------------------------------------------------------------------------
	for (int i=0; i<NB_CHILDREN; i++)
	{
		//	I could do my random generation and allocation of the array to store all
		//	the arguments, but I would have to free that array afterwards.  It is much simpler
		//	to pull a random number of random values from the generator, so that the different
		//	children are now working with different sequences
		int shift = rand() % 10000;
		for (int k=0; k<shift; k++)
			rand();

		//	Before I fork, I decide (randomly) which executable the child will run
		executable[i] = (unsigned char) (rand() %2);

		//	Finally we fork
		//--------------------		
		int p = fork();
		
		//	if this is the child process, prepare to launch either sum or max, with arguments
		if (p == 0)
		{
			//	generate between 5 and 10 random arguments
			int nbArguments = (rand() % 6) + 5;

			//	my array of arguments to execvp needs 2 extra slots:  one for the name of the
			//	executable and one to store a terminating NULL pointer.
			char** command = (char**) calloc(nbArguments+2, sizeof(char*));
		
			//	numArguments random values between 1 and 100 (converted to a C string)
			//	Do you know why I allocate at 4 and not 5 or 10 times sizeof(char) ??
			for (int k=1; k<=nbArguments; k++) {			
				command[k] = (char*) malloc(4*sizeof(char));
				sprintf(command[k], "%d", ((rand() % 100) + 1));
			}
			//	The list **must** be terminated by a NULL pointer
			command[nbArguments+1] = NULL;		
			
			//	now set the executable's name based on our earlier dice roll
			command[0] = (char*) malloc(6*sizeof(char));
			strcpy(command[0], EXECUTABLE_STR[executable[i]]);
						
			//	launch the new code for the child process
			execvp(command[0], command);
			
			//	I can hear some of you gasp:  What about the memory leaking?  You don't 
			//	free the command 2D array?  No need because everything (code and data) will
			//	disappear when the new executable is launched.
		}
		//	case of failure: bail out
		else if (p < 0)
		{
			printf("Fork failed\n");
			exit(-1);
		}		
		//	if still in the parent process, store the child's pid and keep forking
		else
		{
			pid_t termProcess;
			int statusVal;

			//	wait for the child process to terminate.
			//	I set my option parameter (3rd parameter) to 0, the default value.
			//	The second parameter, the status allows the child process to return some simple
			//	information back to the parent.   You can consult the documentation on the waitpid
			//	function for more options: https://linux.die.net/man/2/waitpid
			termProcess = waitpid(p, &statusVal, 0);

			printf("Parent process got signal from child[%d] process %6d terminating %s with status value %5d --> %4d\n", 
						i, termProcess, EXECUTABLE_STR[executable[i]] + 2, statusVal, WEXITSTATUS(statusVal));

		}
	}
		
	return 0;
}

//	Function where the parent process does work, while waiting for children to finish.
void do_stuff(void)
{
	printf("Parent process doing stuff.\n");
	usleep(100000);
}
