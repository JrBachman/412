To compile this program, you simply type

	gcc introC-0.c -o prog

	        ^           ^
	        |           |
	source file         +-- or whatever name you want for the executable

and then you execute the program by typing

	./prog <list of arguments>

For example

	./prog 12 -56 hello world CSC412
	

Actually, it would be preferable for you to take the habit of always
building your programs asking for "all warnings."   

	gcc -Wall introC-0.c -o prog

Warnings are the compiler's way of telling you "I am not quite sure what
you wanted to do here, so I made some assumptions.  I hope it works out
OK." You definitely don't want your code to be ambiguous and force the
compiler to guess your intentions.  So, ask for all warning, and then
make sure to edit your code so that you don't get any warnings anymore.

