#ifndef COMP_H
#define COMP_H

#include "RasterImage.h"

/**
 * Compares two images, returns 0 if the images are identical (dimensions, type,
 * pixel information), and 1 otherwise.
 * 
 * @param image1 reference to the first image
 * @param image2 reference to the second image
 * @return 	0 is the images are identical, 1 otherwise
 */
int compareImages(const RasterImage& image1, const RasterImage& image2);

#endif
