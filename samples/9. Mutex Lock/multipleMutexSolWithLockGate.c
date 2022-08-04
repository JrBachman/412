//
//  main.cpp
//  Threads with multiple mutex
//	I also add a "lock gate" so that all my threads actually start running 
//	and doing work at about the same time.
//
//	One thing to keep in mind is that pthread_create(), although much faster than
//	fork() is still considerably slower than a regular computation statement.  Once
//	a thread has been created, it starts executing the code of its function.  This 
//	means that if you create several threads, by the time the last thread has been
//	created, the first one may already have executed millions of statements.  Sometimes
//	we don't care.  Other times, we would like all our threads to really syart working
//	around the same time (this is often the case in simulation applications).  So we need
//	a mechanism to stop a thread and restart it fast.  

//	Mutex locks let us do that. The idea is to assign a mutex lock to each thread 
//	(it's convenient for us to store that lock in the thread's threadInfo struct). 
//	We are going to "pre-lock" the mutex, so that the first time the thread tries to
//	acquire its own lock, it will block, and will only resume execution when another
//	thread releases the lock. 
//
//	As usual with my code samples that come in series, all the new stuff in the
//	source file is always preceded by a comment line //new
//
//  Created by Jean-Yves Hervé on 2020-11-21

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
	//new
	//	Each thread now has its own lock
	pthread_mutex_t lock;
	
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
#define ENOUGH_TRIALS 15

//new
//	We now have one mutex lock for each array element.
pthread_mutex_t myLock[ARRAY_SIZE];

//	This mutex lock is there to control access to the output console.
pthread_mutex_t myOutputLock;

int main()
{
	//	seed the random generator
	srand ((unsigned int) time(NULL));
	
	//	Generate a permutation of the integers 1..ARRAY_SIZE
	randomizeArray();
	
	//	initialize array of the mutex locks.  Here, I use the default initialization
	//	(the mutex is unlocked, so the first thread to ask for access will
	//	gain it.)
	for (int k=0; k<ARRAY_SIZE; k++)
	{	
		pthread_mutex_init(myLock + k, NULL);
	}
	
	//	initialize the console lock
	pthread_mutex_init(&myOutputLock, NULL);
	
	ThreadInfo* threadInfo = (ThreadInfo*) calloc(NUM_THREADS, sizeof(ThreadInfo));
	for (int k=0; k<NUM_THREADS; k++)
	{
		threadInfo[k].threadIndex = k;
		//new
		//	I need to initialize each thread's lock
		pthread_mutex_init(&(threadInfo[k].lock), NULL);
		//	In this application, I need my locks pre-locked, so that the first time
		//	my thread tries to acquire its own lock it will block
		pthread_mutex_lock(&(threadInfo[k].lock));		
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

	//new
	//	Right now, I know that all the threads that I created are blocked own their
	//	own lock (here I am talking as the main thread).  I need to unleash them all
	//	by unlocking their locks
	for (int k=0; k<NUM_THREADS; k++)
	{
		pthread_mutex_unlock(&(threadInfo[k].lock));
		
	}		
	//	now all threads are running	

	//	Just once, sometime during the execution, the main thread tries to print the
	//	current content of the array.  To do this, it must acquire the lock on *all*
	//	the elements of the array, and also the lock to the console.  
	//	In practice, this rarely going to work before the sorting 
	//	threads terminate.
	usleep(5000);
	pthread_mutex_lock(&myOutputLock);
	printf("\nMidpoint array: [");
	for (int k=0; k< ARRAY_SIZE; k++)
		printf("%3d", array[k]);
	printf("]\n\n");
	pthread_mutex_unlock(&myOutputLock);

	//	Now, wait for the threads to terminate
	for (int k=0; k<NUM_THREADS; k++)
	{
		pthread_join(threadInfo[k].threadID, NULL);
		printf("Thread %d joined.\n", k);
	}

	//	Print the final state of the array
	printf("\nFinal array: [");
	for (int k=0; k< ARRAY_SIZE; k++)
		printf("%3d", array[k]);
	printf("]\n");

	// Here I could destroy (or rather "de-init") my lock (I let my threads take
	//	care of their own lock)
	for (int k=0; k<ARRAY_SIZE; k++)
	{
		pthread_mutex_destroy(myLock + k);
	}
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
	printf("Thread %d launched\n", info->threadIndex);
	
	//new
	//	The first thing I do (the print statement above is there only for demo/debugging
	//	purposes) is to block on my own lock (I talk hare as the sorting thread)
	pthread_mutex_lock(&(info->lock));
	
	int countSinceLastSwap = 0;
	do
	{
		//	I need these variable because I don't want to perform console output
		//	while still holding locks to the array's elements
		int didSwap = 0;
		int swapAt_i = -1;
		int valAt_i = 0, valAt_ip1 = 0;
		
		//	pick a random position
		int i = rand() % (ARRAY_SIZE-1);

		//new
		//	enter critical section for elements i and i+1
		pthread_mutex_lock(myLock + i);
		pthread_mutex_lock(myLock + i+1);
		
		//	if two elements are out of order
		if (array[i] > array[i+1])
		{
			//	swap the elements
			int temp = array[i];
			
			//new
			//	store values to minimize duration of CS and reduce risks of deadlock
			//	(prevent hold-and-wait)
			didSwap = 1;
			swapAt_i = i;
			valAt_i = array[i] = array[i+1];
			valAt_ip1 = array[i+1] = temp;

			// reset the counter;
			countSinceLastSwap = 0;
			
			//new
			//	We can't anymore print routinely the content of the array because this
			//	requires gaining access to the lock of all the elements.
		}
		//	The elements were in order --> increment the counter
		else
		{
			//new
			didSwap = 0;
			
			countSinceLastSwap++;
		}

		//	leave critical section
		pthread_mutex_unlock(myLock + i+1);
		pthread_mutex_unlock(myLock + i);

		//new
		//	We can't anymore print routinely the content of the array because this
		//	requires gaining access to the lock of all the elements.
		//  Furthermore,
		//	In the first version the array lock also worked indirectly as a lock
		//	for the console.  Now we need to lock the console before we can print
		//	into it.
		if (didSwap)
		{
			pthread_mutex_lock(&myOutputLock);
			printf("\tThread %d swap at index %2d: %2d and %2d\n", info->threadIndex,
					swapAt_i, valAt_i, valAt_ip1);
			pthread_mutex_unlock(&myOutputLock);
		}
	}
	while (countSinceLastSwap < ENOUGH_TRIALS);
	
	//new
	//	Before this thread terminates, it destroys (or de-inits) its lock
	pthread_mutex_destroy(&(info->lock));

	printf("Thread %d terminates\n", info->threadIndex);
	
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
