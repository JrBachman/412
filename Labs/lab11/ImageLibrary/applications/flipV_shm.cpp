/*----------------------------------------------------------------------------------+
|	flipV_shm																		|
|																					|
|	Jean-Yves Hervé, 2021-04-20														|
+----------------------------------------------------------------------------------*/

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <errno.h>
//
#include "ImageIO.h"
#include "flipV.h"

using namespace std;


//--------------------------------------------------------------
//--------------------------------------------------------------
int main(int argc, const char* argv[])
{
	//--------------------------------------------
	//	Part 1:	What argument(s) do you expect?
	//--------------------------------------------
	const char* sharedSegmentName = argv[1];
	int segSize, imageWidth, imageHeight, imageBPR;
	if (sscanf(argv[2], "%d", &segSize) -1)
	{
		cout << "Segment size not read" << endl;
		exit(21);
	}
	if (sscanf(argv[3], "%d", &imageWidth) -1)
	{
		cout << "Image width not read" << endl;
		exit(22);
	}
	if (sscanf(argv[4], "%d", &imageHeight) -1)
	{
		cout << "Image width not read" << endl;
		exit(23);
	}
	if (sscanf(argv[5], "%d", &imageBPR) -1)
	{
		cout << "Image bytes per row not read" << endl;
		exit(24);
	}

	cout << "Flip V process width = " << imageWidth << endl;
	//--------------------------------------------
	//	Part 2:	Setup shared memory segment
	//--------------------------------------------

	//-------------------------------------------------------
	//	Part 3:	Produce an image object that stores its 
	//			raster in shared memory
	//-------------------------------------------------------
	RasterImage imageIn  /* = {, , , , } */;
	
	// set image fields (width, height, raster)

	//	Perform the conversion to gray
	RasterImage mirrorImage = flipV(imageIn);

	//----------------------------------------------------------
	//	Part 4:	Copy flipped image raster back in shared memory 
	//----------------------------------------------------------
	//memcpy(      ,        ,  );
	//       ^         ^     ^
	//       |         |     |
	//       |      raster  size (num of bytes)
	//    shared segment	


	//--------------------------------------------
	//	Part 2:	Unmap shared memory
	//--------------------------------------------

	
	//	Cleanup allocations.  Again, this is not really needed, since the full
	//	partition will get cleared when the process terminates, but I like to
	//	keep the good habit of freeing memory that I don't need anymore, and,
	//	if I crash, it's a sign that something went wrong earlier and I may
	//	have produced junk
	freeImage(mirrorImage);

	return 0;
}

