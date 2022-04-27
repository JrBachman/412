#ifndef FLIP_H
#define FLIP_H

#include "RasterImage.h"

/**
 * Flips an image horizontally (symmetry about the middle vertical axis)
 * 
 * @param image	The image to flip
 * @return	a new image, obtained by flipping the input image horizontally
 */
RasterImage flipH(const RasterImage& image);


#endif	//	FLIP_H
