#ifndef FLIPV_H
#define FLIPV_H

#include "RasterImage.h"


/**
 * Flips an image vertically (symmetry about the middle horizontal axis)
 *
 * @param image	The image to flip
 * @return	a new image, obtained by flipping the input image vertically
 */
RasterImage flipV(RasterImage* image);


#endif	// FLIPV_H
