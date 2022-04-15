//
//  mutexReminder.c
//  Not really a functional program, just an example of how to declare, initialize,
//	acquire, and release a mutex lock in the pthread library.
//
//  Created by Jean-Yves Hervé on 2020-04-13, rev. 2021-11-07

#include <stdio.h>			// for printf
#include <pthread.h>		// for pthread_* calls
#include <string.h>			// for strerror()
#include <unistd.h>			// for usleep()
#include <stdlib.h>			// for exit()
#include <sys/types.h>		// for gettid() on Linux



typedef struct ThreadInfo 
{
	pthread_t  threadID;
	int index;
	pthread_mutex_t threadLock;
	//
	//	...
	
} ThreadInfo;

//	Because mutex locks need to be accessed by the threads, you will need to either
//	declare them global in your program or store a copy of the mutex in the Thread info
//	struct

// A lock initialized by pthread_mutex_init
pthread_mutex_t myLock;
//	A lock initialized "statically"
pthread_mutex_t myOtherLock = PTHREAD_MUTEX_INITIALIZER;

//	Of course, nothing prevents you from having an array of locks
pthread_mutex_t* lockArray;




int main(int argc, char* argv[])
{
	//	initialize the mutex lock.  Here, I use the default initialization
	//	(the mutex is unlocked, so the first thread to ask for access will
	//	gain it.)
	pthread_mutex_init(&myLock, NULL);

	// allocate n locks
	lockArray = (pthread_mutex_t*) calloc(n, sizeof(pthread_mutex_t));
	for (int i=0; i<n; i++)
		pthread_mutex_init(lockArray + i);
	//lockArray[0] = 	PTHREAD_MUTEX_INITIALIZER;

	//	acquire the lock --> block if the lock is not free
	pthread_mutex_lock(&myLock);
	
	//	Critical section
	
	//	release the lock
	pthread_mutex_unlock(&myLock);
	
	//	basically "un-init" the lock
	pthread_mutex_destroy(&myLock);
	
	//	re-initalizes the lock
	pthread_mutex_init(&myLock, NULL);
	
	return 0;
}


