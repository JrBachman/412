#include <stdio.h>
#include <stdlib.h>

int main(int argc, char*argv[])
{
   int a,b,c,sum,temp;
   if (argc == 4) 
   {
       //gets variables
      a = strtol(argv[1], NULL, 0);
      b = strtol(argv[2], NULL, 0);
      c = strtol(argv[3], NULL, 0);

//checks for positive integers
      if(sscanf(argv[1], "%d", &a)!=1){
          fprintf(stderr,"The first argument is not a positive integer \n");
          return 0;
      }
      if(sscanf(argv[2], "%d", &b)!=1){
          fprintf(stderr,"The second argument is not a positive integer \n");
          return 0;
      }
      if(sscanf(argv[3], "%d", &c)!=1){
          fprintf(stderr,"The third argument is not a positive integer \n");
          return 0;
      }
//reorders the input into ascending order
      if(a > c){
          temp = c;
          c = a;
          a = temp;
      }
      if(a > b){
          temp = b;
          b = a;
          a = temp;
      }
      if(b > c){
          temp = c;
          c = b;
          b = temp;
      }
//summation of a to b
      for (int i = a; i <= b; i++){
          sum = sum+i;
      }
      printf("The sum of all integers in the range [%d",a);
      printf(",%d",b);
      printf("] is %d\n",sum);
      sum = 0;
//summation of a to c
      for (int i = a; i <= c; i++){
          sum = sum+i;
      }
      printf("The sum of all integers in the range [%d",a);
      printf(",%d",c);
      printf("] is %d\n",sum);
      sum = 0;

//summation of b to c
      for (int i = b; i <= c; i++){
          sum = sum+i;
      }
      printf("The sum of all integers in the range [%d",b);
      printf(",%d",c);
      printf("] is %d\n",sum);






   }
   //prints to error if too many or not enough arguments are provided
   else if(argc != 4){
       fprintf(stderr,"Proper usage: %s", argv[0]);
       fprintf(stderr," num1 num2 num3 \n");
   }
   
   return 0;
}
