#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

int main(int argc, const char* argv[])
{
    int counter = 0;
	char word[50];
    char target[50];
    strcpy(target, argv[1]);
    FILE *in_file;
    
    
    for (int i = 2; i < argc; i++){
        in_file = fopen(argv[i], "r");
        while(fscanf(in_file, "%s", word) != EOF){
            if(strcmp(word, target) == 0){
                counter++;
            }
        }
        fclose(in_file);
        
    }
    printf("%d", counter);
    return 0;
    

}