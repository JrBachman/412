//
//  main.cpp
//  Threads with mutex
//
//	This small program shows how a randomized, multithreaded algorithm can
//	(unefficiently, and not quite completely) sort an array.
//	Each thread keeps repeating
//		- generate a random index i in the array
//		- if elements at array[i] and array[i+1] are not properly ordered, swap them
//	until the array is sorted.
//
//	That last part, figuring out that the array is effectively ordered, is a bit iffy.
//	What I do right now is that a thread that hasn't found out-of-order elements for a while
//	just terminates.  Hopefully, by the time they all end the array has been sorted.
//
//  Created by Jean-Yves Hervé on 2016-04-25.
//							   modified 2019-11-04, 2021-11-09

#include <stdio.h>			// for printf
#include <pthread.h>		// for pthread_* calls
#include <string.h>			// for strerror()
#include <unistd.h>			// for usleep()
#include <stdlib.h>			// for exit()
#include <sys/types.h>		// for gettid() on Linux

//----------------------------
//	Custom Data Types
//----------------------------

typedef struct ThreadInfo
{
	pthread_t threadID;
	int threadIndex;
	//
	//	whatever other input data
	//
	
} ThreadInfo;

//----------------------------
//	Function prototypes
//----------------------------
void* threadFunc(void* param);
void randomizeArray(void);

//----------------------------
//	Global Variables
//----------------------------

//	this is the array that we are going to sort using a multithreaded
//	randomized, bubble sort-inspired algorithm
#define ARRAY_SIZE  30
int array[ARRAY_SIZE];

//	Number of threads that will be doing randomized sorting
#define NUM_THREADS  10

//	A thread that hasn't encountered an out-of-order pair in that
//	many trials will simply terminate.  This is a completely made up
//	number.  Something better thought out, based on the size of the
//	array and number of threads would be better, but this is not meant
//	to be a real solution to the sorting problem.
#define ENOUGH_TRIALS 30

//	this is the mutex lock that we will use to control the access by all
//	the threads to the array.
pthread_mutex_t myDataLock;
//	This mutex lock is there to control access to the output console.
pthread_mutex_t myOutputLock;



int main()
{
	//	seed the random generator
	srand ((unsigned int) time(NULL));
	
	//	Generate a permutation of the integers 1..ARRAY_SIZE
	randomizeArray();
	
	//	initialize the mutex lock.  Here, I use the default initialization
	//	(the mutex is unlocked, so the first thread to ask for access will
	//	gain it.)
	pthread_mutex_init(&myDataLock, NULL);
	pthread_mutex_init(&myOutputLock, NULL);

	//	ThreadInfo is our own application-specific data structure that stores
	//	information that we need to pass to the threads.
	ThreadInfo* threadInfo = (ThreadInfo*) calloc(NUM_THREADS, sizeof(ThreadInfo));
	for (int k=0; k<NUM_THREADS; k++)
	{
		threadInfo[k].threadIndex = k;
	}		
	
	//	Now we can create our threads, passing each a pointer to its own ThreadInfo struct
	for (int k=0; k<NUM_THREADS; k++)
	{
		int err = pthread_create(&(threadInfo[k].threadID), NULL, threadFunc, threadInfo + k);
		if (err != 0)
		{
			printf("Could not create Thread %d. [%d]: %s\n", k, err, strerror(err));
			exit(1);
		}
	}

	//	Now, wait for the threads to terminate
	for (int k=0; k<NUM_THREADS; k++)
	{
		//	The second argument to the pthread_join function can either be
		//		- a void** pointer, to catch the value returned by the thread function
		//		- NULL if that return value is ignored
		//	On the other hand, the first argument (pointer to a pthread_t) cannot be NULL
		//	From pthread_join documentation:
		//	 	There is no pthreads analog of waitpid(-1, &status, 0), that is,
		//		"join with any terminated thread".  If you believe you need this
       	//		functionality, you probably need to rethink your application design.
		pthread_join(threadInfo[k].threadID, NULL);

		pthread_mutex_lock(&myOutputLock);
		printf("Thread %d joined.\n", k);
		pthread_mutex_unlock(&myOutputLock);
	}
	
	// Here I could destroy (or rather "de-init") my lock
	pthread_mutex_destroy(&myDataLock);
	pthread_mutex_destroy(&myOutputLock);
	
	return 0;
}


//	This is my thread function.  I don't really have anything to
//	get back from it (my array is a global variable), so the
//	return value will be NULL.
void* threadFunc(void* param)
{
	//	The parameter is the a pointer to a ThreadInfo struct
	ThreadInfo* info = (ThreadInfo*) param;
	pthread_mutex_lock(&myOutputLock);
	printf("Thread %d launched\n", info->threadIndex);
	pthread_mutex_unlock(&myOutputLock);
	
	int countSinceLastSwap = 0;
	do
	{
		//	pick a random position
		int i = rand() % (ARRAY_SIZE-1);
		
		//	enter critical section
		pthread_mutex_lock(&myDataLock);
		
		//	if two elements are out of order
		if (array[i] > array[i+1])
		{
			//	swap the elements
			int temp = array[i];
			array[i] = array[i+1];
usleep(5000);

			array[i+1] = temp;
			// reset the counter;
			countSinceLastSwap = 0;
		}
		//	The elements were in order --> increment the counter
		else
		{
			countSinceLastSwap++;
		}

		//	leave critical section
		pthread_mutex_unlock(&myDataLock);
		
		
		//	if there was a swap, then printout the array
		if (countSinceLastSwap == 0)
		{
			//	right now, for debugging purposes, print out the current state of the array
			pthread_mutex_lock(&myOutputLock);
			printf("Thread %d swap: [", info->threadIndex);
			for (int k=0; k< ARRAY_SIZE; k++)
			{
				printf("%3d", array[k]);
usleep(5000);
			}
			printf("]\n");
			pthread_mutex_unlock(&myOutputLock);
		}
	}
	while (countSinceLastSwap < ENOUGH_TRIALS);
	
	pthread_mutex_lock(&myOutputLock);
	printf("Thread %d terminates\n", info->threadIndex);
	pthread_mutex_unlock(&myOutputLock);

	return NULL;
}


//	This is not a particularly efficient way to generate a
//	random permutaiton of an array, but, again, this is not
//	the purpose of this code sample.
void randomizeArray(void)
{
	//	Initialize with values 1..ARRAY_SIZE
	for (int k=0; k<ARRAY_SIZE; k++)
		array[k] = k+1;
	
	//	Repeat many times
	for (int i=0; i<3*ARRAY_SIZE; i++)
	{
		//	pick two random elements of the array and swap them
		int j = rand() % ARRAY_SIZE;
		int k = rand() % ARRAY_SIZE;
		if (i != j)
		{
			int temp = array[j];
			array[j] = array[k];
			array[k] = temp;
		}
	}

	printf("Initial array: [");
	for (int k=0; k< ARRAY_SIZE; k++)
		printf("%3d", array[k]);
	printf("]\n");
}

