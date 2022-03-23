//
//  fliph.c
//  Handout code for Lab04, CSC412
//
//	This small program flips an image vertically
//	The output image gets the [h] prefix to its name
//	e.g.   Tyler.tga ---> [h] Tyler.tga

#include <stdio.h>
#include <stdlib.h>	//	for malloc & calloc
#include <string.h>	//	for memcpy
//
#include "utilities.h"
#include "ImageIO.h"
#include "flipH.h"


int main(int argc, char** argv)
{
	if (argc != 3)
	{
		printf("Usage: flipH <input image path>  <output folder path>\n");
		exit(1);
	}
	
	//	Get the arguments.  Check for validity and "cleans up" the folder path
	char* inputImagePath = argv[1];
	char* outputFolderPath = argv[2];

	//	Read the image
	RasterImage image = readImage(inputImagePath);

	//	Perform the horizontal flip
	RasterImage mirrorImage = flipH(&image);

	// Produce the path to the output file
	char* outputFilePath = produceOutFilePath(inputImagePath, "[flipH]", outputFolderPath);
	
	//	Write out the cropped image
	int err = writeImage(&mirrorImage, outputFilePath);
	
	//	Cleanup allocations.  Again, this is not really needed, since the full
	//	partition will get cleared when the process terminates, but I like to
	//	keep the good habit of freeing memory that I don't need anymore, and,
	//	if I crash, it's a sign that something went wrong earlier and I may
	//	have produced junk
	freeImage(&image);
	freeImage(&mirrorImage);
	free(outputFilePath);

	return err;
}
