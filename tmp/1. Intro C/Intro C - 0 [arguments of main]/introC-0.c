/*--------------------------------------------------------------+
|		A simple C program to demonstrate some basic elements	|
|	of the language to Java programmers.						|
|																|
|	Jean-Yves Hervé, 3DG URI									|
|	2017-09-13, nodified 2019-09-03, revised 2020-12-30			|
+--------------------------------------------------------------*/

//	Like all statements prefixed with a #, this one is addressed to the "precompiler,"
//	which does some pre-processing prior to the compiler proper actually starting
//	its work (lexicographic analysis).
//	Here, the precompiler will simply replace the line below by the verbatim contents
//	of the file stdio.h, which defines constants and functions prototypes (more on that
//	in the next  code samples) related to terminal I/O (standard input and standard output,
//	in Unix parlance)
#include <stdio.h>

//	Compare the "parameter signature" of a C program's main function to that of the
//	main method of a Java application.
//	Java Reminder:  int main(String[] arg) {...}
//	The parameters of the main function are in fact the name of the executable
//	and the arguments passed to the program when it is launched from the console.
int main(int argc, char* argv[]) {

	//	Just to prove the point about #include being nothing more than a command to
	//	insert the contents the contents of a file at the current point...
	//	Please note that
	//		1. I use double quotes, not brackets around the name of the file (more on
	//			that in another sample)
	//		2. The file doesn't need to have a .c or .h extension:  The file itself is
	//			not looked at by the compiler.  Its contents are included here and the 
	//			resulting text is fed to the compiler.
	#include "sillyInclude.fragment"
	
	//	The next code sample will dive into formatted output.  Right now, I am
	//	just illustrating the fact that we can get and print the arguments passed
	//	when the program was launched from the command line.
	//	Here the %d is a formating instruction.  It says that an integer should be included
	//	in the output string about there.  So, the first (and only) parameter in the list
	//	following the format string, argc, is to be interpreted as an int.
	//	The \n at the end of the format string is an end of line character.
	printf("argc = %d\n", argc);
	
	//	Here, argv[0] is to be interpreted as a C-style string.  This is what %s means
	printf("name of the executable: %s\n", argv[0]);
	
	printf("executable: %s and argc=%d\n", argv[0], argc);


	for (int k=1; k<argc; k++)
		//	Here I print the first parameter, k as an int and the second, argv[k],
		//	as a C-style string.  The \t at the beginning of the format string is
		//	a "tab" character, so that the output get indented
		printf("\targument %d: %s\n", k, argv[k]);
	
	printf("Bye.\n\n");
	
	//	The declared type of the main function is:  int.  Therefore, you must return
	//	an int value (some compilers will report an error if you don't. Other compilers
	//	may simply report a warning.)
	//	Conventionally, a program returns a 0 value to indicate termination with no error.
	//	Negative values are for system errors.
	//	Positive values are for error codes that you define for different kinds of problems
	//	that may be encountered during the execution of the program (e.g. file not found,
	//	invalid data, etc.).
    return 0;
}
