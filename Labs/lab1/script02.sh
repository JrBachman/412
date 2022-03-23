#!/bin/bash 
#----------------------------------------------------
#	Different ways of getting the list of arguments
#----------------------------------------------------

#	$1 stores the first argument, $2 stores the second argument, etc.  
#	$@ stores the full list of arguments
#	$* uses the "wild card" character * to represent all the $1 $2 $3...
#		So, really, $* means "all the $something variables/arguments"
#	Print arguments 1-3
echo "argument 1: $1"
echo "argument 2: $2"
echo "argument 3: $3"
echo "argument 6: $6"

#-----------------------------------------------------
#	Get the number of arguments (like argc in C/C++)
#-----------------------------------------------------
echo "Get the number of arguments"
#	$# is the bash equivalent of argc in C/C++
echo "number of arguments v1: $#"
#	This way looks messy, but it is going to work to get the number of
#	elements in any array, not just @ ---> ${#myArray}
echo "number of arguments v2: ${#@}"

#-----------------------------------------------------
#	Extract all the elements from a list
#-----------------------------------------------------
echo ""
echo "Extract all elements from the list"

#	Just as $1 stores the first argument, $2 stores the second argument, etc.,  
#		$@ stores the full list of arguments.
#	$@ and $*  can be used interchangeably in many cases,... except when they cannot.

#	An example of where they differ, is in the effect that quote signs have on them
#	"" has no adverse effect on $@, because $@ is an array
echo ""
echo "Version 1"
echo "---------"
for word in $@
do 
	echo $word
done

echo ""
echo "Version 2"
echo "---------"
for word in "$@"
do 
	echo $word
done

#	However, it works differently with $*
echo ""
echo "Version 3"
echo "---------"
for word in $*
do 
	echo $word
done

#	When we put the double quote around $*, it builds a single string that lists
#	all the arguments. So, there is no array, only a single string, and therefore 
#	a single word: the entire line
echo ""
echo "Version 4"
echo "---------"
for word in "$*"
do 
	echo $word
done
echo "(not working because we don't see a list of n values but a single string)"
echo ""
