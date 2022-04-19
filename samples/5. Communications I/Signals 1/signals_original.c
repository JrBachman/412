//
//  main.c
//  Signals 1
//
//  Code adapted from "Interprocess Communications in Unix - The Nooks & Crannies"
//						John Shapley Gray. Prentice Hall PTR, 1998.
//

#include <stdio.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

const char* file_name = "nohup.out";

int main(int argc, char* argv[])
{
	int new_stdout;
	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s command [arguments] \n", argv[0]);
		exit (1);
	}
	
	if (isatty( 1 ))
	{
		fprintf(stderr, "Sending output to %s\n", file_name);
		close( 1 );
		if ((new_stdout = open(file_name, O_WRONLY | O_CREAT | O_APPEND, 0644)) == -1)
		{
			perror(file_name);
			exit(2);
		}
	}
	if (signal(SIGHUP, SIG_IGN) == SIG_ERR)
	{
		perror ("SIGHUP");
		exit (3);
	}

	++argv;
	execvp (*argv, argv);
	perror(*argv);			//	Should not get here unless
	exit (4);				//	the exec fails
}


