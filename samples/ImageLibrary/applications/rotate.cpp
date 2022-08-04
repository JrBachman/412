/*----------------------------------------------------------------------------------+
|	rotate	produced a copy of the input image that has been rotated				|
|																					|
|	Usage:																			|
|			./rotate -<l|L|r|R]>+ <image file path>  <output folder path>			|
|	For example:																	|
|		o rotate -L		rotates the image by 90 degree counterclockwise				|
|		o rotate -LL 	rotates the image by 180 degree								|
|		o rotate -R		rotates the image by 90 degree clockwise					|
|	The program simplifies rotation specifications and ignore case differences.		|
|	For example, -RRr --> -L   -LLlrL --> -R										|
|	If the image is named bottles.tga, and the rotation selected was -l, then 		|
|	the image produced is 															|
|		bottles [l].tga																|
|																					|
|	Jean-Yves Hervé, 2020-10-29														|
+----------------------------------------------------------------------------------*/

//
//  rotate
//
//  Created by Jean-Yves Hervé on 2018-10-03
//	Revised 2020-10-29
//
#include <iostream>
#include <string>
#include <cstdio>
#include <cstring>
#include <cstdlib>
//
#include "ImageIO.h"
#include "rotate.h"

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
//	//	file-related	-->	Not used in this application
//	FILE_NOT_FOUND,
//	CANNOT_OPEN_FILE,
//	WRONG_FILE_TYPE,
//	CANNOT_WRITE_FILE,
	//
	//	command line argument errors
	WRONG_NUMBER_OF_ARGUMENTS,
	NO_DASH_ON_ROT_STRING,
	INVALID_ROT_STRING,
	//
	NUM_ERROR_CODES		//	correct value because I don't skip codes
	
};


/**	An array to store the suffix strings for the output file, for each
 *	rotation value.
 */
string ROT_SUFFIX[NUM_ROTATIONS] = {
	"",
	" [r]",
	" [rr]",
	" [l]"
};


/**	Going overly cute here:  Error message for each of the errors
 *	supported.  This one is tricky because it's easy to get the order
 *	different from that of the enum type.  In C++ enum types, we could
 *	directly associate a string to each enum value.
 */
string ERROR_STR[NUM_ERROR_CODES] = {
	"",		//	NO_ERROR
//	//	file-related errors
//	"",	//	FILE_NOT_FOUND,
//	"",	//	CANNOT_OPEN_FILE,
//	"",	//	WRONG_FILE_TYPE,
//	"",	//	CANNOT_WRITE_FILE,
	//	command line argument errors
	"Incorrect number of arguments.\nProper usage: rotate -{r|l}+ inputImagePath outFolderPath\n",	//	WRONG_NUMBER_OF_ARGUMENTS
	"Rotation specification must start with a dash",	//	NO_DASH_ON_ROT_STRING,
	"Rotation specification can only contain letters l or r"	//	INVALID_ROT_STRING,
};

#if 0
//-------------------------------------------------------------------
#pragma mark -
#pragma mark Function prototypes
//-------------------------------------------------------------------
#endif

/**	Processes the rotation-specifying string to determine the
 *	rotation to apply.
 *	@param	rotStr	the rotation-specifying string in the form -[r|l]+
 *	@param	rotVal	pointer to a RotationVal enum variable
 *	@return	an error code
 */
ErrorCode determineRotation(const char* rotStr, RotationVal* rotVal);

/**	In this app, just prints out an error message to the console and
 *	exits with the proper error code.  In a fancier version, could
 *	write to a log and "swallow" some non-critical errors.
 *
 *	@param code		the code of the error to report/process
 *	@param input	the input string that caused the error (NULL otherwise)
 */
void errorReport(ErrorCode code, const char* input);


/**	Produces a complete path to the output image file.
 *	If the input file path was ../../Images/clown and the 90-degree-rotated is to be
 *	written the output folder path is ../Output [with or without final slash),
 *	then the output file path will be ../Output/clown [r].tga
 *
 *	@param inputImagePath	path to the input image
 *	@param outFolderPath	path to the output folder
 *	@param rotVal			the rotation applied
 *	@return	complete path to the desired output file.
 */
const char* produceOutFilePath(const char* inputImagePath, const char* outFolderPath,
							   RotationVal rotVal);



#if 0
//-------------------------------------------------------------------
#pragma mark -
#pragma mark Function implementations
//-------------------------------------------------------------------
#endif

//--------------------------------------------------------------
//	Main function, expecting as arguments:
//		  {rotation} inputImagePath outFolderPath
//	e.g.	l ../Images/cells.tga ../Output
//	It returns an error code (0 for no error)
//--------------------------------------------------------------
int main(int argc, char* argv[])
{
	//	we need at least one argument
	if (argc != 4)
		errorReport(WRONG_NUMBER_OF_ARGUMENTS, NULL);
	
	//	Just to look prettier in the code, I give meaningful names to my arguments
	const char* inputImagePath = argv[2];
	const char* outFolderPath = argv[3];

	//	Interpret the rotation argument
	RotationVal rot;
	ErrorCode err = determineRotation(argv[1], &rot);
	if (err)
		errorReport(err, argv[1]);
	
	//	Read the image
	RasterImage image = readImage(inputImagePath);

	RasterImage outImage = rotate(image, rot);
	
	//	Produce the path to the output image
	const char* outFilePath = produceOutFilePath(inputImagePath, outFolderPath, rot);
	writeImage(outImage, outFilePath);

	//	Cleanup allocations.  Again, this is not really needed, since the full
	//	partition will get cleared when the process terminates, but I like to
	//	keep the good habit of freeing memory that I don't need anymore, and,
	//	if I crash, it's a sign that something went wrong earlier and I may
	//	have produced junk
	freeImage(outImage);
	freeImage(image);
	free(const_cast<char*>(outFilePath));
	
	return 0;
}

ErrorCode determineRotation(const char* rotStr, RotationVal* rotVal)
{
	//	string should start with a dash
	if (rotStr[0] != '-')
		return NO_DASH_ON_ROT_STRING;
	
	//	string should have characters besides the dash
	if (strlen(rotStr) < 2)
		return INVALID_ROT_STRING;
	
	*rotVal = NO_ROTATION;
	//	Iterate through the string, keep the count of poisitive and
	//	negative rotations
	int rotCount=0;
	for (unsigned int k=1; k<strlen(rotStr); k++)
	{
		switch (rotStr[k])
		{
			case 'r':
			case 'R':
				rotCount = (rotCount + 1) % NUM_ROTATIONS;
				break;
			
			case 'l':
			case 'L':
				rotCount = (rotCount + NUM_ROTATIONS - 1) % NUM_ROTATIONS;
				break;
			
			//	anything else is an error
			default:
				return INVALID_ROT_STRING;
				
		}
	}
	switch (rotCount)
	{
		case 1:
			*rotVal = ROTATE_90;
			break;
			
		case 2:
			*rotVal = ROTATE_180;
			break;
			
		case 3:
			*rotVal = ROTATE_270;
			break;
			
		case 0:
		default:
			*rotVal = NO_ROTATION;
			break;
	}
	
	return NO_ERROR;
}


void errorReport(ErrorCode code, const char* input)
{
	if (input != NULL)
		cout << ERROR_STR[code] << ": " << input << endl;
	else
		cout << ERROR_STR[code] << ": "  << endl;
	exit(code);
}


const char* produceOutFilePath(const char* inputImagePath, const char* outFolderPath,
							   RotationVal rotVal)
{
	// Produce the name of the output files
	//-------------------------------------
	//	First, find the start of the input file's name.  Start from the end
	//	and move left until we hit the first slash or the left end of the string.
	unsigned long index = strlen(inputImagePath) - 5;
	while ((index>=1) && (inputImagePath[index-1] != '/'))
		index--;

	//	Produce the name of the input file minus extension
	char* inputFileRootName = (char*) malloc(strlen(inputImagePath+index) +1);
	strcpy(inputFileRootName, inputImagePath+index);
	//	chop off the extension by replacing the dot by '\0'
	inputFileRootName[strlen(inputFileRootName)-4] = '\0';

	//	The +5 below is for the length of ".tga" plus the final string-terminating \0
	char* outFilePath = (char*) malloc(strlen(outFolderPath) +
									   strlen(inputFileRootName) +
									   ROT_SUFFIX[rotVal].length() + 5);
	strcpy(outFilePath, outFolderPath);
	//	If outFolderPath didn't end with a slash, add it
	if (outFolderPath[strlen(outFolderPath)-1] != '/')
		strcat(outFilePath, "/");

	//	Append root name to output path, add the suffix and the file extension
	strcat(outFilePath, inputFileRootName);
	strcat(outFilePath, ROT_SUFFIX[rotVal].c_str());
	strcat(outFilePath, ".tga");

	//	free heap-allocated data we don't need anymore
	free(inputFileRootName);
	
	return outFilePath;
}

