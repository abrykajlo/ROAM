#include "include/bitmap.h"


#ifndef HEIGHT_MAP_H
#define HEIGHT_MAP_H

class HeightMap {
private:
	CBitmap img;
	RGBA* pixels;
	int h;
	int w;
	float height;
public:
	HeightMap();
	HeightMap(const char*, float);
	int GetHeight();
	int GetWidth();
	void LoadBMP(const char*);
	//~HeightMap();
	float operator()(float, float);	
};

#endif