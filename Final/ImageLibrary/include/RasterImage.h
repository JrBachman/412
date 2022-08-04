#ifndef	RASTER_IMAGE_H
#define	RASTER_IMAGE_H

/**	This enumerated type is used by the image reading code.  You shouldn't have
 *	to touch this
 */
typedef enum ImageFileType
{
		kUnknownType = -1,
		kTGA_COLOR,				//	24-bit color TGA image file
		kTGA_GRAY,				//	8-bit gray-level TGA image file
		kPPM,					//	24-bit color PGM/PPM image file
		kPGM,					//	8-bit gray-level PGM/PPM image file
		kJPEG,					//	jpeg image file
		kPNG					//	png image file
} ImageFileType;

/**	This is the enum type that refers to images loaded in memory, whether
 *	they were read from a file, played from a movie, captured from a live
 *	video stream, or the result of calculations.
 *	Feel free to edit and add types you need for your project.
 */
typedef enum ImageType
{
		/**	Color image with 4 bytes per pixel
		 */
		RGBA32_RASTER,

		/**	Gray image with 1 byte per pixel
		 */
		GRAY_RASTER,

		/**	Monochrome image (either gray or one color channel of a color image)
		 *	stored in a float raster
		 */
		FLOAT_RASTER,
		
			
} ImageType;

/**	This is the data type to store all relevant information about an image.  After
 *	some thought, I have decided to store the 1D and 2D rasters as void* rather than
 *	having separate unsigned char and float pointers
 */
typedef	struct RasterImage {

	/**	Type of image stored
	 */
	ImageType type;

	/**	Number of rows (height) of the image
	 */
	unsigned short height;

	/**	Number of columns (width) of the image
	 */
	unsigned short width;

	/**	Pixel depth
	 */
	unsigned int bytesPerPixel;

	/**	Number of bytes per row (which may be larger than
	 *	bytesPerPixel * nbCols if rows are padded to a particular
	 *	word length (e.g. multiple of 16 or 32))
	 */
	unsigned short bytesPerRow;

	/**	Pointer to the image data, cast to a void* pointer.  To
	 *	access the data, you would have to cast the pointer to the
	 *	proper type, e.g.
	 *	<ul>
	 *		<li><tt>(int*) raster</tt></li>
	 *		<li><tt>(unsigned char*) raster</tt></li>
	 *		<li><tt>(int*) raster</tt></li>
	 *		<li><tt>(float*) raster</tt></li>
	 *	</ul>
	 */
	void* raster;

	/* Similarly here the 2D raster was cast to a void* pointer
	 *  and would need to be cast back to the proper type to be used, e.g.
	 *	<ul>
	 *		<li><tt>(int**) raster</tt></li>
	 *		<li><tt>(unsigned char**) raster</tt></li>
	 *		<li><tt>(int**) raster</tt></li>
	 *		<li><tt>(float**) raster</tt></li>
	 *	</ul>
	 */
	void* raster2D;
	
} RasterImage;


/**	returns a properly initialized RasterImage (including the 1D raster).
 *	@param	width	number of columns of the image
 *	@param	height	number of rows of the image
 *	@param	type	one of RGBA32_RASTER, GRAY_RASTER, or FLOAT_RASTER
 *	@param	wordSizeRowPadding word size at which rows should be rounded up.
 *			Could be one of 0, 4, 8, 16, 32, 64.
 *			You should use a padding size of 4 for a GRAY_RASTER raster
 *			if you intend to use OpenGL to render the image.
 */
RasterImage newImage(unsigned short width, unsigned short height, ImageType type,
					 unsigned char wordSizeRowPadding=0);

/**	Produces a copy of the input image (rotation by 0).
 *	This function currently only works for RGBA32_RASTER images.
 *	@param  image	pointer to the RGBA32_RASTER image to copy
 *	@return	a new image struct that stores a copy of the input image
 */
RasterImage copyRasterImage(const RasterImage& image);



/**	Assigns a new raster (and accompanying 2D raster to an image.  Frees the original
	raster if it had been allocated.  This function cannot verify whether the new
	raster is actually properly allocated, at the correct dimensions.  I was thinking
	of making this function private to ImageIO.cpp, but it is convenient.  Just
	be careful when using it.
	
	@param	img	pointer to the image to which to assign a new raster
	@param  raster	the new, hopefully properly allocated raster to assign to
						the image
 */
void setRaster(RasterImage& img, void* raster);


/**	Frees all heap-allocated storage for a Rasterimage object
 *	@param img	pointer to the RasterImage object to delete.
 */
void freeImage(RasterImage& img);



#endif	//	RASTER_IMAGE_H
