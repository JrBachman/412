/*--------------------------------------------------------------+
|		A simple C program to demonstrate some basic elements	|
|	of the language to Java programmers.						|
|																|
|	Jean-Yves Hervé, 3DG URI									|
|	2016-09-29													|
|	rev. 2019-09-03												|
+--------------------------------------------------------------*/

#include <stdio.h>		//	this one declares standard Input/Output functions
#include <stdlib.h>		//	the standard lib declares the memory allocation functions

//	In C/C++ all lines that start with # are "preprocessor statements."
//	As its name indicates, the "preprocessor runs before the compiler, and
//	typically predefines some environment parameters or substitution rules.
//	The #include statement orders for the text contained in a given file to be
//	inserted at this point in the code to compile.

//	Another very common preprocessor statement is #define, which defines a
//	substitution rule prior to running the compiler.
//	For example, if I write
#define SOME_VAL  1234
//	Then wherever the compiler encounters SOME_VAL in the code, it will
//	replace it by 1234.  So, #define is a very common way to define constants
//	in C (in modern C++, the keyword "const" should be used instead)
int myVar = SOME_VAL;

//	define works even with--partial--expressions
#define PLUS_12 + 12;
int sillyVar = 5 PLUS_12;	//	ugly as hell, but initializes sillyVar to 17

//	You can have conditional preprocessor statements.  The most common one is #if.
//	The #if can come along with #else, or even one or more #elif.  An #if block
//	must end with #endif
//	I tend to use this a lot when I want to enable or disable big chuncks of code
//	Change the value from 1 to 0 below to disable this block of code
#if 0
	#error replace the one above by a zero for the code to compile
	inx x = some bad code;
	//	...more and more of this kind of clever code goes here
	//
#endif


// boolean types are not defined in standard C.  In C boolean expressions, a zero value
// corresponds to "false" and any non-zero value corresponds to "true".  So, you will
// encounter such as
//    int z = a + b;
//    if (z) {   --->  this is the same as "if z ≠ 0"
//		...do something...
//	  }
// You will find a lot of programs roll their own pseudo-boolean values.
// Most common something like this.
#ifndef TRUE
	#define TRUE  1
	#define FALSE 0
#endif
// Yes, in old C we find TRUE and FALSE all-caps.


// Above, I used the other main conditional statement:  #ifndef
// this condition evaluates as 1 if TRUE has *not* been defined in any of the code seen
// so far (including headers loaded and stadard C definition)


// There is always the temptation to try to define true and
//	false, and then a bool type, but this may cause problems when the local version of C
// implements these.  So, my personal rule of thumb is:  In C, go with the boolean as int,
// and the all-cap TRUE and FALSE.

// #pragma is another type of preprocessor statement that is IDE-specific.  Some
// #pragma statement really only concern the editor and not even the preprocessor.  This is
// the case with this #pragma mark.  Because a #pragma not recognized by another
// IDE's compiler can cause the emission of warnings, or even of errors, I "protect"
// these inside an #if 0   ... #endif.  Xcode's editor will still see the #pragma,
// but the preprocessor of another, non-supporting IDE won't see them
#if 0
#pragma mark -
//--------------------------------------------------------------------------
#pragma mark Data type definitions
//--------------------------------------------------------------------------
#endif


//	C has a very basic enumerated type.  Basically, C's enum values are int.
//	By default, the value of an element is that of the previous one + 1,
//	but you can override this default behavior, as shown below.
enum studentType {
	UNDERGRADUATE,		    //	0 default value
	GRADUATE,				//	1 by default
	NON_MATRICULATED = 10,	//  override the default value (2)
	EXCHANGE				//	11 by default
};


//	At this point, if you want to declare a variable to store a studentType value,
//  you have to use the following cumbersome declaration
enum studentType  myType = UNDERGRADUATE;

//	Fortunately, C lets you define aliases for types.  So you can write
typedef enum studentType StudentType;
//	Note that typedef is a C statement (processed by the compiler, not
//	by the preprocessor).


//	and now the following code is fine
StudentType s = GRADUATE;

//	Now look at the following complex aliasing declaration
#define INT_TYPE	0
#define FLOAT_TYPE	1
#define DOUBLE_TYPE	2
//
#define NUM_TYPE_SELECT	FLOAT_TYPE
#if NUM_TYPE_SELECT == INT_TYPE
	typedef int  NumType;
#elif NUM_TYPE_SELECT == FLOAT_TYPE
	typedef float  NumType;
#elif NUM_TYPE_SELECT == DOUBLE_TYPE
	typedef double  NumType;
#else
	#error invalid numerical type selected
#endif
//	And now I can code the rest of my program using only my custom
//	numerical type NumType.  Changing one line of code
//  (where I set the value of NUM_TYPE_SELECT) will determine
//	whether the rest of my programs will be dealing with int/float/double.
//	In case you wonder whether this is somehow related tro how C++ templates work...
//	Yes, in the first implementations of templates in C++, the whole thing was just
//	a gigantic #define/#ifdef hack.  Needless to say, all that spaghetti code broke
//	a lot.

//	Return on the enum type.  One benefit of the "default increment by 1" is that
//	if you start from 0 (default value anyway), then you can add an extra
//	enum value that will count the total number of enum values defined.
//	This may seem trivial to you, but C does not have any of the "how many elements
//	are there in..." capabilities of Java, so any little trick helps.
typedef enum carBrand {
	FORD = 0,
	CHEVY,
	FERRARI,
	//
	NUMBER_OF_BRANDS  // ---> here NUMBER_OF_BRANDS will be 3
} CarBrand;

CarBrand brand = CHEVY;

//	I can also define an array of string storing the names of different brands.
//	Note that:
//		- a C string is really an array of characters (more on that in another sample)
//		- In this case, I *have* to specify a max length for all strings
char brandName[NUMBER_OF_BRANDS][20] = {"Ford", "Chevy", "Ferrari"};

//	In the CarBrand declaration, note that two distinct things happen.
//	1. I declare an enum type
//			enum carBrand {
//							FORD = 0,
//							CHEVY,
//							FERRARI,
//							//
//							NUMBER_OF_BRANDS  // ---> here NUMBER_OF_BRANDS will be 3
//						};
//	2. I use typedef to say that I want to use simply the identifyer CarBrand for enum carBrand
//		typedef enum carBrand Carbrand;
//	I broke this down into two steps for the StudentType enumerated type.  Here I did
//	it in one step

// Note that in fact you don't need to use two different identifiers, one for the
//	enumetion declaration, and one for the typdef, as I did so far.  Note that I was
//	doing
//	enum studentType { ...};
//
//	and then
//	typedef enum studentType StudentType;
//
//	Or in one shot
//	typedef enum carBrand { ... } CarBrand;
//
//	Contrast this to what I do below for TerrainType

// You can also use the default increment by one to associate ranges
//	of values to different categories
typedef enum TerrainType {
	UNKNOWN = -1,
	//
	SEA = 0,
	//
	FORREST = 100,			//	the way I write it, all the terrain types
	TROPICAL_FORREST,		//	in the range 100-199 will be some kind of
	TEMPERATE_FORREST,		//	forest
	//
	DESERT = 200,
	//
	FIELD = 300,
	WHEAT_FIELD,
	RICE_FIELD,
	POLDER,
	//
	PRAIRIE = 400,
	//
	HELL = 10000
} TerrainType;


//	You are also going to encounter structs.  A struct is like a class with
//	no methods, and in which all the variables are public instance variables.
//	Here I grouped in one statement the definition of the struct proper and
//	the definition of the alias.  Feel free to experiment with taking them
//	apart to make sure you understand how it works
//	Note another change compared to the way I defined my StudentType type:
//	Here I used the same identifier for defining the struct SurveyData
//	and for my "final" type SurveyData.  This may seem weird (no tme to explain
//	why here) but it works.  If you find this disturbing, feel free to use
//	different identifiers.
typedef struct SurveyData {
	int x, y;	//	coordinates of a survey point on a grid
	float elev;	//	elevation at that point
	TerrainType type;
} SurveyData;

//	Personally, for a long time, I would have written this as
//typedef struct sSurveyData {
//	int x, y;	//	coordinates of a survey point on a grid
//	float elev;	//	elevation at that point
//	TerrainType type;
//} SurveyData;
//
//	I used an "e" prefix for my "construction" enum type, an "s" prefix
//	for the construction struct type, and "u" prefix for union type

//----------------------------------------
//	Action starts here
//----------------------------------------

int main(int argc, const char* argv[])
{
	//	OK, this program does nothing, it was all about preprocessor and
	//	type definitions

 //   return 0;
}

