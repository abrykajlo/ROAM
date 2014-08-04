#include "HeightMap.h"

HeightMap::HeightMap() {
	pixels = 0;
	h = 0;
	w = 0;
}

HeightMap::HeightMap(const char * Filename, float height = 1.0) {
	CBitmap img;
	img.Load(Filename);
	this->pixels = img.GetBits();
	this->w = img.GetWidth();
	this->h = img.GetHeight();
	this->height = height;
}

int HeightMap::GetHeight() {
	return h;
}

int HeightMap::GetWidth() {
	return w;
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