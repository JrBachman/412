#include <cstdlib>
#include <cstdio>
#include <cstring>
//
#include "RasterImage.h"

/**	returns a properly initialized RasterImage (including the 1D raster).
 *	@param	width	number of columns of the image
 *	@param	height	number of rows of the image
 *	@param	type	one of RGBA32_RASTER, GRAY_RASTER, or FLOAT_RASTER
 *	@param	wordSizeRowPadding word size at which rows should be rounded up.
 *			Could be one of 0, 4, 8, 16, 32, 64.
 *			You should use a padding size of 4 for a GRAY_RASTER raster
 *			if you intend to use OpenGL to render the image.
 */
RasterImage newImage(unsigned short width, unsigned short height, ImageType type,
					 unsigned char wordSizeRowPadding){

	//	by default, there is no padding --> word size is 1 byte
	if (wordSizeRowPadding == 0)
		wordSizeRowPadding = 1;
		
	RasterImage img;
	img.height = height;
	img.width = width;
	img.type = type;
	img.raster = nullptr;

	switch (type)
	{
		case RGBA32_RASTER:
			img.bytesPerPixel = 4;
			break;
		case GRAY_RASTER:
			img.bytesPerPixel = 1;
			break;

		case FLOAT_RASTER:
			img.bytesPerPixel = 4;
			break;
		
		default:
			printf("Unknown image type\n");
			exit(52);
	}

	img.bytesPerRow = ((img.bytesPerPixel * width + wordSizeRowPadding - 1)/wordSizeRowPadding)*wordSizeRowPadding;
	unsigned int effectiveWidth = img.bytesPerRow / img.bytesPerPixel;
	
	//	I had plans for this variable, but not implemented so far. Jut shutting
	//	up the compiler warning in the meantime
	(void) effectiveWidth;

	void* raster = (void *) malloc(img.bytesPerRow * height);
	setRaster(img, raster);
	
	return img;
}


/**	Frees all heap-allocated storage for a Rasterimage object
 *	@param img	pointer to the RasterImage object to delete.
 */
void freeImage(RasterImage& img)
{
    free(img.raster);
}


void setRaster(RasterImage& img, void* raster)
{
	if (img.raster != nullptr)
		free(img.raster);
		
   img.raster = raster;
}

RasterImage copyRasterImage(const RasterImage& image)
{
	RasterImage outImage = newImage(image.width, image.height, image.type);
	memcpy( (unsigned char*) outImage.raster,
			(char*) image.raster,
			image.height * image.bytesPerRow);
	
	return outImage;
}
