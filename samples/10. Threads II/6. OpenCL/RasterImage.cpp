#include <cstdlib>
#include <cstdio>
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
RasterImage newImage(unsigned short width, unsigned short height, ImageType type,unsigned char wordSizeRowPadding){

	RasterImage img;
	img.height = height;
	img.width = width;
	img.type = type;

	switch (type)
	{
		case RGBA32_RASTER:
			img.bytesPerPixel = 4;
			img.bytesPerRow = width * 4;
			break;
		case GRAY_RASTER:
			img.bytesPerPixel = 1;
			img.bytesPerRow = width;
			break;

		case FLOAT_RASTER:
			printf("Type FLOAT_RASTER not supported (yet)\n");
			exit(51);
			break;
		
		default:
			printf("Unknown image type\n");
			exit(52);
	}

	img.raster = (void *) malloc(img.bytesPerRow * height);
	if(img.raster == NULL)
	{
		printf("Raster allocation failed\n");
		exit(53);
	}
    //	raster2D stuff goes here

	return img;
}


/**	Frees all heap-allocated storage for a Rasterimage object
 *	@param img	pointer to the RasterImage object to delete.
 */
void freeImage(RasterImage *img)
{
    free(img->raster);
    
    //	raster2D stuff goes here
}
