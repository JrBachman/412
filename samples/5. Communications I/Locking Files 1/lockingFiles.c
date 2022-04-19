//===================================================================================
//	This program demonstrates the use of file locking routines for interprocess
//	communication.
//
//	The parent process forks to create child processes that will
//	share access, using the file locking mechanism, to a shared file.
//			./prog filePath
//
//	The key difference with the lock file method is that file locking method
//	uses standard functions of the file system to set and removed a lock
//	placed on a file storing the information passed back and forth.
//
//	In this version, we don't read/write in the shared file yet, as we only
//	concentrate on the set/remove lock mechanism.
//
//	As explained in the notes, there are two main categories of locking:
//	advisory and mandatory locking.  In this sample, we look at advisory
//	locking, using the fcntl system call.
//
//  Code adapted from "Interprocess Communications in Unix - The Nooks & Crannies"
//						John Shapley Gray. Prentice Hall PTR, 1998.
//
//	Jean-Yves Hervé, 	University of Rhode Island
//						Department of Computer Science and Statistics
//
//	2018-02-19, revised 2018-10-16, 2019-03-18
//===================================================================================

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
//#include <types.h>


//	How many child processes does the parent process create?
#define NB_CHILDREN		7

//	Default values for the number of tries and sleep time (in second)
//	between tries to acquire the lock.  Here my processes will keep trying.
//	In some cases you may want to keep at it forever.
#define MAX_NB_TRIES		15
#define SLEEP_TIME_LOCK		1

//	How much time (in seconds) does a child process spend doing calculations
//	before trying to access the shared file
#define CALCULATION_TIME	3

//	How much time (in seconds) does a child process spend accessing the
//	shared file after it acquired the lock
#define ACCESS_TIME			5

#if 0
#pragma mark -
#endif

//------------------------------
//	Global variables
//------------------------------
//	All child processes will get a copy of this variable initialized with the start
//	time of the parent.
time_t startTime;


//------------------------------
//	Function prototypes
//------------------------------

void doCalculationsAndExit(char* filePath);

//------------------------------
//	Function implementations
//------------------------------

int main(int argc, char* argv[]) {

	startTime = time(NULL);
	
	//----------------------
	//	Argument check
	//----------------------
	if (argc < 2) {
		printf("Usage %s shared_file_path\n", argv[0]);
		exit (1);
	}
	
	//-----------------------------------------------------------
	//	Create the shared file
	//-----------------------------------------------------------
	FILE* fp = fopen(argv[1], "w");
	fprintf(fp, "Parent process creates the shared file.\n");
	fclose(fp);
	
	//-----------------------------------------------------------
	//	Create the child processes
	//-----------------------------------------------------------
	for (int k=0; k<NB_CHILDREN; k++)
	{
		int p = fork();
		
		//	We send the child process to a function from which it will
		//	not return (ends with a call to exit)
		if (p == 0)
		{
			doCalculationsAndExit(argv[1]);
		}
		else if (p>0)
		{
			printf("Child process %d (PID %d) successfully created\n", k, p);
		}
		else if (p<0)
		{
			printf("Child process creastion failed.\n");
			exit(1);
		}
	}
	
	//	The parent process waits for all the child processes to terminate
	//	The order doesn't matter, we just want to keep the count.
	//	Here I don't even check the status of the child, to keep the code
	//	simple.  We show to do that in the Process Creation series.
	for (unsigned int k=0; k<NB_CHILDREN; k++)
	{
		int status;
		waitpid(-1, &status, 0);
		printf("Child terminated counter: %u (out of %u)\n", (k+1), NB_CHILDREN);
	}

	printf("\n--------------------------\nParent Process terminates");
	printf("\n--------------------------\n");
	
	return 0;
}

//	Note that, because this code is executed after fork(), each
//	child process will have its own pointer to the shared file.
void doCalculationsAndExit(char* filePath)
{
	pid_t pid = getpid();

	//	Don't start immediately (for this demo)
	sleep(1);
	
	//-----------------------------------------------------------
	//	Open the shared file for reading and writing
	//	Options are: 	O_RDONLY	open for reading only
	//					O_WRONLY	open for writing only
	//					O_RDWR		open for reading and writing
	//					O_ACCMODE	mask for above modes
	//-----------------------------------------------------------
	int fileDesc;
	if ((fileDesc = open(filePath, O_RDWR)) < 0)
	{	
		perror(filePath);
		exit(2);
	}
	//	Note that we continue execution with the file *still open*
	//	for reading and writing.  The process could hapilly write
	//	into the file and would not be blocked.  We are doing
	//	*advisory locking*, so the processes need to cooperate
	//	with each other and check the lock before accessing the file

	//-----------------------------------------------------------
	//	Initialize the struct to pass to the fcntl function.
	//	The l_type field seems somewhat redundant with the second
	//		"command" argument of the system call
	//	The l_whence field gives the type of l_start (0: beginning,
	//		1: current, 2: end of file)
	//	The l_start field fives the offset from the start position
	//		defined by l_start
	//	The l_len field stores the number of bytes, starting from
	//		the offset position.  Value 0 means all bytes
	//-----------------------------------------------------------
	struct flock lock;

	lock.l_type = F_WRLCK;		//	set a write lock
	lock.l_whence = 0;			//	start at beginning
	lock.l_start = 0L;			//	with a 0 offset
	lock.l_len = 0L;			//	whole file

	//	Presumably do work
	sleep(CALCULATION_TIME);

	int tryCount = 0;
	//-----------------------------------------------------------
	//	Try to set the lock on the shared file.  The second
	//	argument can be:	F_DUPFD		duplicate file descriptor
	//						F_GETFD		get file descriptor flags
	//						F_SETFD		set file descriptor flags
	//						F_GETFL		get file status flags
	//						F_SETFL		set file status flags
	//						F_GETOWN	get SIGIO/SIGURG proc/pgrp
	//						F_SETOWN	set SIGIO/SIGURG proc/pgrp
	//						F_GETLK		get record locking information
	//						F_SETLK		set record locking information
	//						F_SETLKW	F_SETLK; wait if blocked
	//-----------------------------------------------------------
	while (fcntl(fileDesc, F_SETLK, &lock) < 0)
	{
		printf("Process %d found file locked.\n", pid);

		//	fcntl returns -1 in case of failure, but also sets the
		//	system variable errno
		switch (errno) 
		{
			case EAGAIN:	//	Resource temporarily unavailable
			case EACCES:	//	Permission denied
				if (++tryCount < MAX_NB_TRIES)
					sleep(SLEEP_TIME_LOCK);
				else 
				{
					//	All attempts failed.  Report which other process is
					//	holding the lock and exit.
					//	Note that we don't have to give up, we could keep trying
					//	forever until the resource eventually becomes free.
					fcntl(fileDesc, F_GETLK, &lock);
					printf("\t--> Process %d gives up. File still locked by %d\n",
							pid, lock.l_pid);
					exit(3); 
				}
				continue;
				
			default:
				break;
				
		}
		perror("fcntl"); 
		exit (4);
	}
	
	//	If the process reaches this point, then it successfully set the
	//	lock and could now access the file (in the next demo)
	printf("\t--> Process %d has access to the file\n", pid);

	FILE* fp = fopen(filePath, "a");
	fprintf(fp, "Process %d gained access at time t=%ld\n", pid, (time(NULL)-startTime));
	
	//	spend time accessing the file
	sleep(ACCESS_TIME);
	
	//	Of course, we could repeat the "calculate - set lock - access file" block
	//	multiple times.  For this demo, though, we are done.
	printf("\t--> Process %d done with the file\n", pid);
	
	fprintf(fp, "Process %d released access at time t=%ld\n", pid, (time(NULL)-startTime));
	fclose(fp);

	//	In the code I used at starting point for this demo
	//		"Interprocess Communications in Unix - The Nooks & Crannies"
	//			John Shapley Gray. Prentice Hall PTR, 1998."
	//	the author doesn't bother removing the lock or even closing the
	//	file.  The idea is that the OS keeps track of that informtion and
	//	does it automatically.  True, but I think it's better practice
	//	to do it.
	if (fcntl(fileDesc, F_SETLK, &lock) < 0)
	{
		printf("\t--> Process %d could not release the lock\n", pid);
		exit(5);
	}
	close(fileDesc);
	
	printf("\t--> Process %d terminates\n", pid);
	exit (0);
}

