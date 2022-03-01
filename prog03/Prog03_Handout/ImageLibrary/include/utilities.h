#ifndef UTILITIES_H
#define UTILITIES_H

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

#endif // UTILITIES_H
