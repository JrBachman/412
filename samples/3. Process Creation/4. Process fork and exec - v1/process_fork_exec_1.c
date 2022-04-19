//======================================================================================
//	In this code sample shows how to use a system call of the execXX family to
//	launch another executable.
//	I am going to assume here that this program has been built with the executable
//	name prog (you can build with any name, just replace prog by your favorite 
//	name in what follows).
//
//	This program can built in three different version by changing the value of the
//	EXEC_VERSION constant:
//
//		o VP_PASS_ARGUMENTS: in this version, we use the execvp member of the execXX
//			family.  The "v" stands for vector (really array, as opposed to a list)
//			and the "p" indicates that the current path string should be used to look
//			for executables for the command.
//			You launch the program as  ./prog  <commmand> parameters
//			This will result in your process's entire partition (including code) 
//			being replaced by a new partition (including code) for executing <command> 
//			with the arguments provided.//
//			For example:  
//				./prog ls -la
//			You could even recompile the source code of this very program:
//				./prog gcc process_exec.c -o newProc2
//			or call the sum program 
//				./prog ./sum 12 4 20 8
//
//		o VP_BUILD_ARGUMENTS_1:	This version uses the execvp member of the execXX family,
//			and builds a hard-coded parameter array to launch "ls -la"
//			In this version, ./prog should therefore be launched without any parameter
//		
//		o VP_BUILD_ARGUMENTS_2:	This version uses the execvp member of the execXX family,
//			and dynamic allocation and random generation of arguments for the parameter 
//			array used to to launch the local ./sum program
//			In this version, ./prog should therefore be launched without any parameter
//		
//		o LP_FIXED_LIST_1:	This version uses the execlp member of the execXX family. 
//			The "l" in execlp indicates that the execlp function takes in a fixed list of
//			parameters.  The *number* of arguments must therefore be known at compilation
//			time.  This version is completely hard-coded and launches "./sum 4 23 12 20"
//			In this version, ./prog should be launched without any parameter
//
//		o LP_FIXED_LIST_2:	This version also uses the execlp member of the execXX family, 
//			so the number of arguments is again fixed, but we generate random values for 
//			the arguments, in this case to be passed to our ./sum executable when we
//			launch it.
//			In this version, ./prog should be launched without any parameter
//
//	Jean-Yves Hervé, 	University of Rhode Island
//						Department of Computer Science and Statistics
//	2018-02-15. Edited 2018-10-09
//======================================================================================

#include <stdio.h> 
#include <stdlib.h> 
#include <sys/types.h> 
#include <unistd.h>
#include <string.h> 
#include <time.h> 

//	The three choices of execXX function that this code sample demonstrate.  Read top
//	comments for explanation of the different options.
#define VP_PASS_ARGUMENTS		1
#define VP_BUILD_ARGUMENTS_1	2
#define VP_BUILD_ARGUMENTS_2	3
#define	LP_FIXED_LIST_1			4
#define	LP_FIXED_LIST_2			5

//	Select one of the above three choices
#define EXEC_VERSION	VP_PASS_ARGUMENTS

int main(int argc, char * argv[] )
{
	printf("In %s before doing execxx, with pid %d\n\n", argv[0], getpid());
	
	#if EXEC_VERSION == VP_PASS_ARGUMENTS
		//	here, argv[0] is the name of this executable.  argv[1] is the name of the
		//	executable to launch (and replace the current one by), and the following
		//	arguments are to be passed along to that new executable
		
		//	Again, this is not a function cal that you expect to return from.  If this call
		//	succeeds, then this process will completely replace its partition, including code,
		//	to start the execution of a different program.  Note that this process will
		//	retain its pid.  This is not a new process starting, just the same old process
		//	starting a new life, so to speak.
		//	Alternatively I could have written execvp(argv[1], &(argv[1]));
		execvp(argv[1], argv + 1);
		

	#elif EXEC_VERSION == VP_BUILD_ARGUMENTS_1
	
		//	In this version, the argv list should only have one element, argv[0] = "./prog"
		if (argc > 1) {
			printf("in VP_BUILD_ARGUMENTS_1 version, this program runs with no argument.\n");
			exit(-1);
		}
		
		//	When using execvp, I can also build my own array of command - arguments strings.
		//	Here, I launch ls -la  [list all files (including invisibles) in long display format].
		//	The list **must** be terminated by a NULL pointer
		char executablePath[] = "ls";					//	path to executable (/bin is on your execute path)
		char* command[] = {"ls",
							"-la",
							NULL};
// 		char executablePath[] = "./sum";				//	path to executable
// 		//	argv list for executable
// 		char* command[] = {	"my sum",					//	name of the command
// 							"1", "10", "2000", "8",		//	arguments
// 							NULL};						//	NULL termination
		execvp(executablePath, command);


	#elif EXEC_VERSION == VP_BUILD_ARGUMENTS_2

		//	In this version, the argv list should only have one element, argv[0] = "./prog"
		if (argc > 1) {
			printf("in VP_BUILD_ARGUMENTS_2 version, this program runs with no argument.\n");
			exit(-1);
		}
		
		//----------------------------------------------------------------------------------
		//	execvp allows us to build the list of arguments dynamically, when the number and
		//	value of the arguments was not known at compilation time for this program.
		//----------------------------------------------------------------------------------
		//	Seed the pseudo-random generator, using current time as seed.  This pretty
		//	much guarantees that executing twice the same program will produce the
		//	same random sequence.
		//	Note 1 for future use:  Normally this is only done once per program. Normally,
		//	one would only re-seed the random generator to compare the results of two
		//	algorithms (or same algorithms for 2 different configurations) for the same
		//	(long, not worth storing) pseudo-random sequence. So, run once with a hard-coded
		//	seed for Config. 1.  The reseed and run again for Config. 2.
		//	Note 2:  Under no circumstance should you use the C/C++ standard pseudo-random
		//	generator for serious simulation work:  It is an absolute piece of junk that 
		//	produces deeply correlated data.  Countless millions of $ have been wasted on
		//	R&D that ends up just analyzing artifacts of the C/C++ rand() algorithm.
		//	If you are going to do serious work, use a reputable third-party library
		//	(some are open-source, or even public-domain).  If you are Java or Python
		//	developer, you can use the built-in generator. as they use decent algorithms.	
		srand((unsigned int) time(NULL));
				
		//	Fill in the values
		//---------------------
		//	Path to the executable:  ./sum
		char* executablePath = (char*) malloc(10*sizeof(char));
		strcpy(executablePath, "./sum");  //	path to executable
		
		//	Generate between 5 and 10 arguments for the ./sum binary
		int numArguments = (rand() % 6) + 5;

		//	my array of arguments to execvp needs 2 extra slots:  one for the name of 
		//	the command and one to store a terminating NULL pointer.
		char** arguments = (char**) calloc(numArguments+2, sizeof(char*));

		//	Name of the command  mySum.  This is what will appear as the argv[0]
		//	when the program starts
		arguments[0] = (char*) malloc(10*sizeof(char));
		strcpy(arguments[0], "mySum");

		printf("Will call ./sum with arguments");
		//	numArguments random values between 1 and 100 for the sum (converted to a C string)
		for (int k=1; k<=numArguments+1; k++) {			
			arguments[k] = (char*) malloc(4*sizeof(char));
			sprintf(arguments[k], "%d", ((rand() % 100) + 1));
			printf(" %s", arguments[k]);
		}
		printf("\n");
		
		//	The list **must** be terminated by a NULL pointer
		arguments[numArguments+1] = NULL;		
		
		execvp(executablePath, arguments);
		
		//	But...but..but, you don't free executablePath or the arguments[i], I hear you
		//	scream.  No need to do that:  The execlp command completely wipes out the 
		//	entire partition (including the code, and of course the data).
		//	We are not coming back from that call.
		printf("something went wrong\n");
		exit(11);

	#elif EXEC_VERSION == LP_FIXED_LIST_1

		//	In this version, the argv list should only have one element, argv[0] = "./prog"
		if (argc > 1) {
			printf("in VP_BUILD_ARGUMENTS_2 version, this program runs with no argument.\n");
			exit(-1);
		}

		printf("%s program, with pid=%d\n", argv[0], getpid());
		printf("Will call ./sum with arguments 4 23 12 20\n");

		//	Everything is hard-coded. Don't forget the terminating NULL pointer!
		//		path    name     ------arguments-------  terminator
		execlp("./sum", "My Sum", "4", "23", "12", "20", NULL);
		// never reached
		printf("something went wrong\n");
		exit(11);

	#elif EXEC_VERSION == LP_FIXED_LIST_2

		//	In this version, the argv list should only have one element, argv[0] = "./prog"
		if (argc > 1) {
			printf("in VP_BUILD_ARGUMENTS_2 version, this program runs with no argument.\n");
			exit(-1);
		}

		//	In this version, I am going to launch ./sum with 4 arguments
		char sumArg1[5], sumArg2[5], sumArg3[5], sumArg4[5];
		
		//	Seed the pseudo-random generator, using current time as seed.
		srand (time(NULL));
		//	pseudo-random values between 1 and 5000 for the sum (converted to a C string)
		sprintf(sumArg1, "%d", ((rand() % 5000) + 1));
		sprintf(sumArg2, "%d", ((rand() % 5000) + 1));
		sprintf(sumArg3, "%d", ((rand() % 5000) + 1));
		sprintf(sumArg4, "%d", ((rand() % 5000) + 1));
		printf("Will call ./sum with arguments %s %s %s %s\n", sumArg1, sumArg2, sumArg3, sumArg4);

		//	Don't forget the terminating NULL pointer!
		//		path    name     ----------arguments---------------  terminator
		execlp("./sum", "My Sum", sumArg1, sumArg2, sumArg3, sumArg4, NULL);


	#else
		printf("Invalid choice for execXX version\n");
	#endif
		
	//	if we are are still running this code, then exec failed
	perror("exec failure"); 
	exit (1);

	return 0;
}
