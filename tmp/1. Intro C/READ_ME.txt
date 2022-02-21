How to compile and run the program from the CLI
===============================================

0) Please note that all my source files are meant to be viewed with a tab length = 4.
You can change this is the preferences/settings of your text editor


1) Create a folder where you will build and run the program.
You can of course build in the folder that the code came into when
you expanded the archive.  Generally, I would recommend that you
always build your programs in separate folders.  As your programs get
more complex, building the app will result in the creation of a bunch
of object files, and you don't want to get those mixed with your other
documents

2) cd into that folder

3) build the executable:
       gcc  basicC_0.c -o myApp
    
   gcc         is the C compiler
   basicC_0.c  is the source file to compile
   -o is the gcc option that lets you select the name of the output
   myApp is the name of the executable your want to build [feel free to use another name]

4) execute the program

   ./myApp
   
   In the Unix file system (that we will study this semester),  "."  refers to the
   current folder.  By default,  "." is not on your $PATH configuration list that
   stores all the folders where to look for executables to run.  You could
   add "." to $PATH, but this is not recommended, for security reasons.  This
   is why when you execute from the console, you need to explicitly say where
   to find the executable you want to run.

5) To execute a program with some arguments (if the program is ready to handle them)

   ./myApp arg1 arg2 arg3
   
