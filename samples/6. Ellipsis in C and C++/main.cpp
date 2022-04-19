//
//  main.cpp
//  Ellipsis in C and C++
//
//  Created by Jean-Yves Hervé on 2019-03-05, revised 2020-10-18
//  This code sample demonstrates the use of ... (ellipsis)
//	in a function's argument list.
//
//	Note 1: I show you how it works, so that you are not
//	confused when you encounter it in the future.  However, I *definitely*
//	do not advocate the use of ellipses in your code, except as a solution
//	of very last resort
//
//	Note 2:  I post this code as a C++ source file, but it would work just
//	as well in C (of course, with C headers stdio.h and stdarg.h)

#include <cstdio>
#include <cstdarg>

using namespace std;

//-------------------------------------
//	Function prototypes
//-------------------------------------

//	The same function that everybody starts with :-)
//	Computes the sum of all elements in a list of integers.
//	Important points:
//		o The ... (ellipsis) indicates a variable number of arguments
//		o The function must have *at least* one argument before the ...
//			and it's better if it's an int
//		o The ... must appear last in the argument list
//		o The type of the data in the variable list is not constrained by
//			that of the arguments that precede the ...
int computeSum(unsigned int numArgs, ... );

void someFunc(double x, double y, unsigned int m, int n, ... );

//-------------------------------------
//	Function implementations
//-------------------------------------

int computeSum(unsigned int numArgs, ... )
{
	//	We are now going to pull values out of the variable list
	//	The type for this is va_list.
	va_list argList;
	
	//	We initialize the argument list variable by passing
	//		o the list variable
	//		o the last argument before the ellipsis (it just so happens in
	//			our example that it is the length of the list, but this doesn't
	//			have to be so.
    va_start(argList, numArgs);

    int sum = 0;
    for(unsigned int i = 0; i < numArgs; i++ )
    {
    	//	You need to know the type of the elements that you extract
    	//	from the argument list
        int arg = va_arg(argList, int);
        printf("\targument %u: %d\n", i, arg);

        sum += arg;
    }
	
	//	Important: Before leaving the function, you must release the argument list
    va_end(argList);

    return sum;
}

//	In this function, the variable argument list contains n "items" made up
//	of:	- one int
//		- two double
//		- one 'char' passed as an int and that I need to cast back to char
//	Please note that the length of the list isn't necessarilly the last argument
//	before the ellipsis (even if it makes sense that it should be).
void someFunc(double x, double y, unsigned int numItemsOnVarList, int n, ... )
{
	printf("someFunc, x=%f, y=%f, n= %d. Items on the variable list: %u\n",
			x, y, n, numItemsOnVarList);
	
	//	We are now going to pull values out of the variable list
	//	The type for this is va_list.
	va_list argList;
	
	//	We initialize the argument list variable by passing
	//		o the list variable
	//		o the last argument before the ellipsis (an argument that doesn't
	//			automatically cast to int results in "undefined behavior")
	//	Again, that last argument before the ellipsis doesn’t need be the length
	//	of the variable list.
    va_start(argList, n);

    for(unsigned int i = 0; i < numItemsOnVarList; i++ )
    {
    	//	I get from my arg list: an int, and a double.  va_arg is just a macro
    	//	that doesn't support a lot of data types. For example, it supports double
    	//	but not float, and int but not short or char.
        int intArg = va_arg(argList, int);
        double fltArg1 = va_arg(argList, double);
        double fltArg2 = va_arg(argList, double);
        //	if I want to pass a char I have to cast it as an int
        char charArg = (char) va_arg(argList, int);

        printf( "\targuments for item %u: %d, %f, %f, %c\n", i, intArg, fltArg1, fltArg2, charArg);
    }
	
	//	Important: Before leaving the function, you must release the argument list
    va_end(argList);

}

int main(int argc, char* argv[])
{
    printf("Computing sum of no elements\n");
    printf("\tcomputeSum(0) returns: %d\n",
    computeSum(0));

    printf("Computing sum of elements 24, -7, 23, -6.\n");
    printf("\tcomputeSum(4, 24, -7, 23, -6) returns: %d\n",
    computeSum(4, 24, -7, 23, -6));

    printf("Computing sum of elements 24, -7, 23, -6, 100, 1000.\n");
    printf("\tcomputeSum(6, 24, -7, 23, -6, 100, 1000) returns: %d\n",
    computeSum(6, 24, -7, 23, -6, 100, 1000));
//
	printf("\n");
	
	someFunc(3.5, -1.4, 0, 12);

//	varList length -----+       1 item: 1 int, 2 double, 1 char
//						|     ________________
//						v    |                |
	someFunc(3.5, -1.4, 3, 5, 23, 2.5, 3.7, 'A',
							  45, -1.7, 2.4, 'Z',
							  112, -3.2, 3.5, '*');

    return 0;
}
