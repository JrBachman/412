#ifndef	IMAGE_IO_TGA_H
#define	IMAGE_IO_TGA_H

#include "RasterImage.h"

/*	No-frills function that reads an image file in the <b>uncompressed</b>, un-commented TARGA 
 *	file format. If the image cannot be read (file not found, invalid format, etc.)
 *	the function simply terminates execution.
 */
unsigned char* readTGA(const char* filePath, unsigned short* width, unsigned short* height,
					   ImageType* theType);

/*	Writes an image file in the <b>uncompressed</b>, un-commented TARGA (<tt>.tga</tt>) file format.
 *	Returns 1 if the image was read successfully, 0 otherwise.
 */
int writeTGA(const char* filePath, const unsigned char* theData, 
			 unsigned short width, unsigned short height, ImageType type);

#endif	//	IMAGE_IO_TGA_H
