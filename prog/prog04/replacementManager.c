#include <stdio.h>
#include "memory.h"
#include "replacementManager.h"

void* myMalloc( size_t size){
    return getAddress ( size);
}

void myFree( void * ptr ){
    mmFree( ptr );
}