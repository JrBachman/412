#include <stdbool.h>


#define USE_REPLACEMENT_MANAGER 1 // or
#if USE_REPLACEMENT_MANAGER
    #include "replacementManager.h"
#else
    #include <stdlib.h>
#endif


struct node{
    int start_address;
    int size;
    bool free;
    struct node *next;
    struct node *previous;
};