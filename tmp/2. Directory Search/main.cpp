//
//  main.cpp
//  Directory Operations, C++ version
//
//  Created by Jean-Yves Hervé on 2017-02-15.
//	Edited 2018-02-25, 2019-11-04
//
//	This C++ program takes as argument the path to a folder and
//	outputs the name of all the files found in that folder
//
//	For reference:  file types recognized by dirent
//		#define	DT_UNKNOWN	 0
//		#define	DT_FIFO		 1
//		#define	DT_CHR		 2
//		#define	DT_DIR		 4
//		#define	DT_BLK		 6
//		#define	DT_REG		 8
//		#define	DT_LNK		10
//		#define	DT_SOCK		12
//		#define	DT_WHT		14


#include <iostream>
#include <vector>
#include <sys/stat.h>
#include <dirent.h>

using namespace std;

int main(int argc, const char * argv[]) 
{

	const char* dataRootPath = argv[1];
	
    DIR* directory = opendir(dataRootPath);
    if (directory == NULL) 
    {
		cout << "data folder " << dataRootPath << " not found" << endl;
		exit(1);
	}
	
    struct dirent* entry;
    vector<string>  fileName;
    while ((entry = readdir(directory)) != NULL) 
    {
        const char* name = entry->d_name;
        if ((name[0] != '.') && (entry->d_type == DT_REG))
        {
			fileName.push_back(string(entry->d_name));
        }
    }
	closedir(directory);
	
	cout << fileName.size() << " files found" << endl;
	for (int k=0; k<fileName.size(); k++)
		cout << "\t" << fileName[k] << endl;

	return 0;
}
