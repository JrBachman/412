//
//  main.cpp
//  Lab 06 Code Handout
//
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <list>

using namespace std;

struct MyStruct 
{
	int m;
	float x;
};

int main(int argc, const char * argv[])
{

	//--------------------------------------------------------------------------
	//	Task 1: Insert here code to set up pipes and create child processes
	//--------------------------------------------------------------------------


	ifstream inFile;
	int numElts;

	//-------------------------------------------
	//	Reading data into a vector of MyStruct
	//-------------------------------------------
	inFile.open("data1.txt");
	cout << "Reading from the file... ";
	inFile >> numElts;
	cout << numElts << " data points:" << endl;

	vector<MyStruct> myVect;
	for (int k=0; k<numElts; k++)
	{
		float x;
		int m;
		inFile >> x >> m;
		MyStruct s = {m, x};
		myVect.push_back(s);

		//--------------------------------------------------------------------------
		//	Task 1: Insert here code to send data to child processes
		//	using pipes.
		//--------------------------------------------------------------------------



	}
	inFile.close();
	
	cout << "Content of vector (" << myVect.size() << " elements)" << endl;
	//	Print out the contents of the vector
	for (int k=0; k<myVect.size(); k++)
	{
		cout << "\t" << myVect[k].m << " \t" << myVect[k].x << endl;
	}
	cout  << endl;

	//--------------------------------------------------------------------------
	//	Task 2: Insert here code to get results from child processes
	//--------------------------------------------------------------------------


	//--------------------------------------------------------------------------
	//	Task 1: Insert here code to wait for child processes to finish
	//--------------------------------------------------------------------------
	


	return 0;
}
