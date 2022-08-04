#ifndef	MAP_STUFF_H
#define	MAP_STUFF_H
#include <string>
#include <vector>
#include "RasterImage.h"

/**
 * struct to hold maps
 *
 * @param mapIndex contains value of which map it is, either 1, 2 or 3
 * @param sumRed sumGreen sumBlue contain color values
 * @param countImage the number of pixels added to a given location
 * @param outImage is the final version created for each image
 */
struct Map{
    unsigned short mapIndex;
    RasterImage sumRed, sumGreen, sumBlue, countImage, outImage;

};

/**
 * struct to hold fragments
 *
 * @param mapIndex contains value of which map it belongs to, either 1, 2 or 3
 * @param row the row at which the fragment starts
 * @param col the column at which the fragment starts
 * @param height the height of the fragment
 * @param width the width of the fragment
 * @param validPixel if the pixel in the fragment should be added to the map
 * @param data a RasterImage image of the fragment
 *
 */

struct fragment{
  unsigned short mapIndex, row, col, height, width;
  bool validPixel;
  RasterImage data;
};

/**
 * function of type fragment that reads in a fragment from an image directory 
 *
 * @param filePath is the file path of each fragment
 */

fragment readFragment(const std::string& filePath);

/**
 * function of type Map that creates a map
 *
 * @param index is the index of a map, either 1 2 or 3
 * @param width is the width of the map
 * @param height is the height of the map
 */

Map newMap(unsigned short index, unsigned short width, unsigned short height);

/**
 * function of type void that adds a fragment to the map 
 *
 * @param map is vector of type Map, we stoire our maps in a vector and pass one of them at a time into this function 
 * @param frag is a reference to a fragment
 */

void addFragment( std::vector<Map>& map, fragment& frag);

/**
 * a function to return a RasterImage of the current state of the map at the time it is called
 *
 * @param map is the map image that is to be printed
 * @param setR the value for red pixel to be used if there is no valid data for the pixel
 * @param setG the value for green pixel to be used if there is no valid data for the pixel
 * @param setB the value for blue pixel to be used if there is no valid data for the pixel
 *
 */

RasterImage getCurrentMapSnapshot(const Map &map, int setR, int setG, int setB);

#endif
