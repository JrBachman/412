#!/bin/bash

#The f flag checks for files.
if [ -f $1 ];
then
    echo $1 is a ordinary file
#The d flag check for directories.
else 
	if [ -d $1 ];
	then
    	echo $1 is a directory
    fi
fi
