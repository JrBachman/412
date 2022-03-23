#include "myFunctions.h"

// This is a simple function with two parameters:
//		one is an input parameter
//		the other implements an input/output parameter by using a pointer
//	Of course, because C uses value passing, neither parameter's value is changed, it's
//	the value of what the second parameter points to that is modified.
//	If you want to "return" multiple values without encapsulating them in a struct,
//	your only choice is to pass a series of pointers as arguments).
//	For the purists:  This function has a return value, and also a "side effect," since
//	the value of what the second parameter points to is modified.
int simpleFunc1(int x, int* y)
{
	//	silly code
	*y += x*2;
	x++;
	
	return x * *y;
}

//	Note that this function, like the previous one doesn't do anything meaningful
//	I am just trying to illustrate basic stuff that you can do with pointers
//	Here again, this function has a return value and a side effect
int simpleFunc2(int x, int* y)
{
	//	silly code
	(*y)--;		//	decrements the value at the location that y points to
	x++;
	
	return x - *y;
}

//	This is not Java.  So pt1 is not simply a reference to a SurveyData object.
//	pt1 is a **copy** of the struct that was defined in the caller function.
//	The same thing is happening with the return value:  We are returning a copy of the
//	struct created inside this function.
//	Keep this in mind:
//		1. Passing by value can be quite costly in terms of performance if
//			you end up doing a lot of copying.
//		2. If one of the fields of the structs is a pointer (pointing to an array), then
//			you simply make a copy of the pointer, not of the entire array.  There is only
//			one array and both structs point to it.
SurveyData northmost(SurveyData pt1, SurveyData* pt2)
{
	SurveyData newPt;
	newPt.x = pt1.x + pt2->x;   // (*pt2).x
	newPt.y = 23;
	
//	This is just to belabor a point that I already made:  In C/C++ there is no
//	default initialization.
//	Run the program first and look at the out put, then enable the lines below
//	again, and compare.
//	Your variable (here, the struct and its fields) get
//	a memory slot to occupy, and gets initialized to whatever junk was already
//	there in memory.  Be very careful about that!  This is a *very* common source
//	of hard-to-track bugs (and particularly common for people coming from Java).

// 	newPt.elev = 0.5f * (pt1.elev + pt2->elev);
// 	newPt.type = pt1.type;
	return newPt;
}
