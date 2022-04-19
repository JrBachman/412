//
//  main.cpp
//  Threads and Tasks
//
//  Created by Jean-Yves Hervé [Local] on 2016-04-27.
//	Revised 2020-03-29, 2020-11-11, 2021-04-03

//-----------------------------------------------
//	new style pthreads (with parameter) C++11
//-----------------------------------------------

#include <iostream>
#include <thread>
#include <unistd.h>

using namespace std;

struct DataStruct 
{
	int val;
	float x;
};

void threadFunc(DataStruct data, int n)
{
	cout << "In the thread function, received val=" << data.val << " and x=" << data.x << endl;
	cout << "In the thread function, received n=" << n  << endl;
	
	//	do stuff
	usleep(2000000);

	cout << "Thread terminated" << endl;
}

int main(int argc, const char* argv[])
{
	DataStruct d = {8, 3.5f};
	thread thr(threadFunc, d, 2);
	//	argh!
	//	thread thr(threadFunc, 8, 3.5f, 10);

	cout << "Thread launched from main\n";
	
	//	do stuff

	//	Wait for the thread to terminate
	thr.join();
	cout << "Thread joined in main\n";

	return 0;
}

