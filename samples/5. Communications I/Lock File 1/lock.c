//===================================================================================
//	This program demonstrates how to use a lock file to establish primitive
//	Communication betweeen two or more processes.
//
//	This demo works with a hard-coded file path for the lock file.  A typical
//	location for lock files on a Linux system would be in the /tmp folder.
//
//	A process running this code will attempt to "acquire" the lock file.
//		If it can, then it will do its business, and eventually release
//			the lock, then terminate.
//		If it cannot acquire the lock, this means that another process holds
//			the lock. It will therefore wait for some time, and try again.
//			It will repeat doing so until either it can finally acquire the
//			lock or it has tried a set number of times, in which case it will
//			throw its figurative hands up and terminate.
//	Note that:
//		1. This program makes no attempt at implementing a useful task. Its
//			sole purpose is to demonstrate the lock file mechanism.
//		2. The idea here is to control the access to a shared resource.  The
//			process that acquired the lock gained the right to access the
//			shared resource.
//		3. In a more useful application, the process would not have to
//			terminate after releasng the lock.  It could return to its
//			calculations, to try to acquire the lock again.
//		4. Think of the lock as a way for one process to send a signal to the
//			other processes.  It is really just a kind of trigger, but of course
//			this requires the other processes to come checking the lock
//			regularly.
//
//	This program takes 2 optional arguments.  Assuming it was built as "prog"
//		./lock		uses the default values:	try up to 5 times to acquire the lock,
//					  and sleep 6 seconds between attempts
//		./lock 3	specifies the number of attempts (3 times) and uses the default
//					  sleep time between attemps (6 seconds)
//		./lock 3 8  specifies the number of attempts (3 times) and the sleep
//					  time between attempts (8 seconds)
//
//	You need to lauch the program at least twice from the terminal.  Each time
//	(maybe except the last) you need to launch the program "in the background"
//	using the umpersand & suffix, so that you regain control in the terminal, e.g.:
/*

./lock &
./lock 3 10 &
./lock 6 3

or, to get some failure

./lock &
./lock 3 10 &
./lock 4 2

*/
//
//  Code adapted from "Interprocess Communications in Unix - The Nooks & Crannies"
//						John Shapley Gray. Prentice Hall PTR, 1998.
//	I essentially removed pieces that unnecessarily complicated the program
//	and cleaned up some ugly parts, so that we could concentrate on what is
//	really important.
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
#include <errno.h> 
#include <stdlib.h>

//	I define my own bool type, since C doesn't have one by default.
//	Note that, since enum values are in fact int, false will start at
//	the default value of 0 and true will be 1.  So we will get the
//	expected behavior.
#ifndef bool
	typedef enum bool { false, true } bool;
#endif

//	Default values for the number of tries and sleep time (in second)
//	between tries.
#define DEFAULT_NUM_TRIES	5
#define DEFAULT_SLEEP_TIME	6

//	Here I had-code the amount of time (in seconds)
//	that the process having acquired the lock keeps it until releasing it.
//	Again, in a real taks, this would be time spent doing calculations,
//	perfoming I/O, etc.
#define	RETAIN_LOCK_TIME	30

//	Hard-coded path for the lock file.  The /tmp folder is a typical location
//	for lock files.  All the programs using the lock need to know the path.
//	This is easy if they run the same code, but will require a bit more effort
//	if they run different code.
//	In any case, you should use a fairly "unique" name.  Otherwise, unrelated,
//	not meant to communicate with each other proceses, might end up blocking
//	each other on the same "LOCK.LCK" file.
//	Please note that the extension .LCK is not mandatory.  Any extension will do.
#define LOCK_FILE_PATH	"/tmp/TEST412_Lock1.LCK"

//------------------------------
//	Function prototypes
//------------------------------
void setParameters(int argc, char *argv[], unsigned int* numTries, unsigned int* sleepTime);
bool acquireLock(char* filePath, unsigned int numTries , unsigned int sleepTime);
bool releaseLock(char* filePath);

//------------------------------
//	Function implementations
//------------------------------

int main(int argc, char *argv[])
{
	unsigned int numTries;
	unsigned int sleepTime;
	
	//	I start all processes with a few seconds of sleep time to leave
	//	the user time to clean up the terminal to get a prettier display
	sleep(3);
	printf("\nProcess %d launched.\n", getpid());

	//	Assign values to the parameters, using default values if
	//	the user didn't pass arguments
	setParameters(argc, argv, &numTries, &sleepTime);

	//	Try (as many times as specified) to acquire the lock
	if (acquireLock(LOCK_FILE_PATH, numTries, sleepTime))
	{
		//	If arrive here, it means that we acquired the lock, and therefore
		//	gained the right to access	the shared resource
		//	Here we model a RETAIN_LOCK_TIME access time to the resource
		for (unsigned int lockTime=0, remainingTime=RETAIN_LOCK_TIME; 
			 lockTime < RETAIN_LOCK_TIME; lockTime++, remainingTime--)
		{
			char outStr[100];
			sprintf(outStr, "\t\tProcess %d accessing the resource (%d second",
					getpid(), (RETAIN_LOCK_TIME - lockTime));
			if (remainingTime > 1)
				strcat(outStr, "s");
			strcat(outStr, " left)\n");
			printf("%s", outStr);
			fflush(stdout);
			sleep(1);
		}
		
		//	We are done with the resource, so we release the lock
		releaseLock(LOCK_FILE_PATH);

		printf("--> Process %d terminates.\n", getpid());
		fflush(stdout);
	}
	//	If after several tries we didn't manage to acquire the lock, we
	//	simply bail out.  Of course, we could keep trying forever, but
	//	that's how the Mac OS "network access freeze" situation happen.
	else
	{
		printf("XXX Process %d unable to obtain the lock file. Terminates.\n", getpid());
		fflush(stdout);
	}
	
	return 0;
}

//-------------------------------------------------------------------------
//	Based on the arguments passed when the program was launched, set the
//	number of tries and sleep time between tries.
//-------------------------------------------------------------------------
void setParameters(int argc, char *argv[], unsigned int* numTries, unsigned int* sleepTime)
{
	
	switch(argc)
	{
		//	The user set both parameters. argv[2] specifies sleep time
		case 3:
			//	If the user entered an invalid parameter (not a positive int),
			//	then we ignore it and use the default value instead
			if (sscanf(argv[1], "%u", numTries) == 0)
				*numTries = DEFAULT_NUM_TRIES;
			//	If the user entered an invalid parameter (not a positive int),
			//	then we ignore it and use the default value instead
			if (sscanf(argv[2], "%u", sleepTime) == 0)
				*sleepTime = DEFAULT_SLEEP_TIME;
			break;
			
		case 2:
			//	If the user entered an invalid parameter (not a positive int),
			//	then we ignore it and use the default value instead
			if (sscanf(argv[1], "%u", numTries) == 0)
				*numTries = DEFAULT_NUM_TRIES;
			*sleepTime = DEFAULT_SLEEP_TIME;
			break;
		
		case 1:
			*numTries = DEFAULT_NUM_TRIES;
			*sleepTime = DEFAULT_SLEEP_TIME;
			break;
		
		default:
			fprintf(stderr, "Usage: %s	[[tries] [DEFAULT_SLEEP_TIME]]\n", argv[0]);
			exit(1);
	
	}
}


//-------------------------------------------------------------------------
//	Attemps to acquire the lock by creating the lock file.
//	Tries multiple times, with some sleep time between faled attempts.
//	After the specified max number of tries has been reached, returns false.
//	If a try succeeds, creates a lock file (0 bytes with no RWX permissions)
//	and return true
//-------------------------------------------------------------------------
bool acquireLock(char* filePath, unsigned int numTries , unsigned int sleepTime)
{
	int fd;
	unsigned int count = 0;

	//	Yes, the name of the system call is truly "creat". (and yes, just
	//	like you I try very hard not to roll my eyes thinking of the programmer
	//	who came up with such a generic name and made "better," so much faster
	//	to type by removing *one* letter (and changing the pronunciation).
	
	//	Keep trying to create the lock file as long as...
	//        creation attempt failed        Permission denied
	while ((fd = creat(filePath, 0)) == -1 && errno == EACCES)
	{
		//	Creation attempt failed, so increment the try counter
		if (++count < numTries)
		{
			printf("Process %d unable to acquire the lock, will try again.\n",
					getpid());
			fflush(stdout);
			usleep(sleepTime*1000000);
		}
		//	If the set number of tries has been reached, report failure.
		else
		{
			return (false);
		}
		//	going back to try again
	}
	
	//	We end up here because the lock file was successfully created, but we are
	//	not done yet.
	printf("--> Process %d successfully created the lock file....\n", getpid());
	fflush(stdout);

	//	We are not trying to store anything in the lock file.  Its very
	//	existence *is* the signal.  So we close it right away (0 byte content)
	close(fd);
	
	//	The "acquire" operation is only fully successful if we managed to close
	//	the lock file after creating it.
	printf("  \tProcess %d successfully closed the lock file.  Now it fully \"acquired\" it.\n", 
			getpid());
	fflush(stdout);

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
