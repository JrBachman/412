#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
#include <typeinfo>
#include <cstdlib>
#include <dirent.h>
#include <vector>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include "RasterImage.h"
#include "mapStuff.h"
#include "ImageIO.h"


using namespace std;

vector<string> fragList;
bool keepWatching = true;
void* fragDo(void*);
//void addfragment(map& map, const fragment& frag);
//vector<string> watch(string root);
void* watch1(void* root);
void* fragStuff(void* filePath);
vector<Map> maps;





int main(int argc, char* argv[]){
  time_t startTime = time(NULL);
  string config = argv[1];
  cout << "The config string: " << config << endl;
  ifstream inFile(config);
  string watchFolder;
  string outputFile;
  int numMaps;
  inFile >> watchFolder >> outputFile >> numMaps;


  int pipeP2C[2];
	int pipeC2P[2];	
	if (pipe(pipeP2C) == -1) 
	{
		cout << "Creation of P to C pipe failed" << endl << flush;
		exit(2);
	}
	if (pipe(pipeC2P) == -1) 
	{
		cout << "Creation of C to P pipe failed" << endl << flush;
		exit(3);
	}
  int p;
  
  for(int i = 0; i < numMaps; i++)
    p = fork();
  	if (p < 0){
  		perror("Fork failed");
  		exit(4);
  	}
  	// in the child
  	if (p==0){
  		childFunction(pipeP2C, pipeC2P);
  	}
  
  
  int wid,heigh;
  for(int i = 0; i < numMaps; i++){
    inFile >> wid >> heigh;
    maps.push_back(newMap((i+1),heigh,wid));
    
 
    //maps.rbegin maps.rend();
  }
  
  
  //watch1(watchFolder);
  bool running = true;
  while(running){
    string s;
    int mapInd, rVal, gVal, bVal, p;
    cout << "Command: ";
    cin >> s;
    if (s=="LIST") {
      for(int i =0; i < fragList.size(); i++){
        cout << fragList[i] << endl;
      }
    }
    else if (s== "END"){
      keepWatching = false;
      running = false;
    }
    else if (s== "MAP"){
      cin >> mapInd >> rVal >> gVal >> bVal;
      
      Map& outF = maps[mapInd-1];
      //cout << p;
      RasterImage imgOut = getCurrentMapSnapshot(outF, rVal, gVal, bVal);
      time_t now = time(NULL);
      now = now - startTime;
      //cout << now;
      string name = "map_" +to_string(mapInd) + "_" + to_string(now) +".tga";
      writeImage(imgOut, name.c_str());
    }
  }

  // wait for the watch thread to quit
  void* w;
  void* e;
  pthread_join(doThings, &e);
  pthread_join(watcherID, &w);
 
    return 0;
}
void* fragDo(void*) {
  while(keepWatching){
      
    usleep(200000);
    //for(int i =0; i <= fragList.size(); i++ ){
    //  cout << fragList[0] << endl
    vector<pthread_t> threadList;
    pthread_mutex_lock(&fragLock);
    while (!fragList.empty()){
      
      string* filePath = new string(fragList.back());
      fragList.pop_back();
      pthread_mutex_unlock(&fragLock);
      pthread_t doFrag;
      pthread_create(&doFrag, nullptr, fragStuff, filePath);
      threadList.push_back(doFrag);      
      pthread_mutex_lock(&fragLock);
    }
    pthread_mutex_unlock(&fragLock);

    for (size_t k=0; k<threadList.size(); k++) {
      void* p;     
      pthread_join(threadList[k], &p);

    }
   }
  return nullptr;
}

void* fragStuff(void* filePath){
  string& fileP = *(static_cast<string*>(filePath));
  fragment frag = readFragment(fileP);
  addFragment(maps,frag, mapLocks);
  free(filePath);
  return nullptr;
}


void* watch1(void* rootFile){
  const string& root = *static_cast<const string*>(rootFile);
  //vector<string> fragList;
  // const char * dataRootPath = root.c_str();
  system("mkdir ./processedData");
 while (keepWatching) {
 //becomes thread function
    DIR* directory = opendir(root.c_str());
    if (directory == NULL) {
		  cout << "data folder " << root.c_str() << " not found" << endl;
		  exit(1);
	  }
	
    struct dirent* entry;
  //  vector<string>  fileName;
    while ((entry = readdir(directory)) != NULL) 
    {
        string path;
        const char* name = entry->d_name;
        if(strcmp(root.c_str() + root.length()-1,"/") == 0){
          path = root+string(entry->d_name);
        }
        else{
          path = root+"/"+string(entry->d_name);
        }
        //cout << path << endl;
      // ignore entries thay start with a . . .. 
      if (entry->d_name[0] != '.') {
        string newPath = "./processedData/" + string(entry->d_name);
        if (strcmp(path.c_str() + path.length()-4,".dat") == 0){
          //lock
          pthread_mutex_lock(&fragLock);
			    fragList.push_back(newPath);
          //unlock
          pthread_mutex_unlock(&fragLock);
        }
    
      string mvCommand = ("mv " + path + " " + newPath);
      system(mvCommand.c_str());
        }
      
    }
	closedir(directory);
  usleep(2000000);
   }

  return nullptr;
}
