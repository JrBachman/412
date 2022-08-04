#include <algorithm>	//	for max
//
#include "crop.h"
#include "gray.h"
#include "flipV.h"
#include "flipH.h"
#include "rotate.h"

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


/**	Produces a rotated copy of the input image (rotated by 90 degree clockwise).
 *	This function currently only works for RGBA32_RASTER images.
 *	@param	image	pointer to the RGBA32_RASTER image to rotate
 *	@return	a new image struct that stores the rotated image
 */
RasterImage rotateImage90(const RasterImage& image);

/**	Produces a rotated copy of the input image (rotated by 180 degree clockwise).
 *	This function currently only works for RGBA32_RASTER images.
 *	@param	image	pointer to the RGBA32_RASTER image to rotate
 *	@return	a new image struct that stores the rotated image
 */
RasterImage rotateImage180(const RasterImage& image);

/**	Produces a rotated copy of the input image (rotated by 270 degree clockwise).
 *	This function currently only works for RGBA32_RASTER images.
 *	@param	image	pointer to the RGBA32_RASTER image to rotate
 *	@return	a new image struct that stores the rotated image
 */
RasterImage rotateImage270(const RasterImage& image);


#if 0
//-------------------------------------------------------------------
#pragma mark -
#pragma mark Public functions
//-------------------------------------------------------------------
#endif


int compareImages(const RasterImage& image1, const RasterImage& image2)
{
	//	Note:
	//	I am of the "a function has a single entry point and therefore,
	//	as much as possible, should have a single exit (return point), except
	//	when it completely obfuscates the code, which is not the case here, IMHO.
	bool isSame = true;
	
	//	Only look at pixel data if type and dimensions are the same
	if (image1.type == image2.type &&
		image1.width == image2.width &&
		image1.height == image2.height)
	{
		switch(image1.type)
		{
			case RGBA32_RASTER:
			//	When declaring variables within a switch case,
			{
				int** raster1 = static_cast<int**>(image1.raster2D);
				int** raster2 = static_cast<int**>(image2.raster2D);
				for (unsigned short i = 0; isSame && i < image1.height; i++)
				{
					for (unsigned int j = 0; isSame && j < image1.width; j++)
					{
						isSame = (raster1[i][j] == raster2[i][j]);
					}
				}
			}
			break;
			
			case GRAY_RASTER:
			{
				unsigned char** raster1 = static_cast<unsigned char**>(image1.raster2D);
				unsigned char** raster2 = static_cast<unsigned char**>(image2.raster2D);
				for (unsigned short i = 0; isSame && i < image1.height; i++)
				{
					for (unsigned int j = 0; isSame && j < image1.width; j++)
					{
						isSame = (raster1[i][j] == raster2[i][j]);
					}
				}

			}
			break;
			
			case FLOAT_RASTER:
			{
				float** raster1 = static_cast<float**>(image1.raster2D);
				float** raster2 = static_cast<float**>(image2.raster2D);
				for (unsigned short i = 0; isSame && i < image1.height; i++)
				{
					for (unsigned int j = 0; isSame && j < image1.width; j++)
					{
						isSame = (raster1[i][j] == raster2[i][j]);
					}
				}

			}
			break;
			
			default:
				isSame = false;
				
			break;
		}
	}
	else
	{
		isSame = false;
	}
	
   return static_cast<int>(!isSame);
}



RasterImage cropImage(const RasterImage& imageIn,
					  unsigned short cropCornerX, unsigned short cropCornerY,
					  unsigned short cropWidth, unsigned short cropHeight)
{
	RasterImage imageOut;
	
	switch (imageIn.type)
	{
		case RGBA32_RASTER:
		imageOut = newImage(cropWidth, cropHeight, RGBA32_RASTER, 0);
		break;
		
		case GRAY_RASTER:
		imageOut = newImage(cropWidth, cropHeight, GRAY_RASTER, 0);
		break;

		case FLOAT_RASTER:
		imageOut = newImage(cropWidth, cropHeight, FLOAT_RASTER, 0);
		break;
		
		default:
		exit(31);
		break;
	}
	
	//	Beware that the images are stored upside-down from the way we view them,
	//	So I need to invert the row indices.
	for (unsigned short i = 0; i<cropHeight; i++)
	{
		//	version using the 1D rasters
		const unsigned char* inRow = static_cast<unsigned char*>(imageIn.raster) +
									(imageIn.height - i - cropCornerY - 1)*imageIn.bytesPerRow;
		unsigned char* outRow = static_cast<unsigned char*>(imageOut.raster) +
								(imageOut.height - i - 1)*imageOut.bytesPerRow;
								
		memcpy(outRow, inRow + cropCornerX * imageIn.bytesPerPixel,
			   cropWidth*imageIn.bytesPerPixel);

		//	version using the 2D rasters
//		memcpy(((unsigned char**) imageOut.raster2D)[imageOut.height - i - 1],
//			   ((unsigned char**) imageIn.raster2D)[imageIn.height - i - cropCornerY - 1]
//							   					+ cropCornerX * imageIn.bytesPerPixel,
//			   cropWidth*imageIn.bytesPerPixel);
		
	}

	return imageOut;
}

RasterImage toGray(const RasterImage& imageIn)
{
	RasterImage imageOut = newImage(imageIn.width, imageIn.height,
									GRAY_RASTER, 1);
		
	switch (imageIn.type)
	{
		//	image out is simply a copy of image in
		case GRAY_RASTER:
			memcpy(imageOut.raster, imageIn.raster,
				   imageIn.height*imageIn.bytesPerRow);
			break;
			
		case RGBA32_RASTER:
			{
				const int** rasterIn = static_cast<const int**>(imageIn.raster2D);
				unsigned char** rasterOut = static_cast<unsigned char**>(imageOut.raster2D);
				for (unsigned short i=0; i<imageIn.height; i++)
				{
					for (unsigned short j=0; j<imageIn.width; j++)
					{
						const unsigned char* rgba = reinterpret_cast<const unsigned char*>(rasterIn[i] + j);
						//	access pixel at column j on row i
						rasterOut[i][j] = toGrayMax_(reinterpret_cast<const unsigned char*>(rgba));
					}
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

RasterImage flipV(const RasterImage& imageIn)
{
	RasterImage imageOut = newImage(imageIn.width, imageIn.height,
									imageIn.type, 1);
	
	const unsigned char** rasterIn = static_cast<const unsigned char**>(imageIn.raster2D);
	unsigned char** rasterOut = static_cast<unsigned char**>(imageOut.raster2D);
	for (unsigned short i=0; i<imageIn.height; i++)
	{
		for (unsigned short j=0; j<imageIn.width; j++)
		{
			memcpy(rasterOut[i], rasterIn[imageIn.height-i-1], imageIn.bytesPerRow);
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
		case GRAY_RASTER:
			{
				const unsigned char** rasterIn = static_cast<const unsigned char**>(imageIn.raster2D);
				unsigned char** rasterOut = static_cast<unsigned char**>(imageOut.raster2D);
				for (unsigned short i=0; i<imageIn.height; i++)
				{
					for (unsigned short j=0; j<imageIn.width; j++)
					{
						rasterOut[i][j] = rasterIn[i][imageIn.width-j-1];
					}
				}
			}
			break;
			
		case RGBA32_RASTER:
		case FLOAT_RASTER:
			{
				const int** rasterIn = static_cast<const int**>(imageIn.raster2D);
				int** rasterOut = static_cast<int**>(imageOut.raster2D);
				for (unsigned short i=0; i<imageIn.height; i++)
				{
					for (unsigned short j=0; j<imageIn.width; j++)
					{
						rasterOut[i][j] = rasterIn[i][imageIn.width-j-1];
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


RasterImage rotate(const RasterImage& image, RotationVal rotVal)
{
	RasterImage outImage;
	switch(rotVal)
	{
		case NO_ROTATION:
		{
			outImage = copyRasterImage(image);
		}
		break;
		
		case ROTATE_90:
		{
			outImage = rotateImage90(image);
		}
		break;
		
		case ROTATE_180:
		{
			outImage = rotateImage180(image);
		}
		break;
		
		case ROTATE_270:
		{
			outImage = rotateImage270(image);
		}
		break;
		
		//	do shut warnings up
		default:
			break;
	}

	return outImage;
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


//	In a rotation by 90 degree clockwise, the pixel at row i, col j in the input image
//	ends up at row outHeight - j - 1, col i in the output image
RasterImage rotateImage90(const RasterImage& image)
{
	RasterImage outImage = newImage(image.height, image.width, RGBA32_RASTER);
	int* rasterIn = (int*)(image.raster);
	int* rasterOut = (int*)(outImage.raster);

	for (unsigned i=0; i<image.height; i++)
	{
		for (unsigned j=0; j<image.width; j++)
			rasterOut[(outImage.height-j-1)*outImage.width + i] =
			rasterIn[i*image.width + j];
	}

	return outImage;
}

//	In a rotation by 180 degree clockwise, the pixel at row i, col j in the input image
//	ends up at row height - i - 1, col weight - j in the output image
RasterImage rotateImage180(const RasterImage& image)
{
	RasterImage outImage = newImage(image.width, image.height, RGBA32_RASTER);

	int* rasterIn = (int*)(image.raster);
	int* rasterOut = (int*)(outImage.raster);

	for (unsigned i=0; i<image.height; i++)
	{
		for (unsigned j=0; j<image.width; j++)
			rasterOut[(outImage.height-i-1)*outImage.width + outImage.width - j -1] =
			rasterIn[i*image.width + j];
	}

	return outImage;
}

//	In a rotation by 270 degree clockwise, the pixel at row i, col j in the input image
//	ends up at row j, col outWidth - i - 1 in the output image
RasterImage rotateImage270(const RasterImage& image)
{
	RasterImage outImage = newImage(image.height, image.width, RGBA32_RASTER);
	int* rasterIn = (int*)(image.raster);
	int* rasterOut = (int*)(outImage.raster);

	for (unsigned i=0; i<image.height; i++)
	{
		for (unsigned j=0; j<image.width; j++)
			rasterOut[j*outImage.width + outImage.width - i - 1] =
			rasterIn[i*image.width + j];
	}

	return outImage;
}
