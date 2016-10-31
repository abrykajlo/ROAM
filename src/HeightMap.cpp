#include <iostream>

using namespace std;

#include "HeightMap.h"

HeightMap::HeightMap()
{
	HeightMap(10);
}

HeightMap::HeightMap(int n) 
{
	int side = 1 << n + 1;

	heights = new float[side * side];
}

HeightMap::~HeightMap()
{
	delete [] heights;
}

int HeightMap::get_side_length()
{
	return side;
}



float HeightMap::operator()(int x, int y)
{
    if (x >= side || y >= side || x < 0 || y < 0)
    {
        throw 1;
	}

    return 0.0;
}