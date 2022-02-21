#ifndef DATA_TYPES_H
#define DATA_TYPES_H

//	We saw all of that in Sample 2.  Moving on...
// You can also use the default increment by one to associate ranges
//	of values to different categories
typedef enum TerrainType {
	UNKNOWN = -1,
	//
	SEA = 0,
	//
	FORREST = 100,			//	the way I write it, all the terrain types
	TROPICAL_FORREST,		//	in the range 100-199 will be some kind of
	TEMPERATE_FORREST,		//	forest
	//
	DESERT = 200,
	//
	FIELD = 300,
	WHEAT_FIELD,
	RICE_FIELD,
	POLDER,
	//
	PRAIRIE = 400,
	//
	HELL = 10000
} TerrainType;

typedef struct SurveyData {
	int x, y;	//	coordinates of a survey point on a grid
	float elev;	//	elevation at that point
	TerrainType type;
} SurveyData;

#endif //	DATA_TYPES_H