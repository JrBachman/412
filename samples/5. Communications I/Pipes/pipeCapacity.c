//=========================================================================
//	This code comes from the page "Non-blocking I/O with pipes in C"
//	by Kadam Patel on GeeksForGeeks
//	http://www.geeksforgeeks.org/non-blocking-io-with-pipes-in-c/
//	-----------------------------------------------------------------------
//	Only minor editing done to fix errors and warnings in -Wall mode.
//	In particular, I kept all the identifiers unchanged (even the
//	annoying, so typically "Unix-y" abbreviated 'alrm_action'
//	(yeah, cut one letter out of twelve! This is so k3wl and rad!)
//	Being a complete egomaniac, I prefixed all my changes with a
//	comment line //jyh 2017
//
//	jyh 2017-10-13
//=========================================================================

// C program to illustrate
// finding capacity of pipe
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

//jyh 2017
#include <stdlib.h>	//	for exit system call

int count = 0;

// SIGALRM signal handler
void alrm_action(int signo)
{
    printf("Write blocked after %d characters\n", count);
    exit(0);
}

//jyh 2017
//int main()
int main(int argc, char* argv[])
{
    int p[2];
    char c = 'x';
	
    // SIGALRM signal
    signal(SIGALRM, alrm_action);
	
    // pipe error check
    if (pipe(p) == -1)
        exit(1);
	
    while (1) {
        alarm(5);
 
        // write 'x' at one time when capacity full
        // write() block and after 5 second alarm
        write(p[1], &c, 1);
 
        // send signal and alrm_action handler execute.
        ++count;
        alarm(0);
    }
}
