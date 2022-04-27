/*----------------------------------------------------------------------------------+
|	flipH_shm																		|
|																					|
|	Jean-Yves Hervé, 2021-04-20														|
+----------------------------------------------------------------------------------*/

#include <iostream>
#include <cstdlib>
#include <cstring>
//
#include "ImageIO.h"
#include "flipH.h"

using namespace std;


//--------------------------------------------------------------
//--------------------------------------------------------------
int main(int argc, const char* argv[])
{
	//--------------------------------------------
	//	Part 1:	What argument(s) do you expect?
	//--------------------------------------------
	(void) argc;
	(void) argv;

	//--------------------------------------------
	//	Part 2:	Setup shared memory segment
	//--------------------------------------------

	//-------------------------------------------------------
	//	Part 3:	Produce an image object that stores its 
	//			raster in shared memory
	//-------------------------------------------------------
	RasterImage imageIn;

	//	Perform the conversion to gray
	RasterImage mirrorImage = flipH(imageIn);

	//----------------------------------------------------------
	//	Part 4:	Copy flipped image raster back in shared memory 
	//----------------------------------------------------------


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

