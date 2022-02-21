//======================================================================================
//	In this sample, I return to the code of the first demo, process_fork_1.c
//	and combine with things that we learned in the second demo about the execXX
//	family of system calls.
//
//	The big difference is that I move the code to be executed by the child 
//	processes into separate source files that we are going to build separately.
//	And we will run these programs using an execXX function in the parent process.
//	
//	So, first build the executable for the 2 child programs
//		gcc child1.c -p child1
//		gcc child2.c -p child2
//	and then build and execute this program.
//
//	Jean-Yves Hervé, 	University of Rhode Island
//						Department of Computer Science and Statistics
//	2018-02-15
//======================================================================================
 
#include <stdio.h> 
#include <stdlib.h>
#include <sys/types.h> 
#include <unistd.h>
#include <sys/wait.h> 

int main(int argc, char * argv[] )
{
	//	I am going to use these to store the pids of the child processes
	int p1, p2;
	
	printf("\n\ninitial process PID %6d  PPID %6d  UID %5d  GID %6d\n\n",
			getpid(), getppid(), getuid(), getpgid(0)); 
	
	//	First fork by the parent process
	p1 = fork( );

	//------------------------------------
	//	case of the child process
	//------------------------------------
	if (p1 == 0)
	{
		printf("Doing exec on child 1\n");

		//	Replace by child1 code.
		char* command[] = {"./child1", NULL};
		execvp(command[0], command);
	}

	//------------------------------------
	//	If we are still in the parent process...
	//------------------------------------
	else if (p1 > 0) 
	{
		printf("Parent process PID %6d  PPID %6d  UID %5d  GID %6d created child process %d\n",
				getpid(), getppid(), getuid(), getpgid(0), p1);

		//	create a second child
		p2 = fork( );
		
		//------------------------------------
		//	case of the child process
		//------------------------------------
		if(p2 == 0)
		{
			printf("Doing exec on child 2\n");

			//	Replace by child2 code.
			execlp("./child2", NULL);
		}
		
		//------------------------------------
		//	If we are still in the parent process...
		//------------------------------------
		else if (p2 > 0)
		{
			printf("Parent process PID %6d  PPID %6d  UID %5d  GID %6d created child process %d\n",
					getpid(), getppid(), getuid(), getpgid(0), p2);

		}
		
		//------------------------------------
		//	failure while trying to create child 2
		//------------------------------------
		else 	//	p2 < 0   --> failure
		{
			printf("Creation of child process 2 failed\n");
			exit(-1);
		}
		
	}
	//------------------------------------
	//	failure while trying to create child 1
	//------------------------------------
	else 	//	p1 < 0   --> failure
	{
		printf("Creation of child process 1 failed\n");
		exit(-1);
	}


	//-----------------------------------------------------------
	//	Only the parent process ends up here
	//-----------------------------------------------------------
	pid_t termProcess;
	int statusVal;

	//	wait for child process 1 to terminate.  Here I wait for a specific process pid
	//	I set my option parameter (3rd parameter) to 0, the default value.
	//	The second parameter, the status allows the child process to return some simple
	//	information back to the parent.   You can consult the documentation on the waitpid
	//	function for more options: https://linux.die.net/man/2/waitpid
	termProcess = waitpid(p1, &statusVal, 0);
	printf("Parent process got signal from child process %6d with status value %5d --> %3d\n", 
				termProcess, statusVal, WEXITSTATUS(statusVal));
	
	//	same thing for child process 2
	termProcess = waitpid(p2, &statusVal, 0);
	printf("Parent process got signal from child process %6d with status value %5d --> %3d\n", 
				termProcess, statusVal, WEXITSTATUS(statusVal));

		
	return 0;
}
