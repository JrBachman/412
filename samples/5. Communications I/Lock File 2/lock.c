//===================================================================================
//	This program builds on the previous "Lock File" sample, but, now that
//	we understand how lock files work, we actually access a shared resource
//
//	In this case, the shared resource is an output text files.  The first
//	process to gain access creates the text files and writes some data before
//	closing the file and releasing the lock.  All processes that subsequently
//	gain access open the shared file in "append" mode, so that their output
//	is appended to the file rather then overwriting ealier output.
//
//	In this version, I hard-coded the arguments from the "Lock File 1" sample:
//	The number of tries  and sleep time between tries are set, so as not to
//	take the focus from the main point of this sample (the use of the resource).
//
//	The process needs only to be launched once, as it forks multiple times
//	to create child processes with which to share access to the resource.
//	The only argument is the path to the output data file.
//	Contrary to the lock file, which typically goes to the /tmp folder, the
//	output file should be somewhere in the user's folder, for example:
//
//		./lock  ./output.txt
//
//	Jean-Yves Hervé, 	University of Rhode Island
//						Department of Computer Science and Statistics
//
//	2018-02-16, revised 2018-10-16, 2019-03-18, 2021-04-07
//===================================================================================

#include <stdio.h>
#include <stdlib.h> 
#include <limits.h> 
#include <string.h> 
#include <unistd.h> 
#include <fcntl.h>
#include <time.h>
#include <errno.h>

//	I define my own bool type, since C doesn't have one by default.
//	Note that, since enum values are in fact int, false will start at
//	the default value of 0 and true will be 1.  So we will get the
//	expected behavior.
#ifndef bool
	typedef enum bool { false, true } bool;
#endif

//	Hard-coded lock file Path.  The /tmp folder is typically a good location
//	for a lock file, since all processes should have access to it.
#define LOCK_FILE_PATH	"/tmp/CSC412/LOCK_CSC412_SAMPLE2.LCK"

//	Default values for the number of tries and sleep time (in second)
//	between tries to acquire the lock.  Here my processes will keep trying.
//	In some cases you may want to keep at it forever.
#define NB_TRIES_LOCK	50
#define SLEEP_TIME_LOCK	1

//	Here I had-code the amounts of time (in seconds) that a process
//	spends doing calculations and then, having acquired the lock, keeps
//	it until releasing it.  In this code I spend that time sleeping.
//	In a real program, this would be spend doing calculations or I/O.
#define	USE_SHARED_RESOURCE_TIME	2
#define COMPUTATION_TIME			5

//	How many child processes does the parent process create?
#define NB_CHILDREN		6

//	Define some constants for the computation part of the processes.
//	Yes, I always define constants rather than using literals in my code.
//	Any code with an expected lifetime greater than an hour or two (that
//	is, an in-class demo, where I can goof around with xfo3PtfiWW identifiers),
//	and definitely any code that will survive the night, deserves at least
//	the minimal degree of good-coding practice.
#define MIN_VAL		1
#define MAX_VAL		20
#define MAX_INDEX	15


//------------------------------
//	Function prototypes
//------------------------------
void doCalculationsAndExit(unsigned int k, unsigned int F1, unsigned int F2,
						   unsigned int N, char* filePath);
void generateParameterValues(unsigned int* F1, unsigned int *F2, unsigned int* N);
unsigned int fibo(unsigned int F1, unsigned int F2, unsigned int N);
bool acquireLock(char* filePath, unsigned int nbTries , unsigned int sleepTime);
bool releaseLock(char* filePath);

//------------------------------
//	Function implementations
//------------------------------

int main(int argc, char *argv[])
{
	printf("\nParent process %d launched.\n", getpid());
	fflush(stdout);
	
	srand ((unsigned int) time(NULL));

	//----------------
	//	The parent process creates the output file, using the single argument
	//	as file path, then closes it.
	//----------------
	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s	<file path>\n", argv[0]);
		exit(1);
	}
	char* filePath = argv[1];
	FILE* fp = fopen(filePath, "w");
	if (fp == NULL)
	{
		printf("Could not create output file %s\n", filePath);
		fflush(stdout);
	}
	fprintf(fp, "Parent process %d created the file.\n", getpid());
	fclose(fp);

	//	Parameters of the calculations to be performed by child processes
	unsigned int F1, F2, N;

	//----------------
	//	Now all the pieces are in place, we create child process and let
	//	do their thing
	//----------------
	for (unsigned int k=0; k<NB_CHILDREN; k++)
	{
		//	I generate randomly values for the child's parameters.
		//	We are going to make them compute a given number of numbers in a...
		//	Yes, Fibonacci series.  How original!
		generateParameterValues(&F1, &F2, &N);

		int p = fork();
		//	Send the child process perform calculation
		if (p==0)
		{
			doCalculationsAndExit(k, F1, F2, N, filePath);
		}
		else if (p>0)
		{
			printf("Child process %d (PID %d) successfully created\n", k, p);
			fflush(stdout);
		}
		else if (p<0)
		{
			printf("Child process creastion failed.\n");
			fflush(stdout);
			exit(1);
		}
	}
	
	//	The parent process waits for all the child processes to terminate
	//	The order doesn't matter, we just want to keep the count.
	//	Here I don't even check the status of the child, to keep the code
	//	simple.  We showed how to do that, and what to do with the information,
	//	in the Process Creation series.
	for (unsigned int k=0; k<NB_CHILDREN; k++)
	{
		int status;
		waitpid(-1, &status, 0);
		printf("Parent process gets termination signal from a child process (%u out of %u)\n",
				k, NB_CHILDREN);
		fflush(stdout);
	}

	printf("\n--------------------------\nParent Process terminates");
	printf("\n--------------------------\n");
	fflush(stdout);
	
	return 0;
}

//-------------------------------------------------------------------------
//	This is where a child process does all its work, and terminates when
//	the work is completed.
//	Again:  There is no return from this function, just an exit (and
//			therefore termination) for the process.
//-------------------------------------------------------------------------
void doCalculationsAndExit(unsigned int k, unsigned int F1, unsigned int F2,
						   unsigned int N, char* filePath)
{
	printf("\tChild process (PID %u) set to compute Fib(%u, %u, %u)\n",
			getpid(), F1, F2, N);
			
	bool taskCompleted = false;
	//---------------------------------
	//	Repeat N times
	//---------------------------------
	for (unsigned int i=1, remainingTries=N-1; i<=N; i++, remainingTries--)
	{
		//	Perform the calculations
		unsigned int Fi = fibo(F1, F2, i);
		sleep(COMPUTATION_TIME);

		//---------------------------------
		//	Now try (as many times as specified) to acquire the lock
		//---------------------------------
		if (acquireLock(LOCK_FILE_PATH, NB_TRIES_LOCK, SLEEP_TIME_LOCK))
		{
			//	If we acquired the lock, then we gained the right to access
			//	the shared resource.  We open the file in "append" mode and
			//	write some stuff
			FILE* fp = fopen(filePath, "a");
			fprintf(fp, "Child process %d (PID %d): F%d = %d\n", k, getpid(), i, Fi);
			
			//	supposedly the I/O would take a lot longer than that
			sleep(USE_SHARED_RESOURCE_TIME);
			
			//	I/O is over, close the shared file
			fclose(fp);

			//	Finally release the lock
			releaseLock(LOCK_FILE_PATH);
			
			//	job well done
			taskCompleted = true;
		}
		else
		{
			if (remainingTries > 0)
			{
				char outStr[100];
				sprintf(outStr, "\tProcess %d unable to obtain the lock file.  Will try %u more time", 
								getpid(), remainingTries);
				if (remainingTries >1)
				{
					strcat(outStr, "s");
				}
				printf("%s.\n", outStr);
			}
			else
			{
				printf("\tProcess %d failed to obtain the lock file for the last time.\n", getpid());
			}
			fflush(stdout);
		}
	}
	
	if (taskCompleted)
	{
		printf("--> Process %d completed its task. Terminates.\n", getpid());
	}
	else
	{
		printf("XXX Process %d failed to complete its task. Terminates.\n", getpid());
	}
	fflush(stdout);

	exit(0);
}

//-------------------------------------------------------------------------
//	Inefficient computation of the Nth number in a Fibonacci series
//-------------------------------------------------------------------------
unsigned int fibo(unsigned int F1, unsigned int F2, unsigned int N)
{
	int FN = 0;

	if (N > 2)
	{
		int Fi=F1, Fip1=F2;
		for (int i=2; i<N; i++)
		{
			int Fip2 = Fi + Fip1;
			Fi = Fip1;
			Fip1 = Fip2;
		}
		FN = Fip1;
	}
	else if (N == 2)
	{
		FN = F2;
	}
	else if (N == 1)
	{
		FN = F1;
	}
	return FN;
}

//-------------------------------------------------------------------------
//	Generate random parameters for a Fibonacci series computation
//-------------------------------------------------------------------------
void generateParameterValues(unsigned int* F1, unsigned int *F2, unsigned int* N)
{
	*F1 = MIN_VAL + (rand() % (MAX_VAL - MIN_VAL));
	*F2 = (*F1 < MAX_VAL-1) ? *F1 + (rand() % (MAX_VAL - *F1)) : MAX_VAL;
	*N = 2 + (rand()% (MAX_INDEX-1));
}

//-------------------------------------------------------------------------
//	Attemps to acquire the lock by creating the lock file.
//	Tries multiple times, with some sleep time between faled attempts.
//	After the specified max number of tries has been reached, returns false.
//	If a try succeeds, creates a lock file (0 bytes with no RWX permissions)
//	and return true
//-------------------------------------------------------------------------
bool acquireLock(char* filePath, unsigned int nbTries , unsigned int sleepTime)
{
	int fd;
	unsigned int count = 0;

	//	Yes, the name of the system call is truly "creat". (and yes, just
	//	like you I try very hard not to roll my eyes thinking of the programmer
	//	who came up with such a generic name and made "better," so much faster
	//	to type by removing *one* letter (and changing the pronunciation).
	
	//	Keep trying to create the lock file as long as...
	//        creation attempt failed  -->   Permission denied
	while ((fd = creat(filePath, 0)) == -1  && errno == EACCES)
	{
		//	Creation attempt failed, so increment the try counter
		if (++count < nbTries)
		{
			printf("Process %d unable to acquire the lock, will try again.\n",
					getpid());
			fflush(stdout);
			sleep(sleepTime);
		}
		//	If the set number of tries has been reached, report failure.
		else
			return (false);
	
		//	going back to try again
	}
	
	//	We end up here because the lock file was successfully created, but we are
	//	not done yet.
	printf("--> Process %d successfully created the lock file....\n", getpid());
	fflush(stdout);

	//	We are not trying to store anything in the lock file.  Its very
	//	existence *is* the signal.  So we close it right away (0 byte content)
printf("-------before close file, fd=%d\n", fd);
	close(fd);
printf("-------after close file, fd=%d\n", fd);
	
	//	The "acquire" operation is only fully successful if we managed to close
	//	the lock file after creating it.
	printf("  \tProcess %d successfully closed the lock file.  Now it fully \"acquired\" it.\n", 
			getpid());
	fflush(stdout);
	
	//	The "acquire" operation is only successful if we managed to close
	//	the lock file after creating it.
	return ((bool) (fd != -1));
}


//-------------------------------------------------------------------------
//	As the name indicates, this function releases the lock (by removing the
//	lock file), signaling to all other processes who will come checking
//	that the shared resource is now accessible for the first process to
//	set the lock again.
//-------------------------------------------------------------------------
bool releaseLock(char* filePath)
{
	//	The lock was only truly released if the unlink function returned
	//	a 0 (no error) value.
	bool okRelease = (unlink (filePath) == 0);
	
	if (okRelease)
	{
		printf("--> Process %d successfully released the lock.\n", getpid());
		fflush(stdout);
	}
	else
	{
		printf("--> Process %d failed to release the lock.\n", getpid());
		fflush(stdout);
	}

	return okRelease;
}
