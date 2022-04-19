/*----------------------------------------------------------------------------------+
|	This is the version using modern C++ that you want to use as model.				|
|																					|
|	Jean-Yves Hervé [jyh@cs.uri.edu], 2021-05-07									|
+----------------------------------------------------------------------------------*/

#include <iostream>
#include <random>

using namespace std;


int main(int argc, const char* argv[])
{
	const int NUM_POINTS = 20;
	//
	const int MIN_VAL = -10;
	const int MAX_VAL = +80;
	//
	const double X_MIN = -1.5f;
	const double X_MAX = +12.5f;
	//
	const double HEIGHT_MEAN = 1.82;
	const double HEIGHT_SIGMA = 0.2;

	//	True random generator:  This one uses random electric noise as seed
	random_device myRandDev;

	//	next, I need to define a "random engine".  Here I go for the "default engine",
	//	which is the recommended one for non-specialized use.  
	//	If you get fancy and specialized, you may decide to go for a different engine, 
	//	for exemple
	//		mt19937_64  Mersenne Twister 19937 generator (64 bit)
	default_random_engine myEngine(myRandDev());
	
	//	Next you define random distributions with a type and range
	//	I am going to generate similar data to the "old-style" program
	uniform_int_distribution<int> simpleDist(0, 100);
	uniform_int_distribution<int> intRangeDist(MIN_VAL, MAX_VAL);
	uniform_real_distribution<double> doubleRangeDist(X_MIN, X_MAX);
	
	//	A big gain:  Now I can easily generate data using a Gaussian normal 
	//	distribution (or a Poisson, Bernoulli, binomial, etc. distribution)
	//	Let's say I want to generate people's heights
	//		mean value: µ = 1.82m, standard deviation: σ = 0.3
	//	reminder: 
	//		• 50% of the area under the curve is within the interval [− 2σ/3, 2σ/3],
	//		• 68.3% of the area under the curve is within the interval [−σ, σ],
	//		• 95.5% of the area under the curve is within the interval [−2σ, 2σ],
	//		• 99.7% of the area under the curve is within the interval [−3σ, 3σ].
	normal_distribution<double> heightDist(HEIGHT_MEAN, HEIGHT_SIGMA);

	//------------------------------------------------------------
	//	Simple:  Generate integers in the range 0 - 99
	//------------------------------------------------------------
	cout << "\nPart 1: random ints in the range 0 to 99" << endl << "\t";
	for (int k=0; k<NUM_POINTS; k++)
	{
		//	simply ask your distribution to produce you a new number
		int randVal = simpleDist(myEngine);
		cout << randVal << "  ";
	}	
	cout << endl << endl;
		
	//------------------------------------------------------------------------
	//	Not trickier anymore:  Generate integers in the range MIN_VAL - MAX_VAL
	//------------------------------------------------------------------------
	cout << "\nPart 2: random ints in the range " << MIN_VAL << " to " << MAX_VAL << 
			endl << "\t";
	for (int k=0; k<NUM_POINTS; k++)
	{
		//	simply ask your distribution to produce you a new number
		int randVal = intRangeDist(myEngine);
		cout << randVal << "  ";
	}	
	cout << endl << endl;
	
	//------------------------------------------------------------------------
	//	Now generate double numbers in the range X_MIN - X_MAX
	//------------------------------------------------------------------------
	cout << "\nPart 3: random doubles in the range [" << X_MIN << ", " << X_MAX << "]" << 
			endl << "\t";
	for (int k=0; k<NUM_POINTS; k++)
	{
		//	simply ask your distribution to produce you a new number
		double randVal = doubleRangeDist(myEngine);
		cout << randVal << "  ";
	}	
	cout << endl << endl;

	//------------------------------------------------------------------------
	//	Now generate data with a Gaussian distribution
	//------------------------------------------------------------------------
	cout << "\nPart 4: random heights with mean " << HEIGHT_MEAN << 
			" and standard deviation " << HEIGHT_SIGMA << endl << "\t";

	for (int k=0; k<NUM_POINTS; k++)
	{
		//	simply ask your distribution to produce you a new number
		double randVal = heightDist(myEngine);
		cout << randVal << "  ";
	}	
	cout << endl << endl;

	return 0;
}

