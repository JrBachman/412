# compiler options -- C99 with warnings
OPT_GCC = -std=c++14 -Wall -Wextra

OS_NAME := $(shell uname -s | tr A-Z a-z)

INCLUDES = -I ./ImageLibrary/include
SRC = ./ImageLibrary/src/ImageIO_TGA.cpp ./ImageLibrary/src/ImageIO.cpp  ./ImageLibrary/src/RasterImage.cpp  ./ImageLibrary/src/utilities.cpp

# compiler options and libraries for Linux & Mac OS X
# mac OS
ifeq ($(OS_NAME), darwin)
  OPT = 
  LIB = 
# Linux
else 
  OPT = -D_XOPEN_SOURCE=700
  LIB = -lrt
endif

all: flipV flipH main

flipV: ./ImageLibrary/applications/flipV_shm.cpp
	g++ $(OPT_GCC) $(OPT) -o flipV $(INCLUDES) ./ImageLibrary/applications/flipV_shm.cpp $(SRC) $(LIB)

flipH: ./ImageLibrary/applications/flipH_shm.cpp
	g++ $(OPT_GCC) $(OPT) -o flipH $(INCLUDES) ./ImageLibrary/applications/flipH_shm.cpp $(SRC) $(LIB)

main: ./ImageLibrary/applications/startMain.cpp 
	g++ $(OPT_GCC) $(OPT) -o startMain $(INCLUDES) ./ImageLibrary/applications/startMain.cpp $(SRC) $(LIB)

run: main flipV flipH
	./startMain ./Images/Renoir.tga ./Output

clean:
	rm -f main flipV flipH

os:
	@echo $(OS_NAME)
