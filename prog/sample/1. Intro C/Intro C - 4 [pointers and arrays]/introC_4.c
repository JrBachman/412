/*--------------------------------------------------------------+
|		A simple C program to demonstrate some basic elements	|
|	of the language to Java programmers.						|
|																|
|	Jean-Yves Hervé, 3DG URI									|
|	2016-02-02													|
|	revised 2019-09-03											|
+--------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>


#if 0
#pragma mark -
//--------------------------------------------------------------------------
#pragma mark Function headers
//--------------------------------------------------------------------------
#endif

void part1(void);
//
void part2(void);
int getSize(int n);
int* createDanglingPointer(void);
void createMemoryLeak(void);
//
void part3(void);

//
void part4(void);


#if 0
#pragma mark -
//--------------------------------------------------------------------------
#pragma mark Source code
//--------------------------------------------------------------------------
#endif

int main(int argc, const char * argv[]) {

	// Part 1: Basic allocation of an array
	part1();
	
	// Part 2: Dynamic allocation vs. automatic allocation
	part2();
	
	// Part 3: (related for nomenclature reasons) The static keyword
	part3();
	
	// Part 4: Pointer algebra
	part4();

    return 0;
}

#if 0
#pragma mark -
//--------------------------------------------------------------------------
#pragma mark Part 1: Basic allocation of an array
//--------------------------------------------------------------------------
#endif

void part1(void)
{
	printf("Part 1\n------\n");
	int N = 6;

	// There are 2 elementary functions to allocate arrays in C:
	//		- malloc (memory alloc)
	//		- calloc (clear and alloc)
	//  No, malloc does not clear the memory, so the array you create with malloc is
	//	initialized with whatever junk was in memory at that location!
	
	//	Allocate an array of N float: This allocate a memory block of size N times the
	//	size of a float element, and produces a pointer to the beginning of that block
	//	The array allocated is on the heap.  The pointer fp1 is stored on the stack.
	float* fp1 = (float*) malloc(N*sizeof(float));
	
	// let's see what we found there
	printf("\tPoint 1.1 in fp1: ");
	// Note that you had better not forget the size of your array, because C won't remember it for you
	
	for (int k=0; k<N; k++)
		printf("%6.3f  ", fp1[k]);
	printf("\n");

	//	properly initialize the array's elements
	for (int k=0; k<N; k++)
		fp1[k] = 100*k + 0.1f;
	printf("\tPoint 1.2 in fp1 (after init): ");
	for (int k=0; k<N; k++)
		printf("%6.3f  ", fp1[k]);
	printf("\n");

	// in fact, you can access your array out of the area allocated... until you run into problems
	//	The code below may crash...or not.  You may run it 10 times and it will only crash
	//	one time out of 10.
	printf("\tPoint 1.3 in fp1, and beyond: ");
	for (int k=0; k<5*N; k++)
		printf("%6.3f  ", fp1[k]);
	printf("\n");
	
	//	Clear and allocate an array of N float: This allocate a memory block of size N times the
	//	size of a float element, and produces a pointer to the beginning of that block
	float* fp2 = (float*) calloc(N, sizeof(float));
	// let's see what we found there
	printf("\tPoint 1.4 in fp2: ");
	for (int k=0; k<N; k++)
		printf("%6.3f  ", fp2[k]);
	printf("\n");
	
	//	properly initialize the array's elements
	for (int k=0; k<N; k++)
		fp2[k] = 10*k + 0.5f;
	printf("\tPoint 1.5 in fp2 (after init): ");
	for (int k=0; k<N; k++)
		printf("%6.3f  ", fp2[k]);
	printf("\n");

	//	Let me write the location of the two pointers
	printf("\tPoint 1.6 fp1 = %p    fp2 = %p\n", fp1, fp2);
	
	// I warned you about possible problems, so let's deliberately create some.
	// Verification
//	for (int k=0; k<30*N; k++)
//		fp1[k] = k + 0.3f;
	
	// OK, so I was hoping that my my 2 arrays would get allocated next to each other
	// so that my incorrect loop on fp1 would mess up the values in fp2.  This is not
	// what is happening.  Instead, every now and then, the program will crash in the next
	// loop.  What is interesting is that the code below is just fine.  This is a "feature"
	// of C and C++ that your program may crash in a completely correct portion of your
	// program, due to an error that occurred thousands of statements earlier in the execution.
	printf("\tPoint 1.7 in fp2: ");
	for (int k=0; k<N; k++)
		printf("%6.3f  ", fp2[k]);
	printf("\n");

	printf("\t          in fp1: ");
	for (int k=0; k<N; k++)
		printf("%6.3f  ", fp1[k]);
	printf("\n");
	
	// Don't forget to free the arrays allocated on the heap (unless you
	//	plan to return their addresses out of the function, that is...
	free(fp1);
	free(fp2);
}

#if 0
#pragma mark -
//--------------------------------------------------------------------------
#pragma mark Part 2: Dynamic allocation vs. automatic allocation
//--------------------------------------------------------------------------
#endif

void part2(void)
{
	printf("Part 2\n------\n");

	//	dynamic allocation, whether using malloc or calloc
	double* d = (double*) malloc(10*sizeof(double));
	
	// "automatic allocation" (was often called "static allocation in the past,
	// but nowadays "static" is already ambiguous enough, so this one is not
	// avocated/recommended anymore)
	double ad1[5];
	double ad2[5] = {3.4, 1.2, 8.7, 6.1, -1.3};
	double ad3[] = {8.7, -6.1, 4.3};				//	size deduced from initialization list
	
	// The size of the array has to be known at compilation time
	int n4 = 12+1;
	double ad4[n4];	//	ok

	//	Depending on the version of C that  your compiler refers to by default, the
	//	declaration below may or may not compile.
	int n5 = getSize(3);
	double ad5[n5];
	
	int x = ad5[3] = 8;
	printf("At this point in part2, x = %d", x);
	
	// Automatically allocated arrays's indices are not allowed to get out of bounds
	//ad1[7] = 3.5;
	
	// on the other hand, an array "decays" naturally to a pointer pointing
	//	at the first element of the array
	double* dp = ad1;
	// and nothing protects us against pointers going where they shouldn’t
	dp[7] = 3.5;
	//	if you try to do the same thing with the automatically allocated array,
	//	then you get a warning.  I am pretty sure that it used to be a compiler error
	//	a few years ago, and I don't understand the logic behind the change:
	//		- there is no situation where this should be expected to produce a valid result
	//		- many developers unwisely ignore warnings
	ad1[7] = 6;

	// automatically allocated arrays are allocated on the stack.  This can lead to
	//	dangling pointers
	int* myArray = createDanglingPointer();
	
	// dynamically allocated arrays can result in a memory leak
	createMemoryLeak();
}

int* createDanglingPointer(void)
{
	int newArray[6];
	int* castArray = newArray;
	
	// When we leave the function, the block of int will get freed from memory, so that
	// the pointer return points to data that may soon not be there anymore.
	//return newArray;
	return castArray;
}

void createMemoryLeak(void)
{
	double* memLeak = (double*) malloc(10*sizeof(double));
	
	// when we leave the function, the block of double remains allocated and will
	//	never be freed
}

int getSize(int n)
{
	return 2*n + 1;
}

#if 0
#pragma mark -
//--------------------------------------------------------------------------
#pragma mark Part 3: The static keyword
//--------------------------------------------------------------------------
#endif

//	This function has nothing to do with pointers.  I just put it here to show
//	you what "the other meaning of static" (besides "a variable that is shared
//	y all instances of a class, and functions that access only local or static
//	variables") is, so that you can see that this justified fighting in order
//	that the old "static allocation," which really had nothing particularly
//	"static" about it, be renamed something different, to avoid further confusion.
void myFunc(void)
{
	// A static variable's current value is retained from one call of the
	//	function to the next
	static int callCount = 0;
	
	callCount++;
	printf("The function has been called %d times\n", callCount);
}

void part3(void)
{
	printf("\nPart 3\n------\n");

	for (int k=0; k<10; k++)
		myFunc();
}


#if 0
#pragma mark -
//--------------------------------------------------------------------------
#pragma mark Part 4: Pointer algebra
//--------------------------------------------------------------------------
#endif

void part4(void)
{
	printf("Part 4\n------\n");

	int x[4] = {200, -50, 300, 27};
	printf("Point 0: x = {%d, %d, %d, %d}\n", x[0], x[1], x[2], x[3]);
	
	//	I told you before that in C a 1D array pretty much is the same
	//	thing as a pointer to C data.  This creates a pointer to int
	//	that points to the first element of the array
	int* xp = x;
	
	//	Here I dereference xp (access the element it points to, that is,
	//	the int-sized element at the beginning of the array) to assign its
	//	value to something else
	*xp = 25;
	printf("Point 1: x = {%d, %d, %d, %d}\n", x[0], x[1], x[2], x[3]);
	
	//	Pointer algebra 1
	//	xp[k] is just a shorthand notation for *(xk + k)
	//	that is, the element to which points xk, shifted by k cells of
	//	size whatever the size of what xp points to is (here, int), so
	//	2 int.
	*(xp + 2) = 345;
	printf("Point 2: x = {%d, %d, %d, %d}\n", x[0], x[1], x[2], x[3]);
	
	
}


