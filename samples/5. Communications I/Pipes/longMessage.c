//
//  main_unnamed.c
//  Unnamed Pipes Passing a Long Message
//
//  Code adapted from "Interprocess Communications in Unix - The Nooks & Crannies"
//						John Shapley Gray. Prentice Hall PTR, 1998.
//	Jean-Yves Hervé, 2018-04-09
//	In this version, the parent sends a list of float values to the child
//	process, which returns the sum of the elements in the list

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define READ_END   0
#define WRITE_END  1

void childFunc(int file_desc_PtoC[], int file_desc_CtoP[]);


int main(int argc, const char * argv[])
{
	//	Create the pipe to send data from the parent process to the child process.
	//	After the fork, the pipe will be accessible by both the parent process and
	//	the child process.  The parent will be writing on the WRITE_END while the child
	//	will be reading on the READ_END.
	int file_desc_PtoC[2];
	int file_desc_CtoP[2];
	
	if (pipe(file_desc_PtoC) == -1) {
		perror("Pipe P to C");
		exit(2);
	}
	if (pipe(file_desc_CtoP) == -1) {
		perror("Pipe C to P");
		exit(2);
	}

	int p = fork();
	switch (p) {
		// problem
		case -1:
			perror("Fork failed");
			exit(3);
			break;
		
		// in the child --> call a function, never to return from that call
		case 0:
			//	The child process needs to have access both pipes.  There are two
			//	ways to make the pipes accessible within childFunc:
			//		- make the variables global
			//		- pass them as parameters.
			//	Here, we are only talking of passing two pointers, so 16 bytes total
			//	It's a complete no-brainer that this is preferable to the general ugliness
			//	of unnecessary global declarations.
			childFunc(file_desc_PtoC, file_desc_CtoP);
			break;
		
		//	in the parent
		default: {

			//	These are the values that I want to pass to the child process
			int nbValues = 6;
			float value[] = {	4.5f,
								10.0f,
								-20.0f,
								0.5f,
								1.0f,
								1.5f};

			//	not required but general good practice: Close the end that
			//	we won't be using
			close(file_desc_PtoC[READ_END]);
			close(file_desc_CtoP[WRITE_END]);
			
			//	First, send the number of elements that follow
			if (write(file_desc_PtoC[WRITE_END], &nbValues, sizeof(int)) == -1)
			{
				perror("Parent Write array size failed");
				exit(5);
			}
			printf("Parent sent array size to the child\n");
			
			//	Then send the array's values
			if (write(file_desc_PtoC[WRITE_END], value, nbValues*sizeof(float)) == -1)
			{
				perror("Parent Write array failed");
				exit(5);
			}
			printf("Parent sent array to the child\n");

			usleep(1000000);
			
			printf("Parent now trying to read value back from child\n");

			float valueBack;
			if (read(file_desc_CtoP[READ_END], &valueBack, sizeof(float)) == -1) {
				perror(" Parent Read failed");
				exit(4);
			}
			
			//	We presumably got the sum back, so print it
			printf("Sum value gotten back = %.2f\n", valueBack);
		}
		break;
	}
	
	//	parent process termimates
    return 0;
}

//==========================================================
//	This is the function where the child process ends up.
//	Note that the we don't return from the function: It ends
//	with an exit when the child process is done.
//==========================================================

void childFunc(int file_desc_PtoC[], int file_desc_CtoP[]) {
	//	not required but general good practice: Close the end that
	//	we won't be using
	close(file_desc_PtoC[WRITE_END]);
	close(file_desc_CtoP[READ_END]);

	int nbValues;

	if (read(file_desc_PtoC[READ_END], &nbValues, sizeof(int)) == -1) {
		perror("\tChild Read 1 failed");
		exit(11);
	}
	
	//	Allocate an array to get the data in
	float* a = (float*) calloc(nbValues, sizeof(float));
	
	//	Now we expect to read that many float values in the next message
	if (read(file_desc_PtoC[READ_END], a, nbValues*sizeof(float)) == -1) {
		perror("\tChild Read 2 failed");
		exit(12);
	}

	//	We got the float values and we print them
	printf("\tThe child process received %d float values:\n\t\t", nbValues);
	for (int k=0; k<nbValues; k++) {
		printf("%.2f  ", a[k]);
	}
	printf("\n");

	//	Perform calculations (here, the sum of the elements)
	float sum = 0.f;
	for (int k=0; k<nbValues; k++) {
		sum += a[k];
	}
	
	//	No we send back the sum value to the parent process
	if (write(file_desc_CtoP[WRITE_END], &sum, sizeof(float)) == -1) {
		perror("\tChild write back failed");
		exit(13);
	}
	printf("\tChild sent back sum = %.2f\n", sum);

	printf("\tChild done.  Terminating.\n");
	//	This child process does one piece of calculation and then terminates.
	//	We could keep it going, receiving values from the parent and
	//	passing values back through the pipe.
	exit(0);
}
