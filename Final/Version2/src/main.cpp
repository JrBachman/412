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
/**
 * create a thread per frag locking and unlocking the fragment list as needed
 *
 * @param void pointer
 */
void* fragDo(void*);
/**
 * a thread to monitor the folder at the passed location for new files. Then adds new files to a 
 * fragment list and moves the file to a folder of other processed files.
 *
 * @param void pointer to the folder to watch
 */
void* watch1(void* root);
/**
 * the thread reads the fragment at the givin file path then adds it to the map
 *
 * @param void pointer to the file path
 */
void* fragStuff(void* filePath);
vector<Map> maps;
pthread_mutex_t fragLock;

vector<pthread_mutex_t> mapLocks;



int main(int argc, char* argv[]){
  time_t startTime = time(NULL);
  string config = argv[1];
  cout << "The config string: " << config << endl;
  ifstream inFile(config);
  string watchFolder;
  string outputFolder;
  int numMaps;
  pthread_mutex_init(&fragLock, NULL);
  inFile >> watchFolder >> outputFolder >> numMaps;
  if(outputFolder[outputFolder.length()-1] != '/'){
    outputFolder += "/";
  }

  DIR* directory = opendir(outputFolder.c_str());
    if (directory == NULL) {
		  //cout << "data folder " << root.c_str() << " not found" << endl;
      string outt = "mkdir " + outputFolder;
      system(outt.c_str());
	  }
  
  
  mapLocks = vector<pthread_mutex_t>(numMaps);
  //set up maps and locks
  int wid,heigh;
  for(int i = 0; i < numMaps; i++){
    inFile >> wid >> heigh;
    maps.push_back(newMap((i+1),heigh,wid));
    pthread_mutex_init(&(mapLocks[i]), NULL);
 
  }
  //create a thread to watch the watchfolder and one to execute the processes
  pthread_t watcherID;
  pthread_t doThings;
  pthread_create(&watcherID, nullptr, watch1, &watchFolder);
  pthread_create(&doThings, nullptr, fragDo, NULL);
  bool running = true;
  while(running){
    string s;
    bool mapValid = false;
    bool validRGB = false;
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
      
      while(!mapValid){
        if(mapInd < 1 | mapInd > 3){
          cout << "map index is invalid, correct use is 1-3" << endl << "Input new mapInd: ";
          cin >> mapInd;
        }
        else{
          mapValid = true;
        }
      }
      
      while (!validRGB ){
       if (rVal <= 0 | rVal >= 255 | gVal <= 0 | gVal >= 255 |bVal <= 0 | bVal >= 255){
         cout << "values invalid, correct values between 0-255" << endl << "Input new RGB values: "; 
         cin >> rVal >> gVal >> bVal;
       }
       else{
         validRGB = true;
       }
      } 
      
      RasterImage imgOut = getCurrentMapSnapshot(outF, rVal, gVal, bVal);
      time_t now = time(NULL);
      now = now - startTime;
      string name = outputFolder + "map_" +to_string(mapInd) + "_" + to_string(now) +".tga";
      
      writeImage(imgOut, name.c_str());
    }
  }

  void* w;
  void* e;
  pthread_join(doThings, &e);
  pthread_join(watcherID, &w);
 
  return 0;
}


void* fragDo(void*) {
  while(keepWatching){
      
    usleep(200000);
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
  system("mkdir ./processedData");
 while (keepWatching) {
    DIR* directory = opendir(root.c_str());
    if (directory == NULL) {
		  cout << "data folder " << root.c_str() << " not found" << endl;
		  exit(1);
	  }
	
    struct dirent* entry;
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
