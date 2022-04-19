//
//  main.cpp
//  Threads and Tasks
//
//  Created by Jean-Yves Hervé [Local] on 2016-04-27.
//	Revised 2020-03-29, 2020-11-11, 2021-04-03


//-----------------------------------------------
//	new style pthreads (with i/o param)  C++11
//-----------------------------------------------

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <unistd.h>
#include <pthread.h>

using namespace std;

struct ThreadStruct 
{
	int param1;
	float param2;
	float param3;
	int outputVal1;
	vector<float> outputVal2;
	//	only needed when I use pthread to compare
	pthread_t threadID;
};


//	We will see this next week
mutex myLock;




void threadFunc(void* data) noexcept;

void* threadFuncWrap(void* data) {
	threadFunc(data);
	return nullptr;
}

void threadFunc(void* dataV) noexcept
{
	ThreadStruct* data = static_cast<ThreadStruct*>(dataV);
	
	cout << "In the thread function" << endl;
	
	cout	<< "Received data struct with param1 = " << data->param1
				<< ", param2 = " << data->param2
				<< ", param3 = " << data->param3 << endl;
	
	//	do stuff
	usleep(2000000);
	
	myLock.lock();
	
	data->outputVal1 = 2*data->param1;
	data->outputVal2 = vector<float>(data->outputVal1);
	for (int k=0; k<data->outputVal1; k++)
		data->outputVal2[k] = 1000.f*data->param2 + k*data->param3;
	
		myLock.unlock();

	//	noting to return
	
	cout << "The thread terminated" << endl;
}

void anotherThreadFunc(ThreadStruct* data)
{
	cout << "In the thread function" << endl;
	
	cout	<< "Received data struct with param1 = " << data->param1
				<< ", param2 = " << data->param2
				<< ", param3 = " << data->param3 << endl;
	
	//	do stuff
	
	data->outputVal1 = 2*data->param1;
	data->outputVal2 = vector<float>(data->outputVal1);
	for (int k=0; k<data->outputVal1; k++)
		data->outputVal2[k] = 1000.f*data->param1 + k*data->param2;

	//	noting to return
	
	cout << "The thread terminated" << endl;
}




int main(int argc, const char* argv[])
{
	ThreadStruct myData = {	4, 2.5f, 12.5f	//	params 1, 2, 3
							};
	//	C++ thread
	#if 1
		thread thr(anotherThreadFunc, &myData);

		cout << "Thread launched from main\n";
	
		//	do stuff

		//	Wait for the thread to terminate
		thr.join();

	//	pthread
	#else
		int errCode = pthread_create(&(myData.threadID), NULL, threadFuncWrap, &myData);

		cout << "Thread launched from main\n";

		void* retVal;
		errCode = pthread_join(myData.threadID, &retVal);
	#endif

	cout << "Joined in main thread, got data back:" << endl;
	cout << "\treturn1:\t" << myData.outputVal1 << endl;
	cout << "\treturn2:\t";
	for (int k=0; k<myData.outputVal2.size(); k++)
	{
		if (k>0)
			cout << "\t\t\t";
		cout << myData.outputVal2[k] << endl;
	}

	return 0;
}

