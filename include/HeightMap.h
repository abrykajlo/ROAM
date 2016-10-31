#ifndef SRC_HEIGHT_MAP_H_
#define SRC_HEIGHT_MAP_H_

class HeightMap
{
public:
    HeightMap();
	HeightMap(int);
    ~HeightMap();
	float operator()(int, int);	
    
    int get_side_length();
private:
	float* heights;
    int side;
};

#endif // SRC_HEIGHT_MAP_H_