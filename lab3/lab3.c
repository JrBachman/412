#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct type_char{
    char* Digits;
    char* letters;
    char* other;
};


FILE * readFile(const char* file){
    FILE* readFile = fopen(file, "r");

    if(readFile == NULL){
        printf("Couldn't open provided file\n");
        exit(1);
    }else{
        return readFile;
    }
}


int main(int argc, char* argv[]){

}