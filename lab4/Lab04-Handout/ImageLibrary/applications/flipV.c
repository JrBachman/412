/*----------------------------------------------------------------------------------+
|	flipV	produced a copy of the input image that has been flipped vertically		|
|																					|
|	Usage:																			|
|			./flipV <image file path>  <output folder path>							|
|	If the image is named bottles.tga, then the image produced is 					|
|		bottles [flipV].tga															|
|																					|
|	Jean-Yves Hervé, 2020-10-29, rev. 2021-03-14 [C version]						|
+----------------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//
#include "utilities.h"
#include "ImageIO.h"
#include "flipV.h"

#if 0
//-------------------------------------------------------------------
#pragma mark -
#pragma mark Custom data types and global variables
//-------------------------------------------------------------------
#endif

/**	An enum type for all the errors that this program specifically handles
 */
typedef enum
{
	NO_ERROR = 0,
	//
	//	1x codes:	file-related	--> not used in this program
//	FILE_NOT_FOUND = 10,
//	CANNOT_OPEN_FILE = 11,
//	WRONG_FILE_TYPE = 12,
//	CANNOT_WRITE_FILE = 13,
	//
	//	2x codes:	command line argument errors
	WRONG_NUMBER_OF_ARGUMENTS = 20
	
} ErrorCode;


#if 0
//-------------------------------------------------------------------
#pragma mark -
#pragma mark Function prototypes
//-------------------------------------------------------------------
#endif


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
int main(int argc, char* argv[])
{
	//	We need 2 arguments: filePath outputPath
	if (argc != 3)
	{
		printf("Proper usage: flipV inputImagePath outFolderPath\n");
		return WRONG_NUMBER_OF_ARGUMENTS;
	}
	
	//	Just to look prettier in the code, I give meaningful names to my arguments
	char* inputImagePath = argv[1];
	char* outFolderPath = argv[2];

	//	Read the image
	RasterImage image = readImage(inputImagePath);


	//	Perform the vertical flip
	RasterImage mirrorImage = flipV(&image);

	// Produce the path to the output file
	char* outputFilePath = produceOutFilePath(inputImagePath, "[flipV]", outFolderPath);
	
	//	Write out the cropped image
	int err = writeImage(&mirrorImage, outputFilePath);
	
	//	Cleanup allocations.  Again, this is not really needed, since the full
	//	partition will get cleared when the process terminates, but I like to
	//	keep the good habit of freeing memory that I don't need anymore, and,
	//	if I crash, it's a sign that something went wrong earlier and I may
	//	have produced junk
	freeImage(&image);
	freeImage(&mirrorImage);
	free(outputFilePath);

	return err;
}

