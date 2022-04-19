//
//  main.cpp
//  Threads and Tasks
//
//  Created by Jean-Yves Hervé [Local] on 2016-04-27.
//	Revised 2020-03-29, 2020-11-11, 2021-04-03


//------------------------------------------
//	new style pthreads (no parameter) C++11
//------------------------------------------

#include <iostream>
#include <thread>
#include <unistd.h>

using namespace std;

bool keepGoing = true;

void threadFunc(void)
{
	cout << "In the thread function" << endl;
	
	for (int k=0; k<100; k++)
	{
		cout << "\tthread counting " << k << endl;
		usleep(20000);
		if (!keepGoing)
			return;
	}

	//	do stuff
	usleep(2000000);

	cout << "Thread terminated" << endl;
}

int main(int argc, const char* argv[])
{
	thread myThread(threadFunc);

	cout << "Thread launched from main\n";
	
	//	do stuff
	for (int k=0; k<10; k++)
	{
		cout << "main thread counting " << k << endl;
		usleep(100000);
	}
	
	keepGoing = false;
	
	//	Wait for the thread to terminate
	myThread.join();
	cout << "Thread joined in main\n";

	return 0;
}

