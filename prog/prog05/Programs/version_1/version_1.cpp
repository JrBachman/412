#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <pthread.h>
#include <vector>
#include <list>

using namespace std;

struct Map{
	unsigned short width, height;
	float** data;
};

struct Point{
    int row, col;
    float elev;
};


struct SkierInfo
{

	Map map;
	Point startPoint;
	bool traceMode;
	Point endPoint;
	vector<float> trace;
	pthread_t threadID;
	int index;
};

Map readMap(const string& filePath);
void* skierFunc(void* argument);



void* skierFunc(void* argument, bool trace_path)
{
	SkierInfo* data = static_cast<SkierInfo*>(argument);

	
	unsigned short width = data->map.width;
	unsigned short height = data->map.height;
	
	Point startPoint = data->startPoint;
	
	float** elev = data->map.data;
	vector<float> path;
	unsigned short row = startPoint.row;
	unsigned short col = startPoint.col;
	int count = -1;
	bool continues = true;
	while(continues)
	{
		unsigned short bestRow;
		unsigned short bestCol;
		float bestElev;
		bestElev = elev[row][col];
		continues = true;
		
		count += 1;
        if (row != 0 && row != height-1 && col != 0 && col != width -1){
            //check cardinal directions
			//NW
			if(elev[row-1][col-1] < bestElev){
				bestRow = row-1;
				bestCol = col-1;
				bestElev = elev[bestRow][bestCol];
				continues = true;
			}
			//W
			if(elev[row][col-1] < bestElev){
				bestRow = row;
				bestCol = col-1;
				bestElev = elev[bestRow][bestCol];
				continues = true;
			}
			//SW
			if(elev[row+1][col-1] < bestElev){
				bestRow = row+1;
				bestCol = col-1;
				bestElev = elev[bestRow][bestCol];
				continues = true;
			}
			//N
			if(elev[row-1][col] < bestElev){
				bestRow = row-1;
				bestCol = col;
				bestElev = elev[bestRow][bestCol];
				continues = true;
			}
			//S
			if(elev[row+1][col] < bestElev){
				bestRow = row+1;
				bestCol = col;
				bestElev = elev[bestRow][bestCol];
				continues = true;
			}
			//SE
			if(elev[row-1][col+1] < bestElev){
				bestRow = row-1;
				bestCol = col+1;
				bestElev = elev[bestRow][bestCol];
				continues = true;
			}
			//E
			if(elev[row][col+1] < bestElev){
				bestRow = row;
				bestCol = col+1;
				bestElev = elev[bestRow][bestCol];
				continues = true;
			}
			//SW
			if(elev[row+1][col+1] < bestElev){
				bestRow = row+1;
				bestCol = col+1;
				bestElev = elev[bestRow][bestCol];
				continues = true;
			}

        }

		if (row == 0){
			if(col == 0){
				//only E SE S
				//S
				if(elev[row+1][col] < bestElev){
					bestRow = row+1;
					bestCol = col;
					bestElev = elev[bestRow][bestCol];
					continues = true;
				}
				//SE
				if(elev[row-1][col+1] < bestElev){
					bestRow = row-1;
					bestCol = col+1;
					bestElev = elev[bestRow][bestCol];
					continues = true;
				}
				//E
				if(elev[row][col+1] < bestElev){
					bestRow = row;
					bestCol = col+1;
					bestElev = elev[bestRow][bestCol];
					continues = true;
				}
			}
			else if(col == width-1){
				//only W SW S
				//SW
				if(elev[row+1][col+1] < bestElev){
					bestRow = row+1;
					bestCol = col+1;
					bestElev = elev[bestRow][bestCol];
					continues = true;
				}
				//S
				if(elev[row+1][col] < bestElev){
					bestRow = row+1;
					bestCol = col;
					bestElev = elev[bestRow][bestCol];
					continues = true;
				}
				//W
				if(elev[row][col-1] < bestElev){
					bestRow = row;
					bestCol = col-1;
					bestElev = elev[bestRow][bestCol];
					continues = true;
				}
			}
			else{
				// only  E SE S SW W
				//S
				if(elev[row+1][col] < bestElev){
					bestRow = row+1;
					bestCol = col;
					bestElev = elev[bestRow][bestCol];
					continues = true;
				}
				//SE
				if(elev[row-1][col+1] < bestElev){
					bestRow = row-1;
					bestCol = col+1;
					bestElev = elev[bestRow][bestCol];
					continues = true;
				}
				//E
				if(elev[row][col+1] < bestElev){
					bestRow = row;
					bestCol = col+1;
					bestElev = elev[bestRow][bestCol];
					continues = true;
				}
				//SW
				if(elev[row+1][col+1] < bestElev){
					bestRow = row+1;
					bestCol = col+1;
					bestElev = elev[bestRow][bestCol];
					continues = true;
				}
				//W
				if(elev[row][col-1] < bestElev){
					bestRow = row;
					bestCol = col-1;
					bestElev = elev[bestRow][bestCol];
					continues = true;
				}

			}
		}
		if (row == height-1){
			if(col == 0){
				//only N NE E
				//N
				if(elev[row-1][col] < bestElev){
					bestRow = row-1;
					bestCol = col;
					bestElev = elev[bestRow][bestCol];
					continues = true;
				}
				//NE
				if(elev[row-1][col-1] < bestElev){
					bestRow = row-1;
					bestCol = col-1;
					bestElev = elev[bestRow][bestCol];
					continues = true;
				}
				//E
				if(elev[row][col+1] < bestElev){
					bestRow = row;
					bestCol = col+1;
					bestElev = elev[bestRow][bestCol];
					continues = true;
				}
			}
			else if(col == width-1){
				//only N NW W
				//NW
				if(elev[row-1][col-1] < bestElev){
					bestRow = row-1;
					bestCol = col-1;
					bestElev = elev[bestRow][bestCol];
					continues = true;
				}
				//W
				if(elev[row][col-1] < bestElev){
					bestRow = row;
					bestCol = col-1;
					bestElev = elev[bestRow][bestCol];
					continues = true;
				}
				//N
				if(elev[row-1][col] < bestElev){
					bestRow = row-1;
					bestCol = col;
					bestElev = elev[bestRow][bestCol];
					continues = true;
				}
			}
			else{
				// only E NE N NW W
				//NW
				if(elev[row-1][col-1] < bestElev){
					bestRow = row-1;
					bestCol = col-1;
					bestElev = elev[bestRow][bestCol];
					continues = true;
				}
				//W
				if(elev[row][col-1] < bestElev){
					bestRow = row;
					bestCol = col-1;
					bestElev = elev[bestRow][bestCol];
					continues = true;
				}
				//N
				if(elev[row-1][col] < bestElev){
					bestRow = row-1;
					bestCol = col;
					bestElev = elev[bestRow][bestCol];
					continues = true;
				}
				//NE
				if(elev[row-1][col-1] < bestElev){
					bestRow = row-1;
					bestCol = col-1;
					bestElev = elev[bestRow][bestCol];
					continues = true;
				}
				//E
				if(elev[row][col+1] < bestElev){
					bestRow = row;
					bestCol = col+1;
					bestElev = elev[bestRow][bestCol];
					continues = true;
				}
			}
		}
		if(col == 0 && row != 0 && row != height-1){
			// only N NE E SE S
			//NE
			if(elev[row-1][col+1] < bestElev){
				bestRow = row-1;
				bestCol = col-1;
				bestElev = elev[bestRow][bestCol];
				continues = true;
				}
				//E
			if(elev[row][col+1] < bestElev){
				bestRow = row;
				bestCol = col+1;
				bestElev = elev[bestRow][bestCol];
				continues = true;
			}
			//N
			if(elev[row-1][col] < bestElev){
				bestRow = row-1;
				bestCol = col;
				bestElev = elev[bestRow][bestCol];
				continues = true;
			}
			//S
			if(elev[row+1][col] < bestElev){
				bestRow = row+1;
				bestCol = col;
				bestElev = elev[bestRow][bestCol];
				continues = true;
			}
			//SE
			if(elev[row-1][col+1] < bestElev){
				bestRow = row-1;
				bestCol = col+1;
				bestElev = elev[bestRow][bestCol];
				continues = true;
			}
		}
		if(col == width-1 && row != 0 && row != height-1){
			// only N NW W SW S
			//N
			if(elev[row-1][col] < bestElev){
				bestRow = row-1;
				bestCol = col;
				bestElev = elev[bestRow][bestCol];
				continues = true;
			}
			//NW
			if(elev[row-1][col-1] < bestElev){
				bestRow = row-1;
				bestCol = col-1;
				bestElev = elev[bestRow][bestCol];
				continues = true;
			}
			//W
			if(elev[row][col-1] < bestElev){
				bestRow = row;
				bestCol = col-1;
				bestElev = elev[bestRow][bestCol];
				continues = true;
			}
			//S
			if(elev[row+1][col] < bestElev){
				bestRow = row+1;
				bestCol = col;
				bestElev = elev[bestRow][bestCol];
				continues = true;
			}
			//SW
			if(elev[row+1][col+1] < bestElev){
				bestRow = row+1;
				bestCol = col+1;
				bestElev = elev[bestRow][bestCol];
				continues = true;
			}
		}
		if(trace_path){
			path.push_back((bestElev));
			cout << bestElev << endl;
			data->trace = path;
			
		}
		if(bestRow == row && bestCol == col){
			continues = false;
		}
		if(continues){
			row = bestRow;
			col = bestCol;
			
		

		}
	}
	
	data->endPoint = {row, col};
	return nullptr;
}

int main(int argc, const char* argv[])
{
	//if(argc != 6){
    //    std::cerr << "Usage: Input, Output, Number of processes, Number of runs,flag trace";
    //    return 1;
    //}
	string filePath = argv[1];
    FILE *out = fopen(argv[2], "w");
    int processes = atoi(argv[3]);
    int run = atoi(argv[4]);
    bool trace = atoi(argv[5]);
    
    Map map = readMap(filePath);
    (void) map;
    
    Point startPoint = {3, 4};
    
    SkierInfo skier = {map, startPoint, trace};

    

    skierFunc(&skier,trace);

    return 0;
}

Map readMap(const string& filePath)
{
	 ifstream inFile(filePath);
         unsigned short numRows, numCols;
         inFile >> numRows >> numCols;
         float** mapData = new float*[numRows];
         for (int i = 0; i < numRows; i++)
         {
             mapData[i] = new float[numCols];
             for (int j = 0; j <numCols; j++){
                inFile >>mapData[i][j];
             }
         }
         
        inFile.close();
               
        
	Map map = {numCols, numRows, mapData};
    
	return map;
}