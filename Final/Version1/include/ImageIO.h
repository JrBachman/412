#ifndef	IMAGE_IO_H
#define	IMAGE_IO_H

#include "RasterImage.h"

/**
 * Given a filepath to a .tga file. This function will return a RasterImage type, 
 * which holds the contents of the given TGA file.
 * @param	filePath	path to the file to read
 * @return the image read
 */
RasterImage readImage(const char* filePath);

/**
 * Given an output filename and a RasterImage object. This function will create a TGA image
 * at the specified location.
 * @param image	the raster image to write
 * @param filePath	destination file
 * @return	0 if no error, error code otherwise
 */
int writeImage(const RasterImage& image, const char* filePath);

#endif	//	IMAGE_IO_H
