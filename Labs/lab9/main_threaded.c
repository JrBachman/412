//
//  main.cpp
//  Threads with no mutex
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
//							   modified 2020-11-04

#include <stdio.h>			// for printf
#include <pthread.h>		// for pthread_* calls
#include <string.h>			// for strerror()
#include <unistd.h>			// for usleep()
#include <stdlib.h>			// for exit() and itoa
#include <sys/types.h>		// for gettid() on Linux

typedef struct ThreadInfo
{
	pthread_t sortingThread;
	
	unsigned int threadIndex;
	//
	//	whatever other input or output data may be needed
	//
} ThreadInfo;

//----------------------------
//	Function prototypes
//----------------------------
void* threadFunc(void* param);
void randomizeArray(void);
void printArray(unsigned int threadIndex);

//----------------------------
//	Global variables
//----------------------------

//	this is the array that we are going to sort using a multithreaded
//	randomized, bubble sort-inspired algorithm
#define ARRAY_SIZE  50
unsigned int a[ARRAY_SIZE];

//	Number of threads that will be doing randomized sorting
#define NUM_THREADS  15

//	A thread that hasn't encountered an out-of-order pair in that
//	many trials will simply terminate.  This is a completely made up
//	number.  Something better thought out, based on the size of the
//	array and number of threads would be better, but this is not meant
//	to be a real solution to the sorting problem.
#define ENOUGH_TRIALS 30


void printArray(unsigned int threadIndex)
{
	char line[1024];
	sprintf(line, "T%d: [", threadIndex);
	for (unsigned int k=0; k< ARRAY_SIZE; k++)
	{
		char aStr[8];
		sprintf(aStr, "%d", a[k]);
		strcat(line, aStr);
		if (k < ARRAY_SIZE -1)
			strcat(line, " ");
	}
	printf("%s]\n", line);
	fflush(stdout);
}


int main(int argc, char* argv[])
{
	(void) argc; (void) argv;

	//	seed the random generator
	srand ((unsigned int) time(NULL));
	
	//	Generate a permutation of the integers 1..ARRAY_SIZE
	randomizeArray();
	
	//	ThreadInfo is our own application-specific data structure that stores
	//	information that we need to pass to the threads.
	ThreadInfo* threadInfo = (ThreadInfo*) calloc(NUM_THREADS, sizeof(ThreadInfo));
	for (unsigned int k=0; k<NUM_THREADS; k++)
	{
		threadInfo[k].threadIndex = k;
	}

	for (unsigned int k=0; k<NUM_THREADS; k++)
	{
		int err = pthread_create(&threadInfo[k].sortingThread, NULL, threadFunc, threadInfo + k);
		if (err != 0)
		{
			printf("Could not create Thread %d. [%d]: %s\n", k, err, strerror(err));
			exit(1);
		}
	}

	//	Now, wait for the threads to terminate
	for (unsigned int k=0; k<NUM_THREADS; k++)
	{
		//	The second argument to the pthread_join function can either be
		//		- a void** pointer, to catch the value returned by the thread function
		//		- NULL if that return value is ignored
		//	On the other hand, the first argument (pointer to a pthread_t) cannot be NULL
		//	From pthread_join documentation:
		//	 	There is no pthreads analog of waitpid(-1, &status, 0), that is,
		//		"join with any terminated thread".  If you believe you need this
       	//		functionality, you probably need to rethink your application design.
		pthread_join(threadInfo[k].sortingThread, NULL);

		printf("Thread %d joined.\n", k);
		fflush(stdout);
	}

	printf("Final array: [");
	for (unsigned int k=0; k< ARRAY_SIZE; k++)
		printf("%3d", a[k]);
	printf("]\n");

	//	just freeing to see if something goes bad,  Not really needed.
	free(threadInfo);

	return 0;
}


//	This is my thread function.  I don't really have anything to
//	get back from it (my array is a global variable), so the
//	return value will be NULL.
void* threadFunc(void* param)
{
	//	The parameter is the a pointer to a ThreadInfo struct
	ThreadInfo* info = (ThreadInfo*) param;
	printf("Thread %d launched\n", info->threadIndex);
	
	int countSinceLastSwap = 0;
	do
	{
		//	pick a random position
		unsigned int i = rand() % (ARRAY_SIZE-1);
		
		//	if two elements are out of order
		if (a[i] > a[i+1])
		{
			//	swap the elements
			unsigned int temp = a[i];
			a[i] = a[i+1];
			a[i+1] = temp;
			
			//	print the array right away
			printArray(info->threadIndex);

			// reset the counter;
			countSinceLastSwap = 0;
		}
		//	The elements were in order --> increment the counter
		else
		{
			countSinceLastSwap++;
		}
	}
	while (countSinceLastSwap < ENOUGH_TRIALS);
	
	printf("Thread %d terminates\n", info->threadIndex);
	fflush(stdout);

	return NULL;
}


//	This is not a particularly efficient way to generate a
//	random permutation of an array, but, again, this is not
//	the purpose of this code sample.
void randomizeArray(void)
{
	//	Initialize with values 1..ARRAY_SIZE
	for (unsigned int k=0; k<ARRAY_SIZE; k++)
		a[k] = k+1;
	
	//	Repeat many times
	for (unsigned int i=0; i<3*ARRAY_SIZE; i++)
	{
		//	pick two random elements of the array and swap them
		unsigned int j = rand() % ARRAY_SIZE;
		unsigned int k = rand() % ARRAY_SIZE;
		if (i != j)
		{
			unsigned int temp = a[j];
			a[j] = a[k];
			a[k] = temp;
		}
	}

	printf("Initial array: [");
	for (unsigned int k=0; k< ARRAY_SIZE; k++)
		printf("%3d", a[k]);
	printf("]\n");
}

