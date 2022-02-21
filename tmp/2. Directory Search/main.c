//
//  main.cpp
//  Directory Operations, C version
//
//  Created by Jean-Yves Hervé on 2017-02-15.
//	Edited 2018-02-25, 2019-11-04
//
//	This C program takes as argument the path to a folder and
//	outputs the name of all the files found in that folder
//
//	This version does it in two passes.
//		o The first pass counts the number of files in the directory
//		then the program allocates the array that will store file
//		names (now that the number of files is known)
//		o The second pass now reads and stores the names
//
//	For reference:  file types recognized by dirent
//		#define	DT_UNKNOWN	 0
//		#define	DT_FIFO		 1
//		#define	DT_CHR		 2
//		#define	DT_DIR		 4
//		#define	DT_BLK		 6
//		#define	DT_REG		 8
//		#define	DT_LNK		10
//		#define	DT_SOCK		12
//		#define	DT_WHT		14

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

int main(int argc, const char* argv[])
{
	//	usual test of argc

	const char* rootPath = argv[1];
    DIR* directory = opendir(rootPath);
    if (directory == NULL)
    {
		printf("data folder %s not found\n", rootPath);
		exit(1);
	}
	
    struct dirent* entry;
	int counter = 0;
	
	//	In C, there is no data structure that lets us add elements as we are going,
	//	only arrays that we need to allocate at the proper size.  
	//	For this type of problems, there are three possible solutions:
	//		1. "resize" the array each time a new element must be added (this means
	//			allocate a new array 1 element larger, copy the data from current
	//			array to new array, free the current array, update current array pointer)
	//		2. allocate an array at the maximum possible size for the problem, write
	//			data into this array while updating a counter.  Once the final count is
	//			known, allocate the "final" array at the proper size, copy all the data
	//			to it, free the original oversized array.
	//		3. do a first pass just to count the number of elements in the final output
	//			array.  Allocate the array at the proper size, then run a second pass,
	//			where this time we store the data in the output array.
	//
	//	Which solution to go for depends on your problem.  How large the "maximum" size
	//	of the output could be, how long it takes to check if an element "belongs," etc.
	//	Here, in the directory search problem, Solution 2 is not feasible, because there
	//	is no way to know what the maximum size could be (there could be thousands of 
	//	files in a directory.  Solution 1 is too inefficient, only justified when memory
	//	is *really* tight, so IU go for solution 3)

	//--------------------------------------------------------------------------
	//	First pass: count the entries, excluding . .. and other directories.
	//--------------------------------------------------------------------------
    while ((entry = readdir(directory)) != NULL)
    {
        char* name = entry->d_name;
        if ((name[0] != '.') && (entry->d_type == DT_REG))
        {
			counter++;
        }
    }
	closedir(directory);
	
	printf("%d files found\n", counter);

	//--------------------------------------------------------------------------
	//	Now that we know its size, allocate the array of file names
	//--------------------------------------------------------------------------
    char** fileName = (char**) malloc(counter*sizeof(char*));

	//--------------------------------------------------------------------------
	//	Second pass: actually store the file names that we read
	//--------------------------------------------------------------------------
	int k=0;
	directory = opendir(rootPath);
    while ((entry = readdir(directory)) != NULL)
    {
        char* name = entry->d_name;
        if ((name[0] != '.') && (entry->d_type == DT_REG))
        {
			fileName[k] = (char*) malloc(strlen(name)+1);
			strcpy(fileName[k], name);
			k++;
        }
    }
	closedir(directory);


	for (int k=0; k<counter; k++)
		printf("\t%s\n", fileName[k]);
	
	return 0;
}
