#! /bin/bash


#run this from the final directory

g++ -Wall -std=c++17 -I ./ImageLibrary/include -I ./Version2/include ./Version2/src/main.cpp ./Version2/src/mapStuff.cpp ./ImageLibrary/src/RasterImage.cpp ./ImageLibrary/src/ImageIO.cpp  ./ImageLibrary/src/ImageIO_TGA.cpp -lpthread -o main