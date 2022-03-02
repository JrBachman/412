#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//
#include "RasterImage.h"

//	returns a properly initialized RasterImage (including the 1D raster).
RasterImage newImage(unsigned short width, unsigned short height, ImageType type){

	RasterImage img;
	img.height = height;
	img.width = width;
	img.type = type;
	img.raster = NULL;
	img.raster2D = NULL;

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

	img.bytesPerRow = img.bytesPerPixel * width;
	unsigned int effectiveWidth = img.bytesPerRow / img.bytesPerPixel;
	
	//	I had plans for this variable, but not implemented so far. Jut shutting
	//	up the compiler warning in the meantime
	(void) effectiveWidth;

	void* raster = (void *) malloc(img.bytesPerRow * height);
	setRaster(&img, raster);
	
	return img;
}


/**	Frees all heap-allocated storage for a Rasterimage object
 *	@param img	pointer to the RasterImage object to delete.
 */
void freeImage(RasterImage* img)
{
    free(img->raster);
	free(img->raster2D);
}


void setRaster(RasterImage* img, void* raster)
{
	if (img->raster != NULL)
		free(img->raster);

	img->raster = raster;

	if (img->raster2D != NULL)
	free(img->raster2D);
	img->raster2D = NULL;
}

RasterImage copyRasterImage(RasterImage* image)
{
	RasterImage outImage = newImage(image->width, image->height, image->type);
	memcpy( (unsigned char*) outImage.raster,
			(char*) image->raster,
			image->height * image->bytesPerRow);
	
	return outImage;
}
