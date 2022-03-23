#ifndef FLIPH_H
#define FLIPH_H

#include "RasterImage.h"


/**
 * Flips an image horizontally (symmetry about the middle vertical axis)
 *
 * @param image	The image to flip
 * @return	a new image, obtained by flipping the input image horizontally
 */
RasterImage flipH(RasterImage* image);


#endif	// FLIPH_H
