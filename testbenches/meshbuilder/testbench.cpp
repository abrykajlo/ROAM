#include "include/Angel.h"

#include "HeightMap.h"

#include <iostream>
#include <cstdlib>
#include <cmath> 
#include <fstream>
#include <vector>

using namespace std;

typedef Angel::vec4 point4;
typedef Angel::vec3 point3;

vector<point4> vertex_list;
vector<point3> face_list;

int main(int argc, char const *argv[])
{
	HeightMap height_vals("heightmap513px.bmp", 0.5);

	int granularity = 8; //Must be power of 2

	int w = granularity+1;
	int h = granularity+1;
	int levels = granularity-1; //Minus two for skipping root empty square

	//Calculation of vertices
	for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            point4 new_vertex;
            new_vertex.x = -1.0+((j/((w-1)*1.0))*2.0);
            new_vertex.y = -1.0+((i/((h-1)*1.0))*2.0);
            new_vertex.z = height_vals(new_vertex.x, new_vertex.y); //Height value
            new_vertex.w = 1;
            vertex_list.push_back(new_vertex);
        }
    }

    //Root square with two initial triangles:
    point3 new_face1;
    point3 new_face2;

    new_face1.x = 0; //top left
    new_face1.y = (w*h)-1; //bottom right
    new_face1.z = w-1; //top right
    
    new_face2.x = (w*h)-1; //bottom right
    new_face2.y = 0; //top left
    new_face2.z = (h-1)*w; //bottom left

    face_list.push_back(new_face1);
    face_list.push_back(new_face2);

    levels--;

    //Build all sub triangles for all triangles in face list (goes by levels)
    int i = 0;
    for (int currlevel = 0; currlevel < levels; currlevel++) {
    	int limit = face_list.size();
	    while (i < limit) {
    		point3 new_face3;
			point3 new_face4;

			new_face3.x = face_list[i].z;
			new_face3.y = face_list[i].x;
			new_face3.z = (face_list[i].x + face_list[i].y)/2;
			
			new_face4.x = face_list[i].y;
			new_face4.y = face_list[i].z;
			new_face4.z = (face_list[i].x + face_list[i].y)/2;

			face_list.push_back(new_face3);
			face_list.push_back(new_face4);

	    	i++;
	    }
    }

    cout << vertex_list.size() << " " << face_list.size() << endl;

	return 0;
}