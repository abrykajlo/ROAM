#include "HeightMap.h"

HeightMap::HeightMap() {
	pixels = 0;
	h = 0;
	w = 0;
}

HeightMap::HeightMap(RGBA* pixels, int w, int h, float height = 1.0) {
	this->pixels = pixels;
	this->w = w;
	this->h = h;
	this->height = height;
}

float HeightMap::operator()(float x, float z) {
//x and z coordinates taken in as value between -1.0 and 1.0
	//get in range 0.0 to 2.0
	x += 1.0;
	z += 1.0;
	
	int i, j; //indices of point to fetch

	i = ((float) w) * (x / 2.0);
	j = ((float) h) * (z / 2.0);

	RGBA pVal = pixels[i * w + j];

	return static_cast<float>(pVal.Red) / 255.0 * height;
}