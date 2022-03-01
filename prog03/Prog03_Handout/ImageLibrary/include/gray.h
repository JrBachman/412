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
RasterImage toGray(RasterImage* imageIn);

/**
 * Converts an image to gray-level values. Multithreaded version
 * 
 * @param imageIn	pointer to the image to convert to gray
 * @param numThreads the number of threads between which to divide the task
 * @return a gray version of the input image
 * 
 */
RasterImage toGray_thr(RasterImage* imageIn, unsigned int numThreads);


#endif
