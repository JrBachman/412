/*----------------------------------------------------------------------------------+
|	crop	produced a copy of the input image that has been cropped				|
|																					|
|	Usage:																			|
|			./crop <image file path>  <output folder path>	x y w h					|
|	where x y are the coordinates  of the crop rectangles upper-left corner, and	|
|	w and h are the width and height of that rectangle.								|
|	If the image is named bottles.tga, then the image produced is 					|
|		bottles [cropped].tga														|
|																					|
|	Jean-Yves Hervé, 2020-10-29														|
+----------------------------------------------------------------------------------*/

#include <iostream>
#include <cstdlib>
#include <cstring>
//
#include "ImageIO.h"
#include "crop.h"

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
	//	2x codes:	command line argument errors
	WRONG_NUMBER_OF_ARGUMENTS = 20,
	INVALID_CROP_X_TYPE,
	INVALID_CROP_Y_TYPE,
	INVALID_CROP_CORNER,
	INVALID_CROP_WIDTH_TYPE,
	INVALID_CROP_HEIGHT_TYPE,
	INVALID_CROP_SIZE
	
};


#if 0
//-------------------------------------------------------------------
#pragma mark -
#pragma mark Function prototypes
//-------------------------------------------------------------------
#endif

/** interprets the program's input argument to determine the crop region.
 *	@param	argv	list of input argument to the program
 *	@param	cropCornerX		x coordinate of crop region's corner
 *	@param	cropCornerY		y coordinate of crop region's corner
 *	@param	cropWidth		crop region's width
 *	@param	cropHeight		crop region's height
 */
void extractCropRegion(const char* argv[],
						unsigned short imageWidth, unsigned short imageHeight,
						unsigned short* cropCornerX, unsigned short* cropCornerY,
						unsigned short* cropWidth, unsigned short* cropHeight);


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
 *	then the output file path will be ../Output/cells [cropped].tga
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
//		inputImagePath outFolderPath x y width height
//	It returns an error code (0 for no error)
//--------------------------------------------------------------
int main(int argc, const char* argv[])
{
	//	We need 6 arguments: filePath outputPath cornerX cornerY croppedW croppedW
	if (argc != 7)
	{
		cout << "Proper usage: crop inputImagePath outFolderPath x y width height" << endl;
		return WRONG_NUMBER_OF_ARGUMENTS;
	}
	
	//	Just to look prettier in the code, I give meaningful names to my arguments
	const char* inputImagePath = argv[1];
	const char* outFolderPath = argv[2];

	//	Read the image
	RasterImage image = readImage(inputImagePath);

	//	extract the parameters of the crop region
	unsigned short cropCornerX, cropCornerY, cropWidth, cropHeight;
	extractCropRegion(argv, image.width, image.height,
					  &cropCornerX, &cropCornerY, &cropWidth, &cropHeight);
	
	//	Perform the cropping
	RasterImage croppedImage = cropImage(image, cropCornerX, cropCornerY, cropWidth, cropHeight);

	// Produce the path to the output file
	const char* outFilePath = produceOutFilePath(inputImagePath, outFolderPath);
	
	//	Write out the cropped image
	int err = writeImage(croppedImage, outFilePath);
	
	//	Cleanup allocations.  Again, this is not really needed, since the full
	//	partition will get cleared when the process terminates, but I like to
	//	keep the good habit of freeing memory that I don't need anymore, and,
	//	if I crash, it's a sign that something went wrong earlier and I may
	//	have produced junk
	freeImage(image);
	freeImage(croppedImage);
	free(const_cast<char*>(outFilePath));

	return err;
}

void errorReport(ErrorCode code, const char* input)
{
	if (code != NO_ERROR)
	{
		switch (code)
		{
			case WRONG_NUMBER_OF_ARGUMENTS:
			break;
			
			case INVALID_CROP_X_TYPE:
				printf("Third argument is not a positive integer: %s\n", input);
			break;
			
			case INVALID_CROP_Y_TYPE:
				printf("Fourth argument is not a positive integer: %s\n", input);
			break;
			
			case INVALID_CROP_CORNER:
				printf("The crop region's upper-left corner must be within the image.\n");
			break;
			
			case INVALID_CROP_WIDTH_TYPE:
				printf("Fifth argument is not a positive integer: %s\n", input);
			break;
			
			case INVALID_CROP_HEIGHT_TYPE:
				printf("Sixth argument is not a positive integer: %s\n", input);
			break;
			
			case INVALID_CROP_SIZE:
			break;
			
			default:
				break;
		}
		exit(code);
	}
}

void extractCropRegion(const char* argv[],
					   unsigned short imageWidth, unsigned short imageHeight,
					   unsigned short* cropCornerX, unsigned short* cropCornerY,
					   unsigned short* cropWidth, unsigned short* cropHeight)
{
	if (sscanf(argv[3], "%hu", cropCornerX) != 1)
		errorReport(INVALID_CROP_X_TYPE, argv[3]);

	if (sscanf(argv[4], "%hu", cropCornerY) != 1)
		errorReport(INVALID_CROP_Y_TYPE, argv[3]);

	//	Note: since we read into an unsigned int, a negative value would come out
	//	as a large positive value
	if ((*cropCornerX >= imageWidth) || (*cropCornerY >= imageHeight))
		errorReport(INVALID_CROP_CORNER, nullptr);

	if (sscanf(argv[5], "%hu", cropWidth) != 1)
		errorReport(INVALID_CROP_WIDTH_TYPE, argv[3]);

	if (sscanf(argv[6], "%hu", cropHeight) != 1)
		errorReport(INVALID_CROP_HEIGHT_TYPE, argv[3]);

	//	Note: since we read into an unsigned int, a negative value would come out
	//	as a large positive value
	if ((*cropCornerX + *cropWidth > imageWidth) ||
		(*cropCornerY + *cropHeight > imageHeight))
		errorReport(INVALID_CROP_SIZE, nullptr);
	
	//	Otherwise, all is ok, go back to crop
}

const char* produceOutFilePath(const char* inputImagePath, const char* outFolderPath)
{
	const char suffixPlusExt[] = " [cropped].tga";

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

