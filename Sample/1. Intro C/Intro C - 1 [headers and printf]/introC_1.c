/*--------------------------------------------------------------+
|		A simple C program to demonstrate some basic elements	|
|	of the language to Java programmers.						|
|																|
|	Jean-Yves Hervé, 3DG URI									|
|	2016-09-28	[revised 2019-09-03, 2020-12-31] 				|
+--------------------------------------------------------------*/

// In C, you need to #include header files that give the
//	specifications (we call that the "prototype") of some functions
//	and also the definition of some data types and constants
#include <stdio.h>		//	this one declares standard Input/Output functions


//	In C, you must declare a function before you call it.  This is what we call the
//	prototype.  In fact, you don't need to give the name of the variable.
//	I write the name of the variable because this allows me to use javadoc-style
//	comments to produce documentation for my code through Doxygen.
int sumArray(int a[], int sizeA);		//	could have been just int sumArray(int*, int);

//	We saw in the previous sample that the arguments of the main function were
//	in fact the name of the executable and the arguments passed to the program
//	when it is launched from the console.  In effect, they play the same role as the
//	array of strings received by the main method of a Java application
//	Java Reminder:  int main(String[] arg) {...}
//	The relation between  String[] arg  in Java and char* argv[]  in C will be discussed
//	in another code sample
int main(int argc, const char* argv[]) {

	//	A basic print statement.  printf is declared in stdio.h
	printf("Hello, World!\n");
	
	int m = 12;
	float x = -3.23f;

	//	the 'f' in printf stands for "format."  You can format your output using
	//	various tags.            +-- %d means "integer"  +---- the 1st element in the list
	//                           |                       |     will be formatted as an integer
	//                           v                       v
	printf("In this program, m = %d, while x = %9.4f\n", m, x);
	//                                             ^
	//                                             |
	//                         this one is a bit trickier. %f is a floating point number.
	//						   9.4 is a formatting parameter.  It asks to format the number
	//                         on 9 characters total (including sign and decimal dot), with
	//                         4 characters solely for the fractional part
	
	//	Declaring an initialized array looks almost the same as in Java
	//	Under the hood, though, there is a huge difference: the array is allocated on the
	//	stack, not on the heap.  In C/C++, we used to call that "static allocation."
	//	Because the word static is used with too many different meanings already, the encouraged
	//	appellation for this type of allocation is now "automatic allocation"
	//	We will talk about this, and how this differs from the way Java creates and stores
	//	arrays, in class the next couple of weeks.
	int v[] = {8, 4, -6, 14, 8, 12};
	//	Note that I could have written the size in the declaration   int v[6] = {8, 4, -6, 14, 8, 12};
	//	but *not*    int[] c = {8, 4, -6, 14, 8, 12};

	int n = 6;
	
	//	C passes parameters by value only.  So, I don't "send v and n": I send the 
	//	*value of v* (really a pointer to int values) and the value of n (and int value).
	int sum = sumArray(v, n);
	printf("The sum of the elements in Array v is %d\n", sum);
	
    return 0;
}

// 1. Note that the name of the parameters in the actual implementation of the function
//	  don't need to match those in the prototype.  The type (and order) of the parameters do.
// 2. Yeah, C arrays don't know their own size, because they are just dumb blocks of memory
//	  cells of the same type, and not objects as in Java or Python.  So now you know why you 
//	  may have run into some old Java or Python code where the size of the array was passed 
//	  to functions alongside the array itself:  That code was just a straighforward/lazy 
//	  port of old C code (or maybe, in fairness, a deliberate attempt to preserve API
//	  uniformity across implementations)
int sumArray(int b[], int sizeB)
{
	//	In Java you *have* to initialize your local variables because the compiler won't
	//	let you have it otherwise.
	//  In C/C++ you *should* initialize your variables because there is *no* default
	//  initialization happening. Ever.  Your variable is located somewhere in memory
	//	and inherit as initial value whatever old crap was there.
	int sum = 0;
	
	for (int k=0; k<sizeB; k++)
	{
		sum += b[k];
	}
	
	//	Same thing here:  I don't "return sum" but the *value* of sum (an int value)
	return sum;
}

