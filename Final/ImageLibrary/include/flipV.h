#ifndef MIRRORV_H
#define MIRRORV_H

#include "RasterImage.h"


/**
 * Flips an image vertically (symmetry about the middle horizontal axis)
 *
 * @param image	The image to flip
 * @return	a new image, obtained by flipping the input image vertically
 */
RasterImage flipV(const RasterImage& image);


#endif
