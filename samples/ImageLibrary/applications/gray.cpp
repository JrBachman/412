/*----------------------------------------------------------------------------------+
|	gray	produced a copy of the input image that has been converted to gray		|
|																					|
|	Usage:																			|
|			./gray <image file path>  <output folder path>							|
|	If the image is named bottles.tga, and the rotation selected was -l, then 		|
|	the image produced is 															|
|		bottles [gray].tga																|
|																					|
|	Jean-Yves Hervé, 2020-10-29														|
+----------------------------------------------------------------------------------*/
#include <iostream>
#include <cstdlib>
#include <cstring>
//
#include "ImageIO.h"
#include "gray.h"

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


#if 0
//-------------------------------------------------------------------
#pragma mark -
#pragma mark Function prototypes
//-------------------------------------------------------------------
#endif


/**	In this app, just prints out an error message to the console and
 *	exits with the proper error code.  In a fancier version, could
 *	write to a log and "swallow" some non-critical errors.
 *
 *	@param code		the code of the error to report/process
 *	@param input	the input string that caused the error (NULL otherwise)
 */
void errorReport(ErrorCode code, const char* input);


/**	Produces a complete path to the output image file.
 *	If the input file path was ../../Images/cells.tga and the
 *	and the output folder path is ../Output (with or without final slash),
 *	then the output file path will be ../Output/cells [gray].tga
 *
 *	@param inputImagePath	path to the input image
 *	@param outFolderPath	path to the output folder
 *	@return	complete path to the desired output file.
 */
const char* produceOutFilePath(const char* inputImagePath, const char* outFolderPath);

#if 0
//-------------------------------------------------------------------
#pragma mark -
#pragma mark Function implementations
//-------------------------------------------------------------------
#endif

//--------------------------------------------------------------
//	Main function, expecting as arguments:
//		inputImagePath outFolderPath
//	It returns an error code (0 for no error)
//--------------------------------------------------------------
int main(int argc, const char* argv[])
{
	//	We need 2 arguments: filePath outputPath
	if (argc != 3)
	{
		cout << "Proper usage: " << argv[0] << " inputImagePath outFolderPath" << endl;
//		cout << "Proper usage: gray inputImagePath outFolderPath" << endl;
		return WRONG_NUMBER_OF_ARGUMENTS;
	}

	//	Just to look prettier in the code, I give meaningful names to my arguments
	const char* inputImagePath = argv[1];
	const char* outFolderPath = argv[2];

	//	Read the image
	RasterImage image = readImage(inputImagePath);

	//	Perform the conversion to gray
	RasterImage grayImage = toGray(image);

	// Produce the path to the output file
	const char* outFilePath = produceOutFilePath(inputImagePath, outFolderPath);
		
	//	Write out the gray image
	int err = writeImage(grayImage, outFilePath);

	//	Cleanup allocations.  Again, this is not really needed, since the full
	//	partition will get cleared when the process terminates, but I like to
	//	keep the good habit of freeing memory that I don't need anymore, and,
	//	if I crash, it's a sign that something went wrong earlier and I may
	//	have produced junk
	freeImage(image);
	freeImage(grayImage);
	free(const_cast<char*>(outFilePath));

	return err;
}


const char* produceOutFilePath(const char* inputImagePath, const char* outFolderPath)
{
	const char suffixPlusExt[] = " [gray].tga";

	// Produce the name of the output file
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

	char* outFilePath = (char*) malloc(strlen(outFolderPath) +
										strlen(inputFileRootName) + strlen(suffixPlusExt) + 2);
	strcpy(outFilePath, outFolderPath);
	//	If outFolderPath didn't end with a slash, add it
	if (outFolderPath[strlen(outFolderPath)-1] != '/')
		strcat(outFilePath, "/");

	//	Append root name to output path, add " [cropped].tga"
	strcat(outFilePath, inputFileRootName);
	strcat(outFilePath, suffixPlusExt);
	
	//	free heap-allocated data we don't need anymore
	free(inputFileRootName);
	
	return outFilePath;
}
