#include <stdlib.h>
#include <string.h>
#include <stdio.h>
//
#include "ImageIO_TGA.h"
#include "RasterImage.h"

RasterImage readImage(char* filePath)
{
    unsigned short imgWidth;
    unsigned short imgHeight;
    ImageType imgType;

    if (strcmp(filePath + strlen(filePath) - 4, ".tga") &&
		strcmp(filePath + strlen(filePath) - 4, ".TGA"))
	{
		printf("Input file is not in TGA format\n");
		exit(11);
	}
	
	unsigned char *data = readTGA(filePath, &imgWidth, &imgHeight, &imgType);
	RasterImage img = newImage(imgWidth, imgHeight, imgType);
	setRaster(&img, data);

	return img;
}

int writeImage(RasterImage* image, char* filePath)
{
    int error = writeTGA(filePath, (unsigned char*)(image->raster),
						 image->width, image->height, image->type);

    if(error){
        printf("Error writing new image file\n");
        return error;
    }
    
    return 0;
}

