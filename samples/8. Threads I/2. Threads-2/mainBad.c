//
//  main.cpp
//  Threads-2
//
//  Created by Jean-Yves Hervé on 2016-04-25.
//	revised 2020-03-20

#include <stdio.h>		   // for printf
#include <stdlib.h>        // for exit
#include <pthread.h>       // for pthread_* calls
#include <string.h>        // for strerror()
#include <unistd.h>        // for usleep()
#include <sys/time.h>

#define THREAD_COUNT 6

#if 0
//-----------------------------------------------------------------
#pragma mark -
#pragma mark Custom data types
//-----------------------------------------------------------------
#endif


// A struct data type to pass information to the thread and
//	possibly get values back from it.
typedef struct ThreadInfo {

    pthread_t   threadID;			//	this is the thread's id for the OS
    //--------------------------
    //	application-specific
    //--------------------------
    int         index;				//	this is the thread's index in the app
    //
    int         numberToCountTo;
    int         detachYourself;
    int         sleepTime;      // in microseconds
    float*		outputStuff;
} ThreadInfo;

#if 0
//-----------------------------------------------------------------
#pragma mark -
#pragma mark Function prototypes
//-----------------------------------------------------------------
#endif

void* threadFunc (void* argument);


#if 0
//-----------------------------------------------------------------
#pragma mark -
#pragma mark Global variables: Data accessible to all threads
//-----------------------------------------------------------------
#endif

//	Array of ThreadInfo structs.  To be allocated dynamically.  Here I declare it
//	as a global variable, so that it is accessible from within all functions.  In this
//	particular example, it's not really necessary, because each thread gets all it needs
//	from its own struct.  In some applications, a thread may need to take a peek at 
//	another thread's data (preferably to data that is read-only).
ThreadInfo* threads;

#if 0
//-----------------------------------------------------------------
#pragma mark -
#pragma mark Function implementations
//-----------------------------------------------------------------
#endif

int main (int argc, char *argv[])
{

    int errCode;
//C
    threads = (ThreadInfo*) calloc(THREAD_COUNT, sizeof(ThreadInfo));
//C++
//    threads = new ThreadInfo[THREAD_COUNT];

	//	initialize all thread info structs
    for (int i = 0; i < THREAD_COUNT; i++)
	{
		//	But first, initialize the ThreadInfo struct that will
		//	be sent as parameter to the thread function
        threads[i].index = i;
        threads[i].numberToCountTo = (i + 1) * 2;
        threads[i].detachYourself = (i % 2); // detach odd threads
        threads[i].sleepTime = 2000000 - 300000 * i;
	}
	
    // create the threads
    for (int i = 0; i < THREAD_COUNT; i++)
	{
		printf("main function creating thread %d\n", i);

//								pointer to			thread
///								pthread_t		configuration    thread     pointer to
//								set by library		params      function	thread arguments
//					
        errCode = pthread_create (&threads[i].threadID, NULL, threadFunc, threads + i);
//        threadFunc(threads + i);
        
        if (errCode != 0)
        {
            fprintf (stderr,
                     "could not pthread_create thread %d. %d/%s\n",
                     i, errCode, strerror(errCode));
            exit (EXIT_FAILURE);
        }

        void* retVal = NULL;
		errCode = pthread_join(threads[i].threadID, &retVal);
		if (errCode != 0) {
		   fprintf (stderr, "error joining thread %d. %d/%s\n",
					 i, errCode, strerror(errCode));
		}
		else {
			printf ("joined with thread %d\n", i);
		}
    }

    exit (EXIT_SUCCESS);

} // main


void* threadFunc (void* argument)
{
	//------------------------------------------------
	//	Part 1: Extract argument data
	//------------------------------------------------
	
	//	I cast back the argument to the proper pointer type
    ThreadInfo* info = (ThreadInfo *) argument;

	//	And print to the console
    printf ("\tthread %d, counting to %d, sleep time %dµs, detaching: %s\n",
            info->index, info->numberToCountTo, info->sleepTime,
            (info->detachYourself) ? "yes" : "no");

	//------------------------------------------------
	//	Part 2
	//------------------------------------------------
	//	For whatever reason I would like to record the time spent between
	//	thread creation and end.  So we start this calculation here.
	//	I use the gettimeofday function that should exist on any Unix-like
	//	platform.

	//	struct timeval {
	//		time_t      tv_sec;     /* seconds */
	//		suseconds_t tv_usec;    /* microseconds */
	//	};
	struct timeval startTime;

	//	struct timezone {
	//		int tz_minuteswest;     /* minutes west of Greenwich */
	//		int tz_dsttime;         /* type of DST correction */
	//	};
	struct timezone weAreHere;
	
	int timeErr = gettimeofday(&startTime, &weAreHere);
	if (timeErr != 0)
	{
		fprintf( stderr, "could not gettimeofday at start of thread %d. Error code %d --> %s\n",
				 info->index, timeErr, strerror(timeErr));
	}
	unsigned long startTimeMicroSec = startTime.tv_sec*1000000 + startTime.tv_usec;

	//------------------------------------------------
	//	Part 3
	//------------------------------------------------
	//	If the thread should be detached, do it.
    if (info->detachYourself)
	{
        int detachErr = pthread_detach (pthread_self());
        if (detachErr != 0)
		{
            fprintf( stderr, "could not detach thread %d. Error code %d --> %s\n",
                     info->index, detachErr, strerror(detachErr));
        }
    }
	
	//------------------------------------------------
	//	Part 4: Time to do some work
	//------------------------------------------------
    // now to do the actual "work" of the thread
    for (int i = 0; i < info->numberToCountTo; i++)
	{
        printf ("\t\tthread %d counting %d / %d\n",
				info->index, i, info->numberToCountTo);
        usleep (info->sleepTime);
    }

	//------------------------------------------------
	//	Part 5: Before leaving, check the time again
	//------------------------------------------------
	struct timeval endTime;

	//	presumably we haven't changed time zone while running this demo,
	//	so I recycle my timezone struct and error code variable.
	timeErr = gettimeofday(&endTime, &weAreHere);
	if (timeErr != 0)
	{
		fprintf( stderr, "could not gettimeofday at end of thread %d. Error code %d --> %s\n",
				 info->index, timeErr, strerror(timeErr));
	}

	unsigned long timeSpent = (endTime.tv_sec*1000000 + endTime.tv_usec) - startTimeMicroSec;
	unsigned long timeSpentSec = timeSpent / 1000000;
	unsigned long timeSpentRemainder = timeSpent % 1000000;
	unsigned long timeSpentMilliSec = timeSpentRemainder / 1000;
	unsigned long timeSpentMicroSec = timeSpentRemainder % 1000;

    printf( "\tthread %d done, and it took %ld s, %ld ms, and %ld µs getting there\n",
			info->index, timeSpentSec, timeSpentMilliSec, timeSpentMicroSec);

	//	I showed in the previous code sample how I could return an array.
	//	This time I return nothing.  Note that you can also use the
	//	argument struct to store any values you want to return.  Actually, this 
	//	is the way I personally prefer to do it.
    return (NULL);

} // threadFunction

