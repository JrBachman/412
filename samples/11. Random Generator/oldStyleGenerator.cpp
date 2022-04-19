/*----------------------------------------------------------------------------------+
|	This is for reference only:  Don't use this code!!!								|
|																					|
|	This little program shows the "classical" way to generate pseudo-random data.	|
|	I only give it to you for reference, because this is the kind that you are		|
|	going to encounter out there 99% of the time, because it has been around for	|
|	such a long time (in fact, the generation is all C code).  It looks nice and 	|
|	simple, but there are many reasons to avoid that code:							|
|		1.  The C random generator is a terrible generator.  First, because it		|
|			generates data that are correlated.  Second, because it repeats too 	|
|			fast.  It's a low-ball estimate that hundreds of millions of research	|
|			money has been wasted (and still is, because most programmers still 	|
|			havent wisened up) just analyzing artifacts of a lousy generator!		|
|		2.	The C generator only produces pseudo-random data with a uniform			|
|			distribution.  If you want a different distribution (say, Gaussian or	|
|			Poisson), then you have to code something.								|
|	This was only a problem with C and C++.  Most other programming languages you	|
|	are likely to run into (Java, Python, Swift, R, etc.) have	a decent random 	|
|	generator.																		|
|	As of C++11 (which most C++ programmers still don't know much about), C++ has	|
|	a decent generator too (the one I show in the other code sample), and so there	|
|	is no excuse to use anything else.												|
|																					|
|	So, why did I give you this too?  So that if you see that stuff in code someone	|
|	hands you over, you will recognize the junk and know that you should fix it.	|
|	Well, at least you have been warned.											|
|																					|
|	Jean-Yves Hervé [jyh@cs.uri.edu], 2021-05-07									|
+----------------------------------------------------------------------------------*/

#include <iostream>
#include <cstring>	
#include <cstdlib>

using namespace std;

int main(int argc, const char* argv[])
{
	//	The first thing you need to do is to "seed" the generator.
	//	Typically, people use the current time (seconds since 1900), since this 
	//	means that every time you launch the program you will get a different sequence.
	//
	//	People usually do this
	//		srand((unsigned int) time(NULL));
	//
	//	Here I want to record the time for latter use, so I break it into 2 statements
	unsigned int mySeed = static_cast<unsigned int>(time(NULL));
	srand(mySeed);
	
	//	rand() produces an int value in the range 0-RAND_MAX
	//	RAND_MAX is a constant defined in <cstdlib>.
	cout << "rand() will produce an int in the range [0, " << RAND_MAX << "]" << endl;
	
	const int NUM_POINTS = 20;

	//------------------------------------------------------------
	//	Simple:  Generate integers in the range 0 - 99
	//------------------------------------------------------------
	cout << "\nPart 1: random ints in the range 0 to 99" << endl << "\t";
	for (int k=0; k<NUM_POINTS; k++)
	{
		//	taking the remainder in division by 100 produces a number in the 
		//	range 0-99 
		int randVal = rand() % 100;
		cout << randVal << "  ";
	}	
	cout << endl << endl;
		
	//------------------------------------------------------------------------
	//	Slightly trickier:  Generate integers in the range MIN_VAL - MAX_VAL
	//------------------------------------------------------------------------
	{
		const int MIN_VAL = -10;
		const int MAX_VAL = +80;
	
		//	There are MAX_VAL - MIN_VAL + 1 different integer values in that range
		//	(e.g. 3 values in the range 5 - 7:  5, 6, 7)
		const int NUM_VALS = MAX_VAL - MIN_VAL + 1;
	
		cout << "\nPart 2: random ints in the range " << MIN_VAL << " to " << MAX_VAL << 
				endl << "\t";
		for (int k=0; k<NUM_POINTS; k++)
		{
			//	the remainder % operation gives us a number in the range 0 - (NUM_VALS-1),
			//	that is, 0 - (MAX_VAL-MIN_VAL).  If we add MIN_VAL we are in range MIN_VAL - MAX_VAL
			int randVal = rand() % NUM_VALS + MIN_VAL;
			cout << randVal << "  ";
		}	
		cout << endl << endl;
	}
	
	//------------------------------------------------------------------------
	//	Now generate double numbers in the range X_MIN - X_MAX
	//------------------------------------------------------------------------
	{
		const double X_MIN = -1.5f;
		const double X_MAX = +12.5f;
	
		//	The number of double values in that range makes no sense, but we can get the spread
		const double SPREAD = X_MAX - X_MIN;
	
		cout << "\nPart 3: random doubles in the range [" << X_MIN << ", " << X_MAX << "]" << 
				endl << "\t";
		for (int k=0; k<NUM_POINTS; k++)
		{
			//	We convert our rand() int into a double in the range [0, 1]
			//		0 ≤ (double) rand() / RAND_MAX ≤ 1
			//	We need to cast one of the two terms to a double so that we get a non-zero
			//	ratio (since always rand() < RAND_MAX)
			//	We multiply that ratio by SPREAD to get a number in the range [0, SPREAD]
			//		0 ≤ SPREAD * rand() / RAND_MAX ≤ SPREAD = X_MAX - X_MIN
			//	Finally we add X_MIN
			//		X_MIN ≤ X_MIN + SPREAD * rand() / RAND_MAX ≤ SPREAD + X_MIN = X_MAX
			//	
			double randVal = X_MIN + SPREAD * rand() / RAND_MAX;
			cout << randVal << "  ";
		}	
		cout << endl << endl;
	}
	
	
	//------------------------------------------------------------------------
	//	As mentioned in the start comments, if you need a distribution other
	//	than uniform (say, a nice Gaussian distribution for sizes of things, 
	//	or a Poisson distribution to simulate failure rates of devices, then
	//	you are out of luck.  It's possible, but this will take more work.
	//------------------------------------------------------------------------

	//------------------------------------------------------------------------
	//	Re-using the seed
	//------------------------------------------------------------------------
	//	Finally, one word about the seed.  
	//	Sometimes you may want to use the same random sequence over and over,
	//	to compare slightly different versions of an algorithm, and fine-tune	
	// 	performance
	//	If you generate a small number of data (say, maybe just a few hundreds
	//	of thousands, you can write them in a file, and so make sure that all
	//	your programs use the same sequence).
	//	Another way (or if you need a lot of data, possibly for a long time), is
	//	simply to use the same seed  for the different runs of your program.
	//	So, for example, you could decide:  We are going to use the seed 20210507
	//	for all the runs
	//		srand(20210507);
	
	//	Here I am going to re-seed my randdom generator, and verify that I get the
	//	same data all over again
	
	cout << endl;
	cout << "=====================================" << endl;
	cout << "###### RESEEEDING THE GENERATOR######" << endl;
	cout << "=====================================" << endl << endl;

	srand(mySeed);
	
	//------------------------------------------------------------
	//	Simple:  Generate integers in the range 0 - 99
	//------------------------------------------------------------
	cout << "\nPart 1: random ints in the range 0 to 99" << endl << "\t";
	for (int k=0; k<NUM_POINTS; k++)
	{
		//	taking the remainder in division by 100 produces a number in the 
		//	range 0-99 
		int randVal = rand() % 100;
		cout << randVal << "  ";
	}	
	cout << endl << endl;
		
	//------------------------------------------------------------------------
	//	Slightly trickier:  Generate integers in the range MIN_VAL - MAX_VAL
	//------------------------------------------------------------------------
	{
		const int MIN_VAL = -10;
		const int MAX_VAL = +80;
	
		//	There are MAX_VAL - MIN_VAL + 1 different integer values in that range
		//	(e.g. 3 values in the range 5 - 7:  5, 6, 7)
		const int NUM_VALS = MAX_VAL - MIN_VAL + 1;
	
		cout << "\nPart 2: random ints in the range " << MIN_VAL << " to " << MAX_VAL << 
				endl << "\t";
		for (int k=0; k<NUM_POINTS; k++)
		{
			//	the remainder % operation gives us a number in the range 0 - (NUM_VALS-1),
			//	that is, 0 - (MAX_VAL-MIN_VAL).  If we add MIN_VAL we are in range MIN_VAL - MAX_VAL
			int randVal = rand() % NUM_VALS + MIN_VAL;
			cout << randVal << "  ";
		}	
		cout << endl << endl;
	}
	
	//------------------------------------------------------------------------
	//	Now generate double numbers in the range X_MIN - X_MAX
	//------------------------------------------------------------------------
	{
		const double X_MIN = -1.5f;
		const double X_MAX = +12.5f;
	
		//	The number of double values in that range makes no sense, but we can get the spread
		const double SPREAD = X_MAX - X_MIN;
	
		cout << "\nPart 3: random doubles in the range [" << X_MIN << ", " << X_MAX << "]" << 
				endl << "\t";
		for (int k=0; k<NUM_POINTS; k++)
		{
			//	We convert our rand() int into a double in the range [0, 1]
			//		0 ≤ (double) rand() / RAND_MAX ≤ 1
			//	We need to cast one of the two terms to a double so that we get a non-zero
			//	ratio (since always rand() < RAND_MAX)
			//	We multiply that ratio by SPREAD to get a number in the range [0, SPREAD]
			//		0 ≤ SPREAD * rand() / RAND_MAX ≤ SPREAD = X_MAX - X_MIN
			//	Finally we add X_MIN
			//		X_MIN ≤ X_MIN + SPREAD * rand() / RAND_MAX ≤ SPREAD + X_MIN = X_MAX
			//	
			double randVal = X_MIN + SPREAD * rand() / RAND_MAX;
			cout << randVal << "  ";
		}	
		cout << endl << endl;
	}

	//------------------------------------------------------------------------
	//	Re-using the seed [the wrong way]
	//------------------------------------------------------------------------
	//	Of course, to get the same sequences, you need to generate your data in
	//	in the same order
	
	cout << endl;
	cout << "=====================================" << endl;
	cout << "###### RESEEEDING THE GENERATOR######" << endl;
	cout << "=====================================" << endl << endl;

	srand(mySeed);
	
	//------------------------------------------------------------
	//	Simple:  Generate integers in the range 0 - 99 --> same values
	//------------------------------------------------------------
	cout << "\nPart 1: random ints in the range 0 to 99" << endl << "\t";
	for (int k=0; k<NUM_POINTS; k++)
	{
		//	taking the remainder in division by 100 produces a number in the 
		//	range 0-99 
		int randVal = rand() % 100;
		cout << randVal << "  ";
	}	
	cout << endl << endl;
		
	//------------------------------------------------------------------------
	//	Now generate double numbers in the range X_MIN - X_MAX  --> now different values
	//------------------------------------------------------------------------
	{
		const double X_MIN = -1.5f;
		const double X_MAX = +12.5f;
	
		//	The number of double values in that range makes no sense, but we can get the spread
		const double SPREAD = X_MAX - X_MIN;
	
		cout << "\nPart 3: random doubles in the range [" << X_MIN << ", " << X_MAX << "]" << 
				endl << "\t";
		for (int k=0; k<NUM_POINTS; k++)
		{
			//	We convert our rand() int into a double in the range [0, 1]
			//		0 ≤ (double) rand() / RAND_MAX ≤ 1
			//	We need to cast one of the two terms to a double so that we get a non-zero
			//	ratio (since always rand() < RAND_MAX)
			//	We multiply that ratio by SPREAD to get a number in the range [0, SPREAD]
			//		0 ≤ SPREAD * rand() / RAND_MAX ≤ SPREAD = X_MAX - X_MIN
			//	Finally we add X_MIN
			//		X_MIN ≤ X_MIN + SPREAD * rand() / RAND_MAX ≤ SPREAD + X_MIN = X_MAX
			//	
			double randVal = X_MIN + SPREAD * rand() / RAND_MAX;
			cout << randVal << "  ";
		}	
		cout << endl << endl;
	}

	//------------------------------------------------------------------------
	//	Slightly trickier:  Generate integers in the range MIN_VAL - MAX_VAL
	//------------------------------------------------------------------------
	{
		const int MIN_VAL = -10;
		const int MAX_VAL = +80;
	
		//	There are MAX_VAL - MIN_VAL + 1 different integer values in that range
		//	(e.g. 3 values in the range 5 - 7:  5, 6, 7)
		const int NUM_VALS = MAX_VAL - MIN_VAL + 1;
	
		cout << "\nPart 2: random ints in the range " << MIN_VAL << " to " << MAX_VAL << 
				endl << "\t";
		for (int k=0; k<NUM_POINTS; k++)
		{
			//	the remainder % operation gives us a number in the range 0 - (NUM_VALS-1),
			//	that is, 0 - (MAX_VAL-MIN_VAL).  If we add MIN_VAL we are in range MIN_VAL - MAX_VAL
			int randVal = rand() % NUM_VALS + MIN_VAL;
			cout << randVal << "  ";
		}	
		cout << endl << endl;
	}

	return 0;
}
