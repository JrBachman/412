#include <algorithm>	//	for max
//
#include "flipV.h"
#include "flipH.h"

using namespace std;

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
unsigned char toGrayAvg_(const unsigned char* rgba);

/**
 * Returns the max value among ther red, green, blue channels of a pixel
 * @param rgba	pointer to the start of an RGBA32 pixel
 */
unsigned char toGrayMax_(const unsigned char* rgba);


#if 0
//-------------------------------------------------------------------
#pragma mark -
#pragma mark Public functions
//-------------------------------------------------------------------
#endif


RasterImage flipV(const RasterImage& imageIn)
{
	RasterImage imageOut = newImage(imageIn.width, imageIn.height,
									imageIn.type, 1);
	
	const unsigned char* rasterIn = static_cast<const unsigned char*>(imageIn.raster);
	unsigned char* rasterOut = static_cast<unsigned char*>(imageOut.raster);
	for (unsigned short i=0; i<imageIn.height; i++)
	{
		for (unsigned short j=0; j<imageIn.width; j++)
		{
			memcpy(rasterOut + i*imageIn.bytesPerRow, 
				   rasterIn + (imageIn.height-i-1)*imageIn.bytesPerRow, 
				   imageIn.bytesPerRow);
		}
	}

	return imageOut;
}

RasterImage flipH(const RasterImage& imageIn)
{
	RasterImage imageOut = newImage(imageIn.width, imageIn.height,
									imageIn.type, 1);
	
	switch (imageIn.type)
	{
// 		case GRAY_RASTER:
// 			{
// 				const unsigned char** rasterIn = static_cast<const unsigned char**>(imageIn.raster2D);
// 				unsigned char** rasterOut = static_cast<unsigned char**>(imageOut.raster2D);
// 				for (unsigned short i=0; i<imageIn.height; i++)
// 				{
// 					for (unsigned short j=0; j<imageIn.width; j++)
// 					{
// 						rasterOut[i][j] = rasterIn[i][imageIn.width-j-1];
// 					}
// 				}
// 			}
// 			break;
// 			
		case RGBA32_RASTER:
			{
				const int* rasterIn = static_cast<const int*>(imageIn.raster);
				int* rasterOut = static_cast<int*>(imageOut.raster);
				for (unsigned short i=0; i<imageIn.height; i++)
				{
					for (unsigned short j=0; j<imageIn.width; j++)
					{
						rasterOut[i*imageIn.bytesPerRow + j] = rasterIn[i*imageIn.bytesPerRow + (imageIn.width-j-1)];
					}
				}
			}
			break;
		
		//	unsupported
		default:
			exit(100);
			break;
	}
	
	return imageOut;
}



#if 0
//-------------------------------------------------------------------
#pragma mark -
#pragma mark "Private" functions
//-------------------------------------------------------------------
#endif

unsigned char toGrayAvg_(const unsigned char* rgba)
{
	return static_cast<unsigned char>((rgba[0] + rgba[1] + rgba[2])/3);
}

/**
 * Returns the max value among ther red, green, blue channels of a pixel
 * @param rgba	pointer to the start of an RGBA32 pixel
 */
unsigned char toGrayMax_(const unsigned char* rgba)
{
	return static_cast<unsigned char>(max(rgba[0], max(rgba[1], rgba[2])));
}

