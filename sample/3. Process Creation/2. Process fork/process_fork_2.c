//======================================================================================
//	In this code sample, our initial process creates two child processes, then
//	waits for them to terminates and return a value.  One of the 
//	Several points to look at specifically here:
//		o First, the process creation with a system call to fork()
//		o The values of the pid, ppid, and gid of the parent and child processes
//			(note that all share the same gid, which is the pid of the parent process)
//		o The parent process waits for the child processes to terminate, and it gets
//			the value returned by the child processes when they terminate.
//
//	Jean-Yves Hervé, 	University of Rhode Island
//						Department of Computer Science and Statistics
//	2018-02-15, rev. 2021-10-11
//======================================================================================

#include <stdio.h> 
#include <stdlib.h>
#include <sys/types.h> 
#include <unistd.h>
#include <sys/wait.h> 

#define VERSION_1	1		//	imposed child wait order
#define VERSION_2	2		//	any child wait order
//
#define VERSION	VERSION_1

void codeChildProcess1(void);
void codeChildProcess2(void);


int main(int argc, char * argv[] )
{
	//	I am going to use these to store the pids of the child processes
	int p1 = 0, p2 = 0;

	printf("\n\ninitial process \tPID %6d\tPPID %6d\tUID %d \tGID %6d\n\n",
			getpid(), getppid(), getuid(), getpgid(0)); 
	
	//	First fork by the parent process
	p1 = fork( );

	//------------------------------------
	//	case of the child process
	//------------------------------------
	if (p1 == 0)
	{		
		//	Here I could have included the code of the child process.  Instead I call 
		//	a function from which we don't return (it ends with a system call exit()
		codeChildProcess1();
	}

	//------------------------------------
	//	If we are still in the parent process...
	//------------------------------------
	else if (p1 > 0) 
	{ 
		printf("Parent process \t\tPID %6d\tPPID %6d\tUID %d \tGID %6d\tcreated child process %d\n",
				getpid(), getppid(), getuid(), getpgid(0), p1);

		//	create a second child
		p2 = fork( );
		
		//------------------------------------
		//	case of the child process
		//------------------------------------
		if(p2 == 0)
		{
			codeChildProcess2();
		}
		
		//------------------------------------
		//	If we are still in the parent process...
		//------------------------------------
		else if (p2 > 0)
		{
			printf("Parent process \t\tPID %6d\tPPID %6d\tUID %d \tGID %6d\tcreated child process %d\n",
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
	
	#if VERSION == VERSION_1
	
		//	same thing for child process 2
		termProcess = waitpid(p2, &statusVal, 0);
		printf("Parent process got signal from child process %6d with status value %d --> %d\n", 
					termProcess, statusVal, WEXITSTATUS(statusVal));

		termProcess = waitpid(p1, &statusVal, 0);
		printf("Parent process got signal from child process %6d with status value %d --> %d\n", 
					termProcess, statusVal, WEXITSTATUS(statusVal));

	#else
	
		termProcess = waitpid(-1, &statusVal, 0);
		printf("Parent process got signal from a child process %6d with status value %d --> %d\n", 
					termProcess, statusVal, WEXITSTATUS(statusVal));
	
		termProcess = waitpid(-1, &statusVal, 0);
		printf("Parent process got signal from a child process %6d with status value %d --> %d\n", 
					termProcess, statusVal, WEXITSTATUS(statusVal));
	#endif	
	
	usleep(1000000);
	
	printf("Parent process terminates\n");	
	return 0;
}


//------------------------------------------------------------------------------------
//	I simply put into this function the code that I want Child Process 1 to execute.
//	Note that the function ends with a cal to exit(), so that the process terminates.
//	I somewhat abuse the exit function by using it to return a value (presumably of
//	a calculation) to the parent process.  Normally, exit is meant to return an
//	error code (0: all OK, <0: system error, >0: process error)
//------------------------------------------------------------------------------------
void codeChildProcess1(void)
{
	printf("In first child process\tPID %6d\tPPID %6d\tUID %d \tGID %6d\n",
			getpid(), getppid(), getuid(), getpgid(0));
			

	printf("child process 1 terminates \tPID %6d\tPPID %6d\tUID %d \tGID %6d\n",
			getpid(), getppid(), getuid(), getpgid(0));

	//	Terminates execution by returning a value 
	//	(should be the result of some computations) to the parent process
	exit(162);
}

//------------------------------------------------------------------------------------
//	I simply put into this function the code that I want Child Process 1 to execute.
//	Note that the function ends with a cal to exit(), so that the process terminates.
//	I somewhat abuse the exit function by using it to return a value (presumably of
//	a calculation) to the parent process.  Normally, exit is meant to return an
//	error code (0: all OK, <0: system error, >0: process error)
//------------------------------------------------------------------------------------
void codeChildProcess2(void)
{
	
	printf("In second child process\tPID %6d\tPPID %6d\tUID %d \tGID %6d\n",
			getpid(), getppid(), getuid(), getpgid(0));
		
	usleep(20000000);
	printf("child process 2 terminates \tPID %6d\tPPID %6d\tUID %d \tGID %6d\n",
			getpid(), getppid(), getuid(), getpgid(0));

	//	Terminates execution by returning a value 
	//	(should be the result of some computations) to the parent process
	exit(57);
}

