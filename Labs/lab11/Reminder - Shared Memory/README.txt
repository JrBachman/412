This version is copied, with very minor edits, from
	http://www.cse.psu.edu/~deh25/cmpsc473/notes/OSC/Processes/shm.html
which itself derived if from code in the 9th Ed. of Gavin et al. (the dinosaur textbook).

In this version, a "producer" process creates a shared memory segment,
maps it, write about 64 bytes worth of data in that shared segment, then
terminates (without  deleting/unlinking the shared segment).  Then a "consumer"
process is launched, which opens and maps the shared segment, to verify that
it can access the data written by the producer.  Before terminating, this
"consumer" process deletes the shared segment.

On Linux, either
	make clean run
	
	or:

	gcc -std=c99 -Wall -Wextra -D_XOPEN_SOURCE=700 -o cons shm-posix-consumer.c -lrt
	gcc -std=c99 -Wall -Wextra -D_XOPEN_SOURCE=700 -o prod shm-posix-producer.c -lrt

On mac OS, either 
	make clean run
	
	or:

	gcc -std=c99 -Wall -Wextra -o cons shm-posix-consumer.c
	gcc -std=c99 -Wall -Wextra -o prod shm-posix-producer.c
