//
//  main_unnamed.c
//  Unnamed Pipes Example
//
//  Code adapted from "Interprocess Communications in Unix - The Nooks & Crannies"
//						John Shapley Gray. Prentice Hall PTR, 1998.
//

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>


#define READ_END   0
#define WRITE_END  1

void alrm_action(int signal);

int main(int argc, const char * argv[])
{
	signal(SIGALRM, alrm_action);
	
	if (argc !=2) {
		fprintf(stderr, "Usage: %s message\n", *argv);
		exit(1);
	}
	
	printf("on this system, BUFSIZ = %d\n", BUFSIZ);
	

	//	Create the pipe to send data from the parent process to the child process.
	//	After the fork, the pipe will be accessible by both the parent process and
	//	the child process.  The parent will be writing on the WRITE_END while the child
	//	will be reading on the READ_END.
	int PtoC[2];
	
	if (pipe(PtoC) == -1) {
		perror("pipe creation failed");
		exit(2);
	}

	int p = fork();
	switch (p) {
		// problem
		case -1:
			perror("Fork");
			exit(3);
			break;
		
		// in the child
		case 0: {

				//	not required but general good practice
				close(PtoC[WRITE_END]);

				//	BUFSIZ system constant (Linux typically 64KB --> 65536)
				char message[BUFSIZ];
				memset(message, 0, BUFSIZ);
				if (read(PtoC[READ_END], message, BUFSIZ) != -1) {
					printf("Message received by child: [%s]\n", message);
					fflush(stdout);
				}
				else {
					perror("Read PtC");
					exit(4);
				}
				usleep(3000000);
			}
			printf("Child finishes\n");
			exit(0);
			break;
		
		//	in the parent
		default: {

			alarm(3);
			
			//	not required but general good practice
			close (PtoC[READ_END]);	
			
			printf("Parent about to send message to child, but sleep first\n");
//			usleep(2000000);
			
			//		  file descript.   char array   length of the string (no +1)
			if (write(PtoC[WRITE_END], argv[1], strlen(argv[1])) != -1) {
				printf("Message sent by parent: [%s]\n", argv[1]);
				fflush(stdout);
			}
			else {
				perror("Write");
				exit(5);
			}
			printf("Parent finishes\n");
			
			usleep(8000000);
			exit(0);
			break;				
		}
	}
	
    return 0;
}

void alrm_action(int signal)
{
	printf("alarm signal delivered (and eaten)\n");
}