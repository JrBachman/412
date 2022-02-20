gcc $1 -o prog01_v2

#checking to make sure the right number of arguments is provided
if (($# != 4));
    then
    echo "Proper usage: script01 v1.sh <path to source file> num1 num2 num3"
    exit 1
    fi

#this is where i attempted to error check for a filename that is incorrect
#I could not get it to work in the end
#if ! command -v $1
#    then
#    echo "File does not exist: ../Programs/wrongName.c"



#these check to see if the second through 4th argument is not a positive integer
re='^[0-9]+$'
if ! [[ $2 =~ $re ]]; then
    echo "The second argument is not a positive integer" 
    exit 1
    fi

if ! [[ $3 =~ $re ]]; then
    echo "The third argument is not a positive integer" 
    exit 1
    fi

if ! [[ $4 =~ $re ]]; then
    echo "The fourth argument is not a positive integer" 
    exit 1
    fi

#runs the program
./prog01_v2 $2 $3 $4