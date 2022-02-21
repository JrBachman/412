//======================================================================================
//	In this code sample, our initial process creates one child process, then
//	waits for that child to terminate and return a value.  One of the 
//	several points to look at specifically here:
//		o First, the process creation with a system call to fork()
//		o The values of the pid, ppid, and gid of the parent and child processes
//			(note that all share the same gid, which is the pid of the parent process)
//		o The parent process waits for the child processes to terminate, and it gets
//			the value returned by the child processes when they terminate.
//
//	Jean-Yves Hervé, 	University of Rhode Island
//						Department of Computer Science and Statistics
//	2021-10-08
//======================================================================================

#include <stdio.h> 
#include <stdlib.h>
#include <sys/types.h> 
#include <unistd.h>
#include <sys/wait.h> 		//	for usleep

//sleep(seconds)
//usleep(microseconds)   u --> µ


void codeChildProcess(void);


int main(int argc, char * argv[] )
{
	printf("\n\ninitial process \tPID %6d\tPPID %6d\tUID %d \tGID %6d\n\n",
			getpid(), getppid(), getuid(), getpgid(0)); 
	
	//	First fork by the parent process
	int p = fork( );

	//------------------------------------
	//	case of the child process
	//------------------------------------
	if (p == 0) 
	{		
		//	Here I could have included the code of the child process.  Instead I call 
		//	a function from which we don't return (it ends with a system call exit()
		codeChildProcess();
	}

	//------------------------------------
	//	If we are still in the parent process...
	//------------------------------------
	else if (p > 0) 
	{
		printf("Parent process \t\tPID %6d\tPPID %6d\tUID %d \tGID %6d\tcreated child process %d\n",
				getpid(), getppid(), getuid(), getpgid(0), p);
	}
	//------------------------------------
	//	failure trying to create child 
	//------------------------------------
	else 	//	p < 0   --> failure
	{
		printf("Creation of child process failed\n");
		exit(-1);
	}


	//-----------------------------------------------------------
	//	Only the parent process ends up here
	//-----------------------------------------------------------
	//	do something
	
// 	usleep(2000000);
// 	printf("step 1\n");		
// 	usleep(2000000);
// 	printf("step 2\n");		
// 	usleep(2000000);
// 	printf("step 3\n");		
// 	usleep(2000000);
// 	printf("step 4\n");		
// 	usleep(2000000);
// 	printf("step 5\n");		


	//	wait for child process to terminate.  Here I wait for a specific process pid
	//	I set my option parameter (3rd parameter) to 0, the default value.
	//	The second parameter, the status allows the child process to return some simple
	//	information back to the parent.   You can consult the documentation on the waitpid
	//	function for more options: https://linux.die.net/man/2/waitpid
	int statusVal;
	pid_t termProcess = waitpid(p, &statusVal, 0);
	printf("Parent process got signal from child process %6d with status value %d --> %d\n", 
				termProcess, statusVal, WEXITSTATUS(statusVal));
	
	return 0;
}


//------------------------------------------------------------------------------------
//	I simply put into this function the code that I want the child process to execute.
//	Note that the function ends with a call to exit(), so that the process terminates.
//	I somewhat abuse the exit function by using it to return a value (presumably of
//	a calculation) to the parent process.  Normally, exit is meant to return an
//	error code (0: all OK, <0: system error, >0: process error)
//------------------------------------------------------------------------------------
void codeChildProcess(void)
{
//	usleep(1000000);

	printf("In first child process\tPID %6d\tPPID %6d\tUID %d \tGID %6d\n",
			getpid(), getppid(), getuid(), getpgid(0));
				
	usleep(1000000);
	printf("Child finishes\n");

	//	Terminates execution by returning a value 
	//	(should be the result of some computations) to the parent process
	exit(52234);
}
