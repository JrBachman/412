/*----------------------------------------------------------------------------------+
|	comp	does not produce a new image. It compares two image files				|
|																					|
|	Usage:																			|
|			./rotate <image file 1 path>  <image file 2 path>						|
|	The program returns 0 if the two images are identical, 1 otherwise.				|
|																					|
|	Jean-Yves Hervé, 2020-10-29														|
+----------------------------------------------------------------------------------*/
#include <iostream>
#include <cstdlib>
#include <cstring>
//
#include "ImageIO.h"
#include "comp.h"

using namespace std;

#if 0
//-------------------------------------------------------------------
#pragma mark -
#pragma mark Custom data types and global variables
//-------------------------------------------------------------------
#endif

/**	An enum type for all the errors that this program specifically handles
 */
using ErrorCode = enum
{
	NO_ERROR = 0,
	//
	//	1x codes:	file-related	--> not used in this program
//	FILE_NOT_FOUND = 10,
//	CANNOT_OPEN_FILE = 11,
//	WRONG_FILE_TYPE = 12,
//	CANNOT_WRITE_FILE = 13,
	//
	//	4x codes:	command line argument errors
	WRONG_NUMBER_OF_ARGUMENTS = 30,
	
};


/**  Main function for comp utility*/
 
int main(int argc, char** argv){
	//	We need 2 arguments: filePath1 filePath12
	if (argc != 3)
	{
		cout << "Proper usage: comp inputImagePath1 inputImagePath2" << endl;
		return WRONG_NUMBER_OF_ARGUMENTS;
	}
	
	//	Just to look prettier in the code, I give meaningful names to my arguments
	const char* inputImagePath1 = argv[1];
	const char* inputImagePath2 = argv[2];

	//	Read the image
	RasterImage image1 = readImage(inputImagePath1);
	RasterImage image2 = readImage(inputImagePath2);

	int same = compareImages(image1, image2);
		
	//	As usual, not really needed
	freeImage(image1);
	freeImage(image2);

    return same;
}
