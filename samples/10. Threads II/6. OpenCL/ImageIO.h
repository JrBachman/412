#ifndef	IMAGE_IO_H
#define	IMAGE_IO_H

#include "RasterImage.h"

/**
 * Given a filepath to a .tga file. This function will return a RasterImage type, 
 * which holds the contents of the given TGA file.
 */
RasterImage readImage(const char* filePath);

/**
 * Given an output filename and a RasterImage type. This function will create a TGA image
 * at the specified location.
 */
int writeImage(const char* filePath, const RasterImage* image);

#endif	//	IMAGE_IO_H
