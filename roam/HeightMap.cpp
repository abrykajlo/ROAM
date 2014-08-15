#include <iostream>
#include <math.h>

using namespace std;

#include "HeightMap.h"

HeightMap::HeightMap() {
	pixels = 0;
	h = 0;
	w = 0;
}

HeightMap::HeightMap(const char * Filename, float height = 1.0) {
	img.Load(Filename);
	this->pixels = (RGBA*)img.GetBits();
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

void HeightMap::LoadBMP(const char * filename) {
	
	img.Load(filename);
}

float HeightMap::operator()(float x, float y) {
//x and z coordinates taken in as value between -1.0 and 1.0
	//get in range 0.0 to 2.0
	x += 1.0;
	y += 1.0;

	unsigned int i, j; //indices of point to fetch
	float fi, fj; //float reps of i and j
	fi = ((float) (w-1)) * (x / 2.0);
	fj = ((float) (h-1)) * (y / 2.0);
	int color;
	float h00, h10, h01, h11, h0, h1, h; // height values to interpolate between
	
	i = floor(fi);
	j = floor(fj);
	color = pixels[i * w + j].Red;
	h00 = color / 255.0 * height;

	i = floor(fi);
	j = ceil(fj);
	color = pixels[i * w + j].Red;
	h01 = color / 255.0 * height;

	i = ceil(fi);
	j = floor(fj);
	color = pixels[i * w + j].Red;
	h10 = color / 255.0 * height;

	i = ceil(fi);
	j = ceil(fj);
	color = pixels[i * w + j].Red;
	h00 = color / 255.0 * height;

	h0 = h00 + (h01 - h00) * (fj - floor(fj));
	h1 = h10 + (h11 - h10) * (fj - floor(fj));

	h = h0 + (h1 - h0) * (fi - floor(fi));
	return h;
}