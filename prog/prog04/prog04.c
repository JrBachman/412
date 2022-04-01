#include <stdio.h>

#define USE_REPLACEMENT_MANAGER 1

#if USE_REPLACEMENT_MANAGER


    #undef malloc 
    #define malloc myMalloc

    #undef free
    #define free myFree

    #include "replacementManager.h"

#else 

    #include <stdlib.h>
    void* (*f) (size_t s) = malloc;

#endif 

int main( int argc, const char * argv[] ){
    char * x = (char *) malloc(16);
    free(x);
