//
//  main.cpp
//  This is a somewhat cleaned-up version of standard thread creation & joining,
//	to use for a C++ program.
//
//  Created by Jean-Yves Hervé on 2019-11-07.
//	Revised 2020-03-20

#include <iostream>
#include <cstdlib>
#include <cstring>       // for strerror()
#include <unistd.h>      // for usleep()
#include <sys/time.h>
#include <pthread.h>

using namespace std;

#if 0
//-----------------------------------------------------------------
#pragma mark -
#pragma mark Custom data types
//-----------------------------------------------------------------
#endif

// A struct data type to pass information to the thread and
//	possibly get values back from it.
using ThreadInfo = struct  {
	//	managed by pthreads library
    pthread_t   	threadID;
    //	 our data
    unsigned short	index;	
    bool detachYourself;
    //	application-dependent
    unsigned int timeToSleep;
};

#if 0
//-----------------------------------------------------------------
#pragma mark -
#pragma mark Function prototypes
//-----------------------------------------------------------------
#endif

void* someFunctionName (void* argument);


#if 0
//-----------------------------------------------------------------
#pragma mark -
#pragma mark Global variables
//-----------------------------------------------------------------
#endif

//	Array of ThreadInfo structs.  To be allocated dynamically
ThreadInfo* threadInfo;

#if 0
//-----------------------------------------------------------------
#pragma mark -
#pragma mark Function implementations
//-----------------------------------------------------------------
#endif


int main (int argc, char *argv[])
{
	unsigned short numThreads = 6;

	//	We are programming in C++, so I may as well allocate in C++ style
    //	threadInfo = (ThreadInfo*) calloc(numThreads, sizeof(ThreadInfo));
    threadInfo = new ThreadInfo[numThreads];
	
	//	initialize all thread info structs that will be sent as argument to 
	//	the thread function
    for (unsigned short i = 0; i < numThreads; i++)
	{
		//	Note: the threadID field will be initialized by the pthread library
		//	when the thread is created.
        threadInfo[i].index = i;
	    threadInfo[i].detachYourself = false;
	
	    //	application-dependent
    	threadInfo[i].timeToSleep = (numThreads - i)*500000;
	}
	
    // create the threads
    for (unsigned int i = 0; i < numThreads; i++)
	{
		cout << "main function creating thread " << i << endl;

		int errCode = pthread_create (&threadInfo[i].threadID, nullptr,
                                 		someFunctionName, threadInfo + i);
        
        if (errCode != 0)
        {
            cerr << "could not pthread_create thread " << i <<
					 ", Error code " << errCode << ": " << strerror(errCode) << endl;
            exit (EXIT_FAILURE);
        }
    }

	//	At this point, all the threads have been created and
	//	are busy doing their thing
	//	Possibly, the main thread could have computations of its
	//	own to perform here.  This one doesn't.

    //	Now rendezvous with all the non-detached threadInfo
    for (int i = 0; i<numThreads; i++) 
    {
    	//	Try to join all joinable threads (threads that didn't detach themselves)
        if (!threadInfo[i].detachYourself)
        {
        	//	Note that here I *know* that my threads don't return anything.  Otherwise
        	//	I might get in trouble if the pointer I pass is invalid
	        void* retVal;
            int errCode = pthread_join(threadInfo[i].threadID, &retVal);
            if (errCode != 0) 
            {
			   cerr << "error joining thread " << i <<
						", Error code " << errCode << ": " << strerror(errCode) << endl;
            }
            else 
            {
				cout << "joined with thread " << i << endl;
				cout << flush;
			}
        }
    }

	//	Not really needed because the array will get freed when the process
	//	terminates and its partition gets freed, but some people like to free
	//	explicitly all the things that they have allocated, in reverse order
	//	from the order in which they were allocated.  The explicitly freeing
	//	actually helps detect problems:  If you crash while freeing, them either
	//	something went wrong with your pointers somewhere, or you don't understand
	//	what they point to (e.g. our "scaffolded" 2D raster).  The reverse order
	//	bit, on the other hand, is pure superstition/fetishism.
	//free(threadInfo);		//	C style
	delete []threadInfo;	//	C++ style. Don't mix them (calloc + delete, or new + free)

	return 0;
} // main


//	This is my thread function.
void* someFunctionName (void* argument)
{
	//------------------------------------------------
	//	Part 1: Extract argument data
	//------------------------------------------------
	
	//	I cast back the argument to the proper pointer type
    ThreadInfo* info = (ThreadInfo *) argument;

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
		cerr << "could not gettimeofday at start of thread " << info->index <<
				", Error code " << timeErr << ": " << strerror(timeErr) << endl;
		exit (EXIT_FAILURE);
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
			cerr << "could not detach thread " << info->index <<
					", Error code " << detachErr << ": " << strerror(detachErr) << endl;
			exit (EXIT_FAILURE);
       }
	}
	
	//------------------------------------------------
	//	Part 4: Time to do some work
	//------------------------------------------------
	usleep(info->timeToSleep);
	
	//------------------------------------------------
	//	Part 5: Before leaving, check the time again
	//------------------------------------------------
	struct timeval endTime;

	//	presumably we haven't changed time zone while running this demo,
	//	so I recycle my timezone struct and error code variable.
	timeErr = gettimeofday(&endTime, &weAreHere);
	if (timeErr != 0)
	{
		cerr << "could not gettimeofday at end of thread " << info->index <<
				", Error code " << timeErr << ": " << strerror(timeErr) << endl;
		exit (EXIT_FAILURE);
	}

	unsigned long timeSpent = (endTime.tv_sec*1000000 + endTime.tv_usec) - startTimeMicroSec;
	unsigned long timeSpentSec = timeSpent / 1000000;
	unsigned long timeSpentRemainder = timeSpent % 1000000;
	unsigned long timeSpentMilliSec = timeSpentRemainder / 1000;
	unsigned long timeSpentMicroSec = timeSpentRemainder % 1000;

    cout << "\tthread " << info->index << " done, and it took " << timeSpentSec <<
			" s, " << timeSpentMilliSec << "ms, and " << timeSpentMicroSec <<
			"µs getting there" << endl;
	cout << flush;

	//	I showed in the previous code sample how I could return an array.
	//	This time I return nothing.  Note that you can also use the
	//	argument struct to store any values you want to return.
    return (nullptr);
} // threadFunction

