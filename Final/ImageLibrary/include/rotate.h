#ifndef ROTATE_H
#define ROTATE_H

#include "RasterImage.h"

/**	Simple enum type to report optional arguments for the program
 */
using RotationVal = enum 
{
	NO_ROTATION = 0,
	ROTATE_90,
	ROTATE_180,
	ROTATE_270,
	//
	NUM_ROTATIONS	//	automatically 4
	
};


/**
 * Applies to an image the rotation specified
 * 
 * @param image	The image to rotate
 * @return	a new image, obtained by flipping the input image horizontally
 */
RasterImage rotate(const RasterImage& image, RotationVal rotVal);


#endif	//	ROTATE_H
