//
//  main.cpp
//  Threads and Tasks
//
//  Created by Jean-Yves Hervé [Local] on 2016-04-27.
//	Revised 2020-03-29, 2020-11-11, 2021-04-03

//----------------------------------------------
//	new style task, with return value C++14
//----------------------------------------------

// launch::async vs launch::deferred
#include <iostream>     // std::cout
#include <future>       // std::async, std::future, std::launch
#include <chrono>       // std::chrono::milliseconds
#include <thread>
#include <vector>

//	silly function
template <typename T>
std::vector<T> makePalindrome(const std::vector<T>& vec)
{
	std::vector<T> outVec;
	for (int k=0; k<vec.size(); k++)
	{
		outVec.push_back(vec[k]);
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
	}
	for (int k=vec.size()-1; k>=0; k--)
	{
		outVec.push_back(vec[k]);
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
	}
	
	return outVec;
}

int main(int argc, const char* argv[])
{
	std::vector<int>v1 {12, 34, 35, 22};

	std::cout << "Creating an explicitly asynchronous task" << std::endl;

	auto task1 = std::async(std::launch::async, makePalindrome<int>, v1);
	std::cout << "Waiting in the main" << std::flush << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	std::cout << "End wait" << std::flush << std::endl;

	auto out1 = task1.get();
	std::cout << "Values returned by task 1: ";
	for(auto val : out1)
	{
		std::cout << val << "  ";
	}
	std::cout << "\n\n";
	
	std::cout << "Creating an explicitly deferred task" << std::endl;

	auto task2 = std::async(std::launch::deferred, makePalindrome<int>, v1);
	std::cout << "Waiting in the main" << std::flush << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	std::cout << "End wait" << std::flush << std::endl;
	

	auto out2 = task2.get();
	std::cout << "Values returned by task 2: ";
	for(auto val : out2)
	{
		std::cout << val << "  ";
	}
	std::cout << "\n\n";
	
	
	
	return 0;
}

