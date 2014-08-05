#include "include/Angel.h"

#include <iostream>
#include <cstdlib>
#include <cmath> 
#include <fstream>
#include <vector>

using namespace std;

typedef Angel::vec4 point4;
typedef Angel::vec3 point3;
typedef Angel::vec4 color4;

vector<point4> vertex_list;
vector<point3> face_list;

int main(int argc, char const *argv[])
{
    int w = 100;
    int h = 100;

    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            point4 new_vertex;
            new_vertex.x = -1.0+((j/((w-1)*1.0))*2.0);
            new_vertex.z = -1.0+((i/((h-1)*1.0))*2.0);
            new_vertex.y = 0;
            new_vertex.w = 1;
            vertex_list.push_back(new_vertex);
        }
    }

    for (int i = 0; i < (h-1); ++i) {
        for (int j = 0; j < (w-1); ++j) {
            point3 new_face1;
            point3 new_face2;

            new_face1.x = (i*w)+(j+0);
            new_face1.y = (i*w)+(j+1);
            new_face1.z = ((i+1)*w)+(j+0);
            
            new_face2.x = (i*w)+(j+1);
            new_face2.y = ((i+1)*w)+(j+0);
            new_face2.z = ((i+1)*w)+(j+1);

            face_list.push_back(new_face1);
            face_list.push_back(new_face2);
        }
    }

    for (int i = 0; i < vertex_list.size(); ++i)
    {
        cout << vertex_list[i].x << " " << vertex_list[i].y << " " << vertex_list[i].z << endl;
    }

    return 0;
}