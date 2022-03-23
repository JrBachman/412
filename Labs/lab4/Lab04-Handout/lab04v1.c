#include <stdio.h> 
#include <stdlib.h>
#include <sys/types.h> 
#include <unistd.h>
#include <sys/wait.h> 

int main(int argc, char* argv[])
{
	//	Here I hard-code some tasks, as list of strings.
	char* TASK_LIST[] = {
							"./flipV ../../Images/cells.tga ../../Output",
							"./flipH ../../Images/peppers.tga ../../Output",
// 							"gray ../../Images/cells.tga ../../Output",
// 							"gray ../../Images/clown.tga ../../Output",
							"./flipV ../../Images/peppers.tga ../../Output"};
														
	//	Note, this only works with automatically-allocated arrays, not arrays
	//	allocated by calloc/malloc/new
	int NUM_TASKS = (int)(sizeof(TASK_LIST)/sizeof(char*));	
	
	for (int k=0; k<NUM_TASKS; k++)
	{
		use "system" to execute the task
	}
	
		
	return 0;
}
		