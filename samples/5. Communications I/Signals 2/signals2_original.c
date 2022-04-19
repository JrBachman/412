//
//  main.c
//  Signals 2
//
//  Created by Jean-Yves Hervé on 2018-02-20.
//  Copyright © 2018 URI 3D Group. All rights reserved.
//

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

void signal_catcher1(int);
void signal_catcher2(int);
void signal_catcher2SecondTime(int);
void signal_catcher2Terminal(int the_sig);

int main(int argc, char* argv[])
{
	if (signal(SIGINT, signal_catcher2) == SIG_ERR)
	{
		perror( "SIGINT");
		exit (1);
	}

// 	if (signal(SIGQUIT, signal_catcher1) == SIG_ERR)
// 	{
// 		perror("SIGQUIT");
// 		exit (2);
// 	}
	if (signal(SIGKILL, signal_catcher1) == SIG_ERR)
	{
		perror("SIGKILL");
		exit (12);
	}

	// Forever display a number
	for (int i =0; ; ++i)
	{
		printf("%i\n", i);
		sleep(1);
		
		if (i > 30)
			if (signal(SIGINT, signal_catcher2Terminal) == SIG_ERR)
			{
				perror( "SIGINT");
				exit (1);
			}
		
	}
	
	return 0;
}

void signal_catcher1(int the_sig)
{
//	signal(the_sig, signal_catcher);	//	reset
	printf("\nSignal %d received.\n", the_sig);
// 	if (the_sig == SIGQUIT)
// 		exit (3);
}

void signal_catcher2(int the_sig)
{
// 	signal(the_sig, signal_catcher2SecondTime);	//	reset
// 	printf("\nSignal %d received (first).\n", the_sig);
// 	static int count = 0;
// 	
// 	count++;
// 	if (count == 1)
 		printf("\nSignal %d received (and ignored).\n", the_sig);
// 	else if (count==2){
// 		printf("\nSignal %d received for the second time (and ignored).\n", the_sig);
// 		}
// 	else
// 	{
// 		printf("\nSignal %d received for the third time --> exit.\n", the_sig);
// 		exit(0);
// 	}
}

void signal_catcher2SecondTime(int the_sig)
{
	printf("\nSignal %d received (second time).\n", the_sig);
	exit(1);
}

void signal_catcher2Terminal(int the_sig)
{
	printf("\nSignal %d received (last time).\n", the_sig);
	exit(1);
}


