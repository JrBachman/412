/*****************************************************************************
 Excerpt from "Linux Programmer's Guide - Chapter 6"
 (C)opyright 1994-1995, Scott Burkett
 ***************************************************************************** 
 MODULE: fifoserver.c
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>


//#include <linux/stat.h>

#define FIFO_FILE       "/tmp/MYFIFO"

int main(int argc, char *argv[])
{
	FILE *fp;
	char readbuf[80];

// 	Create the FIFO if it does not exist */
	umask(0);
	mknod(FIFO_FILE, S_IFIFO|0666, 0);
 
	while(1)
	{
		//v1
// 		fp = fopen(FIFO_FILE, "r");
// 		fgets(readbuf, 80, fp);
		//v2
		int fd = open(FIFO_FILE, O_RDONLY);
		ssize_t size = read(fd, readbuf, 80);
		readbuf[size] = '\0';
		
		printf("Received string: %s\n", readbuf);
		fclose(fp);
	}

	return(0);
}
