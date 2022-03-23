#ifndef CROP_H
#define CROP_H

#include "RasterImage.h"


/**
 * Produces the new image resulting from a crop of the input image at the 
 *	location and dimensions specified.
 *
 *	@param	image			the image to crop
 *	@param 	cropCornerX		x coordinate of the upper-left corner of the crop region
 *	@param 	cropCornerY		y coordinate of the upper-left corner of the crop region
 *							(counted from the top, so height - row index - 1
 *	@param 	cropWidth		width of the crop region
 *	@param 	cropHeight		height of the top region
 *	@return		the new image resulting from applying the crop
 */
RasterImage cropImage(const RasterImage *image,
					  unsigned short cropCornerX, unsigned short cropCornerY,
					  unsigned short cropWidth, unsigned short cropHeight);


#endif	// CROP_H
