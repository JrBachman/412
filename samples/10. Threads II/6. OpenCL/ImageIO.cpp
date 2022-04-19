#include <iostream>
#include <cstdlib>
#include <cstring>
//
#include "ImageIO_TGA.h"
#include "RasterImage.h"

using namespace std;

RasterImage readImage(const char *filePath)
{
    unsigned short imgWidth;
    unsigned short imgHeight;
    ImageType imgType;

    if (strcmp(filePath + strlen(filePath) - 4, ".tga") &&
		strcmp(filePath + strlen(filePath) - 4, ".TGA"))
	{
		cout << "Input file is not in TGA format" << endl;
		exit(11);
	}
	
	unsigned char *data = readTGA(filePath, &imgWidth, &imgHeight, &imgType);
	RasterImage img = newImage(imgWidth, imgHeight, imgType, 0);
	free(img.raster);
	img.raster = data;

	return img;
}

/**
 * Given an output filename and a RasterImage type. This function will create a TGA image
 * at the specified location.
 */
int writeImage(const char *filePath, const RasterImage *image){

    int error = writeTGA(filePath, static_cast<const unsigned char*>(image->raster),
						 image->width, image->height, image->type);

    if(error){
        printf("Error writing new image file\n");
        return error;
    }
    
    printf("Wrote image file at %s\n", filePath);

    return 0;
}

