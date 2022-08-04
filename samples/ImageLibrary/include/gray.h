#ifndef GRAY_H
#define GRAY_H

#include "RasterImage.h"

/**
 * Converts an image to gray-level values
 * 
 * @param imageIn	pointer to the image to convert to gray
 * @return a gray version of the input image
 * 
 */
RasterImage toGray(const RasterImage& imageIn);

#endif
