#include "include/bitmap.h"
#include "HeightMap.h"

using namespace std;

int main() {
	CBitmap img;
	img.Load("heightmap.bmp");
	HeightMap h((RGBA*)img.GetBits(), img.GetWidth(), img.GetHeight(), 1.0);
	cout << h(0.5, 1.0);
	return 0;
}