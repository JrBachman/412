typedef	struct map {

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
