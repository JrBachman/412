//
//  main.cpp
//  Threads and Tasks
//
//  Created by Jean-Yves Hervé [Local] on 2016-04-27.
//	Revised 2020-03-29, 2020-11-11, 2021-04-03



//-----------------------------------------------------
//	new style task C++14 [adapted from Cplusplus.com]
//-----------------------------------------------------

// launch::async vs launch::deferred
#include <iostream>     // cout
#include <future>       // async, deferred future, launch
#include <chrono>       // chrono::milliseconds
#include <thread>       // this_thread::sleep_for
#include <unistd.h>

using namespace std;

void print_ten (char c, int ms)
{
	for (int i=0; i<10; ++i)
	{
		this_thread::sleep_for(chrono::milliseconds(ms));
		cout << c;
	}
	cout << "Task terminated\n";
}

int main(int argc, const char* argv[])
{
	//	I separated declaration from initialization so that the first
	//	launch does not appear (unfairly) more complex than the others.
	future<void> foo;
	future<void> bar;

	cout << "with explicit launch::async:\n";
	foo = async(launch::async, print_ten, '*', 100);
	bar = async(launch::async, print_ten, '@', 200);

	// async "get" (wait for foo and bar to be ready):
	bar.get();
	foo.get();
	cout << "\n\n";

	cout << "with explicit launch::deferred:\n";
	foo = async(launch::deferred, print_ten, '*', 100);
	bar = async(launch::deferred, print_ten, '@', 200);
	// deferred "get" (perform the actual calls):
	foo.get();
	bar.get();
	cout << "\n\n";

	cout << "with default launch:\n";
	foo = async(print_ten, '*', 100);
	bar = async(print_ten, '@', 200);
	// deferred "get" (perform the actual calls):
	foo.get();
	bar.get();
	cout << "\n\n";


	//	This is where auto comes handy to make code readable
	cout << "the same, with auto declaration:\n";
	auto foo2 = async(print_ten, '*', 100);
	auto bar2 = async(print_ten, '@', 200);
	// deferred "get" (perform the actual calls):
	foo2.get();
	bar2.get();
	cout << "\n\n";

	return 0;
}

