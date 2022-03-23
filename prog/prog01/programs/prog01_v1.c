#include <stdio.h>
#include <stdlib.h>

int main(int argc, char*argv[])
{
// Initialize variables and set the variables to the passed arguments
   int a,b,sum,temp;
   if (argc == 3) 
   {
      a = strtol(argv[1], NULL, 0);
      b = strtol(argv[2], NULL, 0);

//error checks the arguments
      if(sscanf(argv[1], "%d", &a)!=1){
          fprintf(stderr,"The first argument is not a positive integer \n");
          return 0;
      }
      else if(sscanf(argv[2], "%d", &b)!=1){
          fprintf(stderr,"The second argument is not a positive integer \n");
          return 0;
      }
//re orders the variables if necessary
      if(a > b){
          temp = b;
          b = a;
          a = temp;
      }


//calculates the summation
      for (int i = a; i <= b; i++){
          sum = sum+i;
      }
      printf("%d\n",sum);
   }
//prints to error if not enough or too many arguments are passed
   else if(argc != 3){
       fprintf(stderr,"Proper usage: %s", argv[0]);
       fprintf(stderr," num1 num2 \n");
   }
   
   
   return 0;
}