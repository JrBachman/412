//=========================================================================
//	This code comes from the page "Non-blocking I/O with pipes in C"
//	by Kadam Patel on GeeksForGeeks
//	http://www.geeksforgeeks.org/non-blocking-io-with-pipes-in-c/
//
//	In this program, the parent process is going to operate as a server
//	that gets messages from the child process, which operates as a client.
//	The server checks every second for new messages, but the client only
//	send a message every 3 seconds.  In the default behavior of a pipeline,
//	the server would remain blocked for 2s until the client's message
//	arrives.  In this non-blocking setup, the server actually gets to check
//	the pipeline 3 times per second and get some work done (well, here, it
//	only sleeps) between checks.
//	-----------------------------------------------------------------------
//	Only minor editing done to fix errors and warnings in -Wall mode
//	Being a complete egomaniac, I prefixed all my changes with a
//	comment line //jyh 2017
//
//	jyh 2017-10-13
//=========================================================================

// C program to illustrate
// non I/O blocking calls
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h> 	// library for fcntl function

//jyh 2017
#include <stdlib.h>	//	for exit system call
#include <errno.h>	//	defines some error handling constants and errno global
void parent_read(int p[]);
void child_write(int p[]);


#define MSGSIZE 6
char* msg1 = "hello";
char* msg2 = "bye !!";

//jyh 2017
//int main()
int main(int argc, char* argv[])
{
    int p[2];
	
    // error checking for pipe
    if (pipe(p) < 0)
        exit(1);
	
    // error checking for fcntl
    if (fcntl(p[0], F_SETFL, O_NONBLOCK) < 0)
        exit(2);
	
    // continued
    switch (fork()) {
 
    // error
    case -1:
        exit(3);
 
    // 0 for child process
    case 0:
        child_write(p);
        break;
 
    default:
        parent_read(p);
        break;
    }
    return 0;
}

void parent_read(int p[])
{
	//jyh 2017
    //int nread;
    long nread;
    char buf[MSGSIZE];
	
    // write link
    close(p[1]);
	
    while (1) {
 
        // read call if return -1 then pipe is
        // empty because of fcntl
        nread = read(p[0], buf, MSGSIZE);
        switch (nread) {
        case -1:
 
            // case -1 means pipe is empty and errono
            // set EAGAIN
            if (errno == EAGAIN) {
                printf("(pipe empty)\n");
                sleep(1);
                break;
            }
			//jyh 2017
			//	This should not be happening.  If the read fails for a reason
			//	other than pipe empty, then something went wrong with the pipe.
			//	Here, we simply abort.
            else {
                perror("read");
                exit(4);
            }
 
        // case 0 means all bytes are read and EOF(end of conv.)
        case 0:
            printf("End of conversation\n");
 
            // read link
            close(p[0]);
 
            exit(0);

        default:
 
            // text read
            // by default return no. of bytes
            // which read call read at that time
            printf("MSG = %s\n", buf);
        }
    }
}


void child_write(int p[])
{
    int i;
	
    // read link
    close(p[0]);
	
    // write 3 times "hello" in 3 second interval
    for (i = 0; i < 3; i++) {
        write(p[1], msg1, MSGSIZE);
        sleep(3);
    }
	
    // write "bye" one times
    write(p[1], msg2, MSGSIZE);
	
    // here after write all bytes then write end
    // doesn't close so read end block but
    // because of fcntl block doesn't happen..
    exit(0);
}
