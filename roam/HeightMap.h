#include "include/bitmap.h"


#ifndef HEIGHT_MAP_H
#define HEIGHT_MAP_H

class HeightMap {
private:
	RGBA* pixels;
	int h;
	int w;
	float height;
public:
	HeightMap();
	HeightMap(RGBA*, int, int, float);
	//~HeightMap();
	float operator()(float, float);	
};

#endif