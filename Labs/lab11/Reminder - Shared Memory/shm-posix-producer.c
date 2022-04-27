/**
 * Simple program demonstrating shared memory in POSIX systems.
 *
 * This is the producer process that writes to the shared memory region.
 *
 * Figure 3.17
 *
 * @author Silberschatz, Galvin, and Gagne
 * Operating System Concepts - Ninth Edition
 * Copyright John Wiley & Sons - 2013
 *
 * modifications by dheller@cse.psu.edu, 31 Jan. 2014
 */
 //	accessed at
 //	http://www.cse.psu.edu/~deh25/cmpsc473/notes/OSC/Processes/shm.html

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <errno.h>

void display(const char *prog, const char *bytes, int n);

int main(int argc, const char* argv[])
{
	(void) argc;
	(void) argv;

	const char *name = "/shm-example";	// file name (the memory segment is in kernel space)
	const int SIZE = 4096;		// size of the shared memory segment to create

	char *ptr;		// shm_base is fixed, ptr is movable

	/* create the shared memory segment as if it was a file */
	// shm_fd is a regular file descriptor
	int shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);

	// Note:	O_CREAT specifies that the segment must be created if it doesn't already
	//		exist.  If the segment didn't already exist, then its owner and group will
	//		be the effective ownership and group of the process making the call.  
	//		If the segment already existed, then the owner and group are not changed,
	//		which may poe problems. 
	//		Specifying O_CREAT | O_EXCl instead of just O_CREAT verifies that the caller
	//		is the creator of the object.  Else the call fails with error result EEXISTS
	if (shm_fd == -1) {
		printf("prod: Shared memory failed: %s\n", strerror(errno));
		exit(1);
	}

	/* configure the size of the shared memory segment */
	ftruncate(shm_fd, SIZE);

	/* map the shared memory segment to the address space of the process */
	char* shm_base = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if (shm_base == MAP_FAILED) {
		printf("prod: Map failed: %s\n", strerror(errno));
		// close and shm_unlink?
		exit(1);
	}

	/**
	* Write to the mapped shared memory region.
	*
	* We increment the value of ptr after each write, but we
	* are ignoring the possibility that sprintf() fails.
	*/
	display("prod", shm_base, 64);
	ptr = shm_base;
	const char *message0 = "Studying ";
	const char *message1 = "Operating Systems ";
	const char *message2 = "Is Fun!";
	const char *msg_end  = "\n";
	ptr += sprintf(ptr, "%s", message0);
	ptr += sprintf(ptr, "%s", message1);
	ptr += sprintf(ptr, "%s", message2);
	ptr += sprintf(ptr, "%s", msg_end);
	display("prod", shm_base, 64);

	/* remove the mapped memory segment from the address space of the process */
	if (munmap(shm_base, SIZE) == -1) {
		printf("prod: Unmap failed: %s\n", strerror(errno));
		exit(1);
	}

	/* close the shared memory segment as if it was a file */
	if (close(shm_fd) == -1) {
		printf("prod: Close failed: %s\n", strerror(errno));
		exit(1);
	}

	return 0;
}

void display(const char *prog, const char *bytes, int n)
{
	printf("display: %s\n", prog);
	for (int i = 0; i < n; i++) { 
		printf("%02x%c", bytes[i], ((i+1)%16) ? ' ' : '\n'); 
	}
	printf("\n");
}
