#include <stdio.h> 
#include <stdlib.h>
#include <sys/types.h> 
#include <unistd.h>
#include <sys/wait.h> 

void childProcess(int index, char* commandStr);

int main(int argc, char* argv[])
{
	//	Here I hard-code some tasks, as list of strings.

	system("ls -la");
	
	return 0;
}
		