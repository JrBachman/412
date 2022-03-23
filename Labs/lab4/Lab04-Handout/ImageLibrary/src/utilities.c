#include <string.h>
#include <stdlib.h>
#include <math.h>
//
#include "utilities.h"


#if 0
//-------------------------------------------------------------------
#pragma mark -
#pragma mark Prototypes of "private" functions
//-------------------------------------------------------------------
#endif



/**	Produces a complete path to the output image file.
 *	If the input file path was ../../Images/cells.tga 
 *  the suffix to add is [gray],
 *	and the output folder path is ../Output (with or without final slash),
 *	then the output file path will be ../Output/cells [gray].tga
 *
 *	@param inputImagePath	path to the input image
 *  @param suffix			the suffix to add to the base file name
 *	@param outFolderPath	path to the output folder
 *	@return	complete path to the desired output file.
 */
char* produceOutFilePath(char* inputImagePath, char* suffix, char* outFolderPath);



#if 0
//-------------------------------------------------------------------
#pragma mark -
#pragma mark Public functions
//-------------------------------------------------------------------
#endif


char* produceOutFilePath(char* inputImagePath, char* suffix, char* outFolderPath)
{
	//	Produce the string suffix + extension (e.g. " [gray].tga")		
	//--------------------------------------------------------------
	//                                                    6 for " " + ".tga" + '\0'
 	char* suffixPlusExt = (char*) calloc(strlen(suffix) + 6, 1);
	strcpy(suffixPlusExt, " ");
	strcat(suffixPlusExt, suffix);
	strcat(suffixPlusExt, ".tga");

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
	free(suffixPlusExt);
	
	return outFilePath;
}


#if 0
//-------------------------------------------------------------------
#pragma mark -
#pragma mark "Private" functions
//-------------------------------------------------------------------
#endif


