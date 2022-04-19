//======================================================================================
//	This sample is fairly similar to that of process_fork_exec_2.c except for the
//	fact that we use a loop to create our child processes.
//
//	Again, this program can be built in two different versions by setting the value
//	of the WAIT_VERSION to one of the following four values:
//		o WAIT_FIXED_ORDER_CREATION:	the parent process waits for the children
//			processes to terminate, and it does so in the order in which it created them.
//		o WAIT_FIXED_ORDER_REVERSE:	the parent process waits for the children
//			processes to terminate, and it does so in the reverse of the order in 
//			which it created them.
//		o WAIT_NO_ORDER:	the parent process wait for its children to terminate, but
//			does not care in which order they terminate, as long as they all 
//			terminate.
//		o CHECK_NO_WAIT:	the parent process checks if its children terminated, in no
//			specific order, but does so with a non-blocking call (so that it can do
//			other interesting things while its children are running).  It still
//			waits until all children have terminated to terminate its own execution.
//
//	What about other possible configurations/versions that you may find more 
//	interesting?  Yes, what about them?  Add to this code, try them, and report
//	on the forums.
//
//	Note that I don't test for any abnormal termination of the child processes.  This
//	is a "fair weather" demo.  If you need to handle special cases of a child process
//	crashing without returning a value, you will need to look carefully at the 
//	documentation of waitpid
//
//	So, first build the executable for the 2 child programs
//		gcc child1.c -p child1
//		gcc child2.c -p child2
//	and then build and execute this program.
//
//	Jean-Yves Hervé, 	University of Rhode Island
//						Department of Computer Science and Statistics
//	2018-02-15. Edited 2018-10-09
//======================================================================================

#include <stdio.h> 
#include <stdlib.h>
#include <sys/types.h> 
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/wait.h> 

//	Define possible versions of this program.  See top comments for explanation of
//	the differences.
#define	WAIT_FIXED_ORDER_CREATION	0
#define WAIT_FIXED_ORDER_REVERSE	1
#define WAIT_NO_ORDER				2
#define CHECK_NO_WAIT				3

//	Select one value
#define	WAIT_VERSION		WAIT_NO_ORDER

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
	int childPid[NB_CHILDREN];
	unsigned char executable[NB_CHILDREN];
	
	//	Seed the random generator
	//-----------------------------
	//	I learned something today.  In retrospect it of course makes sense.  When I 
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
			childPid[i] = p;
			printf("Parent process PID %6d  PPID %6d  UID %3d GID %6d created child process %6d to run %s\n",
					getpid(), getppid(), getuid(), getpgid(0), p, EXECUTABLE_STR[executable[i]]+2);
		}
	}
	
	
	//-------------------------------------------------------------------------
	//	Part II: Wait for children to terminate
	//
	//	Only the parent process can end up here.  The children processes have 
	//	all flushed their partition (and this code) and loaded a new one.
	//	This is where we are going to have different ways to wait for 
	//	termination, depending on the choice we made for WAIT_VERSION
	//-------------------------------------------------------------------------
	
	#if WAIT_VERSION == WAIT_FIXED_ORDER_CREATION
	
		//	I wait for the child processes to terminate, in the same order
		//	as the one used to create them
		for (int k=0; k<NB_CHILDREN; k++)
		{
			pid_t termProcess;
			int statusVal;

			//	wait for child process with a specific pid to terminate.
			//	I set my option parameter (3rd parameter) to 0, the default value.
			//	The second parameter, the status allows the child process to return some simple
			//	information back to the parent.   You can consult the documentation on the waitpid
			//	function for more options: https://linux.die.net/man/2/waitpid
			termProcess = waitpid(childPid[k], &statusVal, 0);

			printf("Parent process got signal from child[%d] process %6d terminating %s with status value %5d --> %4d\n", 
						k, termProcess, EXECUTABLE_STR[executable[k]] + 2, statusVal, WEXITSTATUS(statusVal));
		
		}
	
	#elif WAIT_VERSION == WAIT_FIXED_ORDER_REVERSE
	
		//	I wait for the child processes to terminate, in reverse order
		//	from the one used to create them
		for (int k=NB_CHILDREN-1; k>=0; k--)
		{
			pid_t termProcess;
			int statusVal;

			//	wait for child process with a specific pid to terminate.
			//	I set my option parameter (3rd parameter) to 0, the default value.
			//	The second parameter, the status allows the child process to return some simple
			//	information back to the parent.   You can consult the documentation on the waitpid
			//	function for more options: https://linux.die.net/man/2/waitpid
			termProcess = waitpid(childPid[k], &statusVal, 0);

			printf("Parent process got signal from child[%d] process %6d terminating %s with status value %5d --> %4d\n", 
						k, termProcess, EXECUTABLE_STR[executable[k]] + 2, statusVal, WEXITSTATUS(statusVal));
		
		}

	
	#elif WAIT_VERSION == WAIT_NO_ORDER
	
		//	I wait any child processes to terminate, then we find out who it was
		for (int k=0; k<NB_CHILDREN; k++)
		{
			pid_t termProcess;
			int statusVal;
 
			//	wait for any child process to terminate.
			//	I set my option parameter (3rd parameter) to 0, the default value.
			//	The second parameter, the status allows the child process to return some simple
			//	information back to the parent.   You can consult the documentation on the waitpid
			//	function for more options: https://linux.die.net/man/2/waitpid
			termProcess = waitpid(-1, &statusVal, 0);
			
			//	First, find which child it was. If we were in C++ we could store the list
			//	of non-terminated-children in a hash table, and pull elements from the hash table
			//	as they terminate.  In C, we do it the dumb way
			for (int i=0; i<NB_CHILDREN; i++)
			{
				if (childPid[i] == termProcess)
				{
					printf("Parent process got signal from child[%d] process %6d terminating %s with status value %5d --> %4d\n", 
								i, termProcess, EXECUTABLE_STR[executable[i]] + 2, statusVal, WEXITSTATUS(statusVal));
		
				}
			}
		}

	
	#elif WAIT_VERSION == CHECK_NO_WAIT
	
		int nbChildrenFinished = 0;
		while (nbChildrenFinished < NB_CHILDREN)
		{
			pid_t termProcess;
			int statusVal;

			//	Check if any child has terminated, but don't wait for termination.
			//	In other words this form of call to waitpid is *non-blocking*, also
			//	called "asynchronous."
			termProcess = waitpid(-1, &statusVal, WNOHANG);
			
			//	If no child finished, then the value returned by waitpid is 0
			if (termProcess == 0)
			{
				do_stuff();
			}
			//	If a child terminated, then find which child it was.
			else
			{
				for (int i=0; i<NB_CHILDREN; i++)
				{
					if (childPid[i] == termProcess)
					{
						printf("Parent process got signal from child[%d] process %6d terminating %s with status value %5d --> %4d\n", 
									i, termProcess, EXECUTABLE_STR[executable[i]] + 2, statusVal, WEXITSTATUS(statusVal));
		
						nbChildrenFinished++;
						break;
					}
				}
			}
		}
	
	#else

		#error Invalid choice of WAIT_VERSION

	#endif
	
	return 0;
}

//	Function where the parent process does work, while waiting for children to finish.
void do_stuff(void)
{
	printf("Parent process doing stuff.\n");
	usleep(100000);
}
