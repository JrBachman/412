#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
#include <typeinfo>
#include <cstdlib>
#include <dirent.h>
#include <vector>
#include "mapStuff.h"
#include "RasterImage.h"
#include "ImageIO.h"
using namespace std;
/*
struct Map{
    unsigned short mapIndex;
    RasterImage sumRed, sumGreen, sumBlue, countImage, outImage;

};

struct fragment{
  unsigned short mapIndex, row, col, height, width;
  bool validPixel;
  RasterImage data;
};

*/
fragment readFragment(const string& filePath);
Map newMap(unsigned short index, unsigned short width, unsigned short height);
void addFragment( vector<Map>& map, fragment& frag, vector<pthread_mutex_t>& mapLocks);
RasterImage getCurrentMapSnapshot(Map &map, int setR, int setG, int setB);
//pthread_mutex_t mapLock;


fragment readFragment(const string& filePath){
   
    fragment frag;
    ifstream inFile(filePath);
 // cout << filePath;
 //   unsigned short width, height,r,g,b,a;
    
    inFile >> frag.mapIndex >> frag.row >> frag.col >> frag.height >> frag.width;
    //cout << frag.height;
    frag.data = newImage(frag.width, frag.height, RGBA32_RASTER);
    int** raster = static_cast<int**>(frag.data.raster2D);
    for(int i = 0; i < frag.height; i++){
      for(int j = 0; j < frag.width; j++){
        		int r, g, b, v;
			inFile >> r >> g >> b >> v;
    //    cout << r << endl;
			unsigned char* rgba = reinterpret_cast<unsigned char*>(raster[i]+j);
			rgba[0] = static_cast<unsigned char>(r);
			rgba[1] = static_cast<unsigned char>(g);
			rgba[2] = static_cast<unsigned char>(b);
			rgba[3] = static_cast<unsigned char>(v);
    }
  }

  return frag;
  }


Map newMap(unsigned short index, unsigned short width, unsigned short height)
{
	Map map  = {index,
				newImage(width, height, FLOAT_RASTER), // redSum
				newImage(width, height, FLOAT_RASTER), // greenSum
				newImage(width, height, FLOAT_RASTER), // blueSum
				newImage(width, height, FLOAT_RASTER) // count
				};
	
	return map;
}


RasterImage getCurrentMapSnapshot(const Map &map, int setR, int setG, int setB){
  RasterImage imageOut;
   
  
  unsigned short width = map.sumRed.width;
  unsigned short height = map.sumRed.height;
  imageOut =(newImage(width, height, RGBA32_RASTER));
  const float*const* rasterRed = static_cast<float**>(map.sumRed.raster2D);
  const float*const* rasterBlue = static_cast<float**>(map.sumBlue.raster2D);
  const float*const* rasterGreen = static_cast<float**>(map.sumGreen.raster2D);
  const float*const* rasterCount = static_cast<float**>(map.countImage.raster2D);
  int** rasterOut = static_cast<int**>(imageOut.raster2D);
  
  for(int i = 0; i < height; i++){
    for(int j = 0; j < width; j++){
      if(rasterCount[i][j] != 0){
        unsigned char r,g,b;
        r = static_cast<unsigned char>(rasterRed[i][j]/rasterCount[i][j]);
        g = static_cast<unsigned char>(rasterGreen[i][j]/rasterCount[i][j]);
        b = static_cast<unsigned char>(rasterBlue[i][j]/rasterCount[i][j]);
        unsigned char* rgba = reinterpret_cast<unsigned char*>(rasterOut[i]+j);
        rgba[0] = r;
        rgba[1] = g;
        rgba[2] = b;
        rgba[3] = 255;
      }
      else{
        unsigned char* rgba = reinterpret_cast<unsigned char*>(rasterOut[i]+j);
        rgba[0] = setR;
        rgba[1] = setG;
        rgba[2] = setB;
        rgba[3] = 255;
      }
    }
  }
      
     
  return imageOut;
}

void addFragment( vector<Map>& map, fragment& frag, vector<pthread_mutex_t>& mapLocks){
  
  unsigned short ind = frag.mapIndex -1;
  unsigned short width = frag.width;
  unsigned short height = frag.height;
  
  
    //cout << "adding to map: " << k << endl;
    float** rasterRed = static_cast<float**>(map[ind].sumRed.raster2D);
    float** rasterBlue = static_cast<float**>(map[ind].sumBlue.raster2D);
    float** rasterGreen = static_cast<float**>(map[ind].sumGreen.raster2D);
    float** rasterCount = static_cast<float**>(map[ind].countImage.raster2D);
    
    int** rasterIn = static_cast<int**>(frag.data.raster2D);
    pthread_mutex_lock(&mapLocks[frag.mapIndex]);
    for(int i = 0; i < height; i++){
      for(int j = 0; j < width; j++){
       unsigned char* rgba = reinterpret_cast<unsigned char*>(rasterIn[i]+j);
       if(rgba[3] != 0){
          rasterRed[i + frag.row][j + frag.col] += rgba[0];
          rasterGreen[i + frag.row][j + frag.col] += rgba[1];
          rasterBlue[i + frag.row][j + frag.col] += rgba[2];
          rasterCount[i + frag.row][j + frag.col]++;
        }
        
      }
    }
  pthread_mutex_unlock(&mapLocks[frag.mapIndex]);
}
