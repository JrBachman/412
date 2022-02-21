//
//  main.c
//  String to int
//
//	This code sample demonstrates the three standard ways to convert a C string
//	to an int value:
//		- atoi [part of stdlib]
//		- sscanf [part of stdio]
//		- strtol [part of stdlib]
//
//  Created by Jean-Yves Hervé on 2018-09-14, edited 2019-09-03
//

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, const char * argv[])
{
	//	In the latest versions of the C/C++ compilers, you may get a warning when
	//	some function parameters are not used, particularly when you build with
	//	-Wall (as you should).
	//	This is the way to shut off these specific warnings
	(void) argc;
	(void) argv;
	
	//-----------------------------------------
	//	First, let's define some input strings
	//-----------------------------------------
	char goodInt[] = "4517", goodFloat[] = "63.72", negInt[] = "-789",
		 charNum[] = "79", badNum[] = "abc";

	//----------------------------------------------------------------
	//	Version 1:	atoi
	//	This is the one that pretty much all code samples out there
	//	seem to be using.
	//----------------------------------------------------------------
	printf("-----------------------\nVersion 1: Using atoi\n-----------------------\n");
	//	Advantages:  It is very simple to use, and also very fast
	int val11  = atoi(goodInt);
	printf("Converted string %s to int val11 = %d.  Verification: 2*%s = %d\n",
			goodInt, val11, goodInt, (2*val11));

	//	Disadvantage: It doesn't perform any verification, so you may not get what you
	//	want, or even junk
	int val12 = atoi(goodFloat);
	printf("Converted string %s to int val12 = %d.  Verification: 2*%s = %d\n",
			goodFloat, val12, goodFloat, (2*val12));
	int val13 = atoi(badNum);
	printf("Converted string %s to int val13 = %d.  Verification: 2*%s = %d\n",
			badNum, val13, badNum, (2*val13));
	
	//----------------------------------------------------------------
	//	Version 2:	sscanf
	//	This is the one I use all the time
	//----------------------------------------------------------------
	printf("\n-----------------------\nVersion 2: Using sscanf\n-----------------------\n");
	//	Disadvantage:  It is more complex than atoi, even when used in shortest form
	int val21;
	sscanf(goodInt, "%d", &val21);
	printf("Converted string %s to int val21 = %d.  Verification: 2*%s = %d\n",
			goodInt, val21, goodInt, (2*val21));

	//	However, the way I just used the function masks its key advantage: sscanf
	//	(like all members of the scanf family) returns the number of elements
	//	successfully read.  So, the proper syntax should be
	int numRead = sscanf(goodInt, "%d", &val21);
	(void) numRead;
	
	//	and then I could test that I have indeed read one int.
	//	So that when my string does not store an int I know that my conversion failed
	//	Note that I could perform the test directly on the same instruction as the conversion
	int val22;
	if (sscanf(badNum, "%d", &val22) != 1)
	{
		printf("The string \"%s\" does not encode an integer\n", badNum);
	}
	else
	{
		printf("Converted string %s to int val22 = %d.  Verification: 2*%s = %d\n",
				badNum, val22, badNum, (2*val22));
	}

	//	Another advantage of sscanf is that it supports more formats than atoi
	float val23;
	if (sscanf(goodFloat, "%f", &val23) != 1)
	{
		printf("The string \"%s\" does not encode a float number\n", goodFloat);
	}
	else
	{
		printf("Converted string %s to float val23 = %f.  Verification: 2*%s = %f\n",
				goodFloat, val23, goodFloat, (2*val23));
	}
	
	//-----------
	//	However,
	//-----------
	//	beware that this is not perfect, mostly because of the way that C sees
	//	numbers.  In the end, when all is said and done, C is happy to convert anything
	//	to an int.  This means that sscanf is not immune to the "float read as an int"
	//	problem that we had with atoi
	int val24;
	numRead = sscanf(goodFloat, "%d", &val24);
	if (numRead != 1)
	{
		printf("The string \"%s\" does not encode an integer\n", goodFloat);
	}
	else
	{
		printf("Converted string %s to int val24 = %d.  Verification: 2*%s = %d\n",
				goodFloat, val24, goodFloat, (2*val24));
	}
	//	It doesn't even do better with signed vs. unsigned
	unsigned int val25;
	if (sscanf(negInt, "%u", &val25) != 1)
	{
		printf("The string \"%s\" does not encode an unsigned integer\n", negInt);
	}
	else
	{
		printf("Converted string %s to int val25 = %u.  Verification: 2*%s = %u\n",
				negInt, val25, negInt, (2*val25));
	}

	//----------------------------------------------------------------
	//	Version 3:	strtol
	//	This is the least commonly used alternative, but it is also the one
	//	with the most options.  I wil go over the simplest ones here.
	//	For more details, consult
	//		http://www.cplusplus.com/reference/cstdlib/strtol/
	//	(Yes, it's a C++ reference, but the standard library sections are
	//	identical).
	//----------------------------------------------------------------
	printf("\n-----------------------\nVersion 3: Using strtol\n-----------------------\n");

	//	The syntax is:
	//		long int strtol (const char* str, char** endptr, int base);
	//	So, yes, a first disadvantage is that it returns a long int, not an int,
	//	and so we may need to perform a range test against INT_MIN and INT_MAX
	//	(for more info about range limits for various types, consult:
	//		http://www.cplusplus.com/reference/climits/
	//	Here I am always going to pass a NULL pointer for endptr, but you could keep
	//	"pulling values" from a long string if you set endptr properly.
	//	I only work in base 10 here
	long int val31  = strtol(goodInt, NULL, 10);
	printf("Converted string %s to int val31 = %ld.  Verification: 2*%s = %ld\n",
			goodInt, val31, goodInt, (2*val31));

	//	So, what's better than atoi here?  (besides the fact that  this function comes
	//	with sibblings that can convert to float and double values, and the aforementioned
	//	ability to read multiple values from a stream)
	//	This function sets the global errno variable, so that you know when
	//	something went wrong
	//----------
	//	Oops!
	//	The code below works fine, according to specs, on mac OS.  However, when
	//	I checked it on Ubuntu, before posting, I found that errno was still 0,
	//	So this solution has not major advantage over atoi.
	//	I will stick to using sscanf myself.
	errno = 0;	//	initialize to no error
	long int val32 = strtol(badNum, NULL, 10);
	if (errno)
	{
		printf("The string \"%s\" does not encode an integer [error code %d]\n",
				badNum, errno);
	}
	else
	{
		printf("Converted string %s to int val32 = %ld.  Verification: 2*%s = %ld\n",
				badNum, val32, goodFloat, (2*val32));
	}

	return 0;
}
