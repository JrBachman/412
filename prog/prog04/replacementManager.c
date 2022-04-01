#include <stdio.h>
#include "replacementManager.h"
#define HEAP_SIZE   1000
typedef struct Node{
    int             type;
    size_t          size;
    char*           start_address;
    struct Node*    next;
    struct Node*    prev;
}
Node;

Node* head = NULL;
Node* heap = NULL;
void* myMalloc( size_t size){
    Node * curr = head;
    while ( curr -> next != NULL ){
        if ( ( curr -> size < size ) && ( curr -> type == 0 ) ){
            return curr -> start_address;
        } else curr = curr -> next;
    }
    Node* new_node = (Node *) malloc( sizeof(Node) );
    new_node -> type = 1;
    new_node -> size = size;
    new_node -> start_address = ( curr -> start_address ) + ( curr -> size ) + 1;
    new_node -> next = NULL;
    new_node -> prev = curr;
}

void myFree( void * ptr ){
    Node * curr = head; 
    while ( curr != NULL ){
        if  (  curr -> start_address == ptr ){
            curr -> type = 0;
            break; 
        } else curr = curr -> next;
    }

    Node * p = curr -> prev;
    Node * n = curr -> next;

    if ( ( p != NULL ) && ( p -> type  == 0 ) ){
        curr -> start_address = p -> start_address; 
        curr -> size = ( curr -> size ) + ( p -> size );
        p -> prev -> next = curr;
        curr -> prev = p -> prev;
        free ( p );
    }

    if ( ( n != NULL ) && (n -> type == 0) ){
        curr -> size  = ( curr -> size ) + ( n -> size );
        if ( n -> next != NULL ){
            n -> next -> prev = curr;
            curr -> next = n -> next; 
        }else curr -> next = NULL;
        free ( n );
    }
}

void init_heap( void ){
    heap = malloc( HEAP_SIZE );
    head = (Node*) malloc( sizeof(Node) );
    head -> type = 1;
    head -> size = 0;
    head -> start_address = heap;
    head -> next = NULL;
    head -> prev = NULL;
}