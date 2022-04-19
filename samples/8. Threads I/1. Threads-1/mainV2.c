//
//  main.cpp
//  Threads-1:  This is the typical demo code you will find the like of online.
//				Pro:  It is rather simple.
//				Cons: Crude and not really working as intended.
//
//  Created by Jean-Yves Hervé on 2016-04-25.
//	revised 2020-03-20

#include <stdio.h>			// for printf
#include <pthread.h>		// for pthread_* calls
#include <string.h>			// for strerror()
#include <unistd.h>			// for usleep()
#include <stdlib.h>			// for exit()
#include <sys/types.h>		// for gettid() on Linux

#if 0
//-----------------------------------------------------------------
#pragma mark -
#pragma mark Custom data types
//-----------------------------------------------------------------
#endif

typedef struct ThreadInfo {
	//	set by pthread library
	pthread_t threadID;

	// input 
	int	index;

	// output
	//	more?? --> application-specific
	int* v;
	
} ThreadInfo;


#if 0
//-----------------------------------------------------------------
#pragma mark -
#pragma mark Function prototypes
//-----------------------------------------------------------------
#endif

void* myThreadFunc(void* argument);


#if 0
//-----------------------------------------------------------------
#pragma mark -
#pragma mark Global variables: Data accessible to all threads
//-----------------------------------------------------------------
#endif

#define THREAD_COUNT 6
#define LOOP_COUNT   10
#define MAIN_SLEEP_TIME   100000
#define THREAD_INIT_SLEEP_TIME   0
#define THREAD_COMP_SLEEP_TIME   100000

#if 0
//-----------------------------------------------------------------
#pragma mark -
#pragma mark Function implementations
//-----------------------------------------------------------------
#endif

// array of ThreadInfo: one per thread
ThreadInfo* info;

int main (int argc, char* argv[])
{
	//	The pthread_t data type is implementation dependent.  The pthread 
	//	specifications only say that there must exist a type pthread_t but leaves it
	//	up to the implementers of the library to decide what it is.  It could
	//	a simple unsigned int (or long), as on Linux or a pointer to a struct
	//	(as on mac OS, where the struct is "opaque," meaning that we don't know
	//	what it stores).
	info = calloc(THREAD_COUNT, sizeof(ThreadInfo));;
	
	// initialize thread info structs
	for (int i = 0; i < THREAD_COUNT; i++) {
	
		info[i].index = i;
	}

	// create the threads
	for (int i = 0; i < THREAD_COUNT; i++) {
	
		printf("About to create Thread %d\n", i);

		//							  pointer to	pointer to	  pointer to     pointer to
		//							  a pthread_t   conf. param.  thread func    data
		int errCode = pthread_create (&(info[i].threadID), NULL,  myThreadFunc,  info + i);
		//									^
		//									|
		//									+--- threadID[i] set by pthread library
			
		if (errCode != 0)
		{
			printf ("could not pthread_create thread %d. %d/%s\n",
					 i, errCode, strerror(errCode));
			exit (EXIT_FAILURE);
		}
		
		usleep (MAIN_SLEEP_TIME);
	}

	//	possibly do some work
	
	//	Wait for all the threads to terminate.  Note that we will join
	//	the threads in the order in which this loop runs, not in the
	//	order in which the threads effectively terminates.  This is
	//	because pthread_join is what is caled a "blocking" call.
	for (int i = 0; i<THREAD_COUNT; i++)
	{
		void* w;
		pthread_join(info[i].threadID, &w);
		
		printf("Thread %d joined. Its w array stored: %d, %d, %d, %d\n", i, 
				info[i].v[0], info[i].v[1], info[i].v[2], info[i].v[3]);
		
	}
	
	return EXIT_SUCCESS;

} // main

// information to tell the thread what to do.  For all
//	purposes, this is the "main" function of the threads
//	that we are going to create.
void* myThreadFunc(void* argument)
{
	//	This is here to exhibit a problem with this code.  Increase the sleep time to 
	//	see what hapens
	usleep (THREAD_INIT_SLEEP_TIME);

	//	I know that the argument I received is really a pointer to
	//	an int (because I wrote the code, or read the specs, and
	//	therefore I can cast the pointer I received back to a pointer
	//	to int, and dereference that pointer.
	ThreadInfo* data = (ThreadInfo*) argument;
	int tNum = data->index;

	//	This call only works on Linux and returns the thread id (as seen by the OS)
	//pid_t tId = gettid();

	//	The system call pthread_self() works for any implementation of Pthreads,
	//	however, there are no "hard" specs for the pthread_t data type.
	//	It could be as simple as an int and store the thread id, but it may also
	//	be a pointer to an "opaque" struct with lots of undocumented data.
	//	On macOS, the field __sig of the struct stores the thread id as a long.
	pthread_t tInfo = pthread_self();
	(void) tInfo;
	//long tID = tInfo->__sig;
	
	printf("\tThread %d created\n\t================\n", tNum);

	//	My threads will do some silly calculations, store their results into
	//	an array, and return the head pointer of that array as return value
	//	of the thread function.  There are many other ways to return values.
	//	I could have allocated an array of <insert data type> and let the
	//	threads access that array through their own thread index (index for
	//	the process, not the tread id for the OS).
	data->v = (int*) calloc(4, sizeof(int));
	data->v[0] = tNum;
	data->v[1] = tNum*tNum;
	//	Here, because I used calloc, I don't need to initialize v[2] to 0
	data->v[3] = data->v[0];

	for (int i = 0; i < LOOP_COUNT; i++)
	{
		printf ("\t\t--thread %d counting %d\n", tNum, i);
		//	 do something to elements of v
		data->v[2] += 1;
		data->v[3] *= 2;
		//	the u in usleep would like to look like µ, for micro.
		//	This is a traditional cutie abbreviation from the
		//	Unix world.
//		usleep (THREAD_COMP_SLEEP_TIME);
	}

	printf ("\tThread %d done, leaving myThreadFunc\n", tNum);

	//	Remember that the array has been allocated on the heap,
	//	so the recipient of this value will be responsible for
	//	the housecleaning when the array is not needed anymore.
	return NULL;

} // end of threadFunction  --> thread terminates

