#ifndef CHANNEL_H
#define CHANNEL_H

#include "RasterImage.h"


/**
 * "kills" the red color chanel of an image
 *
 * @param image	The image to kill the red channel of
 */
void killRedChannel(RasterImage* image);

/**
 * "kills" the green color chanel of an image
 *
 * @param image	The image to kill the green channel of
 */
void killGreenChannel(RasterImage* image);

/**
 * "kills" the blue color chanel of an image
 *
 * @param image	The image to kill the blue channel of
 */
void killBlueChannel(RasterImage* image);



#endif	//	CHANNEL_H
