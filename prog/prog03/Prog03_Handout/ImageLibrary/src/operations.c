#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//
#include "gray.h"
#include "flipV.h"


#if 0
//-------------------------------------------------------------------
#pragma mark -
#pragma mark Prototypes of "private" functions
//-------------------------------------------------------------------
#endif

/**
 * Returns the unweighted average among ther red, green, blue channels
 * of a pixel
 * @param rgba	pointer to the start of an RGBA32 pixel
 */
unsigned char toGrayAvg_(unsigned char* rgba);

/**
 * Returns the max value among ther red, green, blue channels of a pixel
 * @param rgba	pointer to the start of an RGBA32 pixel
 */
unsigned char toGrayMax_(unsigned char* rgba);

/** computes the max of thre color channels
 *	@param r	red channel
 *	@param g	green channel
 *	@param b	blue channel
 *	@return the max value between r, g, b
 */
unsigned char ucmax(unsigned char r, unsigned char g, unsigned char b);



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



RasterImage toGray(RasterImage* imageIn)
{
	RasterImage imageOut = newImage(imageIn->width, imageIn->height,
									GRAY_RASTER);
		
	switch (imageIn->type)
	{
		//	image out is simply a copy of image in
		case GRAY_RASTER:
			memcpy(imageOut.raster, imageIn->raster,
				   imageIn->height*imageIn->bytesPerRow);
			break;
			
		case RGBA32_RASTER:
			{
				int* rasterIn = (int*)(imageIn->raster);
				unsigned char* rasterOut = (unsigned char*)(imageOut.raster);
				for (unsigned short i=0; i<imageIn->height; i++)
				{
					for (unsigned short j=0; j<imageIn->width; j++)
					{
						unsigned char* rgba = (unsigned char*)(rasterIn + j);
						//	access pixel at column j on row i
						rasterOut[j] = toGrayMax_((unsigned char*)rgba);
					}
					rasterIn += imageIn->width;
					rasterOut += imageIn->width;
				}
			}
			break;
		
		//	unsupported
		case FLOAT_RASTER:
		default:
			exit(100);
			break;
	}
	
	return imageOut;
}

RasterImage flipV(RasterImage* imageIn)
{
	RasterImage imageOut = newImage(imageIn->width, imageIn->height,
									imageIn->type);
	
	//	rowIn starts at the top of the input image (and will go down)
	unsigned char* rowIn = (unsigned char*)(imageIn->raster);
	//	rowOut starts at the bottom of the output image (and will go up)
	unsigned char* rowOut = (unsigned char*)(imageOut.raster) +
								(imageIn->height-1)*imageIn->bytesPerRow;

	for (unsigned short i=0; i<imageIn->height; i++)
	{
		memcpy(rowOut, rowIn, imageIn->bytesPerRow);
		rowIn += imageIn->bytesPerRow;
		rowOut -= imageIn->bytesPerRow;
	}

	return imageOut;
}


RasterImage flipH(RasterImage* imageIn)
{
	RasterImage imageOut = newImage(imageIn->width, imageIn->height,
									imageIn->type);
	
	if (imageIn->type != RGBA32_RASTER)
	{
		printf("flipH currently only supports RGBA32 images");
		exit(10);
	}
	
	//	rowIn and imgOut start at the top of their respective image and
	//	both work their way down
	unsigned int* rowIn = (unsigned int*)(imageIn->raster);
	//	rowOut starts at the bottom of the output image (and will go up)
	unsigned int* rowOut = (unsigned int*)(imageOut.raster);

	for (unsigned short i=0; i<imageIn->height; i++)
	{
		for (unsigned int jIn=0, jOut=imageIn->width-1; jIn<imageIn->width; jIn++, jOut--)
		{
			// copy a pixel's 4 bytes in one shot
			rowOut[jOut] = rowIn[jIn];
		}
		
		rowIn += imageIn->width;
		rowOut += imageIn->width;
	}
	return imageOut;
}


void killRedChannel(RasterImage* image)
{
	int* raster = (int*)(image->raster);
	
	for (unsigned int i=0, mirrorI=image->height-1; i<image->height; i++, mirrorI--)
	{
		for (unsigned int j=0; j<image->width; j++)
		{
			//	x86 CPUs are small-endian, therefore the order of bytes in an int
			//	is inverted.  So within the int it looks as if we have ABGR
			//	
			//										preserve -----------+-+-+
			//															| | |
			//															A B G R
			raster[i*image->width + j] = raster[i*image->width + j] & 0xFFFFFF00;
			//																  |
			//											kill -----------------+
		}
	}		
}
void killGreenChannel(RasterImage* image)
{
	int* raster = (int*)(image->raster);
	
	for (unsigned int i=0, mirrorI=image->height-1; i<image->height; i++, mirrorI--)
	{
		for (unsigned int j=0; j<image->width; j++)
		{
			//	x86 CPUs are small-endian, therefore the order of bytes in an int
			//	is inverted.  So within the int it looks as if we have ABGR
			//	
			//										preserve -----------+-+---+
			//															| |   |
			//															A B G R
			raster[i*image->width + j] = raster[i*image->width + j] & 0xFFFF00FF;
			//																|
			//											kill ---------------+
		}
	}		
}

void killBlueChannel(RasterImage* image)
{
	int* raster = (int*)(image->raster);
	
	for (unsigned int i=0, mirrorI=image->height-1; i<image->height; i++, mirrorI--)
	{
		for (unsigned int j=0; j<image->width; j++)
		{
			//	x86 CPUs are small-endian, therefore the order of bytes in an int
			//	is inverted.  So within the int it looks as if we have ABGR
			//	
			//										preserve -----------+---+-+
			//															|   | |
			//															A B G R
			raster[i*image->width + j] = raster[i*image->width + j] & 0xFF00FFFF;
			//															  |
			//											kill -------------+
		}
	}		
}



#if 0
//-------------------------------------------------------------------
#pragma mark -
#pragma mark "Private" functions
//-------------------------------------------------------------------
#endif

unsigned char toGrayAvg_(unsigned char* rgba)
{
	return (unsigned char)((rgba[0] + rgba[1] + rgba[2])/3);
}

unsigned char toGrayMax_(unsigned char* rgba)
{
	return ucmax(rgba[0], rgba[1], rgba[2]);
}



unsigned char ucmax(unsigned char r, unsigned char g, unsigned char b)
{
	unsigned char maxVal = b;
	
	if (r > maxVal)
	{
		if (g> r)
		{
			maxVal = g;
		}
		else
		{
			maxVal = r;
		}
	}
	else if (g > maxVal)
	{
		maxVal = g;
	}
	
	return maxVal;
}
